#pragma once

#include "stdafx.h"

namespace GL10 {

    struct Renderer : AbstractRenderer {

        Renderer();
        ~Renderer();
        void Render( Simulation& simulation );

        virtual void DeformSurface( DrawSurface& surf );
        void DeformSky( DrawSurface& surf );

    protected:

    private:
        void AmbientPass();
        void ShadowPass( glm::vec3& lightPosition );
        void LightPass( glm::vec4& lightPosition );
    };

}