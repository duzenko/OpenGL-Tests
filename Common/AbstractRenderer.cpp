#include "pch.h"

bool AbstractRenderer::ambient = true;
bool AbstractRenderer::culling = true;
bool AbstractRenderer::lighting = true;
bool AbstractRenderer::shadows = true;
bool AbstractRenderer::wireframe = false;
float AbstractRenderer::cameraAngle = 0;
PerformanceCounters AbstractRenderer::PC;

bool compareByAlpha( DrawSurface* a, DrawSurface* b ) {
    return ( a->texture && a->texture->hasAlpha ) < ( b->texture && b->texture->hasAlpha );
}

void AbstractRenderer::ListSurfaces() {
    drawSurfaces.clear();
    for ( auto& model: renderWorld.models ) {
        for ( auto& s : model->surfaces ) {
            drawSurfaces.push_back( &s );
        }
    }
    std::sort( drawSurfaces.begin(), drawSurfaces.end(), compareByAlpha );
}