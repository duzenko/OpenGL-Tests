#include <glm/ext/matrix_float4x4.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/trigonometric.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

#include "Simulation.h"
#include "Renderer.h"
#include "RenderModel.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include<windows.h>

GLFWwindow* window;
bool swapInterval = true;

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
    if ( action == GLFW_PRESS ) {
        if ( key == GLFW_KEY_ESCAPE )
            glfwSetWindowShouldClose( window, GLFW_TRUE );
        if ( key == GLFW_KEY_W )
            Renderer::wireframe = !Renderer::wireframe;
        if ( key == GLFW_KEY_C )
            Renderer::culling = !Renderer::culling;
        if ( key == GLFW_KEY_V )
            glfwSwapInterval( swapInterval = !swapInterval );
    }
    if ( action != GLFW_RELEASE ) {
        if ( key == GLFW_KEY_LEFT )
            Renderer::cameraAngle -= 1e-2f;
        if ( key == GLFW_KEY_RIGHT )
            Renderer::cameraAngle += 1e-2f;
    }
}

void PrintStats() {
    auto hnd = GetStdHandle( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo( hnd, &csbi );
    auto &pc = Renderer::PC;
    csbi.dwCursorPosition.Y -= 4;
    SetConsoleCursorPosition( hnd, csbi.dwCursorPosition );
    printf( "\n" );
    printf( "Draw calls: %d\n", pc.drawCalls );
    printf( "Draw triangles: %d\n", pc.drawTriangles );
    printf( "Texture switches: %d\n", pc.textureSwitches );
    memset( &pc, 0, sizeof( pc ) );
}

int main() {
    if ( !glfwInit() )
        return 1;

    glfwWindowHint( GLFW_MAXIMIZED, GLFW_TRUE );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 1 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
    window = glfwCreateWindow( 1280, 800, "OpenGL 1.1", NULL, NULL );
    if ( !window ) {
        glfwTerminate();
        return 2;
    }

    glfwSetKeyCallback( window, key_callback );

    glfwMakeContextCurrent( window );
    glfwSwapInterval( swapInterval );

    Renderer renderer;
    Simulation simulation;

    while ( !glfwWindowShouldClose( window ) ) {
        auto time = glfwGetTime();
        simulation.Update( time );
        renderer.Render( simulation );
        glfwSwapBuffers( window );
        glfwPollEvents();
        PrintStats();
    }

    glfwTerminate();
}

extern "C" {
    __declspec( dllexport ) int NvOptimusEnablement = 0x00000001;
    __declspec( dllexport ) int AmdPowerXpressRequestHighPerformance = 1;
}