#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "Entity.hpp"

class Player: public Entity
{

public:


    //animation control

    sf::Sprite stuffSprite;
    sf::Shader *Shader;

    bool noKeyWasPressed = true;
    float desiredVel;
    float jumpLimit = 5.f;

    float distortionFactor = .2f;
    float riseFactor = .5f;
    sf::Clock clock;

public:
            Player(sf::RenderWindow& mWindow, b2World* world, TextureHolder* Textures, float radius, float32 x, float32 y, float w, float h);
    void    loadPlayerSprite(TextureHolder* Textures);
    void    processLogic();
    void    processLogic(sf::RenderWindow& mWindow);
    void    render(sf::RenderWindow& mWindow,sf::Time frameTime, TextureHolder* Textures);
    void    onCommand(sf::Event e);

};


#endif // PLAYER_H_INCLUDED
