#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>

#include "glad.h"
#include "GLFW/glfw3.h"
#include "utils.h"

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

const int LightsPerSphere = 40;

struct LightInfo {
    glm::vec3 rotationOrigin;
    glm::vec3 rotationNormal;
    glm::vec4 color;
    glm::vec4 position = glm::vec4(0);
    float speed = glm::linearRand(1, 3)*1e-1f;

    LightInfo()  {
        rotationOrigin = glm::sphericalRand( 1.1f );
        rotationNormal = glm::cross( glm::sphericalRand( 1.f ), rotationOrigin );
        color = glm::vec4( glm::abs( glm::sphericalRand( 1.f ) ), 1 );
    }

    void update( double time ) {
        auto rotated = glm::rotate( rotationOrigin, (float)time * speed, rotationNormal );
        position = glm::vec4( rotated, 1 );
    }
};

LightInfo lights[LightsPerSphere];

int main() {
    GLFWwindow* window;

    if ( !glfwInit() )
        return -1;

    glfwWindowHint( GLFW_MAXIMIZED, GLFW_TRUE );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 1 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
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

    auto matProj = glm::perspective( glm::radians( 5.0f ), (float)width / height, 11.f, 100.f );  
    glMatrixMode( GL_PROJECTION );
    glLoadMatrixf( glm::value_ptr(matProj) );

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

    while ( !glfwWindowShouldClose( window ) ) {
        float time = (float) glfwGetTime();

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        glRotatef( 23, 1, 0, 0 );
        glRotated( time * 3e1, 0, 1, 0 );

        glBlendFunc( GL_ZERO, GL_ZERO );
        Utils::drawSphere( 1, 99 );
   
        glBlendFunc( GL_ONE, GL_ONE );
        for ( int simLight = 0; simLight < LightsPerSphere;  ) {
            for ( int light = GL_LIGHT0; light <= GL_LIGHT7; light++, simLight++ ) {
                if( simLight < LightsPerSphere)
                    glEnable( light );
                else
                    glDisable( light );
                auto& lightInfo = lights[simLight];
                lightInfo.update( time );
                glLightfv( light, GL_POSITION, glm::value_ptr( lightInfo.position) );
                glLightfv( light, GL_DIFFUSE, glm::value_ptr( lightInfo.color ) );
                //glLightf( light, GL_QUADRATIC_ATTENUATION, 0 );
            }
            Utils::drawSphere( 1, 99 );
        }

        glPopMatrix();

        glfwSwapBuffers( window );

        glfwPollEvents();
    }

    glfwTerminate();
}