#ifndef SPRITEMAPPING_HPP_INCLUDED
#define SPRITEMAPPING_HPP_INCLUDED
#include<SFML/Graphics.hpp>
#include<vector>
#include <map>
#include <sstream>
#include <iostream>
#include "AnimatedSprite.hpp"
#include "pugixml.hpp"


struct SpriteInfo{
    std::string name;
    int Id;
    int x, y, w, h;
};

class SpriteMapping
{
public:
    pugi::xml_document          XMLDocument;

    std::map<std::string, Animation> animationList;
    std::map<int, SpriteInfo> spriteList;
    sf::Texture SpriteSheet;
public:
                SpriteMapping();
    void loadXML(std::string XMLFile);

    SpriteInfo  getSpriteInfo(pugi::xml_node spriteNode);
    void   createAnimation(pugi::xml_node animationNode);

    std::map<std::string, Animation>*    getAnimationList();
};

#endif // SPRITEMAPPING_HPP_INCLUDED
