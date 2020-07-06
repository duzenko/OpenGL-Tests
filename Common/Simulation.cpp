#include "pch.h"

bool Simulation::paused = false;

void Simulation::Update( double newTime ) {
    //newTime = 17.5;
    float delta = 0;
    if ( !paused ) {
        delta = (float) ( newTime - time );
        time = newTime;
    }
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

    float cloudOffset = delta * cloudSpan * 1e-2f;
    auto UpdateCloud = [cloudOffset, this]( glm::vec3 v ) {
        v.x += cloudOffset;
        if ( v.x > cloudSpan )
            v.x = -cloudSpan;
        return v;
    };
    std::transform( clouds.surfaces[0].vertices.begin(), clouds.surfaces[0].vertices.end(), clouds.surfaces[0].vertices.begin(), UpdateCloud );
}