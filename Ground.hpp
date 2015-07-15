#ifndef GROUND_HPP_INCLUDED
#define GROUND_HPP_INCLUDED

#include<SFML/Graphics.hpp>
#include<Box2D/Box2D.h>
#include<iostream>
#include "Globals.h"
//#include "AnimatedSprite.hpp"
#include "TextureHolder.h"
#include "Structures.hpp"

class Ground : public ObjectType
{

    int identificationGround = 10001;


public:

            Ground(b2World* world,std::vector<bodyData> *bDList);
    void    startContact(b2Fixture   *fixture);
    void    endContact(b2Fixture   *fixture);

    int     getObjectType(){return GROUND;}
    int     getClassName(){return GROUND;}

            ~Ground();

};

#endif // GROUND_HPP_INCLUDED
