#pragma once

struct Image {
    unsigned int width, height;
    unsigned char* data;
    Image( const char* fileName );
    ~Image() {
        delete data;
    }
};