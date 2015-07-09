#ifndef PROJECTILE_HPP_INCLUDED
#define PROJECTILE_HPP_INCLUDED
#include <random>
#include<SFML/Graphics.hpp>
#include<Box2D/Box2D.h>
#include<iostream>
#include<cmath>
#include<vector>
#include<sstream>
#include "Globals.h"
#include "AnimatedSprite.hpp"
#include "TextureHolder.h"
#include "Structures.hpp"

class Projectile: public ObjectType{

public:
        int type;///CERTAINES ENTITES NE PEUT PRENDRE CERTAINS ARMES
        int identificationArme = 20001;
        bodyData realData;
        bodyData* data = nullptr;

        b2Joint * joint = nullptr;
        b2Joint * joint2 = nullptr;
        b2RevoluteJointDef weldJointDef, weldJointDef1;
        b2Vec2 worldCoordsAnchorPoint;

        bool dejaPris = false;///SI C EST DEJA SOUS LA POSSESSION D UNE ENTITE CA DEVRAIT PAS
        bool jointExist = false;
        bool sticky = true;

        bool canStick = false;
        b2World *p_world;

public:
                Projectile(b2World* world, bodyData dataa);
        bodyData*   getBodyData(){return data;}
        int     getClassName(){return PROJECTILE;}
        int     getObjectType(){return PROJECTILE;}
        bool isDispo(){return !dejaPris;}
        bool isSticky(){return sticky;}
        bool isSticking(){return jointExist;}
        void unStick();
                ~Projectile();

        ///JOINT MANAGER
        void    impactTo(b2Fixture* fixtureSource, b2Fixture* fixtureTarget, float impulse);
        void    stickIt();
        void    stickProjectile(b2Fixture* fixtureTarget);


};




#endif // PROJECTILE_HPP_INCLUDED
