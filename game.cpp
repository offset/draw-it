#include <iostream>
#include "game.hpp"
#include "errcodes.hpp"
#include "play.hpp"

Game::Game() : window(sf::VideoMode(640,480), "Draw it!"), timePerFrame(sf::seconds(1.f/60.f)), 
    playerTexture(), playerSprite(), view(sf::Vector2f(player.getPosition().x, player.getPosition().y), static_cast<sf::Vector2f>(window.getSize()))
{
    assert(playerTexture.loadFromFile("../drawit/assets/textures/player/dummyPlayer.png"));
    playerSprite.setTexture(playerTexture);
    playerSprite.setPosition(player.getPosition().x, player.getPosition().y);
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
    sf::Vector2f movement(0.f, 0.f);
    
    // the y axis starts from the top, so we need to decrease the y value when we move up
    if(isMovingUp)
    {
        movement.y -= 1.f;
    }
    if(isMovingDown)
    {
        movement.y += 1.f;
    }
    if(isMovingLeft)
    {
        movement.x -= 1.f;
    }
    if(isMovingRight)
    {
        movement.x += 1.f;
    }
    
    player.move(movement * deltaTime.asSeconds());
    
    playerSprite.setPosition(player.getPosition().x, player.getPosition().y);
    
    view.setCenter(player.getPosition().x, player.getPosition().y);
    
    std::cerr << movement.x << " " << movement.y << std::endl;
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
        case sf::Keyboard::W:
            isMovingUp = isPressed;
            break;
        case sf::Keyboard::S:
            isMovingDown = isPressed;
            break;
        case sf::Keyboard::A:
            isMovingLeft = isPressed;
            break;
        case sf::Keyboard::D:
            isMovingRight = isPressed;
            break;
        default:
            break;
    }
}

sf::RenderWindow& Game::getRenderWindow()
{
    return(window);
}
