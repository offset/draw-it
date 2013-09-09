#include <iostream>
#include "game.hpp"

Game::Game() : window(sf::VideoMode(640,480), "Draw it!"), timePerFrame(sf::seconds(1.f/60.f)), playerTexture(), playerSprite()
{
    if (!playerTexture.loadFromFile("assets/p1_front.png"));
    {
        std::cerr << "Could not load texture files for the player." << std::endl
                  << "Now exiting." << std::endl;
        exit();
    }
    playerSprite.setTexture(playerTexture);
    playerSprite.setPosition(player.position.x, player.position.y);
}

Game::processEvents()
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
        }
    }
}

Game::update(sf::Time deltaTime)
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
}

Game::render()
{
    window.clear();
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
    }
}
