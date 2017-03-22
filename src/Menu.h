#ifndef MENU_H
#define MENU_H

#include "GameObject.h"

class Menu {
public:

    Menu();

    ~Menu();

    void init();

    void selectedItemUp();
    void selectedItemDown();

    void setStartMenu();
    void setPauseMenu();

    void performMenuAction();

private:

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

