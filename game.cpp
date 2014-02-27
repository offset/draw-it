#include <iostream>
#include "game.hpp"
#include "errcodes.hpp"
#include "play.hpp"

const float Game::maxVelX = 50.f;
const float Game::maxVelY = -500.f;
const float Game::maxVelYFall = 50.f;

Game::Game() : window(sf::VideoMode(800, 600), "Draw it!"), timePerFrame(sf::seconds(1.f/30.f)), 
    view(sf::Vector2f(player.getPosition().x, player.getPosition().y), static_cast<sf::Vector2f>(window.getSize())),
    isMovingLeft(false), isMovingRight(false), jumps(false), isJumping(false),
    playerTexture(), playerSprite(), tileSize(5,5), velocity(0,0)
{
    if(!playerTexture.loadFromFile(Play::getInstance()->getPlayerTexture()))
    {
        std::cerr << "Could not load texture file." << std::endl
                  << "Now exiting." << std::endl;
        exit(-1);
    }
    playerSprite.setTexture(playerTexture);
    player.setPosition((Play::getInstance()->getLevelMap()[0].size()*tileSize.x)/8, (Play::getInstance()->getLevelMap().size()*tileSize.x)/8);
    player.setBoundingBox(sf::Rect<int>(player.getPosition().x, player.getPosition().y, playerTexture.getSize().x, playerTexture.getSize().y));
    playerSprite.setPosition(player.getPosition().x,player.getPosition().y);
    playerSize = playerTexture.getSize();
    view.zoom(1.5f);
    window.setView(view);
}

void Game::processEvents()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handlePlayerInput(event.key.code, false);
                break;
            case sf::Event::Closed:
                std::cerr << "Received close event, window is being closed." << std::endl;
                window.close();
                break;
            default:
                break;
        }
    }
}

