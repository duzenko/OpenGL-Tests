#include "Simulation.h"
#include "Renderer.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

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
            Renderer::cameraAngle -= 1e-2;
        if ( key == GLFW_KEY_RIGHT )
            Renderer::cameraAngle += 1e-2;
    }
}

int main() {
    if ( !glfwInit() )
        return 1;

    glfwWindowHint( GLFW_MAXIMIZED, GLFW_TRUE );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 1 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
    window = glfwCreateWindow( 1280, 800, "OpenGL 1.0", NULL, NULL );
    if ( !window ) {
        glfwTerminate();
        return 2;
    }

    glfwSetKeyCallback( window, key_callback );

    glfwMakeContextCurrent( window );

    Renderer renderer;
    Simulation simulation;

    while ( !glfwWindowShouldClose( window ) ) {
        auto time = glfwGetTime();
        simulation.Update( time );
        renderer.Render( simulation );
        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    glfwTerminate();
}

extern "C" {
    //__declspec( dllexport ) int NvOptimusEnablement = 0x00000001;
    //__declspec( dllexport ) int AmdPowerXpressRequestHighPerformance = 1;
}