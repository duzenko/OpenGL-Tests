#include "stdafx.h"

namespace GL11 {

    void Image::Bind() {
        if ( !texHandle ) {
            glGenTextures( 1, &texHandle );
            printf( "Load texHandle %d\n", texHandle );
            glBindTexture( GL_TEXTURE_2D, texHandle );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
            auto const format = hasAlpha ? GL_RGBA : GL_RGB;
            glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data.data() );
        }
        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, texHandle );
        RendererParams::PC.textureSwitches++;
    }

}