void Game::update(sf::Time deltaTime)
{   
    sf::Vector2f gravity(0, 9.81);
    
    if(isMovingLeft)
    {
        velocity.x -= 10.f;
    }
    if(isMovingRight)
    {
        velocity.x += 10.f;
    }
    if(jumps && player.getOnGround())
    {
        velocity.y -= 200.f;
    }
    velocity.y += gravity.y;
    
    // some friction
    velocity = sf::Vector2f(velocity.x*0.95, velocity.y);
    
    // make sure the player does not speed up indefinitely
    if(velocity.x > maxVelX)
    {
        velocity.x = maxVelX;
    }
    if(velocity.y > maxVelYFall)
    {
        velocity.y = maxVelYFall;
    }
    if(velocity.x < -maxVelX)
    {
        velocity.x = -maxVelX;
    }
    if(velocity.y < maxVelY)
    {
        velocity.y = maxVelY;
    }
    
    sf::Vector2f movement(velocity.x * deltaTime.asSeconds(), velocity.y * deltaTime.asSeconds());
    sf::Vector2f desiredPosition(player.getPosition().x + movement.x, player.getPosition().y + movement.y);
    
    // we make sure the player doesn't move out of the level
    if(desiredPosition.x <= 0)
    {
        desiredPosition.x = tileSize.x;
    } else if(desiredPosition.x > Play::getInstance()->getMapSize().x)
    {
        desiredPosition.x = Play::getInstance()->getMapSize().x-1;
    }
    if(desiredPosition.y <= 0)
    {
        desiredPosition.y = tileSize.x;
    } else if(desiredPosition.y > Play::getInstance()->getMapSize().y)
    {
        desiredPosition.y = Play::getInstance()->getMapSize().y-1;
    }
    
    std::vector<std::map<std::string, int> > surroundingTiles = getSurroundingTiles(desiredPosition);
    // update the bounding box with the desired movement
    player.setBoundingBox(sf::Rect<int>(desiredPosition.x, desiredPosition.y, player.getBoundingBox().width, player.getBoundingBox().height));
    
    player.setOnGround(false);
    
    for(int i = 0; i < surroundingTiles.size(); ++i)
    {
        sf::Rect<int> boundingBox = player.getBoundingBox();
        
        int gid = surroundingTiles[i].find("gid")->second;
        
        if(gid != 0)
        {
            sf::Rect<int> tileRect(surroundingTiles[i].find("x")->second, surroundingTiles[i].find("y")->second,
                                   tileSize.x, tileSize.y);
            if(boundingBox.intersects(tileRect))
            {
                sf::Rect<int> intersection;
                boundingBox.intersects(tileRect, intersection);
                
                if(i == 0)
                {
                    // tile is under the player
                    desiredPosition = sf::Vector2f(desiredPosition.x, desiredPosition.y - intersection.height);
                    velocity = sf::Vector2f(velocity.x, 0.f);
                    player.setOnGround(true);
                } else if(i == 1)
                {
                    // tile is over the player
                    desiredPosition = sf::Vector2f(desiredPosition.x, desiredPosition.y + intersection.height*5);
                    velocity = sf::Vector2f(velocity.x, 0.f);
                } else if(i == 2)
                {
                    // tile is left of the player
                    desiredPosition = sf::Vector2f(desiredPosition.x + intersection.width*5, desiredPosition.y);
                } else if(i == 3)
                {
                    // tile is right of the player
                    desiredPosition = sf::Vector2f(desiredPosition.x - intersection.width*2, desiredPosition.y);
                    // enables wall-jump
                    player.setOnGround(true);
                } else
                {
                    if(intersection.width > intersection.height)
                    {
                        // tile is diagonal, but resolving collision vertically
                        velocity = sf::Vector2f(velocity.x, 0.f);
                        int intersectionHeight;
                        if(i > 5)
                        {
                            intersectionHeight = intersection.height;
                            player.setOnGround(true);
                        } else
                        {
                            intersectionHeight = -intersection.height;
                        }
                        desiredPosition = sf::Vector2f(desiredPosition.x, desiredPosition.y + intersection.height);
                    } else
                    {
                        // tile is diagonal, but resolving collision horizontally
                        int intersectionWidth;
                        if(i == 6 || i == 4)
                        {
                            intersectionWidth = intersection.width;
                        } else
                        {
                            intersectionWidth = -intersection.width;
                        }
                        desiredPosition = sf::Vector2f(desiredPosition.x + intersection.width, desiredPosition.y);
                    }
                }
            }
        }
    }
    
    player.setPosition(desiredPosition);
    
    playerSprite.setPosition(player.getPosition().x, player.getPosition().y);
    
    view.setCenter(player.getPosition().x, player.getPosition().y);
    
    // debugging
    // bottleneck, slows down program significantly
//    std::cerr << "Velocity" << velocity.x << " " << velocity.y << std::endl;
//    std::cerr << "Movement" << movement.x << " " << movement.y << std::endl;

}

void Game::render()
{
    // calculating fps
//    float fpsVal = 1000/(timeSinceLastFrame.asSeconds());
//    fps.setString(std::to_string(fpsVal));
    
    window.clear();
    
    window.setView(view);
    
    window.draw(Play::getInstance()->getTileMap());
    
    window.draw(playerSprite);
    
//    window.draw(fps);
    
    window.display();
}

int Game::run()
{   
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    // closing the window terminates the application
    while(window.isOpen())
    {
        timeSinceLastUpdate += clock.restart();
        while(timeSinceLastUpdate > timePerFrame)
        {
            timeSinceLastUpdate -= timePerFrame;
            processEvents();
            update(timePerFrame);
        }
        render();
    }
    
    return SUCCESS;
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
    switch(key)
    {
        case sf::Keyboard::A:
            isMovingLeft = isPressed;
            break;
        case sf::Keyboard::D:
            isMovingRight = isPressed;
            break;
        case sf::Keyboard::Space:
            jumps = isPressed;
            break;
        // resets the player to the initial position
        case sf::Keyboard::R:
            player.setPosition((Play::getInstance()->getLevelMap()[0].size()*tileSize.x)/8, (Play::getInstance()->getLevelMap().size()*tileSize.x)/8);
            playerSprite.setPosition(player.getPosition().x, player.getPosition().y);
            break;
        case sf::Keyboard::Escape:
            window.close();
        default:
            break;
    }
}

