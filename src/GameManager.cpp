#include "GameManager.h"

GameManager& GameManager::instance() {
	static GameManager *instance = new GameManager();
	return *instance;
}

Camera& GameManager::getCamera() {
	return *currentCamera_;
}

void GameManager::setCamera(Camera* newCamera) {
	currentCamera_ = newCamera;
}

GameObject& GameManager::getPlayer() {
   return *currentPlayer_;
}

void GameManager::setPlayer(GameObject* newPlayer) {
   currentPlayer_ = newPlayer;
}

GameWorld& GameManager::getGameWorld() {
	return *currentWorld_;
}

void GameManager::setGameWorld(GameWorld* newWorld) {
	currentWorld_ = newWorld;
}

void GameManager::printInfoToConsole(float currentFPS) {
	printf("Current FPS: %-10.2lf", currentFPS);
	std::cout << " Number of non-static objects in world: " << currentWorld_->getNumNonStaticGameObjects();
	std::cout << " Number of non-static objects hit: " << currentWorld_->getNumBunniesHit() << '\r' <<  std::flush;
}
