/* ********************************************************************************* *
 * *                  SIT153 - Introduction to Game Development                    * *
 * *                Assignment #2 - Develop a Classic Arcade Game                  * *
 * *                                                                               * *
 * *  This code is provided to students as a basic framework for the purposes of   * *
 * *  developing a game that meets the requirements of the assessment guidelines.  * *
 * *  This code base does not provide all features necessary to implement such a   * *
 * *  game. Students must modify and extend this framework to achieve the          * *
 * *  specific functionality required of their game.                               * *
 * *                                                                               * *
 * *  The code is provided as-is and while significant efforts have been made to   * *
 * *  ensure that it is bug-free, this is not a guarantee. It is each student's    * *
 * *  responsibility to ensure that their final submission compiles and executes   * *
 * *  without error.                                                               * *
 * *                                                                               * *
 * *  COPYRIGHT NOTICE                                                             * *
 * *  ----------------                                                             * *
 * *  (C) [2012] - [2016] Deakin University                                        * *
 * *  All rights reserved.                                                         * *
 * *  All information contained herein is, and remains the property of Deakin      * *
 * *  University and the authors (Tim Wilkin, Michael Hobbs and Shaun Bangay).     * *
 * *  Dissemination of this information or reproduction of this material is        * *
 * *  strictly forbidden unless prior written permission is obtained from Deakin   * *
 * *  University. The right to create derivative works from this material is       * *
 * *  hereby granted to students enrolled in SIT153, but only for the purposes of  * *
 * *  assessment while an enrolled student at Deakin University.                   * *
 * *                                                                               * *
 * *  Please complete the following information as acknowledgement of the          * *
 * *  above conditions regarding the use of this code. Failure to complete this    * *
 * *  acknowledgement will mean that your assignment submission will not be marked * *
 * *                                                                               * *
 * ********************************************************************************* *
 * *  STUDENT ID:                                                                  * *
 * *  STUDENT NAME:                                                                * *
 * *  DATE:                                                                        * *
 * ********************************************************************************* *
 */

/*****************************************************************************************
 * Demonstration of usage of the WinCanvas class for a Windows console application - to  *
 * support basic '2D graphics' through simplified API surrounding the Windows GDI system.*
 *                                                                                       *
 * This code is based on the code developed by Shaun Bangay (2012) & Tim Wilkin (2013).  *
 * Extended by Michael Hobbs and Tim Wilkin (2014) and re-written by Tim Wilkin (2015).  *
 *                                                                                       *
 *****************************************************************************************/


#include <iostream>
#include <fstream>

#include <time.h>

#include "WinCanvas.h"
#include "InputState.h"
#include "Scene.h"
#include "../SceneManager.h"

// Constant values used in the code - this is the whole window size including
// menu buttons and border - actual graphics canvas size is smaller!
const unsigned int	WINDOW_WIDTH = 616;
const unsigned int	WINDOW_HEIGHT= 640;


// The main function of the program
int main()
{
	// Declare which object from <iostream> and <string> the program will use
	using std::cout;
	using std::cin;
	using std::endl;
	using std::string;

	/********************  DO NOT EDIT THIS CODE *****************************
     *
	 *	Declare a WinCanvas object. We wouldn't normally create an object this way as
	 *  it is non-portable, but this is a very simple 'graphics canvas' interface to
	 *  the Windows Graphical Device Interface (GDI) API.
	 */

	WinCanvas wc(WINDOW_WIDTH, WINDOW_HEIGHT, L"Class Arcade Game");

	/*
	 *  An InputState object provides an interface to the Keyboard and Mouse devices. 
	 */
	
	InputState	  ui;
	OpenUIHandler(ui);	// Initialise the input interface

	/*********************END RESTRICTED CODE *********************************
	 **************************************************************************/

	/*
	 *  Console Splash Screen
	 */

	cout << "  SIT153 Project - 2D Game Framework" << endl;
	cout << "=======================================" << endl << endl;
	
	/*
	 *   Demonstration of game initialisation - You can and should
	 *   modify this code in line with your software design, to
	 *   provide the necessary runtime functionality.
	 */

	// Create a scene object - a container and manager for our sprites
	Scene demo(
		     POINT2(0,0),							// Upper left corner coordinate
			 POINT2(WINDOW_WIDTH - 16,WINDOW_HEIGHT - 40)		// lower right corner coordinate
				);


	// Add the scene manager to the game
	std::shared_ptr<SceneManager> sceneManager = std::make_shared<::SceneManager>(0, 0, &ui, &demo);
	demo.AddEntity(sceneManager);
	demo.SetSceneManager(sceneManager);

	/*
	 * Execute the game loop
	 */

	// The flag that the game loop uses to see if the game has finished
	bool gamefinished = false;

	long lastFrameTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

	// The main game loop
	while (!gamefinished)
	{
		// ****************************************************************		
		// *** STAGE 1: Check InputState										***
		// ****************************************************************		
	
		if (ui.isActive(VK_ESCAPE))		// Check to see if the Game Loop should terminated
		{
			gamefinished = true;	// Break out of this game loop
			wc.Terminate();			// Force a quite message to be passed to the 
									// message handler, which will close that thread
			continue;
		}
		long thisFrameTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
		long longDelta = thisFrameTime - lastFrameTime;
		float dt = longDelta / 1000.0f;

		lastFrameTime = thisFrameTime;

		//printf("%ld\n", longDelta);
		//printf("%lf\n", dt);
		demo.OnInput();
		// ******************************************************************************		
		// *** STAGE 2: Update the state of the scene objects based on the user input ***
		// ******************************************************************************

		demo.OnUpdate(dt);

		// ******************************************************************		
		// *** STAGE 3: Draw the shapes & text to the screen			  ***
		// ******************************************************************		
		wc.Clear(255, 255, 255);		// Need to clear the old screen before drawing new updated screen!
		demo.OnDraw(wc);				// Call the scene object's draw handler

		
		wc.Display();					// Last step is to actually 'display' the elements to the screen
	}

	// Finished the game. 
	cout << "Game finished. Now exiting...." << endl;

	return 0;
}
