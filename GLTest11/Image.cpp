#include "Image.h"

#include <cstdio>
#include <cstring>
#include "glad.h"
#include <im.h>

#include "Renderer.h"

int boundImage;

void Image::Bind() {
    if ( !texHandle ) {
        glGenTextures( 1, &texHandle );
        printf( "Load texHandle %d\n", texHandle );
        glBindTexture( GL_TEXTURE_2D, texHandle );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
    }
    if ( boundImage == texHandle )
        return;
    glBindTexture( GL_TEXTURE_2D, boundImage = texHandle );
    Renderer::PC.textureSwitches++;
}

void Image::Load( const char* fileName ) {
    int error;
    auto f = imFileOpen( fileName , &error);
    int cm, dt;
    imFileReadImageInfo( f, 0, &width, &height, &cm, &dt );
    data = new unsigned char[3 * width * height];
    imFileReadImageData( f, data, 1, IM_PACKED );
}

std::map<std::string, Image> Images::images;