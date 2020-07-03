#pragma once

struct AbstractImage {
    std::string fileName;
    bool hasAlpha = false;

    AbstractImage() {}
    ~AbstractImage() {}
    void BeginLoading();
    void Load();
    virtual void Bind() = 0;

    bool CheckLoaded() {
        if ( state == State::Empty ) {
            state = State::Loading;
            AbstractImage::BeginLoading();
        }
        return state == State::Loaded;
    }

protected:
    enum class State {
        Empty, Loading, Loaded
    };
    State state = State::Empty;

    int width = 0, height = 0;
    std::vector<unsigned char> data;
};

struct AbstractImages {
    AbstractImage* Get( const std::string& fileName ) {
        for ( auto image : images ) {
            if ( image->fileName == fileName )
                return image;
        }
        auto image = genImage();
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

    virtual AbstractImage* genImage() = 0;
    virtual void DisableTexturing() = 0;

private:
    std::vector<AbstractImage*> images;
};

extern AbstractImages *abstractImages;