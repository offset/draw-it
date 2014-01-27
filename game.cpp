#include <iostream>
#include "game.hpp"
#include "errcodes.hpp"
#include "play.hpp"
#define COLLISIONDETECTION1

Game::Game() : window(sf::VideoMode(640,480), "Draw it!"), timePerFrame(sf::seconds(1.f/30.f)), 
    view(sf::Vector2f(player.getPosition().x, player.getPosition().y), static_cast<sf::Vector2f>(window.getSize())),
    isMovingLeft(false), isMovingRight(false), isMovingUp(false), isMovingDown(false),/*isJumping(false), hasBeenJumping(false),*/
    playerTexture(), playerSprite(), gravity(9.81) /*timeJumping(0)*/
{
    if(!playerTexture.loadFromFile("playertexture.png"))
    {
        std::cerr << "Could not load texture file." << std::endl
                  << "Now exiting." << std::endl;
        exit(-1);
    }
    playerSprite.setTexture(playerTexture);
    
    if(getStartingPosition() != 1)
    {
        std::cerr << "Could not calculate starting position: " << std::endl
                  << std::cerr << player.getPosition().x << " " << player.getPosition().y << std::endl;
    }
    player.setPosition(0,0);
    playerSprite.setPosition(0,0);
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
    sf::Vector2f velocity(0.f, 0.f);
    
    if(isMovingLeft)
    {
        velocity.x -= 100.f;
    }
    if(isMovingRight)
    {
        velocity.x += 100.f;
    }
    if(isMovingUp)
    {
        velocity.y -= 100.f;
    }
    if(isMovingDown)
    {
        velocity.y += 100.f;
    }
    
    sf::Vector2f movement(velocity.x * deltaTime.asSeconds(), velocity.y * deltaTime.asSeconds());
    
    

#ifdef COLLISIONDETECTION1
    // collision detection
    // we first check the x-Axis
    // determining the side which faces the moving direction
    int coordFwdFEdgeX;
    if (movement.x < 0)
    {
        coordFwdFEdgeX = player.getPosition().x;
    } else
    {
        coordFwdFEdgeX = player.getPosition().x + playerSize.x;
    }
    // with which line(s) of tiles does the player collide?
    std::vector<int> collidingLinesX;
    collidingLinesX.push_back(player.getPosition().y/5);
    collidingLinesX.push_back((player.getPosition().y-playerSize.y)/5);
    // we scan along these lines for obstacles
    int closestObstacleX = 30111995;
    if(movement.x < 0)
    {
        for(uint i = 0; i < collidingLinesX.size(); ++i)
        {
            for(int x =(coordFwdFEdgeX+movement.x)/5; x < (coordFwdFEdgeX/5); ++x)
            {
                if(Play::getInstance()->getPhysicsMap()[collidingLinesX[i]][x] != 0)
                {
                    closestObstacleX = x*5;
                }
            }
        }
        if(closestObstacleX != 30111995)
        {
            movement.x = -(coordFwdFEdgeX - closestObstacleX);
        }
    } else
    {
        for(uint i = 0; i < collidingLinesX.size(); ++i)
        {
            for(int x = (coordFwdFEdgeX+movement.x)/5; x > (coordFwdFEdgeX/5); --x)
            {
                if(Play::getInstance()->getPhysicsMap()[collidingLinesX[i]][x] != 0)
                {
                    closestObstacleX = x*5;
                }
            }
        }
        if(closestObstacleX != 30111995)
        {
            movement.x = closestObstacleX - coordFwdFEdgeX;
        }
    }
    // now the y-axis
    // determining the side which faces the moving direction
    int coordFwdFEdgeY;
    if(movement.y < 0)
    {
        coordFwdFEdgeY = player.getPosition().y;
    } else
    {
        coordFwdFEdgeY = player.getPosition().y + playerSize.y;
    }
    // with which line(s) of tiles does the player collide?
    std::vector<int> collidingLinesY;
    collidingLinesY.push_back(player.getPosition().x/5);
    collidingLinesY.push_back((player.getPosition().x-playerSize.x)/5);
    int debugPlayerX = player.getPosition().x/5;
    int debugPlayerY = (player.getPosition().x-playerSize.x)/5;
    // we scan along these lines for obstacles
    int closestObstacleY = 30111995;
    if(movement.y < 0)
    {
        for(int i = 0; i < collidingLinesY.size(); ++i)
        {
            for(int y = (coordFwdFEdgeY+movement.y)/5; y < (coordFwdFEdgeY/5); ++y)
            {
                if(Play::getInstance()->getPhysicsMap()[y][collidingLinesY[i]] != 0)
                {
                    closestObstacleY = y*5;
                }
            }
        }
        if(closestObstacleY != 30111995)
        {
            movement.y = closestObstacleY - coordFwdFEdgeY;
        }
    } else
    {
        for(int i = 0; i < collidingLinesY.size(); ++i)
        {
            for(int y = (coordFwdFEdgeY+movement.y)/5; y > (coordFwdFEdgeX/5); --y)
            {
                if(Play::getInstance()->getPhysicsMap()[y][collidingLinesY[i]] != 0)
                {
                    closestObstacleY = y*5;
                }
            }
        }
        if(closestObstacleY != 30111995)
        {
            movement.y = -(coordFwdFEdgeY - closestObstacleY);
        }
    }
#endif
    
    player.move(movement);
    
#ifdef COLLISIONDETECTION2
    for(int x = 0; x < abs(movement.x); ++x)
    {
        for(int y = 0; y < abs(movement.y); ++y)
        {
            int xTile = x/5;
            int yTile = y/5;
        }
    }
    if(isValidLocation(player.getPosition().x + movement.x, player.getPosition().y + movement.y))
    {
        player.move(movement);
    }
#endif
    
    playerSprite.setPosition(player.getPosition().x, player.getPosition().y);
    
    view.setCenter(player.getPosition().x, player.getPosition().y);
    
    // debugging
    std::cerr << "Velocity" << velocity.x << " " << velocity.y << std::endl;
    std::cerr << "Movement" << movement.x << " " << movement.y << std::endl;
//    if(isMovingLeft)
//    {
//        velocity.x -= 1.f * 5.f;
//    }
//    if(isMovingRight)
//    {
//        velocity.x += 1.f * 5.f;
//    }
//    if(isJumping && !hasBeenJumping)
//    {
//        velocity.y -= 3.f * 5.f;
//    }
    
//    if((player.getPosition().x + velocity.x) >= Play::getInstance()->getMapSize().x
//            /*|| Play::getInstance()->getPhysicsMap()[player.getPosition().y/5][player.getPosition().x/5]*/)
//    {
//        velocity.x = Play::getInstance()->getMapSize().x-1;
//    }
//    if((player.getPosition().x + velocity.x) <= 0)
//    {
//        velocity.x = 1;
//    }
//    if((player.getPosition().y + velocity.y) >= Play::getInstance()->getMapSize().y)
//    {
//        velocity.y = Play::getInstance()->getMapSize().y-1;
//    }
//    if((player.getPosition().y + velocity.y) <= 0)
//    {
//        velocity.y = 1;
//    }
    
//    // gravity
//    velocity.y += 9.81;
    
//    player.move(velocity * deltaTime.asSeconds());
    
//    view.setCenter(player.getPosition().x, player.getPosition().y);
    
//    // debugging
//    //std::cerr << velocity.x << " " << velocity.y << std::endl;
}

