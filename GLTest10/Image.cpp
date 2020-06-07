#include "Image.h"

#include <cstdio>

Image::Image( const char* fileName ) {
    FILE* file;
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int size;

    fopen_s( &file, fileName, "rb" );

    if ( file == NULL ) {
        //MessageBox(NULL, L"Error: Invaild file path!", L"Error", MB_OK);
        return;
    }

    if ( fread( header, 1, 54, file ) != 54 ) {
        //MessageBox(NULL, L"Error: Invaild file!", L"Error", MB_OK);
        return;
    }

    if ( header[0] != 'B' || header[1] != 'M' ) {
        //MessageBox(NULL, L"Error: Invaild file!", L"Error", MB_OK);
        return;
    }

    dataPos = *(int*) &( header[0x0A] );
    size = *(int*) &( header[0x22] );
    width = *(int*) &( header[0x12] );
    height = *(int*) &( header[0x16] );

    if ( size == NULL )
        size = width * height * 3;
    if ( dataPos == NULL )
        dataPos = 54;

    data = new unsigned char[size];

    fread( data, 1, size, file );

    for ( unsigned int i = 0; i < size; i += 3 ) {
        unsigned char red = data[i];
        data[i] = data[i + 2];
        data[i + 2] = red;
    }

    fclose( file );
}