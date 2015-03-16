#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "OgreApplication.h"
#include "GameWorld.h"

int main()
{
	//show - hide console window
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, 1);

	shared_ptr<OgreApplication>  application = make_shared<OgreApplication>();
	shared_ptr<GameWorld> game = make_shared<GameWorld>(application.get());

	// Step 1: Create and initialze the ogre application
	if (application == nullptr || !application->Initialize())
	{
		std::cout << __FILE__ << " (" << __LINE__ << ") - " << "Failed to initialise the application" << std::endl;
		system("PAUSE");
		return 0;
	}
	// Start the game
	game->Run();
	
	
	return 0;
}


