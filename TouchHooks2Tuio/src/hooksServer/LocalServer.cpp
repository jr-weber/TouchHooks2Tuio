/*******************************************************************************
LocalServer

PURPOSE: Allows another program to send commands to TouchHooks2Tuio by using
         a QLocalSocket.  This feature is intended primarily for use by the 
         PlaysurfaceLauncher program.  For the TUIO message servers, look in
         the hooksCore::TouchMessageListener class.

AUTHOR:  J.R. Weber <joe.weber77@gmail.com>
CREATED: 6/17/2015
*******************************************************************************/
/*
 TouchHooks2Tuio - Windows 8 Touch to TUIO Bridge
 
 Copyright (c) 2015 J.R.Weber <joe.weber77@gmail.com>

 Look in TouchHook.h or TouchHook.cpp of the TouchHook project
 for the original copyright and license statement by 
 Marc Herrlich and Benjamin Walther-Franks, University of Bremen.
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License along with 
 this program.  If not, go to http://www.gnu.org/licenses/gpl-3.0.en.html or
 write to 
 
 Free Software  Foundation, Inc.
 59 Temple Place, Suite 330
 Boston, MA  02111-1307  USA
*/
#include "hooksServer/LocalServer.h"
#include <QLocalServer>
#include <QLocalSocket>
#include <QByteArray>
#include <iostream>

using hooksServer::LocalServer;

const QString LocalServer::DEFAULT_SERVER_NAME = "TouchHooks2Tuio-LocalServer",
              LocalServer::RELEASE_HOOKS_MESSAGE = "TouchHooks2Tuio:RELEASE_HOOKS",
              LocalServer::SUCCESS_MESSAGE = "TouchHooks2Tuio:Success",
              LocalServer::FAILURE_MESSAGE = "TouchHooks2Tuio:Failure";

LocalServer::LocalServer() :
  server_( new QLocalServer( this ) ),
  serverName_( DEFAULT_SERVER_NAME )
{
}

LocalServer::~LocalServer()
{
    delete server_;
}

void LocalServer::initialize( const QString & serverName )
{
    serverName_ = (serverName.size() > 0) ? serverName : DEFAULT_SERVER_NAME;
    bool ok = server_->listen( serverName_ );

    if( ok ) {
        debugPrintServerStarted();
        connect( server_, 
                 SIGNAL( newConnection() ), 
                 this, 
                 SLOT( newConnection() ) );
    }
    else {
        debugPrintServerFailed();
    }
}

void LocalServer::newConnection()
{
    if( server_->hasPendingConnections() ) {
        QLocalSocket * socket = server_->nextPendingConnection();

        connect( socket, 
                 SIGNAL( readyRead() ), 
                 this, 
                 SLOT( readyRead() ) );
        connect( socket, 
                 SIGNAL( disconnected() ),
                 this, 
                 SLOT( disconnected() ) );
    }
}

void LocalServer::readyRead()
{
    QLocalSocket * socket = static_cast<QLocalSocket *>( sender() );
    QString clientMessage = readClientMessage( socket );
    QString serverMessage;

    if( clientMessage.compare( RELEASE_HOOKS_MESSAGE, Qt::CaseInsensitive ) == 0 ) {
        emit clientRequestsGlobalHookRelease();
        serverMessage = SUCCESS_MESSAGE;
    }
    else {
        serverMessage = FAILURE_MESSAGE;
        std::cout << "LocalServer: " << serverMessage.toStdString() << "\n";
    }
    QByteArray block;
    addMessageToBlock( block, serverMessage );
    socket->write( block );
}

QString LocalServer::readClientMessage( QLocalSocket * clientConnection )
{
    QString clientMessage;
    QDataStream in( clientConnection );
    in.setVersion( QDataStream::Qt_5_4 );
    int bytesAvailable = clientConnection->bytesAvailable();
    quint16 blockSize = 0;

    if( bytesAvailable < (int)sizeof(quint16) ) {
        std::cout << "LocalServer::readClientMessage(): 0 bytes in message.\n";
        return "";
    }
    in >> blockSize;
    
    if( bytesAvailable < blockSize ) {
        return "";
    }
    in >> clientMessage;
    return clientMessage;
}

void LocalServer::addMessageToBlock( QByteArray & block, const QString & message )
{
    QDataStream out( &block, QIODevice::ReadWrite );
    out.setVersion( QDataStream::Qt_5_4 );
    out << (quint16)0;
    out << message;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof( quint16 ));
}

void LocalServer::disconnected()
{
    QLocalServer * socket = static_cast<QLocalServer *>( sender() );
    socket->deleteLater();
}

void LocalServer::debugPrintServerStarted()
{
    QString msg = "The local server started successfully: " + serverName_ + "\n";
    std::cout << msg.toStdString();
}

void LocalServer::debugPrintServerFailed()
{
    // TODO: Open dialog box?  Or throw custom exception? Or just log?
    QString errMsg = server_->errorString();
    std::cout << serverName_.toStdString()
              << " could not be initialized:\n"
              << errMsg.toStdString() << "\n";
}
