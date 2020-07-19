#pragma once

namespace GL20 {

    struct Renderer : public AbstractRenderer {
        Renderer();
        ~Renderer();
        void Render( Simulation& simulation );

    protected:
        virtual void DeformSurface( DrawSurface& surf );
        void DeformSky( DrawSurface& surf );

    private:
        void AmbientPass();
        void ShadowPass( glm::vec3& lightPosition );
        void LightPass( glm::vec4& lightPosition );
    };

}