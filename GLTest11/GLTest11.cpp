#include "stdafx.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

GLFWwindow* window;
bool swapInterval = true;
Images images;
AbstractImages* abstractImages = &images;

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
    if ( action == GLFW_PRESS ) {
        if ( key == GLFW_KEY_ESCAPE )
            glfwSetWindowShouldClose( window, GLFW_TRUE );
        if ( key == GLFW_KEY_SPACE )
            Simulation::paused = !Simulation::paused;
        if ( key == GLFW_KEY_A )
            Renderer::ambient = !Renderer::ambient;
        if ( key == GLFW_KEY_C )
            Renderer::culling = !Renderer::culling;
        if ( key == GLFW_KEY_L )
            Renderer::lighting = !Renderer::lighting;
        if ( key == GLFW_KEY_S )
            Renderer::shadows = !Renderer::shadows;
        if ( key == GLFW_KEY_V )
            glfwSwapInterval( swapInterval = !swapInterval );
        if ( key == GLFW_KEY_W )
            Renderer::wireframe = !Renderer::wireframe;
    }
    if ( action != GLFW_RELEASE ) {
        if ( key == GLFW_KEY_LEFT )
            Renderer::cameraAngleY -= 1e-2f;
        if ( key == GLFW_KEY_RIGHT )
            Renderer::cameraAngleY += 1e-2f;
    }
}

static void cursor_position_callback( GLFWwindow* window, double xpos, double ypos ) {
    Renderer::MouseInput( xpos, ypos );
}

/*void PrintStats() {
    auto hnd = GetStdHandle( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo( hnd, &csbi );
    auto &pc = Renderer::PC;
    csbi.dwCursorPosition.Y -= 4;
    COORD sc = { csbi.srWindow.Right / 2, 0 };
    SetConsoleCursorPosition( hnd, sc );
    printf( "\n" );
    printf( "Draw calls: %d\n", pc.drawCalls );
    printf( "Draw triangles: %d\n", pc.drawTriangles );
    printf( "Texture switches: %d\n", pc.textureSwitches );
    SetConsoleCursorPosition( hnd, csbi.dwCursorPosition );
    memset( &pc, 0, sizeof( pc ) );
}*/

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
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    glfwSetCursorPosCallback( window, cursor_position_callback );


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
        //PrintStats();
    }

    glfwTerminate();
}

extern "C" {
    //__declspec( dllexport ) int NvOptimusEnablement = 0x00000001;
    //__declspec( dllexport ) int AmdPowerXpressRequestHighPerformance = 1;
}