#ifndef MENU_H
#define MENU_H

#include "GameObject.h"

class Menu {
public:

    Menu();

    ~Menu();

    // Indicates if the menu was left this frame
    bool leftMenuThisFrame_ = false;

    // Time spent in menu
    float menuTime_ = 0.0;

    // Time when the menu was entered
    float menuStartTime_ = 0.0;

    void init();

    void selectedItemUp();
    void selectedItemDown();

    void setStartMenu();
    void setPauseMenu();

    void performMenuAction();

    void toggleMenuActive();
    bool isActive();

private:

    bool isActive_ = true;

    int selectedMenuItem = 0;
    bool inStartMenu = true;

    std::vector<std::shared_ptr<GameObject>> currentMenu;
    std::vector<std::shared_ptr<GameObject>> startMenu;
    std::vector<std::shared_ptr<GameObject>> pauseMenu;
    std::vector<std::shared_ptr<GameObject>> winMenu;
    std::vector<std::shared_ptr<GameObject>> lostMenu;

    void changeMenuItem();
};

#endif

