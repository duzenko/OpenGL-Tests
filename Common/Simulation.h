#pragma once

#include "pch.h"

struct Light {
    glm::vec4 position = { 0, 0, 0, 1 };
    glm::vec3 color = { 1, 1, 1 };
};

struct Simulation {
    double time = 0.24;
    
    Light light;
    glm::vec3 skyColor = { 0, 0, 0 };

    TerrainModel terrain;
    std::vector<CubeModel> blocks;
    VegetationModel grass;

    SkyModel sky;

    Simulation() {
        blocks.resize( 64 );
    }

    void Update( double delta );

    static bool paused;
};