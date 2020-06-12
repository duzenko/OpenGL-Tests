#pragma once

#define _USE_MATH_DEFINES // for C++
#include <math.h>

#include <glm/ext/matrix_float4x4.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/trigonometric.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>

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