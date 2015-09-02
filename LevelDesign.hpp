#ifndef LEVELDESIGN_HPP_INCLUDED
#define LEVELDESIGN_HPP_INCLUDED

#include<vector>
#include<iostream>
#include<string>
#include<string.h>

#include <TGUI/TGUI.hpp>

#include "Globals.h"
#include "TextureHolder.h"
#include "tinyfiledialogs.h"
#include "Asset.hpp"
#include "Splitter.hpp"

class LevelDesign
{
public:
    sf::RenderWindow mWindow;

    /*VIEW CONTROL*/
    sf::View    mWorldView;
    bool        moveView=false;
    sf::Vector2f    clickPos;
    /**************/

    sf::Font MyFont;
    sf::Text t;
    TextureHolder textureHolder;

    bool vertexMode = false;
    bool showImages = true;
    bool showVertex = true;

    bool showAssets = false;

    std::vector<sf::CircleShape>    vertexList;
    std::vector<sf::Sprite>    imageList;
    std::vector<Asset>          assetList;
    std::vector<sf::Vector2f>          assetRatio;
    //std::vector<Asset*>          selectedAsset;

    std::vector<std::string>    filenameList;


    sf::Sprite*         tmpSprite = nullptr;
    sf::CircleShape*    tmpNode = nullptr;
    Asset*              tmpAsset = nullptr;
    bool isMoving = false;
    sf::RectangleShape selectRectangle;
    bool selectOn = false;

    float unit = 20.f;

    /*TGUI*/
    tgui::Gui gui;
    std::vector<std::string> listMenu1 = {"Add Image", "Commit", "Clear", "Switch Mode", "Level Editor"};
    std::vector<std::string> listMenu2 = {"Import", "Export", "Save", "Reset All", "Asset Creator"};
    /******/


public:

            LevelDesign();
    void    createGUI();
    /******EVENT HANDLER*******/
    void    tguiEventHandler();
    void    toggleAssetMode();
    void    loadFiles();
    void    zoomIN();
    void    zoomOUT();
    /******END EVENT HANDLER*******/

    void    run ();

    void    processInput();
    void    basicInput(sf::Event e);
    void    mouseInput(sf::Event e);

    sf::CircleShape     createVertex(sf::Vector2f pos); ///>creer un noeud
    sf::Sprite          loadImage(std::string);
    void                createAsset();
    void                deleteImage(sf::Sprite* image);
    void                deleteNode(sf::CircleShape* node);
    void                deleteAsset(Asset* asset);
    void                downZIndex(Asset* asset);

    /******FEATURES*******/
    ///CMD
    void                selectAsset(Asset* asset);
    void                unselectAllA();
    void                getAssetsRatio();
    void                moveSelectedA(sf::Vector2f pos);

    ///SAVE LEVEL

    void                saveLevel();
    void                saveAssets();
    void                saveAsset(Asset a);
    /******END FEATURES*******/


    void    update ();
    void    render (sf::Time frameTime);

    void    renderImages(sf::Time frameTime);
    void    renderVertex(sf::Time frameTime);
    void    renderAssets(sf::Time frameTime);
    void    renderGUI(sf::Time frameTime);


    ///MOUSE POSITION
    sf::Vector2f    getMousePos();
    bool    mouseIsOnTheSprite(sf::Sprite* sp, sf::Vector2f mousePos);


    ///SPLITTER-------------------------------------------
    std::vector<std::string> splitPerso(std::string str,std::string sep)
    {
        char* cstr=const_cast<char*>(str.c_str());
        char* current;
        std::vector<std::string> arr;
        current=strtok(cstr,sep.c_str());
        while(current!=NULL){
            arr.push_back(current);
            current=strtok(NULL,sep.c_str());
        }
        return arr;
    }

    ///SPLITTER-------------------------------------------

};

#endif // LEVELDESIGN_HPP_INCLUDED
