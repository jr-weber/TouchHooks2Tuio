//------------------------------------------------------------------------------
// ofxBuffer - based on ofBuffer, which is found in the ofFileUtils.h and
//             ofFileUtils.cpp files of openFrameworks.
//------------------------------------------------------------------------------
#pragma once
#include "ofConstants.h"

class ofxBuffer
{

public:
    ofxBuffer();
    ofxBuffer( const char * buffer, unsigned int size );
    ofxBuffer( const string & text );
    ofxBuffer( istream & stream );
    ofxBuffer( const ofxBuffer & buffer_ );

    ~ofxBuffer();

    void set( const char * _buffer, unsigned int _size );
    void set( const string & text );
    bool set( istream & stream );
    void append( const string& _buffer );
    void append( const char * _buffer, unsigned int _size );

    bool writeTo( ostream & stream ) const;

    void clear();

    void allocate( long _size );

    char * getBinaryBuffer();
    const char * getBinaryBuffer() const;

    string getText() const;
    operator string() const;  // cast to string, to use a buffer as a string
    ofxBuffer & operator=(const string & text);

    long size() const;
    string getNextLine();
    string getFirstLine();
    bool isLastLine();
    void resetLineReader();

    friend ostream & operator<<(ostream & ostr, const ofxBuffer & buf);
    friend istream & operator>>(istream & istr, ofxBuffer & buf);

private:
    vector<char> 	buffer;
    long 			nextLinePos;
};
