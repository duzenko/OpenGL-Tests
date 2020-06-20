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
#include "RenderModel.h"

struct PerformanceCounters {
    int drawCalls;
    int drawTriangles;
    int textureSwitches;
};

struct Renderer {
    RenderModel house;
    RenderModel house2;
    TerrainModel terrain;

    Renderer();
    ~Renderer();
    void Render(Simulation &simulation );

    static bool wireframe;
    static bool culling;
    static float cameraAngle;

    static PerformanceCounters PC;
};