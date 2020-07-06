#pragma once

#include "pch.h"

struct Light {
    glm::vec4 position = { 0, 0, 0, 1 };
    glm::vec3 color = { 1, 1, 1 };
};

struct Simulation {
    double time = 0;
    
    Light light;
    glm::vec3 skyColor = { 0, 0, 0 };

    std::vector<CubeModel> blocks;

    float cloudSpan = 2e3;
    CloudModel clouds = { cloudSpan };

    Simulation() {
        blocks.resize( 128 );
    }

    void Update( double newTime );

    static bool paused;
};

