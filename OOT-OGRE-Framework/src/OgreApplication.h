//
//  GameDemo.h
//  OOT Tutorial Ogre Framework Demo.
//  Created by Wen Tang on 19/01/15.
//

#pragma once
#ifndef _OGRE_APPLICATION_H_
#define _OGRE_APPLICATION_H_

#include "stdafx.h"
#include "OgreWrapper.h"

using namespace std;

struct SceneNodeAndName
{
	Ogre::String Name;
	Ogre::SceneNode*  Node;
};

class OgreApplication
{

private:
	
	OgreWrapper	ogreWrapper; 
    Ogre::Root	*root; 
	Ogre::RenderWindow	*renderWindow;						
	Ogre::SceneManager	*sceneManager;						
	
	Ogre::SceneNode	 *rootSceneNode;				
	Ogre::Viewport	*viewport;	
	OIS::InputManager *inputManager;
	
	shared_ptr<Ogre::Timer>	timer;				
	shared_ptr<OIS::Mouse>	mouse;						
	shared_ptr<OIS::Keyboard> keyboard;					
																	
	void	CreateLights();													
	void	CreateEnviroment();
	   
public:
																
	OgreApplication(void);
	~OgreApplication(void);
   
	bool Initialize();
	void CleanUp();
											
	std::shared_ptr<Ogre::SceneNode> CreateCamera(Ogre::String  name);	
	std::shared_ptr<Ogre::SceneNode> CreateEntityFromMesh(std::string mesh, std::string name);
 
	void SetEntityColour(Ogre::String      materialName,
		                 Ogre::String      resourceGroupName,
		                 Ogre::ColourValue diffuse,
		                 Ogre::ColourValue specular,
		                 Ogre::Real        shininess);
											
	void CreateIOS(); 										
	void Run(bool updateOption=false, bool synchroOption=true); //The main run loop
	void DestroyNode(Ogre::SceneNode *nodeName);

	shared_ptr<Ogre::Timer> CreateTimer();
	
    const OgreWrapper&	GetOgreWrapper(){ return ogreWrapper; }
	Ogre::SceneManager* GetSceneManager(){return sceneManager ;}
									
	shared_ptr<OIS::Keyboard> GetKeyboard(){return keyboard;}
    shared_ptr<OIS::Mouse> GetMouse(){return mouse;}
	shared_ptr<Ogre::Timer> GetTimer(){return timer;}

};

#endif  // End of _OGRE_APPLICATION_H_