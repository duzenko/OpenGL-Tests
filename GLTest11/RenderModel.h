#pragma once

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
    float scale = 1;
    std::vector<DrawSurface> surfaces;

    void Load();
    void LoadFbx();
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
        scale = 1e3;
    }
};