#pragma once

#define _USE_MATH_DEFINES // for C++
#include <math.h>

#include <glm/ext/matrix_float4x4.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/trigonometric.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>

struct LightInfo {
    glm::vec3 rotationOrigin;
    glm::vec3 rotationNormal;
    glm::vec4 color;
    glm::vec4 position = glm::vec4( 0 );
    float speed = glm::linearRand( 1, 3 ) * 1e-1f;

    LightInfo() {
        rotationOrigin = glm::sphericalRand( 1.1f );
        rotationNormal = glm::cross( glm::sphericalRand( 1.f ), rotationOrigin );
        color = glm::vec4( glm::abs( glm::sphericalRand( 1.f ) ), 1 );
    }

    void update( double time ) {
        auto rotated = glm::rotate( rotationOrigin, (float) time * speed, rotationNormal );
        position = glm::vec4( rotated, 1 );
    }
};

struct Simulation {
    static const int LightsPerSphere = 40;
    LightInfo lights[LightsPerSphere];

    float sphereRotationAngle = 0;

    void Update( double time ) {
        sphereRotationAngle = (float) ( time * 3e1 );
        for ( int simLight = 0; simLight < LightsPerSphere; simLight++ ) {
            auto& lightInfo = lights[simLight];
            lightInfo.update( time );
        }
    }
};