sf::RenderWindow& Game::getRenderWindow()
{
    return(window);
}

int Game::getStartingPosition()
{
    for(uint i = 0; i < Play::getInstance()->getPhysicsMap().size(); ++i)
    {
        for(uint j = 0; j < Play::getInstance()->getPhysicsMap().size(); ++j)
        {
            if(Play::getInstance()->getPhysicsMap()[j][i] == 1)
            {
                player.setPosition(i+10*tileSize.x, j-10*tileSize.y);
                return 1;
            }
        }
    }
    return -1;
}

std::vector<std::map<std::string, int> > Game::getSurroundingTiles(sf::Vector2f position)
{
    // converting the pixel coordinates to tilemap coordinates
    sf::Vector2u pPos(static_cast<int>(position.x/tileSize.x), static_cast<int>(position.y/tileSize.y));
    std::vector<std::map<std::string, int> > surroundingTiles;
    
    for(int i = 0; i < 9; ++i)
    {
        // calculating the relative position of the surrounding tile(s)
        int c = i % 3;
        int r = static_cast<int>(i/3);
        // we subtract 1 to place the player in the middle of the 3x3 grid
        sf::Vector2u tilePos(pPos.x + (c - 1), pPos.y + (r - 1));
        
        // this tells us what kind of block this tile is
        int tGid = Play::getInstance()->getPhysicsMap()[tilePos.y][tilePos.x];
        
        // converts the coords from tile to world coords
        sf::Vector2u tileRect(tilePos.x*tileSize.x, tilePos.y*tileSize.y);
        
        // storing all the information
        std::map<std::string, int> tileDict;
        tileDict.insert(std::make_pair("gid", tGid));
        tileDict.insert(std::make_pair("x", tileRect.x));
        tileDict.insert(std::make_pair("y", tileRect.y));
        tileDict.insert(std::make_pair("tileX", tilePos.x));
        tileDict.insert(std::make_pair("tileY", tilePos.y));
        
        // adding the stored information to our vector
        surroundingTiles.push_back(tileDict);
    }
    
    // we remove the tile in the middle (where the player is) because there are no collisions possible
    surroundingTiles.erase(surroundingTiles.begin()+4);
    // we reorganise the order to check for adjacent tiles (above, below, left, right) first
    // inserting 2 at 6 and deleting 2 afterwards
    surroundingTiles.insert(surroundingTiles.begin()+6, surroundingTiles[2]);
    surroundingTiles.erase(surroundingTiles.begin()+2);
    // exchanging 4 with 6 (hopefully)
    surroundingTiles.insert(surroundingTiles.begin()+6, surroundingTiles[4]);
    surroundingTiles.insert(surroundingTiles.begin()+4, surroundingTiles[7]);
    surroundingTiles.erase(surroundingTiles.begin()+5);
    surroundingTiles.erase(surroundingTiles.begin()+7);
    // exchanging 0 with 4
    surroundingTiles.insert(surroundingTiles.begin()+4, surroundingTiles[0]);
    surroundingTiles.insert(surroundingTiles.begin()+0, surroundingTiles[5]);
    surroundingTiles.erase(surroundingTiles.begin()+1);
    surroundingTiles.erase(surroundingTiles.begin()+5);
    
    /*
     * 4 | 1 | 5
     * --  -- --
     * 2 | / | 3
     * --  -- --
     * 6 | 0 | 7
     *
     */
    return surroundingTiles;
}

template<typename T>
sf::Vector2<T> operator-(sf::Vector2<T> &vec1, sf::Vector2<T> &vec2)
{
    return sf::Vector2<T>(vec1.x-vec2.x, vec1.y-vec2.y);
}
