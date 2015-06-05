#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED
#include <random>
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
        Human,
        Other,
    };
    //class member variables

    b2Body* m_body;
    float   m_radius;
    Type    kind;

    sf::RenderWindow& mWindow;

    //animation control
    sf::Texture* texture;
    TextureHolder* textureHolder;

    bool grounded = false;
    int  nb_contacts = 0;
    b2World *p_world;
    b2Vec2 vel;
    b2Fixture   *basFixture;
    int jump = 0;

    sf::Clock frameClock;
    Animation* currentAnimation;
    Animation walkingAnimationLeft, walkingAnimationRight;
    AnimatedSprite animatedSprite;

    sf::Vector2f mouseInit, mousePos, playerPos, velocityForce;

    float velocityLimit = 5.f;
    float jumpLimit = 5.f;

    sf::Clock clock;

public:
            Entity(sf::RenderWindow& mWindow, b2World* world, TextureHolder* Textures, float radius, float32 x, float32 y, float w, float h);
    //void    loadPlayerSprite(TextureHolder* Textures);
    //void    processLogic();
    //void    processLogic(sf::RenderWindow& mWindow);
    //void    render(sf::RenderWindow& mWindow,sf::Time frameTime, TextureHolder* Textures){}
    //void    onCommand(sf::Event e);
    int     getY();
    int     getX();
    bool    isGrounded();
    b2Vec2  getVelocity();

    void    startContact(b2Fixture   *fixture);
    void    endContact(b2Fixture   *fixture);

    //void    attachStuff(sf::Shader* shader, TextureHolder::TexName tex);


            ~Entity();
};

#endif // ENTITY_HPP_INCLUDED
