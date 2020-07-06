#include "pch.h"

#include <queue>
#include <mutex>
#include <condition_variable>

// A threadsafe-queue.
template <class T>
class SafeQueue {
public:
    SafeQueue( void )
        : q()
        , m()
        , c() {
    }

    ~SafeQueue( void ) {
    }

    // Add an element to the queue.
    void enqueue( T t ) {
        std::lock_guard<std::mutex> lock( m );
        q.push( t );
        c.notify_one();
    }

    // Get the "front"-element.
    // If the queue is empty, wait till a element is avaiable.
    T dequeue( void ) {
        std::unique_lock<std::mutex> lock( m );
        while ( q.empty() ) {
            // release lock as long as the wait and reaquire it afterwards.
            c.wait( lock );
        }
        T val = q.front();
        q.pop();
        return val;
    }

private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
};

SafeQueue<AbstractImage*> imageQueue;

std::thread AbstractImage::ImageLoader( AbstractImage::LoadImages );

void AbstractImage::LoadImages() {
    ImageLoader.detach();
    while ( AbstractImage* image = imageQueue.dequeue() ) {
		image->Load();
    }
}

void AbstractImage::Load() {
    int error;
    auto image = this;
    auto f = imFileOpen( image->fileName.c_str(), &error );
    int cm, dt;
    imFileReadImageInfo( f, 0, &image->width, &image->height, &cm, &dt );
    hasAlpha = IM_ALPHA & cm;
    image->data.resize( (hasAlpha ? 4 : 3) * image->width * image->height );
    imFileReadImageData( f, image->data.data(), 1, hasAlpha ? IM_PACKED | IM_ALPHA : IM_PACKED );
    image->state = AbstractImage::State::Loaded;
}

bool AbstractImage::CheckLoaded() {
    if ( state == State::Empty ) {
        state = State::Loading;
        imageQueue.enqueue( this );
    }
    return state == State::Loaded;
}