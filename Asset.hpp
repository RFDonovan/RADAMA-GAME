#ifndef ASSET_HPP_INCLUDED
#define ASSET_HPP_INCLUDED
#include<vector>
#include<iostream>
#include<string>
#include<sstream>
#include<string.h>

#include "Globals.h"
#include "TextureHolder.h"
#include "tinyfiledialogs.h"
#include "pugixml.hpp"
#include "shlwapi.h"

class Asset
{
public:
    sf::Sprite aSprite;
    std::vector<sf::CircleShape> nodeList;
    std::vector<sf::Vector2f>    nodeRatio;
    bool pinned = false;
    std::string path;
    std::string name;
    TextureHolder                textureHolder;

public:
            Asset(sf::Sprite image, std::vector<sf::CircleShape> nodeList,std::string filename);
            Asset(std::string filename);
    void    render(sf::RenderWindow& mWindow);

    void    setPosition(sf::Vector2f pos);

    void    shrink();
    void    expand();


    sf::Vector2f    getPosition(){return aSprite.getPosition();}

    sf::FloatRect   getGlobalBounds(){return aSprite.getGlobalBounds();}

    void            exportToXML(std::string filename);
    pugi::xml_node  createNode();
    bool            isPinned(){return pinned;}

};


#endif // ASSET_HPP_INCLUDED
