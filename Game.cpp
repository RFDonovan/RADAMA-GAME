#include "Game.h"

Game::Game()
: mWindow(sf::VideoMode(WINDOW_W, WINDOW_H), "Prototype RADAMA", sf::Style::Close)
, mWorld (mWindow)
//, mPlayer()
{
    mWindow.setKeyRepeatEnabled(false);
    //blabla sur les position des statistictext

}

void Game::run()
{
    sf::Clock frameClock;
    //sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen()){
        processInput();
        sf::Time frameTime = frameClock.restart();
        render(frameTime);
    }

}

void Game::processInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            mWindow.close();
    }
    mWorld.processInput(event);

}


void Game::render(sf::Time frameTime)
{
    mWindow.clear(sf::Color::White);
    mWorld.draw(frameTime);
    mWindow.display();
}

