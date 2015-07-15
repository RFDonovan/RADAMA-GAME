#ifndef HUMAN_HPP_INCLUDED
#define HUMAN_HPP_INCLUDED
#include "Entity.hpp"

class Human: public Entity
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
    sf::Clock clock, lifeClock;
    sf::Texture *lifeTex, *deathTex;
    sf::Sprite lifeSprite, deathSprite; ///red and green

public:
            Human(sf::RenderWindow& mWindow, b2World* world, TextureHolder* Textures, float radius, std::vector<bodyData> *bDList, std::map<std::string, b2Joint*> *jMap);
    void    loadSprite(TextureHolder* Textures);
    void    render(sf::RenderWindow& mWindow,sf::Time frameTime, TextureHolder* Textures);
    int     getClassName(){return HUMAN;}
//    void    drawLife(sf::RenderWindow& mWindow);

};



#endif // HUMAN_HPP_INCLUDED
