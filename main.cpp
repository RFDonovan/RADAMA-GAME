//#include "Game.h"
#include "LevelDesign.hpp"
//#include "LoginScreen.h"
//C:\DATA F\DEV\TGUI-0.6.9\Build\lib\libtgui.a

int main (){
    try
	{
	    std::cout << "\nRunning... "<< std::endl;
//		Game game;
//		game.run();
		LevelDesign lDesign;
		lDesign.run();

	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}
}

