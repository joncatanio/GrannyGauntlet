#ifndef PLAYER_INPUT_COMPONENT_H
#define PLAYER_INPUT_COMPONENT_H

#include "InputComponent.h"

class PlayerInputComponent : public InputComponent {
public:
   PlayerInputComponent();

   ~PlayerInputComponent();

   void pollInput();
private: 

};

#endif
