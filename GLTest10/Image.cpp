#include "Image.h"

#include <cstdio>
#include "..\jpeg_decoder.h"

Image::Image( const char* fileName ) {
    unsigned char* buf;
    FILE* f;

    fopen_s( &f, fileName, "rb" );
    if ( !f ) {
        printf( "Error opening the input file.\n" );
        return;
    }
    fseek( f, 0, SEEK_END );
    auto size = ftell( f );
    buf = (unsigned char*) malloc( size );
    fseek( f, 0, SEEK_SET );
    size_t read = fread( buf, 1, size, f );
    fclose( f );

    Jpeg::Decoder decoder( buf, size );
    if ( decoder.GetResult() != Jpeg::Decoder::OK ) {
        printf( "Error decoding the input file\n" );
        return;
    }

    width = decoder.GetWidth();
    height = decoder.GetHeight();
    size = decoder.GetImageSize();

    data = new unsigned char[decoder.GetImageSize()];
    auto row = size / height;
    for ( int ptr = 0; ptr < size; ptr += row )
        memcpy( data + ptr, decoder.GetImage() + size - ptr - row, row );
}