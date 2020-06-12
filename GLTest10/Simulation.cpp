#include "Simulation.h"

void Debri::update( Simulation& simulation ) {
    auto time = simulation.time;
    auto tick = simulation.tick;
    auto rotated = glm::rotate( rotationOrigin, (float) time * speed, rotationNormal );
    auto move = glm::normalize( position ) * -3e-0f;
    position += move * tick;
    if ( glm::length( position ) < 6.0f )
        destroyed = true;
    if ( glm::length( position ) < 6.8f && !flared ) {
        flared = true;
        simulation.CreateFlare( position );
    }
}

void FlareLight::update( Simulation& simulation ) {
    auto time = simulation.time;
    color *= 1 - simulation.tick * 3e-0f;
    if ( glm::length(color) < .1 )
        dead = true;
}