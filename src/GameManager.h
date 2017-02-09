#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Camera.h"
#include "GameWorld.h"
#include "ViewFrustum.h"

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

   // Gets the reference to the current View Frustum
   ViewFrustum& getViewFrustum();

   // Sets a new reference to the view frustum
   void setViewFrustum(ViewFrustum* newViewFrustum);

	// Gets the reference to the current GameWorld
	GameWorld& getGameWorld();

	// Sets a new reference to the current GameWorld
	void setGameWorld(GameWorld* newWorld);

	// Prints information such as the number of non-static objects in the current world, fps, etc. to console
	void printInfoToConsole(float currentFPS);

	//Reports the state of a cookie to count the score
	void reportScore(float score);

    //Sets the initial timelimit
    void setTime(float time);

    //Counts down the remaining time
    void decreaseTime(float deltaTime);

    //Increases the remaining time
    void increaseTime(float deltaTime);

private:

	GameManager() {}

	Camera* currentCamera_;

   GameObject* currentPlayer_;

	GameWorld* currentWorld_;

   ViewFrustum* currentViewFrustum_;

	float score_ = 0.0;

    float time_;
};

#endif
