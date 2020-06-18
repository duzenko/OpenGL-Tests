#include "Renderer.h"

#include "glad.h"
#include "ofbx.h"
#include "Util.h"
#include "Image.h"
#include "GLFW/glfw3.h"

bool Renderer::wireframe = false;
bool Renderer::culling = true;
float Renderer::cameraAngle = 0;
PerformanceCounters Renderer::PC;

struct Viewport {
    int x, y, width, height;
};

ofbx::IScene* LoadFbx() {
    FILE* fp;
    auto fn = "..\\assets\\House N210818\\House N210818.fbx";
    fopen_s( &fp, fn, "rb" );
    if ( !fp ) return nullptr;

    fseek( fp, 0, SEEK_END );
    long file_size = ftell( fp );
    fseek( fp, 0, SEEK_SET );
    auto* content = new ofbx::u8[file_size];
    fread( content, 1, file_size, fp );
    ofbx::IScene* g_scene = ofbx::load( ( ofbx::u8* )content, file_size, ( ofbx::u64 )ofbx::LoadFlags::TRIANGULATE );
    return g_scene;
}

struct DrawSurface {
    ofbx::Color color;
    Image* texture;
    const ofbx::Vec3 *vertices;
    const ofbx::Vec3 *normals;
    const ofbx::Vec2 *texCoords;
    std::vector<int> indices;

    void Draw() {
        auto& surface = *this;
        glVertexPointer( 3, GL_DOUBLE, 0, surface.vertices );
        glNormalPointer( GL_DOUBLE, 0, surface.normals );
        if ( surface.texCoords && surface.texture ) {
            glEnableClientState( GL_TEXTURE_COORD_ARRAY );
            glTexCoordPointer( 2, GL_DOUBLE, 0, surface.texCoords );
            surface.texture->Bind();
            glEnable( GL_TEXTURE_2D );
        } else {
            glDisable( GL_TEXTURE_2D );
            glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        }
        glColor3fv( &surface.color.r );
        glDrawElements( GL_TRIANGLES, surface.indices.size(), GL_UNSIGNED_INT, surface.indices.data() );
        Renderer::PC.drawCalls++;
        Renderer::PC.drawTriangles += surface.indices.size() / 3;
    }
};

void DrawFbx() {
    static std::vector<DrawSurface> surfaces;
    if ( surfaces.empty() ) {
        auto g_scene = LoadFbx();
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
                        auto path = string_format( "..\\assets\\House N210818\\%s", s );
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
    glPushMatrix();
    float s = 1e0f;
    glScalef( s, s, s );
    for ( size_t j = 0; j < surfaces.size(); j++ ) {
        auto& surface = surfaces[j];
        surface.Draw();
    }
    glPopMatrix();
}


Renderer::Renderer() {
    if ( !gladLoadGL() ) {
        printf( "Something went wrong!\n" );
        exit( -1 );
    }
    printf( "OpenGL %d.%d\n", GLVersion.major, GLVersion.minor );

    int maxTextureSize;
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxTextureSize );
    printf( "GL_MAX_TEXTURE_SIZE %d\n", maxTextureSize );

    Viewport viewport;
    glGetIntegerv( GL_VIEWPORT, (int*)&viewport );

    auto matProj = glm::infinitePerspective( glm::radians( 45.0f ), (float) viewport.width / viewport.height, 11.f );
    glMatrixMode( GL_PROJECTION );
    glLoadMatrixf( glm::value_ptr( matProj ) );
    glMatrixMode( GL_MODELVIEW );

    glPointSize( (viewport.height >> 10) + 1.f );

    glEnable( GL_BLEND );
    glm::vec4 ambientLight( 0, 0, 0, 1 );
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, glm::value_ptr( ambientLight ) );

    //glEnable( GL_TEXTURE_2D );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_NORMALIZE );
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );
}


Renderer::~Renderer() {
}

void Renderer::Render( Simulation& simulation ) {
    glm::mat4 view;
    view = glm::lookAt( glm::vec3( 0.0f, 0.0f, 233.0f ),
        glm::vec3( 0.0f, 0.0f, 0.0f ),
        glm::vec3( 0.0f, 1.0f, 0.0f ) );
    view = glm::rotate( view, cameraAngle, glm::vec3( 0, 1, 0 ) );
    glLoadMatrixf( glm::value_ptr( view ) );

    glPolygonMode( GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL );
    if ( culling )
        glEnable( GL_CULL_FACE );
    else
        glDisable( GL_CULL_FACE );

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    DrawFbx();
}