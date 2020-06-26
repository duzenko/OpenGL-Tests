#include "Image.h"

#include <cstdio>
#include <cstring>
#include <thread>
#include <mutex>          // std::mutex, std::unique_lock, std::defer_lock

#include "glad.h"
#include <im.h>

#include "Renderer.h"

int boundImage;

void Image::Bind() {
    if ( state == Empty ) {
        state = Loading;
        std::thread thread( &Image::Load, this );
        thread.detach();
    }
    if ( state != Loaded ) {
        Images::Unbind();
        return;
    }
    if ( !texHandle ) {
        glGenTextures( 1, &texHandle );
        printf( "Load texHandle %d\n", texHandle );
        glBindTexture( GL_TEXTURE_2D, texHandle );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data() );
    }
    if ( boundImage == texHandle )
        return;
    glEnable( GL_TEXTURE_2D );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glBindTexture( GL_TEXTURE_2D, boundImage = texHandle );
    Renderer::PC.textureSwitches++;
}

std::mutex mtx;           // mutex for critical section

void Image::Load() {
    std::unique_lock<std::mutex> lck( mtx, std::defer_lock );
    lck.lock();
    int error;
    auto f = imFileOpen( fileName.c_str(), &error);
    int cm, dt;
    imFileReadImageInfo( f, 0, &width, &height, &cm, &dt );
    data.reserve(3 * width * height);
    imFileReadImageData( f, data.data(), 1, IM_PACKED );
    state = Loaded;
    lck.unlock();
}

std::vector<Image> Images::images;

void Images::Unbind() {
    if ( !boundImage )
        return;
    glDisable( GL_TEXTURE_2D );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    boundImage = 0;
}