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

struct Simulation {
    std::vector<CubeModel> blocks;
    glm::vec3 lightPosition;

    Simulation() {
        blocks.resize( 16 );
    }

    void Update( double newTime ) {
        lightPosition = glm::rotate( glm::vec3( 0, 0, 1 ), -(float)newTime*.5f, glm::vec3( -2, 1, 0 ) );
    }
};

