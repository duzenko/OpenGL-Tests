#pragma once

struct AbstractImage {
    bool hasAlpha = false;

    AbstractImage( const std::string& fileName ) {
        this->fileName = fileName;
    }
    ~AbstractImage() {}

    virtual void Bind() = 0;

    bool CheckLoaded();
    const std::string& GetFileName() {
        return fileName;
    }

protected:
    enum class State {
        Empty, Loading, Loaded
    };
    State state = State::Empty;

    std::string fileName;
    int width = 0, height = 0;
    std::vector<unsigned char> data;

    virtual void Load();
private:
    static void LoadImages();
    static std::thread ImageLoader;
};

struct AbstractImages {
    AbstractImage* Get( const std::string& fileName ) {
        for ( auto image : images ) {
            if ( image->GetFileName() == fileName )
                return image;
        }
        auto image = GenImage( fileName );
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

    virtual AbstractImage* GenImage( const std::string& fileName ) = 0;
    virtual void DisableTexturing() = 0;

private:
    std::vector<AbstractImage*> images;
};

extern AbstractImages *abstractImages;