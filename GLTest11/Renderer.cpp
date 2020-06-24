#include "Renderer.h"

#include "glad.h"
#include "Util.h"
#include "Image.h"
#include "GLFW/glfw3.h"

bool Renderer::wireframe = false;
bool Renderer::culling = true;
float Renderer::cameraAngle = 0;
PerformanceCounters Renderer::PC;

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

    auto matProj = glm::infinitePerspective( glm::radians( 45.0f ), (float) viewport.width / viewport.height, 11.f );
    glMatrixMode( GL_PROJECTION );
    glLoadMatrixf( glm::value_ptr( matProj ) );
    glMatrixMode( GL_MODELVIEW );

    glPointSize( (viewport.height >> 10) + 1.f );

    glClearColor( 0.7f, 0.8f, 1, 1 );

    glEnable( GL_BLEND );
    glm::vec4 ambientLight( 1, 1, 1, 1 );
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, glm::value_ptr( ambientLight ) );

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_NORMALIZE );
    glEnable( GL_LIGHTING );
    glEnableClientState( GL_VERTEX_ARRAY );
    house.Load( "..\\assets\\House N210818\\House N210818.fbx" );
    house.modelMatrix = glm::translate( glm::vec3( -1e2, 0, 0 ) );
    house2.Load( "..\\assets\\Farmhouse Maya 2016 Updated\\farmhouse_fbx.fbx" );
    auto s = 3;
    house2.modelMatrix = glm::translate( glm::vec3( 1e2, 0, 0 ) ) * glm::scale( glm::vec3( s, s, s ) );
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

    glLightfv( GL_LIGHT0, GL_POSITION, glm::value_ptr( glm::vec4( 99, 33, 222, 0 ) ) );
    glEnable( GL_LIGHT0 );

    glPolygonMode( GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL );
    if ( culling )
        glEnable( GL_CULL_FACE );
    else
        glDisable( GL_CULL_FACE );

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    terrain.Draw();
    house.Draw();
    house2.Draw();
}