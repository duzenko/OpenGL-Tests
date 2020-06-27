#include "stdafx.h"

bool Renderer::wireframe = false;
bool Renderer::culling = true;
float Renderer::cameraAngle = 0;
PerformanceCounters Renderer::PC;

struct Viewport {
    int x, y, width, height;
};

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
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnable( GL_LIGHT0 );
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

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glm::vec4 lightPos = { simulation.lightPosition * 1e5f, 1 };
    glLightfv( GL_LIGHT0, GL_POSITION, glm::value_ptr( lightPos ) );
    
    auto h = simulation.lightPosition.y;
    auto ha = fmaxf( h, 0 );
    glm::vec3 color( ha, ha, ha*ha );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, glm::value_ptr( color ) );

    color = { -h + 0.3f };
    // glLightModelfv( GL_LIGHT_MODEL_AMBIENT, glm::value_ptr( color ) );

    glm::vec3 skyColor( 0.7f, 0.8f, 1);
    skyColor += (h - 1) * 0.9;
    glClearColor( skyColor.x, skyColor.y, skyColor.z, 1);

    ListSurfaces( simulation );
    ShadowPass();
    LightPass();
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
    glDrawElements( GL_TRIANGLES, surface.indices.size(), GL_UNSIGNED_INT, surface.indices.data() );
    Renderer::PC.drawCalls++;
    Renderer::PC.drawTriangles += surface.indices.size() / 3;
    glPopMatrix();
}

void Renderer::ShadowPass() {
}

void Renderer::LightPass() {
    for ( auto s : drawSurfaces )
        R_DrawSurface( *s );
}
