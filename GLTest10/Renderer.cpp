#include "Renderer.h"

#include "glad.h"

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

    glEnable( GL_LIGHTING );
    glClearColor( 0, 0.3f, 0, 0 );
    glEnable( GL_BLEND );
    glEnable( GL_CULL_FACE );
    glm::vec4 ambientLight( 0, 0, 0, 1 );
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, glm::value_ptr( ambientLight ) );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    glEnable( GL_TEXTURE_2D );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glm::mat4 view;
    view = glm::lookAt( glm::vec3( 0.0f, 0.0f, 33.0f ),
        glm::vec3( 0.0f, 0.0f, 0.0f ),
        glm::vec3( 0.0f, 1.0f, 0.0f ) );
    glLoadMatrixf( glm::value_ptr( view ) );
}

Renderer::~Renderer() {}

void DrawStars() {
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
}

void Renderer::Render( Simulation & simulation ) {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBlendFunc( GL_ONE, GL_ZERO );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, textureStars.width, textureStars.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureStars.data );
    glDisable( GL_LIGHTING );
    DrawStars();
    glEnable( GL_LIGHTING );

    glPushMatrix();
    glRotatef( 23, 1, 0, 0 );
    glRotated( simulation.sphereRotationAngle, 0, 1, 0 );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, textureEarth.width, textureEarth.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureEarth.data );
   
    glBlendFunc( GL_ZERO, GL_ZERO );
    sphere.Render();

    glBlendFunc( GL_ONE, GL_ONE );
    for ( int simLight = 0; simLight < simulation.LightsPerSphere; ) {
        for ( int light = GL_LIGHT0; light <= GL_LIGHT7; light++, simLight++ ) {
            if ( simLight < simulation.LightsPerSphere )
                glEnable( light );
            else
                glDisable( light );
            auto& lightInfo = simulation.lights[simLight];
            glLightfv( light, GL_POSITION, glm::value_ptr( lightInfo.position ) );
            glLightfv( light, GL_DIFFUSE, glm::value_ptr( lightInfo.color ) );
        }
        sphere.Render();
    }

    glPopMatrix();
}