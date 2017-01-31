#ifndef COOKIE_THROWER_H
#define COOKIE_THROWER_H

#include <memory>
#include <stdio.h>
#include <stdlib.h>

#include "AimInputComponent.h"

class Shape;

class CookieThrower {
public:

    // Constructs a new CookieThrower
    CookieThrower();

    ~CookieThrower();

    // checks for user input and throws a Cookie
    void pollAndThrow(double deltaTime, double totalTime);

private:
    AimInputComponent* aimInputComponent;
    std::shared_ptr<Shape> cookieShape;
    float xRot = 0.0;
    float yRot = 0.0;
    float startVelocity = 20.0f;
    double previousCookieTime = 0.0;

};

#endif
