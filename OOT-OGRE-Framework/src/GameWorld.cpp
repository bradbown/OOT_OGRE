//
//  GameWorld.cpp
//  OOT Tutorial Ogre Framework Demo.
//  Created by Wen Tang on 19/01/15.
//
#include "GameWorld.h"
#include "Camera.h"

#include <string>
#include <iostream>
#include <sstream>

#define STEP_LENGTH 0.03

GameWorld::GameWorld(OgreApplication* application_) 
{
	application = application_;
	initial_position = Ogre::Vector3(0.0f, 0.0f, 0.0f); 
}


GameWorld::~GameWorld()
{
}

void GameWorld::InitilaiseScene()
{
	CreateEnviroment();
	CreateLights();
	CreateEntities();
}

void GameWorld::CreateEntities()
{
	// Create a material object named "RED" and a material object "GREEN"
	application->SetEntityColour("RED", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::ColourValue(1.0f, 0.1f, 0.1f), Ogre::ColourValue(0.6f, 0.0f, 0.0f), 100.0f);
	application->SetEntityColour("GREEN", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::ColourValue(0.1f, 0.8f, 0.1f), Ogre::ColourValue(0.0f, 0.6f, 0.0f), 50.0f);
	
	// Load a cube
	auto cube = application->GetSceneManager()->createEntity("cube.mesh");
	cube->setCastShadows(true);
	cube->setMaterialName("GREEN");

	initial_position = Ogre::Vector3(0.0f, 100.0f, 0.0f);
	
	cubeNode.reset(application->GetSceneManager()->getRootSceneNode()->createChildSceneNode("cube"));  // give a name "cube" to the cubeNode
	cubeNode->setScale(Ogre::Vector3(0.5f, 0.5f, 0.5f));
	cubeNode->setPosition(initial_position);
	cubeNode->attachObject(cube);
}

// Create a ogre world environment with a predefined geometry and a texture
void GameWorld::CreateEnviroment()
{
	// Create a plane and apply a texture to it
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 400, 400, 200, 200, true, 5, 5, 5, Ogre::Vector3::UNIT_Z);

	Ogre::String tempName = "Ground";
	Ogre::Entity* ground = application->GetSceneManager()->createEntity("Ground", "ground");
	ground->setCastShadows(false);

	Ogre::SceneNode* groundNode = application->GetSceneManager()->getRootSceneNode()->createChildSceneNode(tempName.append("Node"));
	groundNode->attachObject(ground);
	groundNode->setPosition(0.0f, 0.0f, 0.0f);

	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("GroundTexture", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::Technique* firstTechnique = material->getTechnique(0);
	Ogre::Pass* firstPass = firstTechnique->getPass(0);

	Ogre::TextureUnitState* textureUnit = firstPass->createTextureUnitState();
	textureUnit->setTextureName("checker.png", Ogre::TEX_TYPE_2D);  // BeachStones.jpg
	textureUnit->setTextureCoordSet(0);

	const Ogre::String& materialName = "GroundTexture";
	ground->setMaterialName(materialName);

	application->GetSceneManager()->setSkyDome(true, "Examples/CloudySky", 10, 8, 100);
	
}


//Create light in the scene
void GameWorld::CreateLights()
{
	
	// create a spot light
	Ogre::String tempName = "SpotLight";
    Ogre::Light* spotLight = application->GetSceneManager()->createLight(tempName);
	spotLight->setType(Ogre::Light::LT_POINT);

	//Set light colour 
	spotLight->setDiffuseColour(0.8f, 0.8f, 0.8f); 
	spotLight->setSpecularColour(0.5f, 0.5f, 0.5f);
	//set the spot light corn beam width
	spotLight->setSpotlightRange(Ogre::Degree(20), Ogre::Degree(50));

	//Create a childSceneNode of the RootSceneNode
	//Attach the light to it
	Ogre::SceneNode* lightNode =  application->GetSceneManager()->getRootSceneNode()->createChildSceneNode(tempName.append("Node"));
	lightNode->attachObject(spotLight);
	lightNode->setDirection(1.0f, 1.0f, 0.0f);
	lightNode->setPosition(Ogre::Vector3(80.0, 180.0, 0.0));

	Ogre::ColourValue ambientColour(0.2f, 0.2f, 0.2f, 1.0f);
	application->GetSceneManager()->setAmbientLight(ambientColour);

}

