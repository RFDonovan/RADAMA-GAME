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

public:
            Entity(b2World* world, float radius, float32 x, float32 y);
    void    render(sf::RenderWindow& mWindow, TextureHolder* Textures);
    void    onCommand(sf::Event e);
            //~Entity();
};

#endif // ENTITY_HPP_INCLUDED
