#include "World.h"

World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
//, mPlayerPosition(mWorldView.getSize().x/2.f, )
{
    loadTextures();
    buildScene();

    //mWorldView.setCenter(mPlayerPosition);

}

void World::update()
{
    sf::Vector2f playerPosition(0.0f,0.0f);
    mWorldView.move(playerPosition);
    adaptViewToPlayer();

    ///depiler commandes
    /*while(!mCommandQueue.isEmpty())
        player.onCommand(mCommandQueue.pop(), dt);
    */

}

void World::draw()
{
    mWindow.setView(mWorldView);
    /*mWindow.draw(sprites);*/
    /*mWincow.draw(player)*/
}

void World::loadTextures()
{
    /**Load the animated sprites & more**/
    //mTextures.load(Textues::Radama, "/path/to/tex")

}

void World::buildScene()
{
    /**prepare layers*/
    /**add background*/
    /**add player*/
}

void World::adaptViewToPlayer()
{
    /**player always at 2/3 of scren*/
    /**except at the begining or at the end of the level*/
}
