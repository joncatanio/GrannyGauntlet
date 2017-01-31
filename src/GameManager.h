#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <memory>
#include <string>

#include "Camera.h"
#include "GameWorld.h"

// Singleton class that handles any global game state not easily categorized into other areas
class GameManager {
public:

	~GameManager() {}

	// Returns the single instance of the GameManager
	static GameManager& instance();

	// Gets the reference to the current Camera
	Camera& getCamera();

	// Sets a new reference to the current Camera
	void setCamera(Camera* newCamera);

   // Gets the reference to the current Player
   GameObject& getPlayer();

   // Sets a new reference to the current Player
   void setPlayer(GameObject* newPlayer);

	// Gets the reference to the current GameWorld
	GameWorld& getGameWorld();

	// Sets a new reference to the current GameWorld
	void setGameWorld(GameWorld* newWorld);

	// Prints information such as the number of non-static objects in the current world, fps, etc. to console
	void printInfoToConsole(float currentFPS);

private:

	GameManager() {}

	Camera* currentCamera_;

   GameObject* currentPlayer_;

	GameWorld* currentWorld_;
};

#endif
