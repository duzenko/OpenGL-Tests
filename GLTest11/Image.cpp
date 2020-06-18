#include "Image.h"

#include <cstdio>
#include "..\nanojpeg.c"
#include <cstring>
#include "glad.h"

void Image::Bind() {
    if ( !texHandle ) {
        glGenTextures( 1, &texHandle );
        printf( "Load texHandle %d\n", texHandle );
        glBindTexture( GL_TEXTURE_2D, texHandle );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
    }
    printf( "Bind texHandle %d\n", texHandle );
    glBindTexture( GL_TEXTURE_2D, texHandle );
}

void Image::Load( const char* fileName ) {
    unsigned char* buf;
    FILE* f;

    fopen_s( &f, fileName, "rb" );
    if ( !f ) {
        printf( "Error opening the input file.\n" );
        return;
    }
    fseek( f, 0, SEEK_END );
    auto size = ftell( f );
    buf = new unsigned char[ size ];
    fseek( f, 0, SEEK_SET );
    size_t read = fread( buf, 1, size, f );
    fclose( f );

    /*Jpeg::Decoder decoder( buf, size );
    if ( decoder.GetResult() != Jpeg::Decoder::OK ) {
        printf( "Error decoding the input file\n" );
        return;
    }

    width = decoder.GetWidth();
    height = decoder.GetHeight();
    size = decoder.GetImageSize();
    auto img = decoder.GetImage();
    */

    if ( njDecode( buf, size ) ) {
        printf( "Error decoding the input file\n" );
        return;
    }

    width = njGetWidth();
    height = njGetHeight();
    size = njGetImageSize();
    auto img = njGetImage();

    delete[] buf;

    data = new unsigned char[size];
    auto row = size / height;
    for ( int ptr = 0; ptr < size; ptr += row )
        memcpy( data + ptr, img + size - ptr - row, row );
}

static bool once = []() {
    njInit();
    return true;
} ( );

std::map<std::string, Image> Images::images;