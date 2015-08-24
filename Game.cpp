#include "Game.h"

Game::Game()
: mWindow(sf::VideoMode(WINDOW_W, WINDOW_H), "Prototype RADAMA", sf::Style::Close)
, mWorld (mWindow)
//, mPlayer()
{

    mWindow.setKeyRepeatEnabled(false);
    //blabla sur les position des statistictext
//    gui.setWindow(mWindow);
//
//    tgui::Button::Ptr button(gui);
//    button->load("widgets/Black.conf");
//    button->setSize(260, 60);
//    button->setPosition(270, 440);
//    button->setText("Login");
//    button->bindCallback(tgui::Button::LeftMouseClicked);
//    button->setCallbackId(1);
//
//    tgui::Picture::Ptr picture(gui);
//    picture->load("background.png");
//    picture->setSize(800, 600);

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
//        gui.handleEvent(event);
    }
    mWorld.processInput(event);

//
//    tgui::Callback callback;
//    while(gui.pollCallback(callback))
//    {
//        if(callback.id == 1)
//        {
//            exit(-1);
//        }
//    }


}

void Game::update()
{
    mWorld.update();
}

void Game::render(sf::Time frameTime)
{

    mWindow.clear(sf::Color::White);
    mWorld.draw(frameTime);

//    gui.draw();
    mWindow.display();
}

