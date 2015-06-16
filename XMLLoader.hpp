#ifndef XMLLOADER_HPP_INCLUDED
#define XMLLOADER_HPP_INCLUDED

#include "Globals.h"
#include<SFML/Graphics.hpp>
#include<Box2D/Box2D.h>
#include<iostream>
#include <sstream>
#include <string>

//#include "AnimatedSprite.hpp"
#include "TextureHolder.h"
#include "pugixml.hpp"

class XMLLoader
{
public:
    b2World*                    p_world;
    pugi::xml_document          XMLDocument;
    std::map<std::string, int>  attributeMap;

public:
                XMLLoader(b2World* world);

    void        loadEntity(std::string);
    b2Body*     createBody(int bodyType, pugi::xml_node bodyNode);
    std::vector<b2Fixture*>         addFixtures(b2Body* body, pugi::xml_node bodyNode);
    std::vector<b2Fixture*>         createEdgeShape(b2Body* body, pugi::xml_node fixtureNode);
    b2Fixture*   createPolygonShape(b2Body* body, pugi::xml_node fixtureNode);
    b2Fixture*   createCircleShape(b2Body* body, pugi::xml_node fixtureNode);
    //void        createEntity(std::string);
    //void        loadMachine(std::string);
};


#endif // XMLLOADER_HPP_INCLUDED
