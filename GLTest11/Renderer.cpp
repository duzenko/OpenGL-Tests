#include "Renderer.h"

#include "glad.h"
#include "ofbx.h"
#include "Util.h"
#include "GLFW/glfw3.h"

bool Renderer::wireframe = false;
bool Renderer::culling = true;
float Renderer::cameraAngle = 0;

struct Viewport {
    int x, y, width, height;
};

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

    auto matProj = glm::infinitePerspective( glm::radians( 5.0f ), (float) viewport.width / viewport.height, 11.f );
    glMatrixMode( GL_PROJECTION );
    glLoadMatrixf( glm::value_ptr( matProj ) );
    glMatrixMode( GL_MODELVIEW );

    glPointSize( (viewport.height >> 10) + 1.f );

    glEnable( GL_BLEND );
    glm::vec4 ambientLight( 0, 0, 0, 1 );
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, glm::value_ptr( ambientLight ) );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    glEnable( GL_TEXTURE_2D );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_NORMALIZE );
}

Renderer::~Renderer() {
}

void Renderer::Render( Simulation& simulation ) {
}