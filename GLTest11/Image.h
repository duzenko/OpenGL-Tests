#pragma once

#include <map>
#include <string>
#include <vector>

struct Image {
    enum {
        Empty, Loading, Loaded
    } state = Empty;

    int width = 0, height = 0;
    std::string fileName;
    std::vector<char> data;
    unsigned int texHandle = 0;

    Image( const std::string& fileName ) :fileName( fileName ) {}
    ~Image() {}
    void Bind();
private:
    void Load();
};

struct Images {
    static std::vector<Image> images;

    static Image* get(const std::string &fileName) {
        for ( auto& image : images ) {
            if ( image.fileName == fileName )
                return &image;
        }
        Image image( fileName );
        auto it = images.insert(images.end(), image );
        return it._Ptr;
    }

    static void Unbind();
};