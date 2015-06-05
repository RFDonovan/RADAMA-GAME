#include<SFML/Graphics.hpp>
#include<Box2D/Box2D.h>
#include<iostream>
#include<functional>
#include<vector>

#include "debugDraw.hpp"
#include "TextureHolder.h"
#include "Entity.hpp"
#include "Human.hpp"
#include "Player.h"
#include "Ground.hpp"
#include "ContactListener.inl"

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

    DebugDraw   debugDrawInstance;


    //DebugDraw debugDrawInstance;
    sf::View    mWorldView;



    std::vector<Human*>    humans;
    std::vector<Ground*>    grounds;




    TextureHolder           mTextures;
    //Layers mSceneLayers;
    //CommandQueue mCommandQueue;
    //float mScrollSpeed;
    sf::Vector2f            mPlayerPosition;
    Player                  *ePlayer;

    sf::Sprite   BG;
    sf::Sprite   BG_pause;
    sf::Vector2f            mBGPosition;

    sf::Texture t_ground;
    sf::Sprite  GroundSprite;

    ///contact listener
    ContactListener CL_Instance;

    /**SHADERS STUFF**/
    sf::Texture objectTexture, distortionMap;
    sf::Sprite object, sprite;
    //sf::RenderTexture renderTexture;
    sf::Shader shader, fogShader;
    float distortionFactor = .01f;
    float riseFactor = .5f;
    sf::Clock clock;


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
    void createGround(b2World& world, float X, float Y, float W = 600.f, float H = 16.f);
    void createBox(b2World& world, int MouseX, int MouseY);
    void createEntity(b2World& world, int MouseX, int MouseY);

    /**PAUSE STUFF**/
    sf::RectangleShape pauseLayer;
    bool paused = false;
    void pause();
    void resume();


};

