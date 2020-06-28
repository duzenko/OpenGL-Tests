#pragma once

#include "stdafx.h"

struct Renderer: AbstractRenderer {
    TerrainModel terrain;

    Renderer();
    ~Renderer();
    void Render(Simulation &simulation );

    static bool wireframe;
    static bool culling;
    static float cameraAngle;

    static PerformanceCounters PC;

private:
    void ListSurfaces( Simulation& simulation );
    void AmbientPass();
    void ShadowPass( glm::vec3& lightPosition );
    void LightPass( glm::vec4& lightPosition );
};