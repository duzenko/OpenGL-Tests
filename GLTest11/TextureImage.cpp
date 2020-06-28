#include "stdafx.h"

void TextureImage::Bind() {
    if ( state == State::Empty ) {
        state = State::Loading;
        Image::Load();
    }
    if ( state != State::Loaded ) {
        TextureImages::Unbind();
        return;
    }
    if ( !texHandle ) {
        glGenTextures( 1, &texHandle );
        printf( "Load texHandle %d\n", texHandle );
        glBindTexture( GL_TEXTURE_2D, texHandle );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data() );
    }
    if ( TextureImages::boundImage == texHandle )
        return;
    glEnable( GL_TEXTURE_2D );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glBindTexture( GL_TEXTURE_2D, TextureImages::boundImage = texHandle );
    Renderer::PC.textureSwitches++;
}

GLuint TextureImages::boundImage = 0;