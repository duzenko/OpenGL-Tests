#include "stdafx.h"

void DrawSurface::Draw() {
    auto& surface = *this;
    glVertexPointer( 3, GL_FLOAT, 0, surface.vertices.data() );
    if ( !surface.normals.empty() ) {
        glEnableClientState( GL_NORMAL_ARRAY );
        glNormalPointer( GL_FLOAT, 0, surface.normals.data() );
    } else
        glDisableClientState( GL_NORMAL_ARRAY );
    if ( !surface.texCoords.empty() && surface.texture ) {
        glTexCoordPointer( 2, GL_FLOAT, 0, surface.texCoords.data() );
        surface.texture->Bind();
    } else {
        Images::Unbind();
    }
    glColor3fv( &surface.color.x );
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, &surface.color.x );
    glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data() );
    Renderer::PC.drawCalls++;
    Renderer::PC.drawTriangles += indices.size() / 3;
}

void RenderModel::Draw() {
    glPushMatrix();
    glMultMatrixf( glm::value_ptr( modelMatrix ) );
    for ( auto & surface : surfaces ) {
        surface.Draw();
    }
    glPopMatrix();
}