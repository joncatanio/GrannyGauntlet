#include "Component.h"
#include "GameObject.h"

void Component::setGameObjectHolder(std::shared_ptr<GameObject> holder) {
	this->holder_ = holder;
}
