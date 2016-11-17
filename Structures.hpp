#ifndef STRUCTURES_HPP_INCLUDED
#define STRUCTURES_HPP_INCLUDED
#include<SFML/Graphics.hpp>
#include<Box2D/Box2D.h>
#include<iostream>
#include<string>
#include<map>

struct bodyData{

    std::map<std::string, b2Fixture*>   mapFixture;
    b2Body * body;
    std::string name;
    sf::Sprite* sprite;
    std::map<std::string, b2Joint*> jointMap;
    //int     type;
};



#endif // STRUCTURES_HPP_INCLUDED
