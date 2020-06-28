#pragma once

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
    void Load();
};

struct Images {
    std::vector<Image*> images;

protected:
    virtual Image* genImage() {
        return new Image();
    }

public:
    Image* get( const std::string& fileName ) {
        for ( auto image : images ) {
            if ( image->fileName == fileName )
                return image;
        }
        auto image = genImage();
        image->fileName = fileName;
        images.push_back( image );
        return image;
    }

};