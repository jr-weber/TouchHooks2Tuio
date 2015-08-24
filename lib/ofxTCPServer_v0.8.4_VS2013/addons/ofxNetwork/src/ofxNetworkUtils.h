/*
 * ofxNetworkUtils.h
 *
 *  Created on: 25/09/2010
 *      Author: arturo
 */
#pragma once

#include "ofConstants.h"
#include <cerrno>
#include <iostream>
#include <sstream>

#ifdef TARGET_WIN32
#ifdef ENOTCONN 
#undef ENOTCONN
#endif
#define ENOTCONN        WSAENOTCONN

#ifdef EWOULDBLOCK 
#undef EWOULDBLOCK
#endif
#define EWOULDBLOCK     WSAEWOULDBLOCK

#ifdef ENOBUFS 
#undef ENOBUFS
#endif
#define ENOBUFS         WSAENOBUFS

#ifdef ECONNRESET 
#undef ECONNRESET
#endif
#define ECONNRESET      WSAECONNRESET

#ifdef ESHUTDOWN 
#undef ESHUTDOWN
#endif
#define ESHUTDOWN       WSAESHUTDOWN

#ifdef EAFNOSUPPORT 
#undef EAFNOSUPPORT
#endif
#define EAFNOSUPPORT    WSAEAFNOSUPPORT

#ifdef EPROTONOSUPPORT 
#undef EPROTONOSUPPORT
#endif
#define EPROTONOSUPPORT WSAEPROTONOSUPPORT

#ifdef EINPROGRESS 
#undef EINPROGRESS
#endif
#define EINPROGRESS     WSAEINPROGRESS

#ifdef EISCONN 
#undef EISCONN
#endif
#define EISCONN         WSAEISCONN

#ifdef ENOTSOCK 
#undef ENOTSOCK
#endif
#define ENOTSOCK        WSAENOTSOCK

#ifdef EOPNOTSUPP 
#undef EOPNOTSUPP
#endif
#define EOPNOTSUPP      WSAEOPNOTSUPP

#ifdef ETIMEDOUT 
#undef ETIMEDOUT
#endif
#define ETIMEDOUT       WSAETIMEDOUT

#ifdef EADDRNOTAVAIL 
#undef EADDRNOTAVAIL
#endif
#define EADDRNOTAVAIL   WSAEADDRNOTAVAIL

#ifdef ECONNREFUSED 
#undef ECONNREFUSED
#endif
#define ECONNREFUSED    WSAECONNREFUSED

#ifdef ENETUNREACH 
#undef ENETUNREACH
#endif
#define ENETUNREACH     WSAENETUNREACH

#ifdef EADDRINUSE 
#undef EADDRINUSE
#endif
#define EADDRINUSE      WSAEADDRINUSE

#ifdef EADDRINUSE 
#undef EADDRINUSE
#endif
#define EADDRINUSE      WSAEADDRINUSE

#ifdef EALREADY 
#undef EALREADY
#endif
#define EALREADY        WSAEALREADY

#ifdef ENOPROTOOPT 
#undef ENOPROTOOPT
#endif
#define ENOPROTOOPT     WSAENOPROTOOPT

#ifdef EMSGSIZE 
#undef EMSGSIZE
#endif
#define EMSGSIZE        WSAEMSGSIZE

#ifdef ECONNABORTED 
#undef ECONNABORTED
#endif
#define ECONNABORTED    WSAECONNABORTED
#endif

#define ofxNetworkCheckError() ofxNetworkCheckErrno( __FILE__, __LINE__ )

