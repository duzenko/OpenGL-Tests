#pragma once

struct TextureImage : Image {
    GLuint displayList = 0;
    void Bind();
};

struct TextureImages :Images {
    
    static void Unbind() {
        glDisable( GL_TEXTURE_2D );
    }

protected:
    Image* genImage() {
        return new TextureImage();
    }
};