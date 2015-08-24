/*******************************************************************************
TouchMessageListener

PURPOSE: Converts custom Windows 8 touch messages into TUIO cursor messages 
         that are sent out by UDP or TCP servers.  Windows 7 is not supported.
         The custom messages are defined in the TouchHook.cpp file of the 
         TouchHook project.
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
#ifndef HOOKSGUI_TOUCHMESSAGELISTENER_H
#define HOOKSGUI_TOUCHMESSAGELISTENER_H

#include <QObject>
#include <QString>
#include <memory>
#include <map>
#include <Windows.h>

namespace hooksCore { class TouchHooks2Tuio; }
namespace TUIO { class TuioCursorServer; }
namespace TUIO{ class TuioCursor; }
class QTimer;

namespace hooksCore
{
    class TouchMessageListener : public QObject
    {
        Q_OBJECT

    public:
        static const unsigned int TIMER_CALL_TIME,
                                  MAX_CURSOR_IDLE_TIME;

        TouchMessageListener();
        virtual ~TouchMessageListener();
        
        void setServerInfo( const QString & host, int udpPortOne, int udpPortTwo, int flashXmlPort );
        void initializeTuioServers();
        void setScreenDimensions( int x, int y, int width, int height );
        QString screenInfo();
        QString serverInfo();
        void setCustomMessageToListenFor( std::shared_ptr<hooksCore::TouchHooks2Tuio> touchHooks2Tuio );
        bool processWindowsGenericMessage( void * message );
        void startTimer();

        QString host();
        int tuioUdpChannelOnePort();
        int tuioUdpChannelTwoPort();
        int flashXmlChannelPort();

        bool useTuioUdpChannelOne();
        bool useTuioUdpChannelTwo();
        bool useFlashXmlTcpChannel();

        QString tuioUdpChannelOneStatus();
        QString tuioUdpChannelTwoStatus();
        QString flashXmlChannelStatus();

        QString tuioUdpServerOneStatus();
        QString tuioUdpServerTwoStatus();
        QString flashXmlTcpServerStatus();

        void useTuioUdpChannelOne( bool b );
        void useTuioUdpChannelTwo( bool b );
        void useFlashXmlTcpChannel( bool b );

    public slots:
        void processTimer();

    private:
        void processPointerDown( const MSG * msg );
        float scaledX( int x );
        float scaledY( int y );
        void processPointerUpdate( const MSG * msg );
        void processPointerUp( const MSG * msg );
        void processTouch( const MSG * msg );

        void printPointerDownMsg( unsigned int id, int x, int y );
        void printPointerDownScaledMsg( unsigned int id, float x, float y );
        void printPointerUpdateMsg( unsigned int id, int x, int y );
        void printPointerUpdateScaledMsg( unsigned int id, float x, float y );
        void printPointerUpMsg( unsigned int id );

        void printTuioCursor( TUIO::TuioCursor * tuioCursor );

        std::map<DWORD, TUIO::TuioCursor *> cursorMap_;
        std::shared_ptr<TUIO::TuioCursorServer> tuioCursorServer_;
        QString host_;
        int serverUdpPortOne_,
            serverUdpPortTwo_,
            serverFlashTcpPort_;
        bool useTuioUdpChannelOne_,
             useTuioUdpChannelTwo_,
             useFlashXmlTcpChannel_;
        unsigned int uwmCustomPointerdown_,
                     uwmCustomPointerUpdate_,
                     uwmCustomPointerUp_,
                     uwmCustomTouch_;
        QTimer * timer_;
        int screenOffsetX_,
            screenOffsetY_,
            screenWidth_,
            screenHeight_;
        bool mirroredMonitors_;
    };
}

#endif
