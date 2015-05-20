#include "Game.h"

Game::Game()
: mWindow(sf::VideoMode(800, 600), "Prototype RADAMA", sf::Style::Close)
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
        update();
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
}

void Game::update()
{
    mWorld.update();
}

void Game::render(sf::Time frameTime)
{
    mWindow.clear(sf::Color::Blue);
    mWorld.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.display();
}
