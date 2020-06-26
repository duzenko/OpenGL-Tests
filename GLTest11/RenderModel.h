#pragma once

#include <glm/ext/matrix_float4x4.hpp> 
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/random.hpp>

#include "Image.h"
#include <vector>

struct DrawSurface {
    glm::vec3 color = { 1, 1, 1 };
    Image* texture = nullptr;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<int> indices;

    void Draw();
};

struct RenderModel {
    glm::mat4 modelMatrix;
    std::vector<DrawSurface> surfaces;

    RenderModel() :modelMatrix( 1 ) {};
    void Draw();
};

struct TerrainModel: RenderModel {
    TerrainModel() {
        DrawSurface surface;
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
        surface.color = { 0, 1.3f, 0 };
        surfaces.push_back( surface );
        modelMatrix = glm::scale( glm::vec3( 1e3, 1e3, 1e3 ) );
    }
};

struct CubeModel : RenderModel {
    CubeModel() {
        surfaces.resize( 1 );
        drawBox();
        modelMatrix = glm::scale( glm::vec3( 1e1 ) ) * glm::translate( glm::vec3( glm::linearRand( -22,15 ), 1, glm::linearRand( -33, 7 ) ) );
    }

    void Draw() {
        auto& surface = surfaces[0];
        if ( !surface.texture ) {
            surface.texture = &Images::images[rand() % Images::images.size()];
        }
        RenderModel::Draw();
    }

private:
    void drawBox() {
        static glm::vec3 n[6] =
        {
          {-1.0, 0.0, 0.0},
          {0.0, 1.0, 0.0},
          {1.0, 0.0, 0.0},
          {0.0, -1.0, 0.0},
          {0.0, 0.0, 1.0},
          {0.0, 0.0, -1.0}
        };
        static int faces[6][4] =
        {
          {0, 1, 2, 3},
          {3, 2, 6, 7},
          {7, 6, 5, 4},
          {4, 5, 1, 0},
          {5, 6, 2, 1},
          {7, 4, 0, 3}
        };
        glm::vec3 v[8];
        int i;

        v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
        v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
        v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
        v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
        v[0][2] = v[3][2] = v[4][2] = v[7][2] = -1;
        v[1][2] = v[2][2] = v[5][2] = v[6][2] = 1;

        auto& surface = surfaces[0];

        for ( i = 0; i < 6; i++ ) {
            for ( int j = 0; j < 4; j++ ) 
                surface.normals.push_back( n[i] );
            auto i1 = i * 4;
            surface.indices.push_back( i1 + 0 );
            surface.indices.push_back( i1 + 1 );
            surface.indices.push_back( i1 + 3 );
            surface.indices.push_back( i1 + 1 );
            surface.indices.push_back( i1 + 2 );
            surface.indices.push_back( i1 + 3 );
            surface.vertices.push_back( v[faces[i][0]] );
            surface.vertices.push_back( v[faces[i][1]] );
            surface.vertices.push_back( v[faces[i][2]] );
            surface.vertices.push_back( v[faces[i][3]] );
            surface.texCoords.push_back( { 0, 0 } );
            surface.texCoords.push_back( { 0, 1 } );
            surface.texCoords.push_back( { 1, 0 } );
            surface.texCoords.push_back( { 1, 1 } );
        }
    }
};