void GameWorld::Run()
{
	// Create a camera
	Ogre::String cameraName = "MainCamera";
	auto cameraNode = application->CreateCamera(cameraName);
	cameraNode->setPosition(Ogre::Vector3(-80.0f, 60.0f, -80.0f));
	static_cast<Ogre::Camera*>(cameraNode->getAttachedObject("MainCamera"))->lookAt(0.0f, 0.0f, 0.0f);
	shared_ptr<Ogre::MovableObject> cameraObject;
	cameraObject.reset(cameraNode->getAttachedObject("MainCamera"));
	std::shared_ptr<Camera> camera = make_shared<Camera>(cameraNode, cameraObject);

	//Create the game world
     InitilaiseScene();
	
	//Create input device listeners 
	application->CreateIOS();
	auto keyboard = application->GetKeyboard();
	auto mouse = application->GetMouse();

	// Create Timer
	auto timer = application->CreateTimer();
	timer->reset();
    unsigned long lastTime = timer->getMilliseconds();
	unsigned long deltaTime = 0;
	float deltaTime_s = 0.0f;
    application->GetOgreWrapper().GetRoot()->clearEventTimes();
    double timeToUpdate = 0.0;
    unsigned int index = 0;
    float elapsedTime = 0.0f;

	//Game loop
	while (!application->GetOgreWrapper().GetWindow()->isClosed())
	{

		//Evaluate the current time and the time elapsed since last frame 
		//Prepare the next iteration. 	
		unsigned long currentTime = timer->getMilliseconds();
		deltaTime = currentTime - lastTime;
		if(deltaTime == 0)  continue;

		lastTime = currentTime;
		deltaTime_s = 0.001f * float(deltaTime);

		keyboard->capture();
		mouse->capture();
		camera->Update(keyboard.get(), mouse.get());

		float coeff = 200.0f * deltaTime_s;
		Ogre::Vector3 translation(Ogre::Vector3::ZERO);

		if (keyboard->isKeyDown(OIS::KC_ESCAPE))
		{
			break;
		}
		else if (keyboard->isKeyDown(OIS::KC_V)){
			auto camera = static_cast<Ogre::Camera*>(cameraNode->getAttachedObject("MainCamera"));
			camera->setPolygonMode(Ogre::PM_WIREFRAME);
		}
		else if (keyboard->isKeyDown(OIS::KC_B))
		{
			auto camera = static_cast<Ogre::Camera*>(cameraNode->getAttachedObject("MainCamera"));
			camera->setPolygonMode(Ogre::PM_POINTS);
		}
		else if (keyboard->isKeyDown(OIS::KC_N))
		{
			auto camera = static_cast<Ogre::Camera*>(cameraNode->getAttachedObject("MainCamera"));
			camera->setPolygonMode(Ogre::PM_SOLID);
		}
		if (keyboard->isKeyDown(OIS::KC_R))
		{
			Reset();
		}
		
		const OIS::MouseState& mouseState = mouse->getMouseState();

		elapsedTime += deltaTime;
		if (elapsedTime > 100)
		{
			// do something
			elapsedTime = 0;
		}

		timeToUpdate += deltaTime_s;
		unsigned int numOfUpdates = 0;

		while (timeToUpdate > STEP_LENGTH && numOfUpdates < 60)
		{
			timeToUpdate -= STEP_LENGTH;
			Update(deltaTime_s);

			numOfUpdates++;
		}
		
		// Update window false
		bool updateWin = false;
		bool verticalSynchro = true;
		application->Run(updateWin, verticalSynchro);

		Ogre::WindowEventUtilities::messagePump();
	}

	application->CleanUp();
 
}

void GameWorld::Reset()
{

}

void GameWorld::UpdateGame(float dt)
{
	// Implement ...... 

}

void GameWorld::UpdateScene(Ogre::Vector3 &pos, Ogre::Quaternion &q )
{
	// show updated position in Ogre
}

void GameWorld::Update(float dt)
{
	UpdateGame(dt);

	float angle = 0.1 * dt;

	GameWorld::rotate_Y(angle);
	GameWorld::rotation_Y(angle);

}

Ogre::Matrix3 GameWorld::rotate_Y(float angle)
{
	Ogre::Matrix3 matY, rotateY, rotateZ;
	matY = Ogre::Matrix3(
		std::cos(angle), 0.0, std::sin(angle),
		0.0, 1.0, 0.0,
		-std::sin(angle), 0.0, std::cos(angle)
		);
	return matY;
}

void GameWorld::rotation_Y(float angle)
{
	Ogre::Matrix3 rotationMatrix = GameWorld::rotate_Y(angle);

	//Orientation
	Ogre::Quaternion q;
	q.FromRotationMatrix(rotationMatrix);
	cubeNode->rotate(q, Ogre::Node::TS_LOCAL);
	//objectSceneNode->rotate(q, Ogre::Node::TS_LOCAL);	
}


