#define GLAD_GL_IMPLEMENTATION
#include "../GLTest20/gl.h"
#undef GLAD_GL_IMPLEMENTATION

#include "../GLTest10/Renderer.h"
#include "../GLTest11/Renderer.h"
#include "../GLTest20/Renderer.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

bool swapInterval = true;

void KeyCallback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
    if ( action == GLFW_PRESS ) {
        if ( key == GLFW_KEY_ESCAPE )
            glfwSetWindowShouldClose( window, GLFW_TRUE );
        if ( key == GLFW_KEY_SPACE )
            Simulation::paused = !Simulation::paused;
        if ( key == GLFW_KEY_A )
            RendererParams::ambient = !RendererParams::ambient;
        if ( key == GLFW_KEY_C )
            RendererParams::culling = !RendererParams::culling;
        if ( key == GLFW_KEY_D )
            RendererParams::deforms = !RendererParams::deforms;
        if ( key == GLFW_KEY_L )
            RendererParams::lighting = !RendererParams::lighting;
        if ( key == GLFW_KEY_S )
            RendererParams::shadows = !RendererParams::shadows;
        if ( key == GLFW_KEY_V )
            glfwSwapInterval( swapInterval = !swapInterval );
        if ( key == GLFW_KEY_W )
            RendererParams::wireframe = !RendererParams::wireframe;
    }
    if ( action != GLFW_RELEASE ) {
        if ( key == GLFW_KEY_LEFT )
            RendererParams::cameraAngleY -= 1e-2f;
        if ( key == GLFW_KEY_RIGHT )
            RendererParams::cameraAngleY += 1e-2f;
    }
}

template<typename T>
void runTest() {
    if ( !glfwInit() )
        return;

    glfwWindowHint( GLFW_MAXIMIZED, GLFW_TRUE );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 1 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
    GLFWwindow* window = glfwCreateWindow( 1280, 800, "Loading...", NULL, NULL );
    if ( !window ) {
        glfwTerminate();
        return;
    }

    glfwSetKeyCallback( window, KeyCallback );

    glfwMakeContextCurrent( window );
    glfwSwapInterval( swapInterval );

    T renderer;
    Simulation simulation;

    while ( !glfwWindowShouldClose( window ) ) {
        static auto time = glfwGetTime();
        simulation.Update( glfwGetTime() - time );
        time = glfwGetTime();
        renderer.Render( simulation );
        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    glfwTerminate();
}

int main() {
    switch ( 20 ) {
    case 10:
        runTest<GL10::Renderer>();
        break;
    case 11:
        runTest<GL11::Renderer>();
        break;
    case 20:
        runTest<GL20::Renderer>();
        break;
    default:
        break;
    }
}

extern "C" {
    __declspec( dllexport ) int NvOptimusEnablement = 0x00000001;
    //__declspec( dllexport ) int AmdPowerXpressRequestHighPerformance = 1;
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
