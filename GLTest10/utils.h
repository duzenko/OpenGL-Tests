#pragma once


#include <iostream>

#include "glad.h"
#include "GLFW/glfw3.h"

#include <glm/ext/matrix_float4x4.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/trigonometric.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define _USE_MATH_DEFINES // for C++
#include <math.h>

struct Utils {
    static bool LoadBMP( const char* fileName ) {
        FILE* file;
        unsigned char header[54];
        unsigned int dataPos;
        unsigned int size;
        unsigned int width, height;
        unsigned char* data;


        file = fopen( fileName, "rb" );

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

    static void drawSphere( float r, int subdiv ) {
        int i, j;
        int lats = subdiv, longs = 2 * subdiv;
        for ( i = 0; i < lats; i++ ) {
            float t1 = (float) i / lats;
            double lat0 = M_PI * ( -0.5 + t1 );
            double y0 = sin( lat0 );
            double yr0 = cos( lat0 );

            float t2 = (float) (i + 1 ) / lats;
            double lat1 = M_PI * ( -0.5 + t2 );
            double y1 = sin( lat1 );
            double yr1 = cos( lat1 );

            glBegin( GL_QUAD_STRIP );
            for ( j = 0; j <= longs; j++ ) {
                float s = (float) j / longs;
                double lng = 2 * M_PI * s;
                double x = -sin( lng );
                double z = -cos( lng );

                glTexCoord2f( s, t2 );
                glm::vec3 n2( x * yr1, y1, z * yr1 );
                glNormal3fv( glm::value_ptr( n2 ) );
                glm::vec3 v2 = n2 * r;
                glVertex3fv( glm::value_ptr( v2 ) );

                glTexCoord2f( s, t1 );
                glm::vec3 n1( x * yr0, y0, z * yr0 );
                glNormal3fv( glm::value_ptr( n1 ) );
                glm::vec3 v1 = n1 * r;
                glVertex3fv( glm::value_ptr( v1 ) );

            }
            glEnd();
        }
    }
};