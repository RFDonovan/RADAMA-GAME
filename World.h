#include<SFML/Graphics.hpp>
#include<Box2D/Box2D.h>
#include<iostream>
#include<functional>
#include<vector>

#include "debugDraw.hpp"
#include "TextureHolder.h"
#include "Entity.hpp"
//#include "CommandQueue.h"

/// class World - recevra une reference du RenderWindow depuis Game, c'est cette classe qui va s'
class World
{
    // Attributes
public:

    sf::RenderWindow& mWindow;
    TextureHolder Textures;
    ///>>>Physic World init
    b2Vec2      gravity;
    b2World     p_world;
    //b2Body* Body1;

    DebugDraw debugDrawInstance;


    //DebugDraw debugDrawInstance;
    sf::View mWorldView;



    std::vector<Entity*> entities;




    TextureHolder mTextures;
    //Layers mSceneLayers;
    //CommandQueue mCommandQueue;
    //float mScrollSpeed;
    sf::Vector2f mPlayerPosition;

    sf::Texture t_ground;
    sf::Sprite GroundSprite;
    // Operations
public:
    explicit World (sf::RenderWindow& window);
    void update (/*sf::Time dt*/);
    void draw (sf::Time frameTime);
    void loadTextures ();
    void buildScene ();
    void adaptViewToPlayer();
    b2World& getWorld();
    sf::Vector2f getMousePos();

    void updateView(sf::Vector2f view);
    void processInput(sf::Event);

    //TextureHolder World::getTextureMap();

    /**CREATING SOMETHINGS*/
    void createGround(b2World& world, float X, float Y);
    void createBox(b2World& world, int MouseX, int MouseY);
    void createEntity(b2World& world, int MouseX, int MouseY);
};