inline int ofxNetworkCheckErrno( const string & file, int line )
{
#ifdef TARGET_WIN32
    int	err = WSAGetLastError();
#else
    int err = errno;
#endif
    switch( err ) {
    case 0:
        break;
    case EBADF:
        std::cerr << "ofxNetwork: " << file << ": " << line << " EBADF: invalid socket.\n";
        break;
    case ECONNRESET:
        std::cerr << "ofxNetwork: " << file << ": " << line << " ECONNRESET: connection closed by peer.\n";
        break;
    case EINTR:
        std::cerr << "ofxNetwork: " << file << ": " << line << " EINTR: receive interrupted by a signal, before any data available.\n";
        break;
    case ENOTCONN:
        std::cerr << "ofxNetwork: " << file << ": " << line << " ENOTCONN: trying to receive before establishing a connection.\n";
        break;
    case ENOTSOCK:
        std::cerr << "ofxNetwork: " << file << ": " << line << " ENOTSOCK: socket argument is not a socket.\n";
        break;
    case EOPNOTSUPP:
        std::cerr << "ofxNetwork: " << file << ": " << line << " EOPNOTSUPP: specified flags not valid for this socket.\n";
        break;
    case ETIMEDOUT:
        std::cerr << "ofxNetwork: " << file << ": " << line << " ETIMEDOUT: timeout.\n";
        break;
    case EIO:
        std::cerr << "ofxNetwork: " << file << ": " << line << " EIO: io error.\n";
        break;
    case ENOBUFS:
        std::cerr << "ofxNetwork: " << file << ": " << line << " ENOBUFS: insufficient buffers to complete the operation.\n";
        break;
    case ENOMEM:
        std::cerr << "ofxNetwork: " << file << ": " << line << " ENOMEM: insufficient memory to complete the request.\n";
        break;
    case EADDRNOTAVAIL:
        std::cerr << "ofxNetwork: " << file << ": " << line << " EADDRNOTAVAIL: the specified address is not available on the remote machine.\n";
        break;
    case EAFNOSUPPORT:
        std::cerr << "ofxNetwork: " << file << ": " << line << " EAFNOSUPPORT: the namespace of the addr is not supported by this socket.\n";
        break;
    case EISCONN:
        std::cerr << "ofxNetwork: " << file << ": " << line << " EISCONN: the socket is already connected.\n";
        break;
    case ECONNREFUSED:
        std::cerr << "ofxNetwork: " << file << ": " << line << " ECONNREFUSED: the server has actively refused to establish the connection.\n";
        break;
    case ENETUNREACH:
        std::cerr << "ofxNetwork: " << file << ": " << line << " ENETUNREACH: the network of the given addr isn't reachable from this host.\n";
        break;
    case EADDRINUSE:
        std::cerr << "ofxNetwork: " << file << ": " << line << " EADDRINUSE: the socket address of the given addr is already in use.\n";
        break;
    case EINPROGRESS:
        std::cerr << "ofxNetwork: " << file << ": " << line << " EINPROGRESS: the socket is non-blocking and the connection could not be established immediately.\n";
        break;
    case EALREADY:
        std::cerr << "ofxNetwork: " << file << ": " << line << " EALREADY: the socket is non-blocking and already has a pending connection in progress.\n";
        break;
    case ENOPROTOOPT:
        std::cerr << "ofxNetwork: " << file << ": " << line << " ENOPROTOOPT: the optname doesn't make sense for the given level.\n";
        break;
    case EPROTONOSUPPORT:
        std::cerr << "ofxNetwork: " << file << ": " << line << " EPROTONOSUPPORT: the protocol or style is not supported by the namespace specified.\n";
        break;
    case EMFILE:
        std::cerr << "ofxNetwork: " << file << ": " << line << " EMFILE: the process already has too many file descriptors open.\n";
        break;
    case ENFILE:
        std::cerr << "ofxNetwork: " << file << ": " << line << " ENFILE: the system already has too many file descriptors open.\n";
        break;
    case EACCES:
        std::cerr << "ofxNetwork: " << file << ": " << line << " EACCES: the process does not have the privilege to create a socket of the specified style or protocol.\n";
        break;
    case EMSGSIZE:
        std::cerr << "ofxNetwork: " << file << ": " << line << " EMSGSIZE: the socket type requires that the message be sent atomically, but the message is too large for this to be possible.\n";
        break;
    case EPIPE:
        std::cerr << "ofxNetwork: " << file << ": " << line << " EPIPE: this socket was connected but the connection is now broken.\n";
        break;
    case EINVAL:
        std::cerr << "ofxNetwork: " << file << ": " << line << " EINVAL: invalid argument.\n";
        break;
    case EAGAIN:
        ////std::cerr << "ofxNetwork: " << file << ": " << line << " EAGAIN: try again.\n";
        break;
#ifdef TARGET_WIN32
    case WSAEWOULDBLOCK:
        // represents "resource temporarily unavailable", can be ignored
        break;
#endif
    default:
        std::cerr << "ofxNetwork: " << file << ": " << line << " unknown error: " << err << " see errno.h for description of the error.\n";
        break;
    }
    return err;
}
