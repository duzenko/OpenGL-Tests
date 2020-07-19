#pragma once

namespace GL10 {

    struct Image : AbstractImage {
        using AbstractImage::AbstractImage;

        GLuint displayList = 0;
        void Bind();
    };

    struct Images :AbstractImages {
    protected:
        Image* GenImage( const std::string& fileName ) {
            return new Image( fileName );
        }

        virtual void DisableTexturing() {
            glDisable( GL_TEXTURE_2D );
        }
    };

}