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
        bodyData* data;

        bool dejaPris = false;///SI C EST DEJA SOUS LA POSSESSION D UNE ENTITE CA DEVRAIT PAS

public:
                Projectile(bodyData* data);
        bodyData*   getBodyData(){return data;}
        int     getClassName(){return PROJECTILE;}
        int     getObjectType(){return PROJECTILE;}
        bool isDispo(){return !dejaPris;}


};




#endif // PROJECTILE_HPP_INCLUDED
