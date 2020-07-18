#include "pch.h"

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
    RendererParams::cameraAngleY += sensitivity*(float)(xpos - lastX);
    RendererParams::cameraAngleX -= sensitivity*(float)(ypos - lastY);
    lastX = xpos;
    lastY = ypos;
}