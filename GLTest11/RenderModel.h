#pragma once

struct RenderModel;

struct SurfaceTriangle {
    glm::vec3 vertices[3] = { {}, {}, {} };
    glm::vec3& a = vertices[0];
    glm::vec3& b = vertices[1];
    glm::vec3& c = vertices[2];

    SurfaceTriangle( glm::vec3& a, glm::vec3& b, glm::vec3& c ) {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
    }

    glm::vec3 Normal() {
        return glm::normalize( glm::cross( b-a, c-a ) );
    }

    bool Contains( glm::vec3& v ) {
        return a == v || b == v || c == v;
    }

    bool Degenerate() {
        return a == b || a == c || b == c;
    }
};

struct SurfaceEdge {
    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 n1;
    glm::vec3 n2;
};

struct DrawSurface {
    glm::vec3 color = { 1, 1, 1 };
    Image* texture = nullptr;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<int> indices;

    std::vector<SurfaceEdge> edges;

    RenderModel* model = nullptr;

    void BuildEdges();
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