#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Camera.h"
#include "GameWorld.h"
#include "ViewFrustum.h"
#include "ShadowMap.h"
#include "Menu.h"

// Singleton class that handles any global game state not easily categorized into other areas
class GameManager {
public:

	~GameManager() {}

	// Indicates if the game if over
	bool gameOver_ = false;

	// Returns the single instance of the GameManager
	static GameManager& instance();

	// Gets the reference to the current Camera
	Camera& getCamera();

	// Sets a new reference to the current Camera
	void setCamera(Camera* newCamera);

   // Gets the reference to the current Player
   std::shared_ptr<GameObject> getPlayer();

   // Sets a new reference to the current Player
   void setPlayer(std::shared_ptr<GameObject> newPlayer);

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

	//Shows the score
	void showScore();

   // Gets the score
   float getScore();

   // Gets the time
   float getTime();

    //Sets the initial timelimit
    void setTime(float time);

    //Counts down the remaining time
    void decreaseTime(float deltaTime);

    //Increases the remaining time
    void increaseTime(float deltaTime);

	// set a Shadow Map Object
	void setShadowMap(ShadowMap* shadowMap);

	// Returns the Shadow Map object
	ShadowMap* getShadowMap();

    // Sets the Menu Object
    void setMenu(Menu* menu);

    // Returns the Menu Object
    Menu* getMenu();

	// Toggles the menu state
	void toggleMenu();

	// Returns true if the game is currently in the menu
	bool isInMenu();

	// Toggles the hell mode stat
	void enterHell();

	// Returns if in hellmode
	bool isInHellMode();

	// Sets the global color scale for things like houses (greyscale mode)
	void setGlobalColorScale(const float colorScale);

	// Gets the global color scale for things like houses (greyscale mode)
	float getGlobalColorScale();

    static constexpr float cullFarPlane = 100.0;
    static constexpr float camFarPlane = 300.0;
    static constexpr float nearPlane = 0.01;

private:

	GameManager() {}

	Camera* currentCamera_;

   std::shared_ptr<GameObject> currentPlayer_;

	GameWorld* currentWorld_;

   ViewFrustum* currentViewFrustum_;

	ShadowMap* shadowMap_;

    Menu* menu_;

	float score_ = 0.0;

    float time_;

	bool hellMode_ = false;

	float globalColorScale_;

};

#endif
