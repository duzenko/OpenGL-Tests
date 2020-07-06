#include "pch.h"

std::filesystem::directory_iterator randomTextureDI( "..\\assets\\pdtextures" );
std::vector<std::string> randomTextures;
RenderWorld renderWorld;

void DrawSurface::BuildEdges() {
    for ( size_t i = 0; i < indices.size(); i += 3 ) {
        SurfaceTriangle t1 = {
            vertices[indices[i]],
            vertices[indices[i+1]],
            vertices[indices[i+2]],
        };
        assert( !t1.Degenerate() );
        for ( size_t j = i+3; j < indices.size(); j += 3 ) {
            SurfaceTriangle t2 = {
                vertices[indices[j]],
                vertices[indices[j + 1]],
                vertices[indices[j + 2]],
            };
            assert( !t2.Degenerate() );
            std::vector<glm::vec3> found;
            bool reverse = false;
            for ( int k = 0; k < 3; k++ ) {
                if ( t2.Contains( t1.vertices[k] ) )
                    found.push_back( t1.vertices[k] );
                else
                    if ( k == 1 )
                        reverse = true;
            }
            assert( found.size() < 3 );
            if ( found.size() < 2 )
                continue;
            SurfaceEdge edge;
            if ( !reverse ) {
                edge.v1 = found[1];
                edge.v2 = found[0];
            } else {
                edge.v1 = found[0];
                edge.v2 = found[1];
            }
            edge.n1 = t1.Normal();
            edge.n2 = t2.Normal();
            if ( edge.n1 == edge.n2 )
                continue; // rect halves
            edges.push_back( edge );
        }
    }
}

void DrawSurface::ForEachTriangle( void ( *callback )( glm::vec3* ) ) {
    for ( size_t i = 0; i < indices.size(); i += 3 ) {
        callback( &vertices[indices[i]] );
    }
}

RenderModel::RenderModel() :modelMatrix( 1 ) {
    renderWorld.models.insert( this );
};

RenderModel::~RenderModel() {
    renderWorld.models.erase( this );
}

CubeModel::CubeModel() {
    name = "cube #XX";
    modelMatrix = glm::scale( glm::vec3( 1e1 ) ) * glm::translate( glm::vec3( glm::linearRand( -15, 15 ), 1, glm::linearRand( -35, 15 ) ) );
    drawBox();
    auto& surface = surfaces[0];
    if ( randomTextures.empty() )
        for ( auto entry : randomTextureDI )
            randomTextures.push_back( entry.path().string() );
    surface.texture = abstractImages->Get( randomTextures[rand() % randomTextures.size()] );
    surface.BuildEdges();
}

void CubeModel::drawBox() {
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

    auto& surface = add();

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

CloudModel::CloudModel( float span ) {
    name = "cloud #XX";
    DrawSurface& surface = add();
    surface.color = { 1, 1, 1, 1 };
    surface.texture = abstractImages->Get( "..\\assets\\hiclipart.com 1.png" );
    std::vector<glm::vec3> verts = {
        {-1, 0, -1},
        {-1, 0, 1},
        {1, 0, -1},
        {1, 0, 1},
    };
    auto tVerts = verts;
    std::vector<glm::vec2> texCoords = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1},
    };
    std::vector<int> indices = { 0, 2, 1, 1, 2, 3 };
    auto tIndices = indices;
    for ( int i = 0; i < 1280; i++ ) {
        auto translate = glm::translate( glm::vec3( glm::linearRand( -span, span ), 1e2, glm::linearRand( -span, span ) ) );
        auto scale = glm::scale( glm::vec3( glm::linearRand<float>( 3, 33 ), 1, glm::linearRand<float>( 3, 33 ) ) );
        glm::mat4 mat;
        int offset;
        mat = translate * scale;
        offset = surface.vertices.size();
        auto matMul = [mat]( glm::vec3 v ) {
            return glm::vec3( mat * glm::vec4( v, 1 ) );
        };
        auto idxOffset = [offset]( int idx ) {
            return idx + offset;
        };
        std::transform( verts.begin(), verts.end(), tVerts.begin(), matMul );
        surface.vertices.insert( surface.vertices.end(), tVerts.begin(), tVerts.end() );
        surface.texCoords.insert( surface.texCoords.begin(), texCoords.begin(), texCoords.end() );
        std::transform( indices.begin(), indices.end(), tIndices.begin(), idxOffset );
        surface.indices.insert( surface.indices.begin(), tIndices.begin(), tIndices.end() );
    }
}