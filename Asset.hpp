#ifndef ASSET_HPP_INCLUDED
#define ASSET_HPP_INCLUDED
#include<vector>
#include<iostream>
#include<string>
#include<string.h>

#include "Globals.h"
#include "TextureHolder.h"

class Asset
{
public:
    sf::Sprite aSprite;
    std::vector<sf::CircleShape> nodeList;
    std::vector<sf::Vector2f>    nodeRatio;
    bool pinned = false;

public:
            Asset(sf::Sprite image, std::vector<sf::CircleShape> nodeList);
    void    render(sf::RenderWindow& mWindow);

    void    setPosition(sf::Vector2f pos);


    sf::Vector2f    getPosition(){return aSprite.getPosition();}

    sf::FloatRect   getGlobalBounds(){return aSprite.getGlobalBounds();}

    bool            isPinned(){return pinned;}

};


#endif // ASSET_HPP_INCLUDED
