#include "ofxTCPServer.h"
#include "ofxTCPClient.h"
#include <iostream>

//--------------------------
ofxTCPServer::ofxTCPServer()
{
    connected = false;
    idCount = 0;
    port = 0;
    str = "";
    messageDelimiter = "[/TCP]";
    bClientBlocking = false;
}

//--------------------------
ofxTCPServer::~ofxTCPServer()
{
    close();
}

//--------------------------
bool ofxTCPServer::setup( int _port, bool blocking )
{
    if( !TCPServer.Create() ) {
        std::cerr << "ofxTCPServer: setup(): couldn't create server.\n";
        return false;
    }
    if( !TCPServer.Bind( _port ) ) {
        std::cerr << "ofxTCPServer: setup(): couldn't bind to port " << _port << "\n";
        return false;
    }
    connected = true;
    port = _port;
    bClientBlocking = blocking;

    startThread( true );
    return true;
}

//--------------------------
void ofxTCPServer::setMessageDelimiter( string delim )
{
    if( delim != "" ) {
        messageDelimiter = delim;
    }
}

//--------------------------
bool ofxTCPServer::close()
{

    if( !connected ) return true;

    mConnectionsLock.lock();
    map<int, ofPtr<ofxTCPClient> >::iterator it;
    for( it = TCPConnections.begin(); it != TCPConnections.end(); it++ ) {
        it->second->close();
    }
    TCPConnections.clear();
    mConnectionsLock.unlock();	//	unlock for thread
    stopThread();

    if( !TCPServer.Close() ) {
        std::cerr << "ofxTCPServer: close(): couldn't close connections.\n";
        // Temporarily commented out next line due to crash when using with TouchHooks2Tuio.
        ////waitForThread(false); //stop the thread
        return false;
    }
    else {
        connected = false;
        // Temporarily commented out next line due to crash when using with TouchHooks2Tuio.
        ////waitForThread(false); //stop the thread
        return true;
    }
}

ofxTCPClient & ofxTCPServer::getClient( int clientID )
{
    return *TCPConnections.find( clientID )->second;
}

//--------------------------
bool ofxTCPServer::disconnectClient( int clientID )
{
    ofMutex::ScopedLock Lock( mConnectionsLock );

    if( !isClientSetup( clientID ) ) {
        std::cerr << "ofxTCPServer: disconnectClient(): client " << clientID << " doesn't exist.\n";
        return false;
    }
    else if( getClient( clientID ).close() ) {
        TCPConnections.erase( clientID );
        return true;
    }
    return false;
}

//--------------------------
bool ofxTCPServer::send( int clientID, string message )
{
    ofMutex::ScopedLock Lock( mConnectionsLock );
    if( !isClientSetup( clientID ) ) {
        std::cerr << "ofxTCPServer: send(): client " << clientID << " doesn't exist.\n";
        return false;
    }
    else {
        getClient( clientID ).send( message );
        if( !getClient( clientID ).isConnected() ) TCPConnections.erase( clientID );
        return true;
    }
}

//--------------------------
bool ofxTCPServer::sendToAll( string message )
{
    ofMutex::ScopedLock Lock( mConnectionsLock );
    if( TCPConnections.size() == 0 ) return false;

    map<int, ofPtr<ofxTCPClient> >::iterator it;
    vector<int> disconnect;
    for( it = TCPConnections.begin(); it != TCPConnections.end(); it++ ) {
        if( it->second->isConnected() ) it->second->send( message );
        else disconnect.push_back( it->first );
    }
    for( int i = 0; i < (int)disconnect.size(); i++ ) {
        TCPConnections.erase( disconnect[i] );
    }
    return true;
}

//--------------------------
string ofxTCPServer::receive( int clientID )
{
    ofMutex::ScopedLock Lock( mConnectionsLock );
    if( !isClientSetup( clientID ) ) {
        std::cerr << "ofxTCPServer: receive(): client " << clientID << " doesn't exist.\n";
        ostringstream out;
        out << clientID;
        return "client " + out.str() + "doesn't exist";
    }
    if( !getClient( clientID ).isConnected() ) {
        disconnectClient( clientID );
        return "";
    }
    return getClient( clientID ).receive();
}

//--------------------------
bool ofxTCPServer::sendRawBytes( int clientID, const char * rawBytes, const int numBytes )
{
    ofMutex::ScopedLock Lock( mConnectionsLock );
    if( !isClientSetup( clientID ) ) {
        std::cerr << "ofxTCPServer: sendRawBytes(): client " << clientID << " doesn't exist.\n";

        return false;
    }
    else {
        return getClient( clientID ).sendRawBytes( rawBytes, numBytes );
    }
}

//--------------------------
bool ofxTCPServer::sendRawBytesToAll( const char * rawBytes, const int numBytes )
{
    ofMutex::ScopedLock Lock( mConnectionsLock );
    if( TCPConnections.size() == 0 || numBytes <= 0 ) return false;

    map<int, ofPtr<ofxTCPClient> >::iterator it;

    for( it = TCPConnections.begin(); it != TCPConnections.end(); it++ ) {
        if( it->second->isConnected() )it->second->sendRawBytes( rawBytes, numBytes );
    }
    return true;
}


//--------------------------
bool ofxTCPServer::sendRawMsg( int clientID, const char * rawBytes, const int numBytes )
{
    ofMutex::ScopedLock Lock( mConnectionsLock );

    if( !isClientSetup( clientID ) ) {
        std::cerr << "ofxTCPServer: sendRawMsg(): client " << clientID << " doesn't exist.\n";
        return false;
    }
    else {
        return getClient( clientID ).sendRawMsg( rawBytes, numBytes );
    }
}

