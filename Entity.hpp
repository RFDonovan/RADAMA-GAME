#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED
#include<SFML/Graphics.hpp>
#include<Box2D/Box2D.h>
#include<iostream>
#include "Globals.h"
#include "AnimatedSprite.hpp"
#include "TextureHolder.h"

class Entity {

public:
    enum Type{
        Player,
        Other,
    };
    //class member variables

    b2Body* m_body;
    float   m_radius;
    Type    kind;

    //animation control
    sf::Texture* texture;
    bool noKeyWasPressed = true;
    b2World *p_world;
    b2Vec2 vel;

    sf::Clock frameClock;
    Animation* currentAnimation;
    Animation walkingAnimationLeft, walkingAnimationRight;
    float desiredVel;
    AnimatedSprite animatedSprite;

public:
            Entity(b2World* world, TextureHolder* Textures, float radius, float32 x, float32 y, float w, float h);
    void    loadPlayerSprite(TextureHolder* Textures);
    void    processLogic();
    void    render(sf::RenderWindow& mWindow,sf::Time frameTime, TextureHolder* Textures);
    void    onCommand(sf::Event e);
    int     getY();
    bool    isGrounded();

    void    startContact();
    void    endContact();


            ~Entity();
};

#endif // ENTITY_HPP_INCLUDED
