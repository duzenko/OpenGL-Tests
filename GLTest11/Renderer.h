#pragma once


class Renderer : public AbstractRenderer {
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