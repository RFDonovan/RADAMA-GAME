#include "debugDraw.hpp"
#include "TextureHolder.h"
//#include "CommandQueue.h"

/// class World - recevra une reference du RenderWindow depuis Game, c'est cette classe qui va s'
class World
{
    // Attributes
public:

    sf::RenderWindow& mWindow;
    //DebugDraw debugDrawInstance;
    sf::View mWorldView;
    TextureHolder mTextures;
    //Layers mSceneLayers;
    //CommandQueue mCommandQueue;
    //float mScrollSpeed;
    sf::Vector2f mPlayerPosition;
    // Operations
public:
    explicit World (sf::RenderWindow& window);
    void update (/*sf::Time dt*/);
    void draw ();
    void loadTextures ();
    void buildScene ();
    void adaptViewToPlayer();
};

