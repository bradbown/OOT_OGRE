//
//  OgreApplication.cpp
//  OOT Tutorial Ogre Framework Demo.
//  This class sets up ogre root, scene manager
//  create camera, create input ....
//
//  Created by Wen Tang on 19/01/15.
//
#include "stdafx.h"
#include "OgreApplication.h"


using namespace std;

OgreApplication::OgreApplication(void):
	root(nullptr), 
	renderWindow( nullptr), 
	sceneManager ( nullptr), 
	rootSceneNode ( nullptr), 
	viewport( nullptr),
	inputManager( nullptr), 
	mouse( nullptr), 
	keyboard( nullptr), 
	timer( nullptr)
{ }


OgreApplication::~OgreApplication(void)
{
	this->CleanUp();

	root = nullptr;
	renderWindow = nullptr;
	sceneManager = nullptr;
	rootSceneNode = nullptr;
	viewport = nullptr;
    inputManager = nullptr;
	timer = nullptr;

}


void OgreApplication::CleanUp()
{
  renderWindow->removeAllViewports();
  sceneManager ->destroyAllLights();
  sceneManager ->destroyAllManualObjects();
  sceneManager ->destroyAllEntities();
	
}

//Setup and initialize
bool OgreApplication::Initialize()
{
	
	if(!ogreWrapper.Initialize())
	{
		std::cout << __FILE__ << " (" << __LINE__ << ") - " << "Failed to initialise the application" << std::endl; 
		return false;
	}
	
	// Create  Root, RenderWindow, SceneManager, and SceneNode 
	root = ogreWrapper.GetRoot();
	renderWindow = ogreWrapper.GetWindow();
	sceneManager  = root->createSceneManager(Ogre::ST_GENERIC, "MySceneManager");
	rootSceneNode  = sceneManager ->getRootSceneNode();

	// load resources
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	return true;
}

//Create a camera and attach it to a scene node
shared_ptr<Ogre::SceneNode>  OgreApplication::CreateCamera(Ogre::String  name)
{

	// Create a camera using SceneManager. 
	Ogre::Camera* camera = sceneManager->createCamera(name);

	// Create a child scene node of the RootSceneNode,
	// Attach the camera to the child SceneNode. It will be easier then to move it in the scene.
	shared_ptr<Ogre::SceneNode> cameraNode;
	cameraNode.reset(rootSceneNode ->createChildSceneNode(name.append("Node")));
	cameraNode->attachObject(camera);

	// Setup viewport parameters 
	// Add viewport to the window with the camera 
	float viewportWidth = 1.0f;
	float viewportHeight = 1.0f;
	float viewportLeft	= (1.0f - viewportWidth) * 0.5f;
	float viewportTop = (1.0f - viewportHeight) * 0.5f;
	unsigned short viewportZOrder = 100;
	
	viewport = renderWindow->addViewport(camera, viewportZOrder, viewportLeft, viewportTop, viewportWidth, viewportHeight);

	// Set the viewport to draw the scene automatically
	// whenever the window is updated.
	viewport->setAutoUpdated(true);
	viewport->setBackgroundColour(Ogre::ColourValue(0.2f, 0.2f, 0.3f));

	// Setup the visual ratio of the camera. To make it looks real, same as the viewport.
	float ratio = static_cast<float>(viewport->getActualWidth()) / static_cast<float>(viewport->getActualHeight());
	camera->setAspectRatio(ratio);
	camera->setNearClipDistance(0.2f);
	camera->setFarClipDistance(1000.0f);
	
	// Set the window to be active
	renderWindow->setActive(true);

	// Update the content of the window not automatically.
	renderWindow->setAutoUpdated(false);

	root->clearEventTimes();

	return cameraNode;
}


