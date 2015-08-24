//------------------------------------------------------------------------------
// ofxBuffer - based on ofBuffer, which is found in the ofFileUtils.h and
//             ofFileUtils.cpp files of openFrameworks.
//------------------------------------------------------------------------------
#include "ofxBuffer.h"
#ifndef TARGET_WIN32
#include <pwd.h>
#endif

//--------------------------------------------------
ofxBuffer::ofxBuffer()
{
    nextLinePos = 0;
    buffer.resize( 1 );
}

//--------------------------------------------------
ofxBuffer::ofxBuffer( const char * buffer, unsigned int size )
{
    set( buffer, size );
}

//--------------------------------------------------
ofxBuffer::ofxBuffer( const string & text )
{
    set( text );
}

//--------------------------------------------------
ofxBuffer::ofxBuffer( istream & stream )
{
    set( stream );
}

//--------------------------------------------------
ofxBuffer::ofxBuffer( const ofxBuffer & buffer_ )
{
    buffer = buffer_.buffer;
    nextLinePos = buffer_.nextLinePos;
}

//--------------------------------------------------
ofxBuffer::~ofxBuffer()
{
    clear();
}

//--------------------------------------------------
bool ofxBuffer::set( istream & stream )
{
    clear();
    if( stream.bad() ) {
        return false;
    }

    char aux_buffer[1024];
    std::streamsize size = 0;
    stream.read( aux_buffer, 1024 );
    std::streamsize n = stream.gcount();
    while( n > 0 ) {
        // we resize to size+1 initialized to 0 to have a 0 at the end for strings
        buffer.resize( size + n + 1, 0 );
        memcpy( &(buffer[0]) + size, aux_buffer, n );
        size += n;
        if( stream ) {
            stream.read( aux_buffer, 1024 );
            n = stream.gcount();
        }
        else {
            n = 0;
        }
    }
    return true;
}

//--------------------------------------------------
bool ofxBuffer::writeTo( ostream & stream ) const
{
    if( stream.bad() ) {
        return false;
    }
    stream.write( &(buffer[0]), buffer.size() - 1 );
    return true;
}

//--------------------------------------------------
void ofxBuffer::set( const char * _buffer, unsigned int _size )
{
    buffer.assign( _buffer, _buffer + _size );
    buffer.resize( buffer.size() + 1 );
    buffer.back() = 0;
}

//--------------------------------------------------
void ofxBuffer::set( const string & text )
{
    set( text.c_str(), text.size() );
}

//--------------------------------------------------
void ofxBuffer::append( const string& _buffer )
{
    append( _buffer.c_str(), _buffer.size() );
}

//--------------------------------------------------
void ofxBuffer::append( const char * _buffer, unsigned int _size )
{
    buffer.insert( buffer.end() - 1, _buffer, _buffer + _size );
    buffer.back() = 0;
}

//--------------------------------------------------
void ofxBuffer::clear()
{
    buffer.resize( 1 );
    nextLinePos = 0;
}

//--------------------------------------------------
void ofxBuffer::allocate( long _size )
{
    clear();
    buffer.resize( _size );
}

//--------------------------------------------------
char *ofxBuffer::getBinaryBuffer()
{
    if( buffer.empty() ) {
        return NULL;
    }
    return &buffer[0];
}

//--------------------------------------------------
const char *ofxBuffer::getBinaryBuffer() const
{
    if( buffer.empty() ) {
        return "";
    }
    return &buffer[0];
}

//--------------------------------------------------
string ofxBuffer::getText() const
{
    if( buffer.empty() ) {
        return "";
    }
    return &buffer[0];
}

//--------------------------------------------------
ofxBuffer::operator string() const
{
    return getText();
}

//--------------------------------------------------
ofxBuffer & ofxBuffer::operator=(const string & text)
{
    set( text );
    return *this;
}

//--------------------------------------------------
long ofxBuffer::size() const
{
    if( buffer.empty() ) {
        return 0;
    }
    //we always add a 0 at the end to avoid problems with strings
    return buffer.size() - 1;
}

//--------------------------------------------------
string ofxBuffer::getNextLine()
{
    if( buffer.empty() || (int)(buffer.size() - 1) == nextLinePos ) {
        return "";
    }
    long currentLinePos = nextLinePos;
    bool lineEndWasCR = false;
    while( nextLinePos < (int)buffer.size() - 1 && buffer[nextLinePos] != '\n' ) {
        if( buffer[nextLinePos] != '\r' ) {
            nextLinePos++;
        }
        else {
            lineEndWasCR = true;
            break;
        }
    }
    string line( getBinaryBuffer() + currentLinePos, nextLinePos - currentLinePos );
    if( nextLinePos < (int)(buffer.size() - 1) ) {
        nextLinePos++;
    }
    // if lineEndWasCR check for CRLF
    if( lineEndWasCR && nextLinePos < (int)(buffer.size() - 1) && buffer[nextLinePos] == '\n' ) {
        nextLinePos++;
    }
    return line;
}

//--------------------------------------------------
string ofxBuffer::getFirstLine()
{
    resetLineReader();
    return getNextLine();
}

//--------------------------------------------------
bool ofxBuffer::isLastLine()
{
    return (int)(buffer.size() - 1) == nextLinePos;
}

//--------------------------------------------------
void ofxBuffer::resetLineReader()
{
    nextLinePos = 0;
}

//--------------------------------------------------
ostream & operator<<(ostream & ostr, const ofxBuffer & buf)
{
    buf.writeTo( ostr );
    return ostr;
}

//--------------------------------------------------
istream & operator>>(istream & istr, ofxBuffer & buf)
{
    buf.set( istr );
    return istr;
}
