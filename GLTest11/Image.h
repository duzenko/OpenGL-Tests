#pragma once

#include <map>
#include <string>

struct Image {
    unsigned int width = 0, height = 0;
    unsigned char* data = nullptr;
    unsigned int texHandle = 0;
    Image() {}
    ~Image() {
        printf( "~Image\n" );
        delete data;
    }
    void Load( const char* fileName );
    void Bind();
};

struct Images {
    static std::map<std::string, Image> images;

    static Image* get(const std::string &fileName) {
        auto it = images.find( fileName );
        if ( it != images.end() ) {
            return &it->second;
        } else {
            printf( "Texture %s... ", fileName.c_str() );
            auto& image = images[fileName];
            image.Load( fileName.c_str() );
            printf( "OK\n" );
            return &image;
        }
    }
};