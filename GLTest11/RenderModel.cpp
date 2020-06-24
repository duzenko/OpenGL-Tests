#include "RenderModel.h"

#include <thread>
#include <glm/mat4x4.hpp>

#include "glad.h"
#include "Renderer.h"
#include "Util.h"
#include <filesystem> // C++17 (or Microsoft-specific implementation in C++14)

void DrawSurface::Draw() {
    auto& surface = *this;
    glVertexPointer( 3, GL_DOUBLE, 0, surface.vertices );
    if ( surface.normals ) {
        glEnableClientState( GL_NORMAL_ARRAY );
        glNormalPointer( GL_DOUBLE, 0, surface.normals );
    } else
        glDisableClientState( GL_NORMAL_ARRAY );
    if ( surface.texCoords && surface.texture ) {
        glTexCoordPointer( 2, GL_DOUBLE, 0, surface.texCoords );
        surface.texture->Bind();
    } else {
        Images::Unbind();
    }
    glColor3fv( &surface.color.r );
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, &surface.color.r );
    glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data() );
    Renderer::PC.drawCalls++;
    Renderer::PC.drawTriangles += indices.size() / 3;
}

void RenderModel::LoadFbx( char* fileName ) {
    std::filesystem::path fnPath( fileName );
    FILE* fp;
    //auto fn = ;
    fopen_s( &fp, fileName, "rb" );
    if ( !fp ) return;
    fseek( fp, 0, SEEK_END );
    long file_size = ftell( fp );
    fseek( fp, 0, SEEK_SET );
    auto* content = new ofbx::u8[file_size];
    fread( content, 1, file_size, fp );
    ofbx::IScene* g_scene = ofbx::load( ( ofbx::u8* )content, file_size, ( ofbx::u64 )ofbx::LoadFlags::TRIANGULATE );
    surfaces.reserve( g_scene->getMeshCount() );
    for ( int j = 0; j < g_scene->getMeshCount(); j++ ) {
        auto mesh = g_scene->getMesh( j );
        auto geometry = mesh->getGeometry();
        DrawSurface surface;
        surface.texture = nullptr;
        if ( mesh->getMaterialCount() != 1 ) {
            printf( "Material count %d\n", mesh->getMaterialCount() );
        } else {
            auto material = mesh->getMaterial( 0 );
            surface.color = material->getDiffuseColor();
            for ( int t = 0; t < ofbx::Texture::TextureType::COUNT; t++ ) {
                auto tt = ( ofbx::Texture::TextureType )t;
                if ( material->getTexture( tt ) ) {
                    auto fn = material->getTexture( tt )->getFileName();
                    auto len = fn.end - fn.begin;
                    char* s = (char*) _malloca( len + 1 );
                    memcpy( s, fn.begin, len );
                    s[len] = '\0';
                    std::filesystem::path full( s ); // Construct the path from a string.
                    auto path = string_format( "%s\\%s", 
                        fnPath.parent_path().string().c_str(), full.filename().string().c_str() );
                    surface.texture = Images::get( path );
                    break;
                }
            }
        }
        auto ind = geometry->getFaceIndices();
        surface.vertices = geometry->getVertices();
        surface.normals = geometry->getNormals();
        surface.texCoords = geometry->getUVs();
        for ( int i = 0; i < geometry->getIndexCount(); i++ ) {
            int index = ind[i];
            index = index < 0 ? -index - 1 : index;
            surface.indices.push_back( index );
        }
        surfaces.push_back( surface );
    }
}

void RenderModel::Load(char *fileName) {
    std::thread t1( &RenderModel::LoadFbx, this, fileName );
    t1.detach();
}

void RenderModel::Draw() {
    glPushMatrix();
    glMultMatrixf( glm::value_ptr( modelMatrix ) );
    for ( auto & surface : surfaces ) {
        surface.Draw();
    }
    glPopMatrix();
}