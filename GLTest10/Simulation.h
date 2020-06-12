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

struct LightInfo {
    glm::vec3 color = glm::vec3( 1 );
    glm::vec3 position = glm::vec3( 0 );
    float speed = glm::linearRand( 1, 3 ) * 1e-1f;
    bool directional = false;

    LightInfo() {
    }

    void update( double time ) {
    }
};

struct SunLight: LightInfo {
    SunLight() {
        directional = true;
        position = glm::vec4( 150e3, 0, 0, 1 );
    }
};

struct Simulation;

struct FlareLight : LightInfo {
    double startTime;
    bool dead = false;
    FlareLight(double time) {
        startTime = time;
        color = glm::vec4( glm::abs( glm::sphericalRand( 1.f ) ), 1 );
    }
    void update( Simulation& simulation );
};

struct Debri {
    glm::vec3 rotationOrigin;
    glm::vec3 rotationNormal;
    glm::vec3 position = glm::sphericalRand( 33.f );
    float speed = glm::linearRand( 1, 3 ) * 1e-1f;
    int rockType = rand() % 11;
    bool destroyed = false;
    bool flared = false;

    Debri() {
        rotationOrigin = glm::sphericalRand( glm::linearRand(7.f, 11.f) );
        rotationNormal = glm::cross( glm::sphericalRand( 1.f ), rotationOrigin );
    }

    void update( Simulation& simulation );
};

struct Simulation {
    double time = 0;
    float tick = 0;
    SunLight sunLight;
    
    std::vector<FlareLight> lights;

    std::vector<Debri> debris;

    float sphereRotationAngle = 0;

    Simulation() {
    }

    void Update( double newTime ) {
        tick = fmin(1e-1f, (float) ( newTime - time ));
        time = newTime;
        sphereRotationAngle = (float) ( newTime * 3e1 );
        for ( size_t simLight = 0; simLight < lights.size(); simLight++ ) {
            auto& lightInfo = lights[simLight];
            lightInfo.update( *this );
        }
        lights.erase( std::remove_if( lights.begin(), lights.end(),
            []( FlareLight light ) { return light.dead; } ), lights.end() );
        static float newDebris = 0;
        newDebris += 1e2f * tick * rand() / RAND_MAX;
        for ( int i = 1; i < newDebris; newDebris-- )
            SpawnDebri();
        for ( size_t i = 0; i < debris.size(); i++ ) {
            auto& debri = debris[i];
            debri.update( *this );
            if ( debri.destroyed ) {
                SpawnDebri();
            }
        }
        debris.erase( std::remove_if( debris.begin(), debris.end(),
            []( Debri debri ) { return debri.destroyed; } ), debris.end() );
    }

    void SpawnDebri() {
        Debri newDebri;
        debris.push_back( newDebri );
    }

    void CreateFlare( glm::vec3& position ) {
        FlareLight newLight( time );
        newLight.position = glm::normalize( position ) * 7.f;
        lights.push_back( newLight );
    }
};

