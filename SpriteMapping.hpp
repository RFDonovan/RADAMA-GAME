#ifndef SPRITEMAPPING_HPP_INCLUDED
#define SPRITEMAPPING_HPP_INCLUDED
#include<SFML/Graphics.hpp>
#include<vector>
#include <map>
#include <sstream>
#include <iostream>
#include "AnimatedSprite.hpp"
#include "TextureHolder.h"
#include "pugixml.hpp"

typedef std::map<std::string, Animation> AnList_t;

struct SpriteInfo{
    std::string name;
    int Id;
    int x, y, w, h;
};

class SpriteMapping
{
public:
    pugi::xml_document          XMLDocument, XMLDocument0;

    TextureHolder               textureHolder;

    std::map<std::string , AnList_t > animationDB;

    AnList_t animationList;
    std::map<int, SpriteInfo> spriteList;
    sf::Texture SpriteSheet;

    std::string textureName;

    std::vector<Animation> vAnimation;

    static AnList_t mAnimation;

public:
                SpriteMapping();
                SpriteMapping(std::string filename);

    AnList_t    createSpriteMapping(std::string filename);
    void loadXML(std::string XMLFile);

    SpriteInfo  getSpriteInfo(pugi::xml_node spriteNode);
    void   createAnimation(pugi::xml_node animationNode);

    std::map<std::string, Animation>*    getAnimationList();
};

#endif // SPRITEMAPPING_HPP_INCLUDED
