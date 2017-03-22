#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <algorithm>
#include <ctime>
#include <iostream>
#include <iterator>
#include <vector>
#include <queue>

#include <stdio.h>
#include <stdlib.h>

#include "GameObject.h"

#include "BunnyPhysicsComponent.h"
#include "BunnyRenderComponent.h"
#include "CookiePhysicsComponent.h"
#include "OctreeNode.h"
#include "PlayerInputComponent.h"
#include "PlayerPhysicsComponent.h"
#include "PlayerRenderComponent.h"
#include "ParticleSystem.h"

// Forward-declare the Light struct in ShaderManager.h
struct Light;

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
	void addDynamicGameObject(std::shared_ptr<GameObject> obj);

   // Removes a GameObject to the World's internal list of non-static GameObjects
   void rmDynamicGameObject(std::shared_ptr<GameObject> obj);
	
	// Adds a GameObject to the World's internal list of static GameObjects (non-moving)
	void addStaticGameObject(std::shared_ptr<GameObject> obj);

	// Removes a GameObject to the World's internal list of static GameObjects (non-moving)
	void rmStaticGameObject(std::shared_ptr<GameObject> obj);

	// Sets the GameObject representing the menu
	void setMenu(std::shared_ptr<GameObject> menu);

	// Adds a new light of any type to the game world
	void addLight(const std::shared_ptr<Light> newLight);

	// Adds a new point light to the current world
	void addPointLight(const std::shared_ptr<Light> newLight);
	
	// Adds a new directional light to the current world
	void addDirectionalLight(const std::shared_ptr<Light> newLight);

	// Adds a new area light to the current world
	void addAreaLight(const std::shared_ptr<Light> newLight);

	// Adds a Particle System to the World's internal list of Particle Systems
	void addParticleSystem(std::shared_ptr<ParticleSystem> particleSystem);

    // Removes a Particle System from the World's internal list of Particle Systems
    void rmParticleSystem(std::shared_ptr<ParticleSystem> particleSystem);

	// Gets the current total number of non-static GameObjects in the world
	int getNumDynamicGameObjects();

	// Gets the current total number of static objects in the world
	int getNumStaticGameObjects();

	// Returns a reference to the list of point lights currently in the world
	const std::vector<std::shared_ptr<Light>>& getPointLights();

	// Returns a reference to the list of directional lights currently in the world
	const std::vector<std::shared_ptr<Light>>& getDirectionalLights();

	// Returns a reference to the list of area lights currently in the world
	const std::vector<std::shared_ptr<Light>>& getAreaLights();

	// Clears the world of all dynamic GameObjects
	void clearDynamicGameObjects();

	// Clears the world of all static GameObjects
	void clearStaticGameObjects();
	
	// Initializes the game world (e.g. loads the map)
	void init();

	// Calls the update function on all GameObjects in the world
	void updateGameObjects(double deltaTime, double totalTime);

    // Calls tghe update function on the menu
    void updateMenu();

	// Calls the draw function on all GameObjects in the world
	void drawGameObjects();

	// Renders all GameObjects to the shadow map
	void renderShadowMap();

   // Draws a small view port to see view frustum culling
   void drawVFCViewport();

	// Checks to see if the passed Game Object collides with any other object in the world.
	// Returns an array of all objects collided with
	std::vector<std::shared_ptr<GameObject>> checkCollision(std::shared_ptr<GameObject> objToCheck);

	// Returns the number of render iterations performed so far
	unsigned long getRenderCount();

	// Returns the number of currently hit bunnies by the player in the world
	int getNumBunniesHit();

	// Called whenever a bunny has been detected to have been hit by the player camera. Updates
	// internal total of total bunny hits
	void registerBunnyHit();

private:
	// Collection of GameObjects in the world
	std::vector<std::shared_ptr<GameObject>> dynamicGameObjects_;

	// Collection of static geometry in the world - these should never move
	std::vector<std::shared_ptr<GameObject>> staticGameObjects_;

	// Collection of Particle Systems in the world
	std::vector<std::shared_ptr<ParticleSystem>> particleSystems_;

	// GameObject that represents the menu
	std::shared_ptr<GameObject> menu_;

	// Queue of dynamic objects added to the world but that have yet to be added to the vector
	std::queue<std::shared_ptr<GameObject>> dynamicGameObjectsToAdd_;

	// Queue of static objects added to the world but that have yet to be added to the vector
	std::queue<std::shared_ptr<GameObject>> staticGameObjectsToAdd_;

    // Queue of particle systems added to the world but that have yet to be added to the vector
    std::queue<std::shared_ptr<ParticleSystem>> particleSystemsToAdd_;

	// Queue of dynamic objects removed to the world but that have yet to be removed to the vector
	std::queue<std::shared_ptr<GameObject>> dynamicGameObjectsToRemove_;

	// Queue of static objects removed to the world but that have yet to be removed to the vector
	std::queue<std::shared_ptr<GameObject>> staticGameObjectsToRemove_;

    // Queue of particle systems removed to the world but that have yet to be removed to the vector
    std::queue<std::shared_ptr<ParticleSystem>> particleSystemsToRemove_;

	// Octree of static objects that are in the world - these objects should never move.
	// If they do, the tree must be rebuilt
	OctreeNode staticGameObjectsTree_;

	// List of the lights currently in the world
	std::vector<std::shared_ptr<Light>> pointLights;

	// List of the directional lights currently in the world
	std::vector<std::shared_ptr<Light>> directionalLights;

	// List of the area lights currently in the world
	std::vector<std::shared_ptr<Light>> areaLights;

	// Number of update iterations
	unsigned long updateCount;

	// Number of render iterations
	unsigned long renderCount;

	// Number of "hit" bunnies
	int numBunniesHit;

	// Adds a bunny model to the game world under the rules of 476 Lab 1
	void addBunnyToGameWorld();

	// Updates the GameObject lists from the incoming object queues
	void updateInternalGameObjectLists();

};

#endif
