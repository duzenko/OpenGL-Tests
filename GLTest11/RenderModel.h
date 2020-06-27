#pragma once

struct RenderModel;

struct DrawSurface {
    glm::vec3 color = { 1, 1, 1 };
    Image* texture = nullptr;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<int> indices;

    RenderModel* model = nullptr;
};

struct RenderModel {
    glm::mat4 modelMatrix;
    std::vector<DrawSurface> surfaces;
    std::string name = "unnamed";

    RenderModel() :modelMatrix( 1 ) {};

    DrawSurface &add() {
        int idx = surfaces.size();
        surfaces.resize( idx + 1 );
        auto &s = surfaces[idx];
        s.model = this;
        return s;
    }
};

struct TerrainModel: RenderModel {
    TerrainModel() {
        name = "terrain";
        modelMatrix = glm::scale( glm::vec3( 1e3, 1e3, 1e3 ) );
        DrawSurface& surface = add();
        surface.vertices = {
            {-1, 0, -1},
            {-1, 0, 1},
            {1, 0, -1},
            {1, 0, 1},
        };
        surface.normals = {
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
        };
        surface.indices = { 0, 1, 2, 1, 3, 2 };
        surface.color = { 0, 0.3f, 0 };
    }
};

struct CubeModel : RenderModel {
    CubeModel();
private:
    void drawBox();
};