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
            Human(sf::RenderWindow& mWindow, b2World* world, TextureHolder* Textures, float radius, std::vector<bodyData> *bDList, std::map<std::string, b2Joint*> *jMap, std::map<std::string, Animation>* animationList);
    void    loadSprite(TextureHolder* Textures);
    void    render(sf::RenderWindow& mWindow,sf::Time frameTime, TextureHolder* Textures, sf::Shader* shader);
    int     getClassName(){return HUMAN;}

    void    doNormalThings();
    void    doAlertThings();
    void    doHuntingThings();
    void    doAttackThings();
    void    doShockedThings();
//    void    drawLife(sf::RenderWindow& mWindow);

};



#endif // HUMAN_HPP_INCLUDED
