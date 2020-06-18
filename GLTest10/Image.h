#pragma once

struct Image {
    int width = 0, height = 0;
    unsigned char* data = nullptr;
    Image( const char* fileName );
    ~Image() {
        delete data;
    }
};