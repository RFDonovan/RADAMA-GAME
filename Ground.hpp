#ifndef GROUND_HPP_INCLUDED
#define GROUND_HPP_INCLUDED

#include<SFML/Graphics.hpp>
#include<Box2D/Box2D.h>
#include<iostream>
#include "Globals.h"
//#include "AnimatedSprite.hpp"
#include "TextureHolder.h"

class Ground
{
public:
    enum Type{
        Grass,
        Water,
        Default,
    };

    b2World*    p_world;
    b2Body*     m_body;
    Type        kind;
    sf::Texture*    Texture;
    sf::Sprite      groundSprite;
    sf::Vector2f    groundSize;
    sf::Vector2f    groundPosition;

public:

            Ground(b2World* world, TextureHolder* Textures, float32 x, float32 y, float w = 600.f , float h = 16.f);
    void    loadSprites(TextureHolder* Textures);
    void    render(sf::RenderWindow& mWindow);

    int     getY();
    int     getX();

    void    startContact();
    void    endContact();

            ~Ground();

};

#endif // GROUND_HPP_INCLUDED
