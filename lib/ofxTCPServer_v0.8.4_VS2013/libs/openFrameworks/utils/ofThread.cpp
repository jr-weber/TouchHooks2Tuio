#include "ofThread.h"
#include <iostream>
#include <sstream>

#ifdef TARGET_ANDROID
#include <jni.h>
#include "ofxAndroidUtils.h"
#endif

//-------------------------------------------------
ofThread::ofThread() :
  _threadRunning( false ), 
  _mutexBlocks( true ), 
  threadBeingWaitedFor( false )
{
    ostringstream out;
    out << thread.id();
    thread.setName( "Thread " + out.str() );
}

//-------------------------------------------------
ofThread::~ofThread()
{
}

//-------------------------------------------------
bool ofThread::isThreadRunning() const
{
    return _threadRunning;
}

//-------------------------------------------------
int ofThread::getThreadId() const
{
    return thread.id();
}

//-------------------------------------------------
std::string ofThread::getThreadName() const
{
    return thread.name();
}

//-------------------------------------------------
void ofThread::startThread( bool mutexBlocks )
{
    if( thread.isRunning() ) {
        std::cerr << "ofThread-name: " << getThreadName() << " - Cannot start, thread already running.\n";
        return;
    }

    _threadRunning = true;
    _mutexBlocks = mutexBlocks;

    thread.start( *this );
}

//-------------------------------------------------
void ofThread::startThread( bool mutexBlocks, bool verbose )
{
    std::cerr << "ofThread-name: " << getThreadName() << " - Calling startThread with verbose is deprecated.\n";
    startThread( mutexBlocks );
}

//-------------------------------------------------
bool ofThread::lock()
{
    if( _mutexBlocks ) {
        if( isCurrentThread() ) {
            std::cerr << "ofThread-name: " << getThreadName() << " - ofThread waiting for its own mutex to be unlocked.\n";
        }
        else {
            std::cerr << "ofThread-name: " << getThreadName() << " - External thread waiting for ofThread mutex to be unlocked.\n";
        }
        mutex.lock();

    }
    else {
        if( !mutex.tryLock() ) {
            std::cerr << "ofThread-name: " << getThreadName() << " - Mutex is already locked, tryLock failed.\n";
            return false;
        }
    }

    if( isCurrentThread() ) {
        std::cerr << "ofThread-name: " << getThreadName() << " - ofThread locked its own mutex.\n";
    }
    else {
        std::cerr << "ofThread-name: " << getThreadName() << " - External thread locked the ofThread mutex.\n";
    }

    return true;
}

//-------------------------------------------------
void ofThread::unlock()
{
    mutex.unlock();

    if( isCurrentThread() ) {
        std::cerr << "ofThread-name: " << getThreadName() << " - ofThread unlocked its own mutex.\n";
    }
    else {
        std::cerr << "ofThread-name: " << getThreadName() << " - External thread unlocked the ofThread mutex.\n";
    }
}

//-------------------------------------------------
void ofThread::stopThread()
{
    _threadRunning = false;
}

//-------------------------------------------------
void ofThread::waitForThread( bool callStopThread, long milliseconds )
{
    if( thread.isRunning() ) {
        threadBeingWaitedFor = true;

        // tell thread to stop
        if( callStopThread ) {
            stopThread();
            std::cerr << "ofThread-name: " << getThreadName() << " - Signaled to stop.\n";
        }

        // wait for the thread to finish
        std::cerr << "ofThread-name: " << getThreadName() << " - waiting to stop.\n";

        if( isCurrentThread() ) {
            std::cerr << "ofThread-name: " << getThreadName() << " - waitForThread should only be called from outside the this ofThread.\n";
            return;
        }

        if( INFINITE_JOIN_TIMEOUT == milliseconds ) {
            thread.join();
        }
        else {
            // Wait for "joinWaitMillis" milliseconds for thread to finish
            if( !thread.tryJoin( milliseconds ) ) {
                std::cerr << "ofThread-name: " << getThreadName() << " - Unable to completely waitForThread.\n";
            }
        }
    }
}

//-------------------------------------------------
void ofThread::sleep( long milliseconds )
{
    Poco::Thread::sleep( milliseconds );
}

//-------------------------------------------------
void ofThread::yield()
{
    Poco::Thread::yield();
}

//-------------------------------------------------
bool ofThread::isCurrentThread() const
{
    return ofThread::getCurrentPocoThread() == &getPocoThread();
}

//-------------------------------------------------
Poco::Thread& ofThread::getPocoThread()
{
    return thread;
}

//-------------------------------------------------
const Poco::Thread& ofThread::getPocoThread() const
{
    return thread;
}

//-------------------------------------------------
bool ofThread::isMainThread()
{
    return !Poco::Thread::current();
}

//-------------------------------------------------
ofThread* ofThread::getCurrentThread()
{
    // assumes all created threads are ofThreads ...
    // might be dangerous if people are using Poco::Threads directly
    return (ofThread*)Poco::Thread::current();
}

//-------------------------------------------------
Poco::Thread* ofThread::getCurrentPocoThread()
{
    return Poco::Thread::current();
}

//-------------------------------------------------
void ofThread::threadedFunction()
{
    std::cerr << "ofThread-name: " << getThreadName() << " - Override ofThread::threadedFunction() in your ofThread subclass.\n";
}

//-------------------------------------------------
void ofThread::run()
{
    std::cerr << "ofThread-name: " << getThreadName() << " - Started Thread.\n";
#ifdef TARGET_ANDROID
    JNIEnv * env;
    jint attachResult = ofGetJavaVMPtr()->AttachCurrentThread(&env,NULL);
#endif
    // user function
    // should loop endlessly.
    threadedFunction();

    _threadRunning = false;

#if !defined(TARGET_WIN32) && !defined(TARGET_ANDROID)
    // FIXME: this won't be needed once we update POCO https://github.com/pocoproject/poco/issues/79
    if(!threadBeingWaitedFor){ //if threadedFunction() ended and the thread is not being waited for, detach it before exiting.
        pthread_detach(pthread_self());
    }
#endif
#ifdef TARGET_ANDROID
    attachResult = ofGetJavaVMPtr()->DetachCurrentThread();
#endif

    std::cerr << "ofThread-name: " << getThreadName() << " - Thread Finished.\n";
}
