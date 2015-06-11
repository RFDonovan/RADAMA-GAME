#ifndef LEVEL_HPP_INCLUDED
#define LEVEL_HPP_INCLUDED

#include "Globals.h"
#include<SFML/Graphics.hpp>
#include<Box2D/Box2D.h>
#include<iostream>
#include<string>

//#include "AnimatedSprite.hpp"
#include "TextureHolder.h"
#include "pugixml.hpp"

class GameLevel : public ObjectType
{
public:
    enum Type{
        Grass,
        Water,
        Default,
    };
    Type        kind;
    b2World*    p_world;
    b2Body * mBody;
    pugi::xml_document XMLDocument;


public:

            GameLevel(b2World* world);
    int     loadLevel(std::string filename);
    void    createBody(pugi::xml_node body, pugi::xml_node fixtures);
    void    clearAll();
    void    render(sf::RenderWindow& mWindow);
    void    render(sf::RenderWindow& mWindow, sf::Shader* shader);

    int     getObjectType(){return GROUND;}
    int     getClassName(){return GROUND;}

    void    startContact(b2Fixture   *fixture);
    void    endContact(b2Fixture   *fixture);
            ~GameLevel();

};

#endif // LEVEL_HPP_INCLUDED
