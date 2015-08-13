#include<SFML/Graphics.hpp>
#include<Box2D/Box2D.h>
#include<iostream>
#include<functional>
#include<vector>
#include<map>
#include<algorithm>
#include<sys/stat.h>

///FILESYSTEM
#include<dirent.h>

#include "pugixml.hpp"
#include "debugDraw.hpp"
#include "TextureHolder.h"
#include "GameLevel.hpp"
#include "XMLLoader.hpp"
#include "Entity.hpp"
#include "Human.hpp"
#include "Player.h"
#include "Ground.hpp"
#include "Structures.hpp"
#include "Projectile.hpp"
#include "Item.hpp"
#include "SpriteMapping.hpp"
#include "Statistiques.hpp"

#include "ContactListener.inl"
#include "DestructionListener.inl"


//#include "CommandQueue.h"
/// class World - recevra une reference du RenderWindow depuis Game, c'est cette classe qui va s'

class QueryCallback : public b2QueryCallback
{
public:
	QueryCallback(const b2Vec2& point)
	{
		m_point = point;
		m_fixture = NULL;
	}

	bool ReportFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_fixture = fixture;

				// We are done, terminate the query.
				return false;
			}
		}

		// Continue the query.
		return true;
	}

	b2Vec2 m_point;
	b2Fixture* m_fixture;
};

class World
{
    // Attributes
public:

    sf::RenderWindow& mWindow;
    TextureHolder Textures;
    ///>>>Physic World init
    b2Vec2      gravity;
    b2World     p_world;
    b2World     p_world2;
    //b2Body* Body1;

    DebugDraw   debugDrawInstance;


    //DebugDraw debugDrawInstance;
    sf::View    mWorldView;

    //GameLevel*  level;
    XMLLoader* xLoad;
    XMLLoader* xLoad2;

    std::vector<Human*>         humans;
    std::vector<Human*>         listOfDeletedHuman;
    //std::vector<bodyData>   WeaponList;
    std::vector<bodyData>       LevelObjectList;
    std::vector<bodyData>       bDList;
    std::vector<Projectile*>    pList;
    std::vector<Item*>          itemList;
    std::map<std::string, SpriteMapping*>   spriteMap;

    bool rebuild = false;




    TextureHolder           mTextures;
    sf::Vector2f            mPlayerPosition;
    Player                  *ePlayer;

    sf::Sprite   BG;
    sf::Sprite   BG_pause;
    sf::Vector2f            mBGPosition;

    sf::Texture t_ground;
    sf::Sprite  GroundSprite;

    ///contact listener
    MyDestructionListener DL_Instance;
    ContactListener CL_Instance;


    /**SHADERS STUFF**/
    sf::Texture objectTexture, distortionMap;
    sf::Sprite object, sprite;
    sf::Shader shader, fogShader;
    float distortionFactor = .1f;
    float riseFactor = .5f;
    sf::Clock clock;

    bool deletetime_restart = false;

    ///FOR DRAGABLE:
    b2Body* m_groundBody;
    b2MouseJoint* m_mouseJoint;
    b2Vec2 m_mouseWorld;
    bool editMode = true;
    std::string     levelPath;

    ///STATISTIQUES
    Statistiques statInfo;

    // Operations
public:
            explicit World (sf::RenderWindow& window);
    void    update (/*sf::Time dt*/);
    void    draw (sf::Time frameTime);
    void    loadTextures ();
    ///LOADING LEVEL
    void    loadInfo(std::string xmlCfg);
    ///LOADING SPRITES
    void    loadSprites(std::string listFile);
    ///BUILDING SCENE
    void    buildScene (std::string CurrentDir);
    void    rebuildScene ();

    ///VIEW CONTROLLER
    void    adaptViewToPlayer();

    ///GETTERS
    b2World&    getWorld();
    sf::Vector2f    getMousePos();

    void    updateView(sf::Vector2f view);
    void    processInput(sf::Event);

    ///REMOVE
    void    sheduleRemove(float elapsedTime);

    /**STICKING PROJECTILE**/
    void    sticking();
    /**PAUSE STUFF**/
    sf::RectangleShape pauseLayer;
    bool paused = false;
    void pause();
    void resume();

    /**MOUSE DRAG STUFF**/
    void    MouseDown(const b2Vec2& p);
    void    MouseUp();
    void    MouseMove(const b2Vec2& p);

};



