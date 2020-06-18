#include "Image.h"

#include <cstdio>
#include <im.h>

Image::Image( const char* fileName ) {
    int error;
    auto f = imFileOpen( fileName, &error );
    int cm, dt;
    imFileReadImageInfo( f, 0, &width, &height, &cm, &dt );
    data = new unsigned char[3 * width * height];
    imFileReadImageData( f, data, 1, IM_PACKED );
}