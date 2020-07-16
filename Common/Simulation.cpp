#include "pch.h"

bool Simulation::paused = false;

void Simulation::Update( double newTime ) {
    //newTime = 17.5;
    float delta = 0;
    if ( !paused ) {
        delta = (float) ( newTime - time );
        time = newTime;
    }
    float hackTime = (float) time * .05f - 0.3f;
    sky.info["time"] = hackTime;
    sky.info["delta"] = delta;
    glm::vec3 lightDir = glm::rotate( glm::vec3( 0, 0, 1 ), -hackTime + 3.1416f, glm::vec3( -2, 1, 0 ) );
    light.position = { lightDir * 1e5f, 1 };
    auto h = lightDir.y;
    skyColor = { 0.7f, 0.8f, 1 };
    skyColor += ( h - 1 ) * 0.9;
    sky.info["skyColor"] = skyColor.z;

    auto ha = fmaxf( h, 0 );
    light.color = { ha, ha, ha * ha };
}