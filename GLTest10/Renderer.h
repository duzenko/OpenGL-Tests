#pragma once

#include "stdafx.h"

struct Renderer: AbstractRenderer {

    Renderer();
    ~Renderer();
    void Render(Simulation &simulation );

private:
    void AmbientPass();
    void ShadowPass( glm::vec3& lightPosition );
    void LightPass( glm::vec4& lightPosition );
};