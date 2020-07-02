#include "stdafx.h"

std::vector<DrawSurface*> drawSurfaces;

Renderer::Renderer() {
    if ( !gladLoadGL() ) {
        printf( "Something went wrong!\n" );
        exit( -1 );
    }
    printf( "OpenGL %d.%d\n", GLVersion.major, GLVersion.minor );

    int maxTextureSize;
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxTextureSize );
    printf( "GL_MAX_TEXTURE_SIZE %d\n", maxTextureSize );

    Viewport viewport;
    glGetIntegerv( GL_VIEWPORT, (int*)&viewport );

    auto matProj = glm::infinitePerspective( glm::radians( 45.0f ), (float) viewport.width / viewport.height, 11.f );
    glMatrixMode( GL_PROJECTION );
    glLoadMatrixf( glm::value_ptr( matProj ) );
    glMatrixMode( GL_MODELVIEW );

    glPointSize( (viewport.height >> 10) + 1.f );

    glEnable( GL_BLEND );

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_NORMALIZE );
    glEnable( GL_LIGHTING );
    glEnable( GL_STENCIL_TEST );
    glClearStencil( 128 );
}

Renderer::~Renderer() {
}

void Renderer::Render( Simulation& simulation ) {
    glm::mat4 view;
    view = glm::lookAt( glm::vec3( 99.0f, 33, 233.0f ),
        glm::vec3( 0.0f, 0.0f, 0.0f ),
        glm::vec3( 0.0f, 1.0f, 0.0f ) );
    view = glm::rotate( view, cameraAngle, glm::vec3( 0, 1, 0 ) );
    glLoadMatrixf( glm::value_ptr( view ) );

    glPolygonMode( GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL );
    if ( culling )
        glEnable( GL_CULL_FACE );
    else
        glDisable( GL_CULL_FACE );

    auto &skyColor = simulation.skyColor;
    glClearColor( skyColor.x, skyColor.y, skyColor.z, 1);
    glDepthMask( GL_TRUE );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    ListSurfaces( simulation );
    AmbientPass();
  
    ShadowPass( glm::vec3( simulation.light.position ) );
    
    glLightfv( GL_LIGHT0, GL_DIFFUSE, glm::value_ptr( simulation.light.color ) );
    LightPass( simulation.light.position );

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void Renderer::ListSurfaces( Simulation& simulation ) {
    drawSurfaces.clear();
    for ( auto& s : terrain.surfaces ) {
        drawSurfaces.push_back( &s );
    }
    for ( auto& block : simulation.blocks ) {
        for ( auto& s : block.surfaces ) {
            drawSurfaces.push_back( &s );
        }
    }
}

void R_DrawSurface(DrawSurface &surface) {
    glPushMatrix();
    glMultMatrixf( glm::value_ptr( surface.model->modelMatrix ) );
    glColor3fv( glm::value_ptr( surface.color ) );
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, glm::value_ptr( surface.color ) );
    images.Bind( surface.texture );
    glBegin( GL_TRIANGLES );
    for ( auto index : surface.indices ) {
        if ( !surface.texCoords.empty() && surface.texture )
            glTexCoord2fv( glm::value_ptr( surface.texCoords[index] ) );
        glNormal3fv( glm::value_ptr( surface.normals[index] ) );
        glVertex3fv( glm::value_ptr( surface.vertices[index] ) );
    }
    glEnd();
    Renderer::PC.drawCalls++;
    Renderer::PC.drawTriangles += surface.indices.size() / 3;
    glPopMatrix();
}

void R_DrawSurfaceShadow( DrawSurface& surface, glm::vec3& lightPosition ) {
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
    }
    
    Renderer::PC.drawCalls++;
    Renderer::PC.drawTriangles += surface.indices.size() / 3;
}

void Renderer::AmbientPass() {
    glBlendFunc( GL_ONE, GL_ZERO );
    glDepthMask( GL_TRUE );
    glm::vec3 color1( 0.2f );
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, glm::value_ptr( color1 ) );
    for ( auto s : drawSurfaces )
        R_DrawSurface( *s );
    glm::vec3 color0( 0.0f );
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, glm::value_ptr( color0 ) );
    glDepthMask( GL_FALSE );
}

void Renderer::ShadowPass( glm::vec3& lightPosition ) {
    if ( !shadows )
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
    glBlendFunc( GL_ONE, GL_ONE );
    glEnable( GL_LIGHT0 );
    glLightfv( GL_LIGHT0, GL_POSITION, glm::value_ptr( lightPosition ) );

    glStencilFunc( GL_EQUAL, 128, 255 );
    for ( auto s : drawSurfaces )
        R_DrawSurface( *s );
    glStencilFunc( GL_ALWAYS, 0, 255 );
    glDisable( GL_LIGHT0 );
}
