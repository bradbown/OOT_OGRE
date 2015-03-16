//
//  Camera.h
//  OOT Tutorial Ogre Framework Demo.
//  Created by Wen Tang on 19/01/15.
//

#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "stdafx.h"
#include <memory>

using namespace std;

class Camera
{
private:

	shared_ptr<Ogre::SceneNode> sceneNode;
	shared_ptr<Ogre::MovableObject> camera;

	Ogre::Vector3 rotation;

public:
	Camera(shared_ptr<Ogre::SceneNode> sceneNode, shared_ptr<Ogre::MovableObject> camera);
	~Camera() {};

	void Update(OIS::Keyboard* keyboard, OIS::Mouse* mouse);
};

#endif // end of _CAMERA_H_