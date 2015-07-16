#ifndef ITEM_HPP_INCLUDED
#define ITEM_HPP_INCLUDED
#include<typeinfo>
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

class Item: public ObjectType{

public:
        //int type;///CERTAINES ENTITES NE PEUT PRENDRE CERTAINS ARMES
        int identificationItem = 60001;

        bool dejaPris = false;///SI C EST DEJA SOUS LA POSSESSION D UNE ENTITE CA DEVRAIT PAS
        b2World *p_world;
        b2Body* m_body;
        int bonusLife = 10;
        int bonusMana = 0;


        ///Sprite things
        sf::Clock clock;
        sf::Sprite itemSprite;
        sf::Texture itemTexture;

public:
                Item(b2World* world, std::string filename, float x, float y, int mana, int life);
        int     getClassName(){return ITEM;}
        int     getObjectType(){return ITEM;}
        bool isDispo(){return !dejaPris;}
        void    render(sf::RenderWindow& mWindow, sf::Shader* shader);

                ~Item();

        ///Collision MANAGER
        void    collideWith(b2Fixture* fixture);


};




#endif // PROJECTILE_HPP_INCLUDED

