#pragma once

namespace GL11 {

    struct Renderer : AbstractRenderer {
    public:
        Renderer();
        ~Renderer();
        void Render( Simulation& simulation );

        virtual void DeformSurface( DrawSurface& surf );
        void DeformSky( DrawSurface& surf );

    private:
        void AmbientPass();
        void ShadowPass( glm::vec3& lightPosition );
        void LightPass( glm::vec4& lightPosition );
    };

}