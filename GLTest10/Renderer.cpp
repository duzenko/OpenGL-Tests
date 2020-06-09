#include "Renderer.h"

#include "glad.h"
#include "ofbx.h"

bool Renderer::wireframe = false;
bool Renderer::culling = true;
ofbx::IScene* g_scene;

struct Viewport {
    int x, y, width, height;
};

void SphereRenderModel::Render(float scale) {
    glPushMatrix();
    glScalef( scale, scale, scale );
    SurfaceVertex* vertPointer = vertices;
    for ( int i = 0; i < strips; i++ ) {
        glBegin( GL_TRIANGLE_STRIP );
        for ( int j = 0; j < vertsPerStrip; j++ ) {
            glTexCoord2fv( glm::value_ptr( vertPointer->texCoord ) );
            glNormal3fv( glm::value_ptr( vertPointer->normal ) );
            glVertex3fv( glm::value_ptr( vertPointer->position ) );
            vertPointer++;
        }
        glEnd();
    }
    glPopMatrix();
}

void LoadFbx() {
    FILE* fp;
    fopen_s(&fp, "..\\assets\\Rocks\\Rock0.fbx", "rb" );
    if ( !fp ) return;

    fseek( fp, 0, SEEK_END );
    long file_size = ftell( fp );
    fseek( fp, 0, SEEK_SET );
    auto* content = new ofbx::u8[file_size];
    fread( content, 1, file_size, fp );
    g_scene = ofbx::load( ( ofbx::u8* )content, file_size, ( ofbx::u64 )ofbx::LoadFlags::TRIANGULATE );
}

void DrawFbx() {
    glDisable( GL_TEXTURE_2D );
    auto mesh = g_scene->getMesh( 0 );
    auto geometry = mesh->getGeometry();
    auto verts = geometry->getVertices();
    auto normals = geometry->getNormals();
    auto ind = geometry->getFaceIndices();
    glPushMatrix();
    float s = 3e-3f;
    glScalef( s, s, s );
    glBegin( GL_TRIANGLES );
    for ( int i = 0; i < geometry->getIndexCount(); i++ ) {
        int index = ind[i];
        index = index < 0 ? -index : index + 1;
        auto vert = verts[index - 1];
        auto norm = normals[index - 1];
        glNormal3dv( &norm.x );
        glColor3d( abs( norm.x ), abs( norm.y ), abs( norm.z ) );
        glVertex3dv( &vert.x );
    }
    glEnd();
    glPopMatrix();
    glEnable( GL_TEXTURE_2D );
}

void Renderer::DrawStars() {
    glDepthMask( GL_FALSE );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, textureStars.width, textureStars.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureStars.data );
    glPushMatrix();
    glScalef( 2, 2, 2 );
    glBegin( GL_TRIANGLE_STRIP );
    glTexCoord2f( 0, 1 );
    glVertex2f( -2, +1 );
    glTexCoord2f( 0, 0 );
    glVertex2f( -2, -1 );
    glTexCoord2f( 1, 1 );
    glVertex2f( +2, +1 );
    glTexCoord2f( 1, 0 );
    glVertex2f( +2, -1 );
    glEnd();
    glPopMatrix();
    glDepthMask( GL_TRUE );
}

void Renderer::DrawEarth() {
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, textureEarth.width, textureEarth.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureEarth.data );
    sphere.Render();
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

    auto matProj = glm::perspective( glm::radians( 5.0f ), (float) viewport.width / viewport.height, 11.f, 100.f );
    glMatrixMode( GL_PROJECTION );
    glLoadMatrixf( glm::value_ptr( matProj ) );
    glMatrixMode( GL_MODELVIEW );

    glEnable( GL_BLEND );
    glm::vec4 ambientLight( .1, .1, .1, 1 );
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, glm::value_ptr( ambientLight ) );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    glEnable( GL_TEXTURE_2D );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_NORMALIZE );

    glm::mat4 view;
    view = glm::lookAt( glm::vec3( 0.0f, 0.0f, 33.0f ),
        glm::vec3( 0.0f, 0.0f, 0.0f ),
        glm::vec3( 0.0f, 1.0f, 0.0f ) );
    glLoadMatrixf( glm::value_ptr( view ) );

    dlStars = glGenLists( 1 );
    glNewList( dlStars, GL_COMPILE );
    DrawStars();
    glEndList();

    dlSphere = glGenLists( 1 );
    glNewList( dlSphere, GL_COMPILE );
    DrawEarth();
    glEndList();

    LoadFbx();
}

Renderer::~Renderer() {
    glDeleteLists( dlStars, 1 );
}

void setLight( LightInfo lightInfo, int light ) {
    if ( lightInfo.directional ) {
        glLightfv( light, GL_POSITION, glm::value_ptr( lightInfo.position ) );
    } else {
        glLightfv( light, GL_POSITION, glm::value_ptr( lightInfo.position ) );
    }
    glLightfv( light, GL_DIFFUSE, glm::value_ptr( lightInfo.color ) );
}

void Renderer::Render( Simulation & simulation ) {
    glPolygonMode( GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL );
    if(culling)
        glEnable( GL_CULL_FACE );
    else
        glDisable( GL_CULL_FACE );

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBlendFunc( GL_ONE, GL_ZERO );
    glDisable( GL_LIGHTING );
    glCallList( dlStars );
    glEnable( GL_LIGHTING );

    glEnable( GL_LIGHT0 );
    for ( int light = GL_LIGHT1; light <= GL_LIGHT7; light++ )
        glDisable( light );
    setLight( simulation.sunLight, GL_LIGHT0 );

    glPushMatrix(); 
    glRotatef( simulation.sphereRotationAngle, 0, 1, 0 );
    DrawFbx();
    glPopMatrix();
    return;

    glPushMatrix();
    glRotatef( -23, 0, 0, 1 );
    glRotated( simulation.sphereRotationAngle, 0, 1, 0 );

    glBlendFunc( GL_ONE, GL_ZERO );
    glCallList( dlSphere );

    glBlendFunc( GL_ONE, GL_ONE );
    for ( int simLight = 0; simLight < simulation.LightsPerSphere; ) {
        for ( int light = GL_LIGHT0; light <= GL_LIGHT7; light++, simLight++ ) {
            if ( simLight < simulation.LightsPerSphere )
                glEnable( light );
            else
                glDisable( light );
            setLight( simulation.lights[simLight], light );
        }
        glCallList( dlSphere );
    }

    glPopMatrix();
}