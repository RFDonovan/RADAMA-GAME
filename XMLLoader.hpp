#ifndef XMLLOADER_HPP_INCLUDED
#define XMLLOADER_HPP_INCLUDED

#include "Globals.h"
#include "Structures.hpp"
#include<SFML/Graphics.hpp>
#include<Box2D/Box2D.h>
#include<iostream>
#include <sstream>
#include <string>

//#include "AnimatedSprite.hpp"
#include "TextureHolder.h"
#include "pugixml.hpp"

class XMLLoader : public ObjectType
{
public:
    b2World*                    p_world;
    pugi::xml_document          XMLDocument;

    std::vector<sf::Sprite>     spriteList;
    std::vector<sf::Texture>    texList;
    std::vector<b2Body*>        bodyList;
    std::vector<b2Vec2>         ratioList;

    std::map<std::string, int>  attributeMap;

public:
                XMLLoader(b2World* world);

    bodyData*        loadXML(std::string);
    b2Body*     createBody(int bodyType, pugi::xml_node bodyNode);
    std::vector<b2Fixture*>         addFixtures(b2Body* body, pugi::xml_node bodyNode);
    std::vector<b2Fixture*>         createEdgeShape(b2Body* body, pugi::xml_node fixtureNode);
    b2Fixture*  createPolygonShape(b2Body* body, pugi::xml_node fixtureNode);
    b2Fixture*  createCircleShape(b2Body* body, pugi::xml_node fixtureNode);

    sf::Sprite*        loadImage(std::string imageName);
    void        render(sf::RenderWindow& mWindow, sf::Shader* shader);


    int     getObjectType(){return GROUND;}
    int     getClassName(){return GROUND;}
    //void        createEntity(std::string);
    //void        loadMachine(std::string);
};


#endif // XMLLOADER_HPP_INCLUDED
