//
//  OgreWrapper.h
//  OOT Tutorial Ogre Framework Demo.
//  This class is a wrapper to setup ogre 
//
//  Created by Wen Tang on 19/01/15.
//

#pragma once

#ifndef _OGREWRAPPER_H_
#define _OGREWRAPPER_H_

// Using pre-compiled headers to speed up compilation time
// stdafx.h includes all necessary Ogre header for Ogre 
// root and render windows. Check stdfa.h file for details of
// include header files
#include "stdafx.h"

#include <memory>

// This class contains a function to initialise Ogre in one go.
class OgreWrapper
{
private:
	Ogre::Root *root;					
	Ogre::RenderWindow *window;					

private:
	// These functions will prevent the user and the class to creates copies of the original object.
	OgreWrapper(const OgreWrapper& source);						
    OgreWrapper& operator=(const OgreWrapper& source);			
    void UnLoadResources();	

public:
	
	OgreWrapper();
	~OgreWrapper();

	bool Initialize();
									
	Ogre::Root* GetRoot() const {return root;}					
    Ogre::RenderWindow* GetWindow() const { return window; }
									
};

#endif

