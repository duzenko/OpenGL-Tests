#pragma once

#include <map>
#include <string>
#include <vector>

struct Image {
    enum class State {
        Empty, Loading, Loaded
    };
    State state = State::Empty;

    int width = 0, height = 0;
    std::string fileName;
    std::vector<char> data;
    unsigned int texHandle = 0;

    Image() {}
    ~Image() {}
    void Bind();
private:
    void Load();
};

struct Images {
    static std::list<Image> images;

    static Image* get(const std::string &fileName) {
        for ( auto& image : images ) {
            if ( image.fileName == fileName )
                return &image;
        }
        int idx = images.size();
        images.resize( idx + 1 );
        auto& image = images.back();
        image.fileName = fileName;
        return &image;
    }

    static void Unbind();
};