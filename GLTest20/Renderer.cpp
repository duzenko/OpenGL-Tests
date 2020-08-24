#include "stdafx.h"

namespace GL20 {

    Renderer::Renderer() {
        //printf( "OpenGL %d.%d\n", GLVersion.major, GLVersion.minor );

        int maxTextureSize;
        glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxTextureSize );
        printf( "GL_MAX_TEXTURE_SIZE %d\n", maxTextureSize );

        Viewport viewport;
        glGetIntegerv( GL_VIEWPORT, (int*) &viewport );

        RendererParams::aspectRatio = (float) viewport.width / viewport.height;
        auto matProj = glm::infinitePerspective( 2 * atan( 1 / RendererParams::aspectRatio ), RendererParams::aspectRatio, 1.f );
        glMatrixMode( GL_PROJECTION );
        glLoadMatrixf( glm::value_ptr( matProj ) );
        glMatrixMode( GL_MODELVIEW );

        glPointSize( ( viewport.height >> 10 ) + 1.f );

        glEnable( GL_BLEND );

        glEnable( GL_DEPTH_TEST );
        glDepthFunc( GL_LEQUAL );
        glEnable( GL_NORMALIZE );
        glEnable( GL_LIGHTING );
        glEnableClientState( GL_VERTEX_ARRAY );
        glEnable( GL_STENCIL_TEST );
        glClearStencil( 128 );
        new Images();
    }

    Renderer::~Renderer() {
        delete abstractImages;
    }

    void Renderer::Render( Simulation& simulation ) {
        glm::mat4 view( 1 );
        view *= glm::rotate( RendererParams::cameraAngleX, glm::vec3( 1, 0, 0 ) );
        view *= glm::rotate( RendererParams::cameraAngleY, glm::vec3( 0, 1, 0 ) );
        view *= glm::translate( -glm::vec3( 99.0f, 33, 233.0f ) ),
            glLoadMatrixf( glm::value_ptr( view ) );

        glPolygonMode( GL_FRONT_AND_BACK, RendererParams::wireframe ? GL_LINE : GL_FILL );
        if ( RendererParams::culling )
            glEnable( GL_CULL_FACE );
        else
            glDisable( GL_CULL_FACE );

        glClearColor( 0, 0, 0, 1 );
        glDepthMask( GL_TRUE );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

        ListSurfaces();
        AmbientPass();

        ShadowPass( ( glm::vec3& ) simulation.light.position );

        glLightfv( GL_LIGHT0, GL_DIFFUSE, glm::value_ptr( simulation.light.color ) );
        LightPass( simulation.light.position );

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

    void R_DrawSurface( DrawSurface& surface ) {
        glPushMatrix();
        glMultMatrixf( glm::value_ptr( surface.model->modelMatrix ) );
        abstractImages->Bind( surface.texture );
        glColor3fv( glm::value_ptr( surface.color ) );
        glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, glm::value_ptr( surface.color ) );
        glVertexPointer( 3, GL_FLOAT, 0, surface.vertices.data() );
        if ( !surface.normals.empty() ) {
            glEnableClientState( GL_NORMAL_ARRAY );
            glNormalPointer( GL_FLOAT, 0, surface.normals.data() );
        } else
            glDisableClientState( GL_NORMAL_ARRAY );
        if ( !surface.texCoords.empty() ) {
            glEnableClientState( GL_TEXTURE_COORD_ARRAY );
            glTexCoordPointer( 2, GL_FLOAT, 0, surface.texCoords.data() );
        } else {
            glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        }
        glDrawElements( GL_TRIANGLES, surface.indices.size(), GL_UNSIGNED_INT, surface.indices.data() );
        glPopMatrix();
        RendererParams::PC.drawCalls++;
        RendererParams::PC.drawTriangles += surface.indices.size() / 3;
    }

    void R_DrawSurfaceShadow( DrawSurface& surface, glm::vec3& lightPosition ) {
        glVertexPointer( 3, GL_FLOAT, 0, surface.vertices.data() );
        glDisableClientState( GL_NORMAL_ARRAY );
        abstractImages->Bind( nullptr );
        for ( auto& edge : surface.edges ) {
            auto v1w = glm::vec3( surface.model->modelMatrix * glm::vec4( edge.v1, 1 ) );
            auto n1w = glm::vec3( surface.model->modelMatrix * glm::vec4( edge.n1, 0 ) );
            auto n2w = glm::vec3( surface.model->modelMatrix * glm::vec4( edge.n2, 0 ) );
            auto v2l = lightPosition - v1w;
            auto d1 = glm::dot( n1w, v2l );
            auto d2 = glm::dot( n2w, v2l );
            if ( d1 * d2 >= 0 )
                continue; // not a silhouette edge
            auto backFace = d1 > 0;
            auto v2w = glm::vec3( surface.model->modelMatrix * glm::vec4( edge.v2, 1 ) );
            auto v1 = backFace ? v1w : v2w;
            auto v2 = backFace ? v2w : v1w;
            auto v3 = v1 * 2.f - lightPosition;
            auto v4 = v2 * 2.f - lightPosition;
            glBegin( GL_TRIANGLE_STRIP );
            glVertex3fv( glm::value_ptr( v1 ) );
            glVertex3fv( glm::value_ptr( v2 ) );
            glVertex3fv( glm::value_ptr( v3 ) );
            glVertex3fv( glm::value_ptr( v4 ) );
            glEnd();
            RendererParams::PC.drawCalls++;
        }

        //Renderer::PC.drawTriangles += surface.indices.size() / 3;
    }

    void Renderer::AmbientPass() {
        if ( !RendererParams::ambient )
            return;
        glm::vec3 color1( 0.3f );
        glLightModelfv( GL_LIGHT_MODEL_AMBIENT, glm::value_ptr( color1 ) );
        auto alphaSurfs = false;
        glBlendFunc( GL_ONE, GL_ZERO );
        glDepthMask( GL_TRUE );
        for ( auto s : drawSurfaces ) {
            if ( !alphaSurfs && s->texture && s->texture->hasAlpha ) {
                glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
                glDepthMask( GL_FALSE );
                alphaSurfs = true;
            }
            R_DrawSurface( *s );
        }
        glm::vec3 color0( 0.0f );
        glLightModelfv( GL_LIGHT_MODEL_AMBIENT, glm::value_ptr( color0 ) );
    }

    void Renderer::ShadowPass( glm::vec3& lightPosition ) {
        if ( !RendererParams::shadows )
            return;
        glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
        glCullFace( GL_FRONT );
        glStencilOp( GL_KEEP, GL_KEEP, GL_INCR );
        for ( auto s : drawSurfaces )
            if ( s->texture )
                R_DrawSurfaceShadow( *s, lightPosition );
        glCullFace( GL_BACK );
        glStencilOp( GL_KEEP, GL_KEEP, GL_DECR );
        for ( auto s : drawSurfaces )
            if ( s->texture )
                R_DrawSurfaceShadow( *s, lightPosition );
        glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
        glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    }

    void Renderer::LightPass( glm::vec4& lightPosition ) {
        if ( !RendererParams::lighting )
            return;
        glBlendFunc( GL_ONE, GL_ONE );
        glEnable( GL_LIGHT0 );
        glLightfv( GL_LIGHT0, GL_POSITION, glm::value_ptr( lightPosition ) );

        glStencilFunc( GL_EQUAL, 128, 255 );
        for ( auto s : drawSurfaces )
            if ( !s->normals.empty() )
                R_DrawSurface( *s );
        glStencilFunc( GL_ALWAYS, 0, 255 );
        glDisable( GL_LIGHT0 );
    }

    void Renderer::DeformSurface( DrawSurface& surf ) {
        switch ( surf.deform ) {
        case DrawSurface::Deform::Sky:
            DeformSky( surf );
            return;
        }
        AbstractRenderer::DeformSurface( surf );
    }

    void Renderer::DeformSky( DrawSurface& surf ) {
        static DrawSurface s = surf;
        drawSurfaces.push_back( &s );
        s.texture = abstractImages->Get( "..\\glsl\\110\\sky" );
        Shader& shader = *(Shader*) s.texture;
        UniformsMap uniforms = surf.model->info;
        shader.UpdateUniforms( uniforms );
    }

}