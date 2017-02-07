#ifndef ACTION_COMPONENT_H
#define ACTION_COMPONENT_H

#include "Component.h"

class ActionComponent : public Component {
public:
    ActionComponent() {}

    virtual ~ActionComponent() {}

    // Initializes the object's physics specific to the implementing component
    virtual void initActionComponent() = 0;

    // Updates the object's physics by the given timestep |deltaTime|
    virtual void checkAndPerformAction(double deltaTime, double totalTime)  = 0;

private:

};

#endif
