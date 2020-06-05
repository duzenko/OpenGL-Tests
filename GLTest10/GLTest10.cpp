#include <iostream>

#include "glad.h"
#include "GLFW/glfw3.h"
#include <glm/ext/matrix_float4x4.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/trigonometric.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define _USE_MATH_DEFINES // for C++
#include <math.h>

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
        glfwSetWindowShouldClose( window, GL_TRUE );
}

void drawSphere( double r, int lats, int longs ) {
    int i, j;
    for ( i = 0; i <= lats; i++ ) {
        double lat0 = M_PI * ( -0.5 + (float) ( i - 1 ) / lats );
        double z0 = sin( lat0 );
        double zr0 = cos( lat0 );

        double lat1 = M_PI * ( -0.5 + (float) i / lats );
        double z1 = sin( lat1 );
        double zr1 = cos( lat1 );

        glBegin( GL_QUAD_STRIP );
        for ( j = 0; j <= longs; j++ ) {
            double lng = 2 * M_PI * ( j - 1 ) / longs;
            double x = cos( lng );
            double y = sin( lng );

            glNormal3d( x * zr0, y * zr0, z0 );
            glVertex3d( r * x * zr0, r * y * zr0, r * z0 );
            glNormal3d( x * zr1, y * zr1, z1 );
            glVertex3d( r * x * zr1, r * y * zr1, r * z1 );
        }
        glEnd();
    }
}

int main() {
    GLFWwindow* window;

    if ( !glfwInit() )
        return -1;

    glfwWindowHint( GLFW_MAXIMIZED, GLFW_TRUE );
    window = glfwCreateWindow( 1280, 800, "OpenGL 1.0", NULL, NULL );
    if ( !window ) {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback( window, key_callback );

    glfwMakeContextCurrent( window );

    if ( !gladLoadGL() ) {
        printf( "Something went wrong!\n" );
        exit( -1 );
    }
    printf( "OpenGL %d.%d\n", GLVersion.major, GLVersion.minor );

    int width, height;
    glfwGetWindowSize( window, &width, &height );
    
    glm::mat4 view;
    view = glm::lookAt( glm::vec3( 0.0f, 0.0f, 3.0f ),
        glm::vec3( 0.0f, 0.0f, 0.0f ),
        glm::vec3( 0.0f, 1.0f, 0.0f ) );
    glMatrixMode( GL_MODELVIEW );
    glLoadMatrixf( glm::value_ptr( view ) );

    auto matProj = glm::perspective( glm::radians( 45.0f ), (float)width / height, 0.1f, 100.f );  
    glMatrixMode( GL_PROJECTION );
    glLoadMatrixf( glm::value_ptr(matProj) );
    glEnable( GL_LIGHTING );
    glm::vec3 lightRotNormals[8];
    glm::vec3 lightRotOrigins[8];
    for ( int light = GL_LIGHT0; light <= GL_LIGHT7; light++ ) {
        int lightNo = light - GL_LIGHT0;
        lightRotOrigins[lightNo] = glm::sphericalRand( 1.2f );
        lightRotNormals[lightNo] = glm::cross( glm::sphericalRand( 1.f ), lightRotOrigins[lightNo] );
        glEnable( light );
        glLightf( light, GL_QUADRATIC_ATTENUATION, 2 );
        glm::vec4 color( glm::abs( glm::sphericalRand( 1.f ) ), 1);
        glLightfv( light, GL_DIFFUSE, glm::value_ptr( color ) );
    }
    glClearColor( 0, 0.3f, 0, 0 );

    while ( !glfwWindowShouldClose( window ) ) {
        glClear( GL_COLOR_BUFFER_BIT );

        for ( int light = GL_LIGHT0; light <= GL_LIGHT7; light++ ) {
            int lightNo = light - GL_LIGHT0;
            float time = (float) glfwGetTime();
            auto rotated = glm::rotate( lightRotOrigins[lightNo], time * (lightNo + 1) * 1e-1f, lightRotNormals[lightNo] );
            glm::vec4 pos( rotated, 1 );
            glLightfv( light, GL_POSITION, glm::value_ptr( pos ) );
        }

        drawSphere( 1, 99, 99 );

        glfwSwapBuffers( window );

        glfwPollEvents();
    }

    glfwTerminate();
}