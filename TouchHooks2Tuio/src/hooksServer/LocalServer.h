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
#ifndef HOOKSSERVER_LOCALSERVER_H
#define HOOKSSERVER_LOCALSERVER_H

#include <QObject>
#include <QString>

class QLocalServer;
class QLocalSocket;
class QByteArray;

namespace hooksServer
{
    class LocalServer : public QObject
    {
        Q_OBJECT

    public:
        static const QString DEFAULT_SERVER_NAME,
                             RELEASE_HOOKS_MESSAGE,
                             SUCCESS_MESSAGE,
                             FAILURE_MESSAGE;

        LocalServer();
        ~LocalServer();

        void initialize( const QString & serverName );

    public slots:
        void newConnection();
        void readyRead();
        void disconnected();

    signals:
        void clientRequestsGlobalHookRelease();

    protected:

    private:
        QString readClientMessage( QLocalSocket * clientConnection );
        void addMessageToBlock( QByteArray & block, const QString & message );
        void debugPrintServerStarted();
        void debugPrintServerFailed();

        QLocalServer * server_;
        QString serverName_;
    };
}

#endif
