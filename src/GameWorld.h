#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <ctime>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include "GameObject.h"
#include "ShaderHelper.h"

#include "BunnyPhysicsComponent.h"
#include "BunnyRenderComponent.h"
#include "CookiePhysicsComponent.h"
#include "PlayerInputComponent.h"
#include "PlayerPhysicsComponent.h"
#include "PlayerRenderComponent.h"

/* 
 * The holder class for anything that could be considered "in" or pertaining
 * to the physical world of the game.  This includes things like a list of 
 * the GameObjects in the world, collsion detection and other game object 
 * interaction logic.
 */
class GameWorld {
public:

	// Constructs a new GameWorld
	GameWorld();

	~GameWorld();

	// Adds a GameObject to the World's internal list of non-static GameObjects (could move)
	void addNonStaticGameObject(GameObject* obj);
	
	// Adds a GameObject to the World's internal list of static GameObjects (non-moving)
	void addStaticGameObject(GameObject* obj);

	// Gets the current total number of non-static GameObjects in the world
	int getNumNonStaticGameObjects();

	// Gets the current total number of static objects in the world
	int getNumStaticGameObjects();

	// Clears the world of all GameObjects
	void clearGameObjects();

	// Resets the world to it's default state (clears game objects)
	void resetWorld();

	// Calls the update function on all GameObjects in the world
	void updateGameObjects(double deltaTime, double totalTime);

	// Calls the draw function on all GameObjects in the world
	void drawGameObjects();

	// Checks to see if the passed Game Object collides with any other object in the world.
	// Returns the type of object hit or not hit
	GameObjectType checkCollision(GameObject* objToCheck);

	// Returns the number of render iterations performed so far
	unsigned long getRenderCount();

	// Returns the number of currently hit bunnies by the player in the world
	int getNumBunniesHit();

	// Called whenever a bunny has been detected to have been hit by the player camera. Updates
	// internal total of total bunny hits
	void registerBunnyHit();

private:
	// Collection of GameObjects in the world
	std::vector<GameObject*> nonStaticGameObjects_;

	// Collection of static geometry in the world - these should never move
	std::vector<GameObject*> staticGameObjects_;

	// Number of "hit" bunnies
	int numBunniesHit;

	// Number of update iterations
	unsigned long updateCount;

	// Number of render iterations
	unsigned long renderCount;

	// Adds a bunny model to the game world under the rules of 476 Lab 1
	void addBunnyToGameWorld();

    // Throw a cookie.
    void throwCookie();
};

#endif
