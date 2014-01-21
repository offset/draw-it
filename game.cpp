#include <iostream>
#include "game.hpp"
#include "errcodes.hpp"
#include "play.hpp"

Game::Game() : window(sf::VideoMode(640,480), "Draw it!"), timePerFrame(sf::seconds(1.f/30.f)), 
    view(sf::Vector2f(player.getPosition().x, player.getPosition().y), static_cast<sf::Vector2f>(window.getSize())),
    //miniMap(sf::Vector2f(window.getSize().x/2, window.getSize().y/2), static_cast<sf::Vector2f>(window.getSize())),
    isMovingLeft(false), isMovingRight(false), isJumping(false), hasBeenJumping(false)
{
    if(getStartingPosition() != 1)
    {
        std::cerr << "Could not calculate starting position: " << std::endl
                  << std::cerr << player.getPosition().x << " " << player.getPosition().y << std::endl;
    }
    playerVertex.setPrimitiveType(sf::Quads);
    sf::Vertex* quad = &playerVertex[0];
    quad[0] = sf::Vertex(sf::Vector2f(player.getPosition().x, player.getPosition().y), sf::Color::Blue);
    quad[1] = sf::Vertex(sf::Vector2f(player.getPosition().x+5, player.getPosition().y), sf::Color::Blue);
    quad[2] = sf::Vertex(sf::Vector2f(player.getPosition().x, player.getPosition().y+5), sf::Color::Blue);
    quad[3] = sf::Vertex(sf::Vector2f(player.getPosition().x+5, player.getPosition().y+5), sf::Color::Blue);
    view.zoom(1.5f);
    //view.setViewport(sf::FloatRect(0, 0, 1, 1));
    //miniMap.setViewport(sf::FloatRect(0.75f, 0.f, 0.25f, 0.25f));
    window.setView(view);
    //window.setView(miniMap);
//    playerBottom = sf::Vector2u(player.getPosition().x, player.getPosition().y+playerSprite.getTextureRect().height/2);
//    playerUp = sf::Vector2u(player.getPosition().x, player.getPosition().y-playerSprite.getTextureRect().height/2);
//    playerLeft = sf::Vector2u(player.getPosition().x-playerSprite.getTextureRect().width/2, player.getPosition().y);
//    playerRight = sf::Vector2u(player.getPosition().x+playerSprite.getTextureRect().width/2, player.getPosition().y);
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
        velocity.x -= 1.f * 5.f;
    }
    if(isMovingRight)
    {
        velocity.x += 1.f * 5.f;
    }
    if(isJumping && !hasBeenJumping)
    {
        velocity.y -= 3.f * 5.f;
    }
    
    if((player.getPosition().x + velocity.x) >= Play::getInstance()->getMapSize().x
            /*|| Play::getInstance()->getPhysicsMap()[player.getPosition().y/5][player.getPosition().x/5]*/)
    {
        velocity.x = Play::getInstance()->getMapSize().x-1;
    }
    if((player.getPosition().x + velocity.x) <= 0)
    {
        velocity.x = 1;
    }
    if((player.getPosition().y + velocity.y) >= Play::getInstance()->getMapSize().y)
    {
        velocity.y = Play::getInstance()->getMapSize().y-1;
    }
    if((player.getPosition().y + velocity.y) <= 0)
    {
        velocity.y = 1;
    }
    
    // gravity
    velocity.y += 9.81;
    
    player.move(velocity * deltaTime.asSeconds());
    
    sf::Vertex* quad = &playerVertex[0];
    quad[0] = sf::Vertex(sf::Vector2f(player.getPosition().x, player.getPosition().y));
    quad[1] = sf::Vertex(sf::Vector2f(player.getPosition().x+5, player.getPosition().y));
    quad[2] = sf::Vertex(sf::Vector2f(player.getPosition().x, player.getPosition().y+5));
    quad[3] = sf::Vertex(sf::Vector2f(player.getPosition().x+5, player.getPosition().y+5));
    
    view.setCenter(player.getPosition().x, player.getPosition().y);
    
    // debugging
    //std::cerr << velocity.x << " " << velocity.y << std::endl;
}

void Game::render()
{
    window.clear();
    
    window.setView(view);
    
    window.draw(Play::getInstance()->getTileMap());
    
    window.draw(playerVertex);
    
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
    for(int i = 0; i < Play::getInstance()->getPhysicsMap().size(); ++i)
    {
        for(int j = 0; j < Play::getInstance()->getPhysicsMap().size(); ++j)
        {
            if(Play::getInstance()->getPhysicsMap()[j][i] == 0)
            {
                player.setPosition(i+1*5, j-1*5);
                return 1;
            }
        }
    }
    return -1;
}
