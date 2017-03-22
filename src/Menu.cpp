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
            shapeManager.getShape("Cube"), "Billboard", materialManager.getMaterial("Bright Green"), textureManager.getTexture("start1"));
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
            shapeManager.getShape("Cube"), "Billboard", materialManager.getMaterial("Bright Green"), textureManager.getTexture("start2"));
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
            shapeManager.getShape("Cube"), "Billboard", materialManager.getMaterial("Bright Green"), textureManager.getTexture("pause1"));
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
            shapeManager.getShape("Cube"), "Billboard", materialManager.getMaterial("Bright Green"), textureManager.getTexture("pause2"));
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




    setStartMenu();
    gameManager.getGameWorld().setMenu(currentMenu[0]);
}

void Menu::setStartMenu() {
    currentMenu = startMenu;
}

void Menu::setPauseMenu() {
    currentMenu = pauseMenu;
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
    GameManager& gameManager = GameManager::instance();
    if(selectedMenuItem == 0) {
        gameManager.toggleMenu();
        gameManager.leftMenuThisFrame_ = true;
        gameManager.menuTime_ = glfwGetTime() - gameManager.menuStartTime_;
    } else if(selectedMenuItem == 1) {
        WindowManager::instance().close();
    }

    if(inStartMenu) {
        inStartMenu = false;
        currentMenu = pauseMenu;
    }

    selectedMenuItem = 0;
    changeMenuItem();
}
