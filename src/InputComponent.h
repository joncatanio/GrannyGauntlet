#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

#include "Component.h"

class InputComponent : public Component {
public:
	InputComponent() {}

	virtual ~InputComponent() {}

	virtual void pollInput() = 0;

private:

};

#endif
