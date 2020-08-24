#include "pch.h"

bool Simulation::paused = false;

void Simulation::Update( double delta ) {
    //newTime = 17.5;
    float simDelta = 0;
    if ( !paused ) {
        time += simDelta = (float)delta * 1e-2f;
    }
    sky.info["time"] = (float)time;
    sky.info["delta"] = simDelta;
    glm::vec3 lightDir = glm::rotate( glm::vec3( 0, 0, 1 ), glm::radians( (float)( -time + 0.75 ) * 360 ), glm::vec3( -2, 1, 0 ) );
    auto h = lightDir.y;
    skyColor = { 0.7f, 0.8f, 1 };
    skyColor += ( h - 1 ) * 0.9;
    sky.info["skyColor"] = skyColor.z;

    auto ha = sqrt( fmaxf( h+0.1f, 1e-9f ) );
    light.color = glm::sqrt( glm::max( glm::vec3(lightDir.y) + glm::vec3( 0.2, 0.1, 0 ), glm::vec3(1e-9f) ) ) * 0.7f;

    if ( lightDir.y < 0 )
        lightDir = glm::normalize( glm::vec3( lightDir.x, 0, lightDir.z ) );
    light.position = { lightDir * 1e5f, 0 };
}