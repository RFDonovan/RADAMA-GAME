#ifndef LEVELDESIGN_HPP_INCLUDED
#define LEVELDESIGN_HPP_INCLUDED

#include<vector>
#include<iostream>
#include<string>
#include<string.h>

#include "Globals.h"
#include "TextureHolder.h"
#include "tinyfiledialogs.h"
#include "Asset.hpp"

class LevelDesign
{
public:
    sf::RenderWindow mWindow;
    sf::View    mWorldView;

    TextureHolder textureHolder;

    bool vertexMode = false;
    bool showImages = true;
    bool showVertex = true;

    bool showAssets = false;

    std::vector<sf::CircleShape>    vertexList;
    std::vector<sf::Sprite>    imageList;
    std::vector<Asset>          assetList;


    sf::Sprite*         tmpSprite = nullptr;
    sf::CircleShape*    tmpNode = nullptr;
    Asset*              tmpAsset = nullptr;
    bool isMoving = false;

    float unit = 20.f;




public:

            LevelDesign();
    void    run ();

    void    processInput();
    void    basicInput(sf::Event e);
    void    mouseInput(sf::Event e);

    sf::CircleShape     createVertex(sf::Vector2f pos);
    void                deleteNode(sf::CircleShape* node);
    sf::Sprite          loadImage(std::string);
    void                deleteImage(sf::Sprite* image);
    void                createAsset();

    void    update ();
    void    render (sf::Time frameTime);

    void    renderImages(sf::Time frameTime);
    void    renderVertex(sf::Time frameTime);
    void    renderAssets(sf::Time frameTime);


    ///MOUSE POSITION
    sf::Vector2f    getMousePos();
    bool    mouseIsOnTheSprite(sf::Sprite* sp, sf::Vector2f mousePos);


};

#endif // LEVELDESIGN_HPP_INCLUDED
