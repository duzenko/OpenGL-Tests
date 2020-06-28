#pragma once

#define _USE_MATH_DEFINES // for C++
#include <math.h>
#include <vector>

#include <glm/ext/matrix_float4x4.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/trigonometric.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "RenderModel.h"

struct Light {
    glm::vec4 position = { 0, 0, 0, 1 };
    glm::vec3 color = { 1, 1, 1 };
};

struct Simulation {
    std::vector<CubeModel> blocks;
    Light light;
    glm::vec3 skyColor = { 0, 0, 0 };

    Simulation() {
        blocks.resize( 4 );
    }

    void Update( double newTime ) {
        newTime = 17.5;
        glm::vec3 lightDir = glm::rotate( glm::vec3( 0, 0, 1 ), 3 - (float) newTime * .05f, glm::vec3( -2, 1, 0 ) );
        light.position = { lightDir * 1e5f, 1 };
        auto h = lightDir.y;
        skyColor = { 0.7f, 0.8f, 1 };
        skyColor += ( h - 1 ) * 0.9;

        auto ha = fmaxf( h, 0 );
        light.color = { ha, ha, ha * ha };
    }
};

