#include "Game.h"
#include "LevelDesign.hpp"
//#include "LoginScreen.h"

int main (){
    try
	{
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

