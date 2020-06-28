#pragma once


class Renderer : public AbstractRenderer {
public:
    Renderer();
    ~Renderer();
    void Render( Simulation& simulation );

private:

    void ListSurfaces( Simulation& simulation );
    void AmbientPass();
    void ShadowPass( glm::vec3& lightPosition );
    void LightPass( glm::vec4& lightPosition );
};