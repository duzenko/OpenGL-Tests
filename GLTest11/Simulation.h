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

struct Simulation {
    Simulation() {
    }

    void Update( double newTime ) {
    }
};

