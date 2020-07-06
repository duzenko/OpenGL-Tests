#pragma once

struct AbstractImage {
    std::string fileName;
    bool hasAlpha = false;

    AbstractImage() {}
    ~AbstractImage() {}

    virtual void Bind() = 0;

    bool CheckLoaded();

protected:
    enum class State {
        Empty, Loading, Loaded
    };
    State state = State::Empty;

    int width = 0, height = 0;
    std::vector<unsigned char> data;
private:
    static void LoadImages();
    static std::thread ImageLoader;

    void Load();
};

struct AbstractImages {
    AbstractImage* Get( const std::string& fileName ) {
        for ( auto image : images ) {
            if ( image->fileName == fileName )
                return image;
        }
        auto image = GenImage();
        image->fileName = fileName;
        images.push_back( image );
        return image;
    }

    void Bind( AbstractImage* image ) {
        if ( image == boundImage )
            return;
        if ( image && !image->CheckLoaded() )
            image = nullptr;
        if ( image ) {
            image->Bind();
        } else {
            DisableTexturing();
        }
        boundImage = image;
    }

protected:
    AbstractImage* boundImage;

    virtual AbstractImage* GenImage() = 0;
    virtual void DisableTexturing() = 0;

private:
    std::vector<AbstractImage*> images;
};

extern AbstractImages *abstractImages;