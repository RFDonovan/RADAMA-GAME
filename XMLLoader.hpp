#ifndef XMLLOADER_HPP_INCLUDED
#define XMLLOADER_HPP_INCLUDED

#include "Globals.h"
#include "Structures.hpp"
#include<SFML/Graphics.hpp>
#include<Box2D/Box2D.h>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include <sstream>
#include <string>
#include <map>

//#include "AnimatedSprite.hpp"
#include "TextureHolder.h"
#include "pugixml.hpp"
#include "Item.hpp"

class XMLLoader : public ObjectType
{
public:
    b2World*                    p_world;
    pugi::xml_document          XMLDocument;

    std::vector<sf::Sprite>     spriteList;
    std::vector<sf::IntRect>     spriteRectList;
    std::vector<sf::Texture>    texList;
    std::vector<b2Body*>        bodyList;
    std::vector<b2Vec2>         ratioList;
    std::vector<float>          angleRatioList;
    std::map<std::string, b2Body*>  nameToBody;
    std::map<std::string, sf::Sprite*>  nameToSprite;
    std::map<std::string, b2Joint*> jointMap;

    std::map<std::string, int>  attributeMap;
    sf::Clock randomClock;

public:
        XMLLoader(b2World* world);

    std::vector<Item*>    loadItems(std::string dir);

    std::vector<bodyData>        loadXML(std::string XMLFile, std::string dir = "./Resources/");
    b2Body*     createBody(int bodyType, pugi::xml_node bodyNode);
    std::map<std::string, b2Fixture*>         addFixtures(b2Body* body, pugi::xml_node bodyNode);
    //std::vector<b2Fixture*>         addFixtures(b2Body* body, pugi::xml_node bodyNode);
    std::vector<b2Fixture*>         createEdgeShape(b2Body* body, pugi::xml_node fixtureNode);
    b2Fixture*  createPolygonShape(b2Body* body, pugi::xml_node fixtureNode);
    b2Fixture*  createRectangleShape(b2Body* body, pugi::xml_node fixtureNode);
    b2Fixture*  createCircleShape(b2Body* body, pugi::xml_node fixtureNode);

    b2Joint*    addWeldJoint(pugi::xml_node jointNode);
    b2Joint*    addRevJoint(pugi::xml_node jointNode);

    std::map<std::string, b2Joint*> GetCurrentJointMap();

    sf::Sprite*        loadImage(std::string imageName, std::string dir);

    void        render(sf::RenderWindow& mWindow, sf::Shader* shader);
    void        move(float x, float y);

    std::vector<sf::IntRect>    getSpriteRectList(){return spriteRectList;}
    std::map<std::string, sf::Sprite*>    getNameToSpriteList(){return nameToSprite;}



    int     getObjectType()
    {
        return GROUND;
    }
    int     getClassName()
    {
        return GROUND;
    }
    //void        createEntity(std::string);
    //void        loadMachine(std::string);
};


#endif // XMLLOADER_HPP_INCLUDED