shared_ptr<Ogre::SceneNode> OgreApplication::CreateEntityFromMesh( std::string mesh, std::string name)
{
	// add the second object
	Ogre::String meshName = mesh.substr(0, mesh.find(".mesh")); 

	Ogre::Entity* meshEntity =  nullptr;
	try 
	{
		meshEntity = sceneManager->getEntity(meshName);
		meshEntity = meshEntity->clone(meshName + "-" + name);
	}
	catch(Ogre::ItemIdentityException ex) 
	{
		try
		{
			meshEntity = sceneManager->createEntity(meshName, mesh);
		}
		catch (Ogre::Exception ex)
		{
			return  nullptr;
		}
	}

	// create a scene node to the entity scene node for our character
	shared_ptr<Ogre::SceneNode>  node;
    node.reset( rootSceneNode->createChildSceneNode(name));
	
	//attach the mesh
	try
	{
		node->attachObject(meshEntity);
		node->setPosition(0.0f, 0.0f, 0.0f);
	}
	catch (Ogre::InvalidParametersException ex)
	{
		return  nullptr;
	}

	return node;
}


//Set surface material property for an entity
void OgreApplication::SetEntityColour(Ogre::String  materialName, 
		                              Ogre::String  resourceGroupName,
									  Ogre::ColourValue diffuse,
									  Ogre::ColourValue specular,
									  Ogre::Real shininess)
{
	
	Ogre::MaterialPtr lMaterial = Ogre::MaterialManager::getSingleton().create(materialName, resourceGroupName);
	Ogre::Technique* lFirstTechnique = lMaterial->getTechnique(0);
	Ogre::Pass* lFirstPass = lFirstTechnique->getPass(0);

	// Lighting is allowed on this pass.
	lFirstPass->setLightingEnabled(true);

	// diffuse color is the traditional color of the object surface.
	lFirstPass->setDiffuse(diffuse);

	// Generating specular highlight on the surface
	lFirstPass->setSpecular(specular);

	lFirstPass->setShininess(shininess);
}


void OgreApplication::Run(bool updateOption, bool synchroOption)
{
	renderWindow->update(updateOption);
    renderWindow->swapBuffers();
    root->renderOneFrame();
}

// Create input and out stream
void OgreApplication::CreateIOS()
{

	//The input/output system needs an handle to a window as input, in a string format. 
	//1/ Create a size_t object that we need to use to retrieve a window handle from our render window 
	size_t windowHandle = 0;
	renderWindow->getCustomAttribute("WINDOW", &windowHandle);

	//2/convert it into a string
	std::string windowsHandleAsString="";
	{
		std::ostringstream windowHndStr;
		windowHndStr << windowHandle;
		windowsHandleAsString = windowHndStr.str();
	}

	// 3/ we translate it into a format accepted by OIS (the input library). 

	OIS::ParamList lSpecialParameters;
	lSpecialParameters.insert(std::make_pair(std::string("WINDOW"), windowsHandleAsString));

	//4/ we create the input/output system itself.
	inputManager = OIS::InputManager::createInputSystem( lSpecialParameters );
	
	//the events can be stored or not in a buffer 
	bool lBufferedKeys = false;
	bool lBufferedMouse = false;

	//creation of the keyboard-representing object 
	keyboard.reset(static_cast<OIS::Keyboard*>(inputManager->createInputObject(OIS::OISKeyboard, lBufferedKeys)));
	//creation of the mouse-representing object 
	mouse.reset(static_cast<OIS::Mouse*>(inputManager->createInputObject( OIS::OISMouse, lBufferedMouse)));
	//then must tell the mouse how much it is allowed to move. 
	unsigned int width, height, colourDepth;
    int top, left;
	renderWindow->getMetrics(width, height, colourDepth, left, top);
	const OIS::MouseState &infoSouris = mouse->getMouseState();
	infoSouris.width = width; 
	infoSouris.height = height; 
	
}

shared_ptr<Ogre::Timer> OgreApplication::CreateTimer()
{
	timer = make_shared<Ogre::Timer>(*root->getTimer());

	return timer;
}

void OgreApplication::DestroyNode(Ogre::SceneNode *node)
{
	if (node != nullptr)
	{
		node->detachAllObjects();
		node->removeAndDestroyAllChildren();
		rootSceneNode->removeAndDestroyChild(node->getName());
	}
}