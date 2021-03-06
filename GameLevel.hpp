#ifndef GAMELEVEL_HPP_INCLUDED
#define GAMELEVEL_HPP_INCLUDED

#include "Globals.h"
#include<SFML/Graphics.hpp>
#include<Box2D/Box2D.h>
#include<iostream>
#include <sstream>
#include <string>

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
    b2Body *    mBody;
    pugi::xml_document  XMLDocument;

    std::vector<sf::Sprite>     spriteList;
    std::vector<sf::Texture>    texList;
    std::vector<b2Body*>        bodyList;
    std::vector<b2Vec2>         ratioList;
    std::map<sf::Sprite, sf::Texture>   Tex;



public:

            GameLevel(b2World* world);
    void    loadSprites();
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
