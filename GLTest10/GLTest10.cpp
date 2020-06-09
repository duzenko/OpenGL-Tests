#include "Simulation.h"
#include "Renderer.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

GLFWwindow* window;

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
    if(action != GLFW_PRESS)
        return;
    if ( key == GLFW_KEY_ESCAPE  )
        glfwSetWindowShouldClose( window, GLFW_TRUE );
    if ( key == GLFW_KEY_W )
        Renderer::wireframe = !Renderer::wireframe;
    if ( key == GLFW_KEY_C )
        Renderer::culling = !Renderer::culling;
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