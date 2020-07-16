#include "pch.h"

bool AbstractRenderer::ambient = true;
bool AbstractRenderer::culling = true;
bool AbstractRenderer::lighting = true;
bool AbstractRenderer::shadows = true;
bool AbstractRenderer::wireframe = false;
float AbstractRenderer::cameraAngleX = 0;
float AbstractRenderer::cameraAngleY = 0;
float AbstractRenderer::aspectRatio = 1;
PerformanceCounters AbstractRenderer::PC;

bool compareByAlpha( DrawSurface* a, DrawSurface* b ) {
    return ( a->texture && a->texture->hasAlpha ) < ( b->texture && b->texture->hasAlpha );
}

void AbstractRenderer::ListSurfaces() {
    drawSurfaces.clear();
    for ( auto& model: renderWorld.models ) {
        for ( auto& s : model->surfaces ) {
            if ( s.deform == DrawSurface::Deform::None )
                drawSurfaces.push_back( &s );
            else
                DeformSurface( s );
        }
    }
    std::sort( drawSurfaces.begin(), drawSurfaces.end(), compareByAlpha );
}

void AbstractRenderer::DeformSurface( DrawSurface& surf ) {
    drawSurfaces.push_back( &surf );
}

void AbstractRenderer::MouseInput( double xpos, double ypos ) {
    static double lastX = xpos, lastY = ypos;
    float sensitivity = 1e-3f;
    cameraAngleY += sensitivity*(float)(xpos - lastX);
    cameraAngleX -= sensitivity*(float)(ypos - lastY);
    lastX = xpos;
    lastY = ypos;
}