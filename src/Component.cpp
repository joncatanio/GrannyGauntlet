#include "Component.h"
#include "GameObject.h"

void Component::setGameObjectHolder(GameObject* holder) {
	this->holder_ = holder;
}
