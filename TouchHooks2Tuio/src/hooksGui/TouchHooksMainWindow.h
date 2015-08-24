/*******************************************************************************
TouchHooksMainWindow

PURPOSE: Provides a graphical user interface for the TouchHooks2Tuio program,
         which intercepts Windows 8 touch events and converts them to 
         "/tuio/2Dcur" messages that are sent out on either a TUIO UDP 
         server or a Flash XML TCP server.  Emulated mouse clicks are also
         blocked so that a multitouch app can be restricted to TUIO touch
         only.

AUTHOR:  J.R. Weber <joe.weber77@gmail.com>
CREATED: 6/16/2015
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
#ifndef HOOKSGUI_TOUCHHOOKSMAINWINDOW_H
#define HOOKSGUI_TOUCHHOOKSMAINWINDOW_H

#include "ui_TouchHooksMainWindow.h"
#include <QMainWindow>
#include <memory>

namespace hooksXml { class XmlSettings; }
namespace hooksCore { class TouchHooks2Tuio; }
namespace hooksCore { class TouchMessageListener; }
namespace hooksGui { class SignalsToSlots; }
namespace hooksServer { class LocalServer; }

namespace hooksGui
{
    class TouchHooksMainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        TouchHooksMainWindow();
        virtual ~TouchHooksMainWindow();

        void readXmlConfigFile();
        void updateServerHostAndPorts();
        std::shared_ptr<hooksCore::TouchMessageListener> touchMessageListener();
        std::shared_ptr<hooksXml::XmlSettings> xmlSettings();
        void setNetworkMenuCheckboxes( bool udpChannelOne, bool udpChannelTwo, bool flashXml );
        void initializeConsolePidForDebugging( int consolePid );
        void initializeCustomMessagesForHook();
        void initializeTuioServers();
        void setTuioChannelsOnOrOff();
        void writeScreenInfo();
        void writeServerInfo();
        void writeToGuiTextArea( const QString & message );
        void initializeLocalServer( const QString & serverName );
        void startTimer();
        
        void initializeGlobalTouchHook();
        bool useGlobalTouchHook();
        void quietlySetHooksMenuOption( bool useGlobalTouchHook );

    public slots:
        void useGlobalTouchHook( bool b );
        void attachGlobalTouchHook();
        void removeGlobalTouchHook();

        void useTuioUdpChannelOne( bool b );
        void useTuioUdpChannelTwo( bool b );
        void useFlashXmlTcpChannel( bool b );

    signals:


    protected:
        virtual void closeEvent( QCloseEvent * event );
        virtual bool nativeEvent( const QByteArray & eventType, void * message, long * result );

    private:
        Ui::TouchHooksMainWindowUi ui_;
        std::shared_ptr<hooksXml::XmlSettings> xmlSettings_;
        std::shared_ptr<hooksCore::TouchHooks2Tuio> touchHooks2Tuio_;
        std::shared_ptr<hooksCore::TouchMessageListener> touchMessageListener_;
        std::shared_ptr<hooksServer::LocalServer> localServer_;
        std::unique_ptr<hooksGui::SignalsToSlots> signalsToSlots_;
    };
}

#endif
