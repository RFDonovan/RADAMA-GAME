
//#include <TGUI/TGUI.hpp>

//#include "Animation.hpp"
#include "AnimatedSprite.hpp"
//#include "Command.hpp"
#include "Globals.h"
//#include "DebugDraw.hpp"

#include "World.h"

/// class Game - classe qui va contenir la boucle principale
class Game
{
    // Attributes
private:
    sf::RenderWindow mWindow;
    World mWorld;
    //Player mPlayer;
    // Operations
//    tgui::Gui gui;
public:
         Game();
    void run ();
    //void processInput ();
    void processInput();
    void update ();
    void render (sf::Time frameTime);
};

