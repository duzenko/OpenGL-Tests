#pragma once

#include "pch.h"

struct Light {
    glm::vec4 position = { 0, 0, 0, 1 };
    glm::vec3 color = { 1, 1, 1 };
};

struct Simulation {
    std::vector<CubeModel> blocks;
    CloudModel clouds;
    Light light;
    glm::vec3 skyColor = { 0, 0, 0 };
    double time = 0;

    Simulation() {
        blocks.resize( 128 );
    }

    void Update( double newTime ) {
        //newTime = 17.5;
        if ( !paused )
            time = newTime;
        glm::vec3 lightDir = glm::rotate( glm::vec3( 0, 0, 1 ), 3 - (float) time * .05f, glm::vec3( -2, 1, 0 ) );
        light.position = { lightDir * 1e5f, 1 };
        auto h = lightDir.y;
        skyColor = { 0.7f, 0.8f, 1 };
        skyColor += ( h - 1 ) * 0.9;

        auto ha = fmaxf( h, 0 );
        light.color = { ha, ha, ha * ha };
        auto cloudColor = glm::vec4( 5 * skyColor.z, 5 * skyColor.z, 5 * skyColor.z, 1 );
        for ( auto& s : clouds.surfaces )
            s.color = cloudColor;
    }

    static bool paused;
};

