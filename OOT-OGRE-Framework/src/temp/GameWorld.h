//
//  GameWorld.h
// This class is the interface class between your game to Ogre.
// The access point is the pointer to OgreApplication class 
//  OOT Tutorial Ogre Framework Demo.
//  Created by Wen Tang on 19/01/15.
//

#pragma once
#ifndef _GAME_WORLD_H_
#define _GAME_WORLD_H_

#include "stdafx.h"
#include <iostream>
#include "OgreApplication.h"

class GameWorld
{
private:
										
	void UpdateGame(float dt);
	void UpdateScene(Ogre::Vector3 &pos, Ogre::Quaternion &q);
	
	std::shared_ptr<Ogre::SceneNode> cubeNode;
	Ogre::Vector3 initial_position;
	OgreApplication* application;
	
public:

	GameWorld(OgreApplication* application_);
	~GameWorld();
					
	void Run();
	void InitilaiseScene();
	void CreateEnviroment();
	void CreateLights();
	void CreateEntities();
	void Update(float dt);
	void Reset();

	Ogre::Matrix3 rotate_Y(float angle);
	void rotation_Y(float angle);
};

#endif // End of _GAME_WORLD_H_
