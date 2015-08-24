/*******************************************************************************
FlashXmlTcpServer

PURPOSE: Wrapper class for ofxTCPServer class, which was borrowed from 
         the ofxNetwork module of openFrameworks.  
         
NOTES: 
I had orignally planned to use the TcpSender class of the TUIO_CPP library, 
which is in the lib/TUIO_CPP/TUIO directory.  However, for unclear reasons, 
the TcpSender would not intialize (kept getting a return value of false from 
the isConnected() function).  As I was using ofxTCPServer in another project, 
I decided to just use that instead.

The ofxTCPServer class along with a few related classes and their support 
libraries can be found in lib/ofxTCPServer_v0.8.4_VS2013.  These classes were
extracted from openFrameworks version 0.8.4 (http://openframeworks.cc/download).
This version of openFrameworks was built for Visual Studio 2012 Express Edition.
When I first built tried to use ofxTCPServer with Visual Studio 2013 Community 
Edition, it would compile, but the static libraries it needed (POCO and OpenSSL)
would not link due to differences in the version of the C/C++ compiler for 
Visual Studio 2012 and 2013 (Microsoft C/C++ compiler 11.0 versus 12.0).  
To get around this problem, I had to download the POCO project from
http://pocoproject.org/index.html and the OpenSSL project from 
http://www.openssl.org/source, and then build both of them with Visual Studio
2013 Community Edtion (with C/C++ compiler version 12.0).  These static
libraries can be found in the lib/ofxTCPServer_v0.8.4_VS2013/libs directory, so
you should not need to build them.  I did not include the rest of openFrameworks
version 0.8.4 because there are several other static libraries that would all
have the same problem (they would work with Visual Studio 2012, but not with
Visual Studio 2013).

Unfortunately, the POCO and OpenSSL libraries take up a lot of space (over 
400 MB combined), so if somebody else wants to fix up the TcpSender class 
of TUIO_CPP and use that instead, that would greatly reduce the size of the
libraries needed for building.

J.R.Weber <joe.weber77@gmail.com>
*******************************************************************************/
#include "FlashXmlTcpServer.h"
#include <ofxTCPServer.h>

FlashXmlTcpServer::FlashXmlTcpServer() :
  tcpServer_( new ofxTCPServer() )
{
}

FlashXmlTcpServer::~FlashXmlTcpServer()
{
    delete tcpServer_;
}

bool FlashXmlTcpServer::setup( int port )
{
    return tcpServer_->setup( port );
}

bool FlashXmlTcpServer::isConnected()
{
    return tcpServer_->isConnected();
}

bool FlashXmlTcpServer::sendtoAll( const std::string & message )
{
    return tcpServer_->sendToAll( message );
}
