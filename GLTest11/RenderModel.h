#pragma once

#include <glm/ext/matrix_float4x4.hpp> 
#include <glm/gtx/transform.hpp> 
#include "ofbx.h"
#include "Image.h"
#include <vector>

struct DrawSurface {
    ofbx::Color color = { 1, 1, 1 };
    Image* texture = nullptr;
    const ofbx::Vec3* vertices = nullptr;
    const ofbx::Vec3* normals = nullptr;
    const ofbx::Vec2* texCoords = nullptr;
    std::vector<int> indices;

    void Draw();
};

struct RenderModel {
    glm::mat4 modelMatrix;
    std::vector<DrawSurface> surfaces;

    RenderModel() :modelMatrix( 1 ) {};
    void Load( char* fileName );
    void LoadFbx( char* fileName );
    void Draw();
};

struct TerrainModel: RenderModel {
    ofbx::Vec3 vert[4] = {
        {-1, 0, -1},
        {-1, 0, 1},
        {1, 0, -1},
        {1, 0, 1},
    };
    ofbx::Vec3 norm[4] = {
        {0, 1, 0},
    };

    TerrainModel() {
        DrawSurface surface;
        surface.vertices = vert;
        surface.normals = norm;
        surface.indices = { 0, 1, 2, 1, 3, 2 };
        surface.color = { 0, .3f, 0 };
        surfaces.push_back( surface );
        modelMatrix = glm::scale( glm::vec3( 1e3, 1e3, 1e3 ) );
    }
};