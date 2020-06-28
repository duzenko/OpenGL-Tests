#pragma once

struct TextureImage : Image {
    unsigned int texHandle = 0;
    void Bind();
};

struct TextureImages :Images {
    static GLuint boundImage;

    static void Unbind() {
        if ( !boundImage )
            return;
        glDisable( GL_TEXTURE_2D );
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        boundImage = 0;
    }

protected:
    Image* genImage() {
        return new TextureImage();
    }
};