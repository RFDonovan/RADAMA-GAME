#ifndef LEVELDESIGN_HPP_INCLUDED
#define LEVELDESIGN_HPP_INCLUDED

#include<vector>
#include<iostream>
#include<string>
#include<string.h>

#include "Globals.h"
#include "TextureHolder.h"
#include "tinyfiledialogs.h"

class LevelDesign
{
public:
    sf::RenderWindow mWindow;
    sf::View    mWorldView;

    TextureHolder textureHolder;

    bool vertexMode = false;
    bool showImages = true;
    bool showVertex = true;

    sf::Sprite* tmpSprite;
    sf::CircleShape* tmpNode;
    bool isMoving = false;

    std::vector<sf::CircleShape>    vertexList;

    std::map<std::string, std::vector<sf::CircleShape>> vertexGroupList;

    std::vector<sf::Sprite>    imageList;

public:

            LevelDesign();
    void    run ();

    void    processInput();
    void    basicInput(sf::Event e);
    void    mouseInput(sf::Event e);

    sf::CircleShape     createVertex(sf::Vector2f pos);
    void    deleteNode(sf::CircleShape* node);
    sf::Sprite          loadImage(std::string);
    void    deleteImage(sf::Sprite* image);

    void    update ();
    void    render (sf::Time frameTime);

    void    renderImages(sf::Time frameTime);
    void    renderVertex(sf::Time frameTime);


    ///MOUSE POSITION
    sf::Vector2f    getMousePos();
    bool    mouseIsOnTheSprite(sf::Sprite* sp, sf::Vector2f mousePos);


};

#endif // LEVELDESIGN_HPP_INCLUDED
