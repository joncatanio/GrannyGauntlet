#include "Menu.h"

#include <GLFW/glfw3.h>

#include "GameManager.h"
#include "MaterialManager.h"
#include "ShapeManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "WindowManager.h"
#include "MenuBillboardPhysicsComponent.h"
#include "BillboardRenderComponent.h"


Menu::Menu() {
}

Menu::~Menu() {

}

void Menu::init() {
    GameManager& gameManager = GameManager::instance();
    MaterialManager& materialManager = MaterialManager::instance();
    ShapeManager& shapeManager = ShapeManager::instance();
    ShaderManager& shaderManager = ShaderManager::instance();
    TextureManager& textureManager = TextureManager::instance();


    MenuBillboardPhysicsComponent* menuBillboardPhysicsComponentS1 = new MenuBillboardPhysicsComponent();
    BillboardRenderComponent* billboardRenderComponentS1 = new BillboardRenderComponent(
            shapeManager.getShape("Cube"), "Menu", materialManager.getMaterial("Bright Green"), textureManager.getTexture("start1"));
    std::shared_ptr<GameObject> start1 = std::make_shared<GameObject>(GameObjectType::DYNAMIC_OBJECT,
                                                                         glm::vec3(0.0),
                                                                         glm::vec3(0.0f, 1.0f, 0.0f),
                                                                         0.0f,
                                                                         glm::vec3(5.0f),
                                                                         nullptr,
                                                                         menuBillboardPhysicsComponentS1,
                                                                         billboardRenderComponentS1,
                                                                         nullptr
    );
    start1->initComponents();
    startMenu.push_back(start1);

    MenuBillboardPhysicsComponent* menuBillboardPhysicsComponentS2 = new MenuBillboardPhysicsComponent();
    BillboardRenderComponent* billboardRenderComponentS2 = new BillboardRenderComponent(
            shapeManager.getShape("Cube"), "Menu", materialManager.getMaterial("Bright Green"), textureManager.getTexture("start2"));
    std::shared_ptr<GameObject> start2 = std::make_shared<GameObject>(GameObjectType::DYNAMIC_OBJECT,
                                                                      glm::vec3(0.0),
                                                                      glm::vec3(0.0f, 1.0f, 0.0f),
                                                                      0.0f,
                                                                      glm::vec3(5.0f),
                                                                      nullptr,
                                                                      menuBillboardPhysicsComponentS2,
                                                                      billboardRenderComponentS2,
                                                                      nullptr
    );
    start2->initComponents();
    startMenu.push_back(start2);

    MenuBillboardPhysicsComponent* menuBillboardPhysicsComponentP1 = new MenuBillboardPhysicsComponent();
    BillboardRenderComponent* billboardRenderComponentP1 = new BillboardRenderComponent(
            shapeManager.getShape("Cube"), "Menu", materialManager.getMaterial("Bright Green"), textureManager.getTexture("pause1"));
    std::shared_ptr<GameObject> pause1 = std::make_shared<GameObject>(GameObjectType::DYNAMIC_OBJECT,
                                                                      glm::vec3(0.0),
                                                                      glm::vec3(0.0f, 1.0f, 0.0f),
                                                                      0.0f,
                                                                      glm::vec3(5.0f),
                                                                      nullptr,
                                                                      menuBillboardPhysicsComponentP1,
                                                                      billboardRenderComponentP1,
                                                                      nullptr
    );
    pause1->initComponents();
    pauseMenu.push_back(pause1);

    MenuBillboardPhysicsComponent* menuBillboardPhysicsComponentP2 = new MenuBillboardPhysicsComponent();
    BillboardRenderComponent* billboardRenderComponentP2 = new BillboardRenderComponent(
            shapeManager.getShape("Cube"), "Menu", materialManager.getMaterial("Bright Green"), textureManager.getTexture("pause2"));
    std::shared_ptr<GameObject> pause2 = std::make_shared<GameObject>(GameObjectType::DYNAMIC_OBJECT,
                                                                      glm::vec3(0.0),
                                                                      glm::vec3(0.0f, 1.0f, 0.0f),
                                                                      0.0f,
                                                                      glm::vec3(5.0f),
                                                                      nullptr,
                                                                      menuBillboardPhysicsComponentP2,
                                                                      billboardRenderComponentP2,
                                                                      nullptr
    );
    pause2->initComponents();
    pauseMenu.push_back(pause2);


    MenuBillboardPhysicsComponent* menuBillboardPhysicsComponentW1 = new MenuBillboardPhysicsComponent();
    BillboardRenderComponent* billboardRenderComponentW1 = new BillboardRenderComponent(
            shapeManager.getShape("Cube"), "Menu", materialManager.getMaterial("Bright Green"), textureManager.getTexture("won1"));
    std::shared_ptr<GameObject> won1 = std::make_shared<GameObject>(GameObjectType::DYNAMIC_OBJECT,
                                                                      glm::vec3(0.0),
                                                                      glm::vec3(0.0f, 1.0f, 0.0f),
                                                                      0.0f,
                                                                      glm::vec3(5.0f),
                                                                      nullptr,
                                                                      menuBillboardPhysicsComponentW1,
                                                                      billboardRenderComponentW1,
                                                                      nullptr
    );
    won1->initComponents();
    winMenu.push_back(won1);

    MenuBillboardPhysicsComponent* menuBillboardPhysicsComponentW2 = new MenuBillboardPhysicsComponent();
    BillboardRenderComponent* billboardRenderComponentW2 = new BillboardRenderComponent(
            shapeManager.getShape("Cube"), "Menu", materialManager.getMaterial("Bright Green"), textureManager.getTexture("won2"));
    std::shared_ptr<GameObject> won2 = std::make_shared<GameObject>(GameObjectType::DYNAMIC_OBJECT,
                                                                      glm::vec3(0.0),
                                                                      glm::vec3(0.0f, 1.0f, 0.0f),
                                                                      0.0f,
                                                                      glm::vec3(5.0f),
                                                                      nullptr,
                                                                      menuBillboardPhysicsComponentW2,
                                                                      billboardRenderComponentW2,
                                                                      nullptr
    );
    won2->initComponents();
    winMenu.push_back(won2);



    MenuBillboardPhysicsComponent* menuBillboardPhysicsComponentL1 = new MenuBillboardPhysicsComponent();
    BillboardRenderComponent* billboardRenderComponentL1 = new BillboardRenderComponent(
            shapeManager.getShape("Cube"), "Menu", materialManager.getMaterial("Bright Green"), textureManager.getTexture("lost1"));
    std::shared_ptr<GameObject> lost1 = std::make_shared<GameObject>(GameObjectType::DYNAMIC_OBJECT,
                                                                      glm::vec3(0.0),
                                                                      glm::vec3(0.0f, 1.0f, 0.0f),
                                                                      0.0f,
                                                                      glm::vec3(5.0f),
                                                                      nullptr,
                                                                      menuBillboardPhysicsComponentL1,
                                                                      billboardRenderComponentL1,
                                                                      nullptr
    );
    lost1->initComponents();
    lostMenu.push_back(lost1);


    MenuBillboardPhysicsComponent* menuBillboardPhysicsComponentL2 = new MenuBillboardPhysicsComponent();
    BillboardRenderComponent* billboardRenderComponentL2 = new BillboardRenderComponent(
            shapeManager.getShape("Cube"), "Menu", materialManager.getMaterial("Bright Green"), textureManager.getTexture("lost2"));
    std::shared_ptr<GameObject> lost2 = std::make_shared<GameObject>(GameObjectType::DYNAMIC_OBJECT,
                                                                      glm::vec3(0.0),
                                                                      glm::vec3(0.0f, 1.0f, 0.0f),
                                                                      0.0f,
                                                                      glm::vec3(5.0f),
                                                                      nullptr,
                                                                      menuBillboardPhysicsComponentL2,
                                                                      billboardRenderComponentL2,
                                                                      nullptr
    );
    lost2->initComponents();
    lostMenu.push_back(lost2);

    setStartMenu();
    gameManager.getGameWorld().setMenu(currentMenu[0]);
}

