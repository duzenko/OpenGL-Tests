#pragma once

#define _USE_MATH_DEFINES // for C++
#include <math.h>

#include "Simulation.h"
#include "Image.h"

struct Renderer {
    int dlStars;
    int dlEarth;
    int dlMoon;
    int dlSun;

    Renderer();
    ~Renderer();
    void Render(Simulation &simulation );

    static bool wireframe;
    static bool culling;
    static float cameraAngle;
};