#include "GameManager.h"

#include "AudioManager.h"

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

std::shared_ptr<GameObject> GameManager::getPlayer() {
   return currentPlayer_;
}

void GameManager::setPlayer(std::shared_ptr<GameObject> newPlayer) {
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

void GameManager::reportScore(float score) {
	score_ += score;
}

void GameManager::setTime(float time) {
    time_ = time;
}

void GameManager::decreaseTime(float deltaTime) {
    time_ -= deltaTime;
	if(time_ <= 0.0) {
		gameOver_ = true;
	}
}

void GameManager::increaseTime(float deltaTime) {
    time_ += deltaTime;
}

void GameManager::setShadowMap(ShadowMap *shadowMap) {
    shadowMap_ = shadowMap;
}

ShadowMap* GameManager::getShadowMap() {
    return shadowMap_;
}

void GameManager::setMenu(Menu *menu) {
    menu_ = menu;
}

Menu* GameManager::getMenu() {
    return  menu_;
}

void GameManager::enterHell() {
    if(!hellMode_) {
        hellMode_ = true;
        currentWorld_->resetDeliverables();
        AudioManager::instance().swapSoundtrack();
        setTime(666.0f);
    }
}

bool GameManager::isInHellMode() {
    return hellMode_;
}

void GameManager::showScore() {
    if(gameOver_) {
        if(time_ <= 0.0) {
            menu_->setLostMenu();
            menu_->toggleMenuActive();
        } else {
            reportScore(time_ * 100);
            menu_->setWonMenu();
            menu_->toggleMenuActive();
        }
    }
	std::cout << "SCORE " << score_ << std::endl;
}

float GameManager::getScore() {
   return score_;
}

float GameManager::getTime() {
   return time_;
}