void Menu::setStartMenu() {
    currentMenu = startMenu;
    pauseMenuActive = false;
    changeMenuItem();
}

void Menu::setPauseMenu() {
    currentMenu = pauseMenu;
    pauseMenuActive = true;
    changeMenuItem();
}

void Menu::setWonMenu() {
    currentMenu = winMenu;
    pauseMenuActive = false;
    changeMenuItem();
}

void Menu::setLostMenu() {
    currentMenu = lostMenu;
    pauseMenuActive = false;
    changeMenuItem();
}

void Menu::changeMenuItem() {
    GameManager& gameManager = GameManager::instance();
    gameManager.getGameWorld().setMenu(currentMenu[selectedMenuItem]);
}

void Menu::selectedItemUp() {
    selectedMenuItem = (selectedMenuItem + 1) % currentMenu.size();
    changeMenuItem();
}

void Menu::selectedItemDown() {
    selectedMenuItem = (selectedMenuItem - 1) % currentMenu.size();
    changeMenuItem();
}

void Menu::performMenuAction() {
    //TODO(nurgan) check for state
    if(selectedMenuItem == 0) {
		if (currentMenu == winMenu) {
			WindowManager::instance().close();
		}
		else {
			toggleMenuActive();
			leftMenuThisFrame_ = true;
			menuTime_ = glfwGetTime() - menuStartTime_;

			GameManager& gameManager = GameManager::instance();
			if (currentMenu == lostMenu && gameManager.getTime() <= 0.0f) {

				// If the player has lost due to running out of time let them continue to explore
				// but nullify the run
				gameManager.increaseTime(9999.0f);
				gameManager.reportScore(-999999.0f);
				gameManager.gameOver_ = false;
				setPauseMenu();
			}
		}
    } else if(selectedMenuItem == 1) {
        WindowManager::instance().close();
    }

    if(inStartMenu) {
        inStartMenu = false;
        setPauseMenu();
    }

    selectedMenuItem = 0;
    changeMenuItem();
}

void Menu::toggleMenuActive() {
    isActive_ = !isActive_;
}

bool Menu::isActive() {
    return isActive_;
}

bool Menu::isInPause() {
    return pauseMenuActive;
}