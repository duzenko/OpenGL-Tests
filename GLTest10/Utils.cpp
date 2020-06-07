#include "Utils.h"

#include <iostream>

#include <glm/ext/matrix_float4x4.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/trigonometric.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define _USE_MATH_DEFINES // for C++
#include <math.h>

#include "glad.h"

bool Utils::LoadBMP( const char* fileName ) {
    FILE* file;
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int size;
    unsigned int width, height;
    unsigned char* data;


    fopen_s( &file, fileName, "rb" );

    if ( file == NULL ) {
        //MessageBox(NULL, L"Error: Invaild file path!", L"Error", MB_OK);
        return false;
    }

    if ( fread( header, 1, 54, file ) != 54 ) {
        //MessageBox(NULL, L"Error: Invaild file!", L"Error", MB_OK);
        return false;
    }

    if ( header[0] != 'B' || header[1] != 'M' ) {
        //MessageBox(NULL, L"Error: Invaild file!", L"Error", MB_OK);
        return false;
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

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );

    delete data;
    return true;
}