//--------------------------
bool ofxTCPServer::sendRawMsgToAll( const char * rawBytes, const int numBytes )
{
    ofMutex::ScopedLock Lock( mConnectionsLock );
    if( TCPConnections.size() == 0 || numBytes <= 0 ) return false;

    map<int, ofPtr<ofxTCPClient> >::iterator it;

    for( it = TCPConnections.begin(); it != TCPConnections.end(); it++ ) {
        if( it->second->isConnected() )it->second->sendRawMsg( rawBytes, numBytes );
    }
    return true;
}

//--------------------------
int ofxTCPServer::getNumReceivedBytes( int clientID )
{
    ofMutex::ScopedLock Lock( mConnectionsLock );

    if( !isClientSetup( clientID ) ) {
        std::cerr << "ofxTCPServer: getNumReceivedBytes(): client " << clientID << " doesn't exist.\n";
        return 0;
    }
    return getClient( clientID ).getNumReceivedBytes();
}

//--------------------------
int ofxTCPServer::receiveRawBytes( int clientID, char * receiveBytes, int numBytes )
{
    ofMutex::ScopedLock Lock( mConnectionsLock );

    if( !isClientSetup( clientID ) ) {
        std::cerr << "ofxTCPServer: receiveRawBytes(): client " << clientID << " doesn't exist.\n";
        return 0;
    }
    return getClient( clientID ).receiveRawBytes( receiveBytes, numBytes );
}


//--------------------------
int ofxTCPServer::receiveRawMsg( int clientID, char * receiveBytes, int numBytes )
{
    ofMutex::ScopedLock Lock( mConnectionsLock );

    if( !isClientSetup( clientID ) ) {
        std::cerr << "ofxTCPServer: receiveRawMsg(): client " << clientID << " doesn't exist.\n";
        return 0;
    }
    return getClient( clientID ).receiveRawMsg( receiveBytes, numBytes );
}

//--------------------------
int ofxTCPServer::getClientPort( int clientID )
{
    ofMutex::ScopedLock Lock( mConnectionsLock );

    if( !isClientSetup( clientID ) ) {
        std::cerr << "ofxTCPServer: getClientPort(): client " << clientID << " doesn't exist.\n";
        return 0;
    }
    else return getClient( clientID ).getPort();
}

//--------------------------
string ofxTCPServer::getClientIP( int clientID )
{
    ofMutex::ScopedLock Lock( mConnectionsLock );

    if( !isClientSetup( clientID ) ) {
        std::cerr << "ofxTCPServer: getClientIP(): client " << clientID << " doesn't exist.\n";
        return "000.000.000.000";
    }
    else return getClient( clientID ).getIP();
}

//--------------------------
int ofxTCPServer::getNumClients()
{
    ofMutex::ScopedLock Lock( mConnectionsLock );
    return TCPConnections.size();
}

//--------------------------
int ofxTCPServer::getLastID()
{
    return idCount;
}

//--------------------------
int ofxTCPServer::getPort()
{
    return port;
}

//--------------------------
bool ofxTCPServer::isConnected()
{
    return connected;
}

//--------------------------
bool ofxTCPServer::isClientSetup( int clientID )
{
    return TCPConnections.find( clientID ) != TCPConnections.end();
}

//--------------------------
bool ofxTCPServer::isClientConnected( int clientID )
{
    ofMutex::ScopedLock Lock( mConnectionsLock );
    return isClientSetup( clientID ) && getClient( clientID ).isConnected();
}

//don't call this
//--------------------------
void ofxTCPServer::threadedFunction()
{
    std::cerr << "ofxTCPServer: listening thread started.\n";

    while( isThreadRunning() ) {

        int acceptId;
        for( acceptId = 0; acceptId <= idCount; acceptId++ ) {
            if( !isClientConnected( acceptId ) ) break;
        }

        if( acceptId == TCP_MAX_CLIENTS ) {
            std::cerr << "ofxTCPServer: no longer accepting connections, maximum number of clients reached: " << TCP_MAX_CLIENTS << "\n";
            break;
        }

        if( !TCPServer.Listen( TCP_MAX_CLIENTS ) ) {
            if( isThreadRunning() ) {
                std::cerr << "ofxTCPServer: listening failed.\n";
            }
        }

        //	we need to lock here, but can't as it blocks...
        //	so use a temporary to not block the lock 
        ofPtr<ofxTCPClient> client( new ofxTCPClient );
        if( !TCPServer.Accept( client->TCPClient ) ) {
            if( isThreadRunning() ) {
                std::cerr << "ofxTCPServer: couldn't accept client " << acceptId << "\n";
            }
        }
        else {
            ofMutex::ScopedLock Lock( mConnectionsLock );
            //	take owenership of socket from NewClient
            TCPConnections[acceptId] = client;
            TCPConnections[acceptId]->setup( acceptId, bClientBlocking );
            TCPConnections[acceptId]->setMessageDelimiter( messageDelimiter );
            std::cerr << "ofxTCPServer: client " << acceptId << " connected on port " << TCPConnections[acceptId]->getPort() << "\n";
            if( acceptId == idCount ) idCount++;
        }
    }
    idCount = 0;
    std::cerr << "ofxTCPServer: listening thread stopped.\n";
}
