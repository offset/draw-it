#include <iostream>
#include "game.hpp"
#include "errcodes.hpp"
#include "play.hpp"

Game::Game() : window(sf::VideoMode(640,480), "Draw it!"), timePerFrame(sf::seconds(1.f/30.f)), 
    view(sf::Vector2f(player.getPosition().x, player.getPosition().y), static_cast<sf::Vector2f>(window.getSize())),
    isMovingLeft(false), isMovingRight(false), isJumping(false), hasBeenJumping(false),
    playerTexture(), playerSprite()
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
    
    sf::Vector2f movement(velocity.x * deltaTime.asSeconds(), velocity.y * deltaTime.asSeconds());
    
    if(isValidLocation(player.getPosition().x + movement.x, player.getPosition().y + movement.y))
    {
        player.move(movement);
    }
    
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
        case sf::Keyboard::Space:
            isJumping = isPressed;
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
