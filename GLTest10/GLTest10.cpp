#include "Simulation.h"
#include "Renderer.h"

int main() {
    Renderer renderer;

    while ( !renderer.Render() );
}