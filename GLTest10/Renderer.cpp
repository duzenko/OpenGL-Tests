#include "Renderer.h"

#include "glad.h"
#include "GLFW/glfw3.h"
#include "Utils.h"

GLFWwindow* window;

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
        glfwSetWindowShouldClose( window, GL_TRUE );
}

void SphereRenderModel::Render() {
    SurfaceVertex* vertPointer = vertices;
    for ( int i = 0; i < strips; i++ ) {
        glBegin( GL_TRIANGLE_STRIP );
        for ( int j = 0; j < vertsPerStrip; j++ ) {
            glTexCoord2fv( glm::value_ptr( vertPointer->texCoord ) );
            glNormal3fv( glm::value_ptr( vertPointer->normal ) );
            glVertex3fv( glm::value_ptr( vertPointer->position ) );
            vertPointer++;
        }
        glEnd();
    }
}

Renderer::Renderer() {
    if ( !glfwInit() )
        return;

    glfwWindowHint( GLFW_MAXIMIZED, GLFW_TRUE );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 1 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
    window = glfwCreateWindow( 1280, 800, "OpenGL 1.0", NULL, NULL );
    if ( !window ) {
        glfwTerminate();
        return;
    }

    glfwSetKeyCallback( window, key_callback );

    glfwMakeContextCurrent( window );

    if ( !gladLoadGL() ) {
        printf( "Something went wrong!\n" );
        exit( -1 );
    }
    printf( "OpenGL %d.%d\n", GLVersion.major, GLVersion.minor );
    int maxTextureSize;
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxTextureSize );
    printf( "GL_MAX_TEXTURE_SIZE %d\n", maxTextureSize );

    int width, height;
    glfwGetWindowSize( window, &width, &height );

    glm::mat4 view;
    view = glm::lookAt( glm::vec3( 0.0f, 0.0f, 33.0f ),
        glm::vec3( 0.0f, 0.0f, 0.0f ),
        glm::vec3( 0.0f, 1.0f, 0.0f ) );
    glMatrixMode( GL_MODELVIEW );
    glLoadMatrixf( glm::value_ptr( view ) );

    auto matProj = glm::perspective( glm::radians( 5.0f ), (float) width / height, 11.f, 100.f );
    glMatrixMode( GL_PROJECTION );
    glLoadMatrixf( glm::value_ptr( matProj ) );

    glEnable( GL_LIGHTING );
    glClearColor( 0, 0.3f, 0, 0 );
    glEnable( GL_BLEND );
    glEnable( GL_CULL_FACE );
    glm::vec4 ambientLight( 0, 0, 0, 1 );
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, glm::value_ptr( ambientLight ) );

    Utils::LoadBMP( "..\\assets\\2k_earth_daymap.bmp" );
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}

Renderer::~Renderer() {
    glfwTerminate();
}

bool Renderer::Render() {
    float time = (float) glfwGetTime();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glRotatef( 23, 1, 0, 0 );
    glRotated( time * 3e1, 0, 1, 0 );

    glBlendFunc( GL_ZERO, GL_ZERO );
    sphere.Render();

    glBlendFunc( GL_ONE, GL_ONE );
    for ( int simLight = 0; simLight < LightsPerSphere; ) {
        for ( int light = GL_LIGHT0; light <= GL_LIGHT7; light++, simLight++ ) {
            if ( simLight < LightsPerSphere )
                glEnable( light );
            else
                glDisable( light );
            auto& lightInfo = lights[simLight];
            lightInfo.update( time );
            glLightfv( light, GL_POSITION, glm::value_ptr( lightInfo.position ) );
            glLightfv( light, GL_DIFFUSE, glm::value_ptr( lightInfo.color ) );
            //glLightf( light, GL_QUADRATIC_ATTENUATION, 0 );
        }
        sphere.Render();
    }

    glPopMatrix();

    glfwSwapBuffers( window );

    glfwPollEvents();

    return glfwWindowShouldClose( window );
}