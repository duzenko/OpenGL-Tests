#include "stdafx.h"

void Image::Bind() {
    if ( !displayList ) {
        displayList = glGenLists( 1 );
        printf( "Load texHandle %d\n", displayList );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
        glNewList( displayList, GL_COMPILE );
        glEnable( GL_TEXTURE_2D );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data() );
        glEndList();
    }
    glCallList( displayList );
    Renderer::PC.textureSwitches++;
}