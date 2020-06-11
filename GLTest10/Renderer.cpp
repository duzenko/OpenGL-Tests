#include "Renderer.h"

#include "glad.h"
#include "ofbx.h"
#include "Util.h"

bool Renderer::wireframe = false;
bool Renderer::culling = true;

struct Viewport {
    int x, y, width, height;
};

int dlRock[11];

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

void LoadFbx( int rockType ) {
    FILE* fp;
    auto fn = string_format( "..\\assets\\Rocks\\Rock%d.fbx", rockType );
    fopen_s(&fp, fn.c_str(), "rb" );
    if ( !fp ) return;

    fseek( fp, 0, SEEK_END );
    long file_size = ftell( fp );
    fseek( fp, 0, SEEK_SET );
    auto* content = new ofbx::u8[file_size];
    fread( content, 1, file_size, fp );
    ofbx::IScene* g_scene = ofbx::load( ( ofbx::u8* )content, file_size, ( ofbx::u64 )ofbx::LoadFlags::TRIANGULATE );

    dlRock[rockType] = glGenLists( 1 );
    glNewList( dlRock[rockType], GL_COMPILE );
    glDisable( GL_TEXTURE_2D );
    auto mesh = g_scene->getMesh( 0 );
    auto geometry = mesh->getGeometry();
    auto verts = geometry->getVertices();
    auto normals = geometry->getNormals();
    auto ind = geometry->getFaceIndices();
    glPushMatrix();
    float s = 1e-4f;
    glScalef( s, s, s );
    glBegin( GL_TRIANGLES );
    for ( int i = 0; i < geometry->getIndexCount(); i++ ) {
        int index = ind[i];
        index = index < 0 ? -index : index + 1;
        auto vert = verts[index - 1];
        auto norm = normals[index - 1];
        glNormal3dv( &norm.x );
        glVertex3dv( &vert.x );
    }
    glEnd();
    glPopMatrix();
    glEnable( GL_TEXTURE_2D );
    glEndList();
    //delete g_scene;
}

void DrawFbx( int rockType ) {
    if ( !dlRock[rockType] )
        LoadFbx( rockType );
    glCallList( dlRock[rockType] );
}

GLuint DrawStars() {
    Image textureStars = { "..\\assets\\2k_stars_milky_way.bmp" };
    auto dlStars = glGenLists( 1 );
    glNewList( dlStars, GL_COMPILE );
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
    glEndList();
    return dlStars;
}

GLuint DrawEarth() {
    auto dlSphere = glGenLists( 1 );
    Image textureEarth = { "..\\assets\\2k_earth_daymap.bmp" };
    SphereRenderModel sphere = { 90 };
    glNewList( dlSphere, GL_COMPILE );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, textureEarth.width, textureEarth.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureEarth.data );
    glPushMatrix();
    sphere.Render();
    glPopMatrix();
    glEndList();
    return dlSphere;
}

GLuint DrawMoon() {
    auto dlSphere = glGenLists( 1 );
    Image textureEarth = { "..\\assets\\2k_moon.bmp" };
    SphereRenderModel sphere = { 30 };
    glNewList( dlSphere, GL_COMPILE );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, textureEarth.width, textureEarth.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureEarth.data );
    glScalef( 1.737f, 1.737f, 1.737f );
    sphere.Render();
    glEndList();
    return dlSphere;
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

    auto matProj = glm::perspective( glm::radians( 5.0f ), (float) viewport.width / viewport.height, 11.f, 1e3f );
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

    dlStars = DrawStars();
    dlEarth = DrawEarth();
    dlMoon = DrawMoon();
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

void Renderer::Render( Simulation& simulation ) {
    glPolygonMode( GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL );
    if ( culling )
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

    for ( int i = 0; i < simulation.DebriCount; i++ ) {
        auto &debris = simulation.debris[i];
        glPushMatrix();
        glTranslatef( debris.position.x, debris.position.y, debris.position.z );
        glRotatef( simulation.sphereRotationAngle, debris.rotationNormal.x, debris.rotationNormal.y, debris.rotationNormal.z );
        DrawFbx( debris.rockType );
        glPopMatrix();
    }

    glPushMatrix();
    glRotatef( 3, 0, 1, 0 );
    glTranslatef( 0, 0, -384 );
    glCallList( dlMoon );
    glPopMatrix();

    glPushMatrix();
    glRotatef( -23, 0, 0, 1 );
    glRotated( simulation.sphereRotationAngle, 0, 1, 0 );

    glCallList( dlEarth );

    glBlendFunc( GL_ONE, GL_ONE );
    for ( int simLight = 0; simLight < simulation.LightsPerSphere; ) {
        for ( int light = GL_LIGHT0; light <= GL_LIGHT7; light++, simLight++ ) {
            if ( simLight < simulation.LightsPerSphere )
                glEnable( light );
            else
                glDisable( light );
            setLight( simulation.lights[simLight], light );
        }
        glCallList( dlEarth );
    }

    glPopMatrix();
}