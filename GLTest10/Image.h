#pragma once

struct Image : AbstractImage {
    GLuint displayList = 0;
    void Bind();
};

struct Images :AbstractImages {
protected:
    Image* GenImage() {
        return new Image();
    }

    virtual void DisableTexturing() {
        glDisable( GL_TEXTURE_2D );
    }
};