#include "Renderer.h"

#include "glad.h"
#include "ofbx.h"
#include "Util.h"
#include "Image.h"
#include "GLFW/glfw3.h"

bool Renderer::wireframe = false;
bool Renderer::culling = true;
float Renderer::cameraAngle = 0;

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


void DrawFbx() {
    static auto g_scene = LoadFbx();
    glPushMatrix();
    float s = 1e0f;
    glScalef( s, s, s );
    for ( int j = 0; j < g_scene->getMeshCount(); j++ ) {
        auto mesh = g_scene->getMesh( j );
        auto geometry = mesh->getGeometry();
        auto texCoords = geometry->getUVs();
        if ( mesh->getMaterialCount() != 1 ) {
            printf( "Material count %d\n", mesh->getMaterialCount() );
        } else {
            auto material = mesh->getMaterial( 0 );
            auto c = material->getDiffuseColor();
            glColor3fv( &c.r );
            glDisable( GL_TEXTURE_2D );
            if ( texCoords )
                for ( int t = 0; t < ofbx::Texture::TextureType::COUNT; t++ ) {
                    auto tt = ( ofbx::Texture::TextureType )t;
                    if ( material->getTexture( tt ) ) {
                        auto fn = material->getTexture( tt )->getFileName();
                        auto len = fn.end - fn.begin;
                        char* s = (char*) _malloca( len + 1 );
                        memcpy( s, fn.begin, len );
                        s[len] = '\0';
                        glEnable( GL_TEXTURE_2D );
                        auto path = string_format( "..\\assets\\House N210818\\%s", s );
                        auto texture = Images::get( path );
                        texture->Bind();
                    }
                }
        }
        auto verts = geometry->getVertices();
        auto normals = geometry->getNormals();
        auto ind = geometry->getFaceIndices();
        glVertexPointer( 3, GL_DOUBLE, 0, verts );
        glNormalPointer( GL_DOUBLE, 0, normals );
        if ( texCoords ) {
            glEnableClientState( GL_TEXTURE_COORD_ARRAY );
            glTexCoordPointer( 2, GL_DOUBLE, 0, texCoords );
        }  else
            glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        glBegin( GL_TRIANGLES );
        for ( int i = 0; i < geometry->getIndexCount(); i++ ) {
            int index = ind[i];
            index = index < 0 ? -index-1 : index;
            glArrayElement( index );
        }
        glEnd();
        //glDrawElements( GL_TRIANGLES, geometry->getIndexCount(), GL_UNSIGNED_INT, ind );
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