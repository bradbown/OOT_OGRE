//
//  Camera.cpp
//  OOT Tutorial Ogre Framework Demo.
//  Created by Wen Tang on 19/01/15.
//
#include "stdafx.h"
#include "Camera.h"

Camera::Camera(shared_ptr<Ogre::SceneNode> sceneNode, shared_ptr<Ogre::MovableObject> camera)
{
	this->sceneNode = sceneNode;
	this->camera = camera;

	this->rotation = Ogre::Vector3::ZERO;
}

void Camera::Update(OIS::Keyboard* keyboard, OIS::Mouse* mouse)
{
	Ogre::Vector3 currentPosition = this->sceneNode->getPosition();
	Ogre::Quaternion currentRotation = this->sceneNode->getOrientation();

	Ogre::Vector3 right = Ogre::Vector3::UNIT_X;
	Ogre::Vector3 up = Ogre::Vector3::UNIT_Y;
	Ogre::Vector3 look = Ogre::Vector3::UNIT_Z;

	right = currentRotation * right;
	up = currentRotation * up;
	look = currentRotation * look;

	float forwardScale = 0.0f;
	float sideScale = 0.0f;
	float verticalScale = 0.0f;

	if (keyboard->isKeyDown(OIS::KC_W) || keyboard->isKeyDown(OIS::KC_UP))
		forwardScale = 1.0f;
	else if (keyboard->isKeyDown(OIS::KC_S) || keyboard->isKeyDown(OIS::KC_DOWN))
		forwardScale = -1.0f;

	if (keyboard->isKeyDown(OIS::KC_D) || keyboard->isKeyDown(OIS::KC_RIGHT))
		sideScale = 1.0f;
	else if (keyboard->isKeyDown(OIS::KC_A) || keyboard->isKeyDown(OIS::KC_LEFT))
		sideScale = -1.0f;

	if (keyboard->isKeyDown(OIS::KC_Q) || keyboard->isKeyDown(OIS::KC_HOME))
		verticalScale = 1.0f;
	else if (keyboard->isKeyDown(OIS::KC_Z) || keyboard->isKeyDown(OIS::KC_END))
		verticalScale = -1.0f;

	currentPosition += (look * forwardScale);
	currentPosition += (up * verticalScale);
	currentPosition += (right * sideScale);

	this->sceneNode->setPosition(currentPosition);
	static_pointer_cast<Ogre::Camera>(this->camera)->lookAt(currentPosition + look);

	if (mouse->getMouseState().X.abs > mouse->getMouseState().width)
		return;

	if (mouse->getMouseState().Y.abs > mouse->getMouseState().height)
		return;

	const int mouseX = mouse->getMouseState().X.rel;
	const int mouseY = mouse->getMouseState().Y.rel;

	this->rotation.x += mouseX;
	this->rotation.y += mouseY;

	Ogre::Quaternion newRotationX, newRotationY;
	newRotationX.FromAngleAxis((Ogre::Radian)(this->rotation.x * -0.0017f), Ogre::Vector3::UNIT_Y);
	newRotationY.FromAngleAxis((Ogre::Radian)(this->rotation.y *  0.0017f), Ogre::Vector3::UNIT_X);

	this->sceneNode->setOrientation(newRotationX * newRotationY);
}