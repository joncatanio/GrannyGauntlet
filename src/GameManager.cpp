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

ViewFrustum& GameManager::getViewFrustum() {
   return *currentViewFrustum_;
}

void GameManager::setViewFrustum(ViewFrustum* newViewFrustum) {
   currentViewFrustum_ = newViewFrustum;
}

GameWorld& GameManager::getGameWorld() {
	return *currentWorld_;
}

void GameManager::setGameWorld(GameWorld* newWorld) {
	currentWorld_ = newWorld;
}

void GameManager::printInfoToConsole(float currentFPS) {
	printf("Current FPS: %-10.2lf", currentFPS);
	std::cout << " Number of dynamic objects in world: " << currentWorld_->getNumDynamicGameObjects();
	std::cout << " Number of dynamic objects hit: " << currentWorld_->getNumBunniesHit() << '\r' <<  std::flush;
}
