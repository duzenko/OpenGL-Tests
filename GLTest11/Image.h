#pragma once

struct Image: AbstractImage {
    using AbstractImage::AbstractImage;

    GLuint texHandle = 0;
    void Bind();
};

struct Images :AbstractImages {
protected:
    Image* GenImage( const std::string& fileName ) {
        return new Image(fileName);
    }

    virtual void DisableTexturing() {
        glDisable( GL_TEXTURE_2D );
    }
};