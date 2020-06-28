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

SafeQueue<Image*> imageQueue;

void LoadImages();

std::thread ImageLoader( LoadImages );

void LoadImages() {
    ImageLoader.detach();
    while ( Image* image = imageQueue.dequeue() ) {
        int error;
        auto f = imFileOpen( image->fileName.c_str(), &error );
        int cm, dt;
        imFileReadImageInfo( f, 0, &image->width, &image->height, &cm, &dt );
        image->data.resize( 3 * image->width * image->height );
        imFileReadImageData( f, image->data.data(), 1, IM_PACKED );
        image->state = Image::State::Loaded;
    }
}


void Image::Load() {
    imageQueue.enqueue( this );
}