#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED
#include<SFML/Graphics.hpp>
#include<Box2D/Box2D.h>
#include "Globals.h"

class Entity {

public:
    //class member variables
    b2Body* m_body;
    float   m_radius;

public:
            Entity(b2World* world, float radius, float32 x, float32 y);
    void    render();
            //~Entity();
};

#endif // ENTITY_HPP_INCLUDED
