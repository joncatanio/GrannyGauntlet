#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>

// Forward declaration to prevent circular definition issues
// GameObject.h must be included in any component file that uses it directly
class GameObject;

class Component {
public:

	Component() {}

	virtual ~Component() {}

	// Sets the GameObject that is "holding" the Component
	void setGameObjectHolder(std::shared_ptr<GameObject> holder);

protected:

	// Reference to the GameObject that uses this component (for things like position data)
	std::shared_ptr<GameObject> holder_;
};

#endif