void Game::render()
{
    window.clear();
    
    window.setView(view);
    
    window.draw(Play::getInstance()->getTileMap());
    
    window.draw(playerSprite);
    
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
        case sf::Keyboard::W:
            isMovingUp = isPressed;
            break;
        case sf::Keyboard::S:
            isMovingDown = isPressed;
//        case sf::Keyboard::Space:
//            if (!hasBeenJumping)
//            {
//                isJumping = isPressed;
//            }
            //break;
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
                player.setPosition(i+1*5, j-1*5);
                return 1;
            }
        }
    }
    return -1;
}

bool Game::isValidLocation(float x, float y)
{
    int nx = player.getPosition().x + x;
    int ny = player.getPosition().y + y;
    if(isBlocked(nx-playerSize.x, ny-playerSize.y))
    {
        return false;
    }
    if(isBlocked(nx+playerSize.x, ny-playerSize.y))
    {
        return false;
    }
    if(isBlocked(nx-playerSize.x, ny+playerSize.y))
    {
        return false;
    }
    if(isBlocked(nx+playerSize.x, ny+playerSize.y))
    {
        return false;
    }
    
    return true;
}

bool Game::isBlocked(float x, float y)
{
    return !(x < 0 || x > Play::getInstance()->getMapSize().x
            || y < 0 || Play::getInstance()->getMapSize().y
            || Play::getInstance()->getPhysicsMap()[y][x] == 1);
}
