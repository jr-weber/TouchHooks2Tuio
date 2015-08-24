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
#include "hooksGui/TouchHooksMainWindow.h"
#include "hooksXml/XmlSettings.h"
#include "hooksGui/SignalsToSlots.h"
#include "hooksCore/TouchHooks2Tuio.h"
#include "hooksCore/TouchMessageListener.h"
#include "hooksServer/LocalServer.h"
#include <iostream>

using hooksGui::TouchHooksMainWindow;

TouchHooksMainWindow::TouchHooksMainWindow() :
  ui_(),
  xmlSettings_( std::make_shared<hooksXml::XmlSettings>() ),
  touchHooks2Tuio_( std::make_shared<hooksCore::TouchHooks2Tuio>() ),
  touchMessageListener_( std::make_shared<hooksCore::TouchMessageListener>() ),
  localServer_( std::make_shared<hooksServer::LocalServer>() ),
  signalsToSlots_( std::make_unique<hooksGui::SignalsToSlots>( this, 
                                                               touchHooks2Tuio_.get(), 
                                                               touchMessageListener_.get(),
                                                               localServer_.get(),
                                                               &ui_ ) )
{
    ui_.setupUi( this );
    signalsToSlots_->makeConnections();
}

TouchHooksMainWindow::~TouchHooksMainWindow()
{
    removeGlobalTouchHook();
}

void TouchHooksMainWindow::readXmlConfigFile()
{
    xmlSettings_->readXmlConfigFile();
}

void TouchHooksMainWindow::updateServerHostAndPorts()
{
    xmlSettings_->updateServerHostAndPorts( this );
}

void TouchHooksMainWindow::setTuioChannelsOnOrOff()
{
    xmlSettings_->setTuioChannelsOnOrOff( this );
}

std::shared_ptr<hooksCore::TouchMessageListener> TouchHooksMainWindow::touchMessageListener()
{
    return touchMessageListener_;
}

std::shared_ptr<hooksXml::XmlSettings> TouchHooksMainWindow::xmlSettings()
{
    return xmlSettings_;
}

/**
 * Checks or unchecks the Network menu options quietly (no signals fired off).
 */
void TouchHooksMainWindow::setNetworkMenuCheckboxes( bool udpChannelOne, bool udpChannelTwo, bool flashXml )
{
    ui_.action_tuioUdpChannelOne->blockSignals( true );
    ui_.action_tuioUdpChannelTwo->blockSignals( true );
    ui_.action_flashXmlTcpChannel->blockSignals( true );

    ui_.action_tuioUdpChannelOne->setChecked( udpChannelOne );
    ui_.action_tuioUdpChannelTwo->setChecked( udpChannelTwo );
    ui_.action_flashXmlTcpChannel->setChecked( flashXml );

    ui_.action_tuioUdpChannelOne->blockSignals( false );
    ui_.action_tuioUdpChannelTwo->blockSignals( false );
    ui_.action_flashXmlTcpChannel->blockSignals( false );
}

void TouchHooksMainWindow::initializeConsolePidForDebugging( int consolePid )
{
    touchHooks2Tuio_->initializeConsolePidForDebugging( consolePid );
}

void TouchHooksMainWindow::initializeCustomMessagesForHook()
{
    touchMessageListener_->setCustomMessageToListenFor( touchHooks2Tuio_ );
}

void TouchHooksMainWindow::initializeTuioServers()
{
    touchMessageListener_->initializeTuioServers();
}

void TouchHooksMainWindow::useTuioUdpChannelOne( bool b )
{
    touchMessageListener_->useTuioUdpChannelOne( b );
    writeToGuiTextArea( touchMessageListener_->tuioUdpChannelOneStatus() );
}

void TouchHooksMainWindow::useTuioUdpChannelTwo( bool b )
{
    touchMessageListener_->useTuioUdpChannelTwo( b );
    writeToGuiTextArea( touchMessageListener_->tuioUdpChannelTwoStatus() );
}

void TouchHooksMainWindow::useFlashXmlTcpChannel( bool b )
{
    touchMessageListener_->useFlashXmlTcpChannel( b );
    writeToGuiTextArea( touchMessageListener_->flashXmlChannelStatus() );
}

void TouchHooksMainWindow::initializeLocalServer( const QString & serverName )
{
    localServer_->initialize( serverName );
}


void TouchHooksMainWindow::initializeGlobalTouchHook()
{
    xmlSettings_->initializeGlobalTouchHook( this );
}

void TouchHooksMainWindow::useGlobalTouchHook( bool b )
{
    if( b ) {
        attachGlobalTouchHook();
    }
    else {
        removeGlobalTouchHook();
    }
}

void TouchHooksMainWindow::attachGlobalTouchHook()
{
    bool ok = touchHooks2Tuio_->initializeGlobalTouchHook();
    QString msg = ok ? "Attached " : "Failed to attach ";
    writeToGuiTextArea( msg + "global touch hook." );
}

void TouchHooksMainWindow::removeGlobalTouchHook()
{
    bool ok = touchHooks2Tuio_->removeGlobalTouchHook();
    QString msg = ok ? "Removed " : "Failed to remove ";
    writeToGuiTextArea( msg + "global touch hook." );
}

bool TouchHooksMainWindow::useGlobalTouchHook()
{
    return touchHooks2Tuio_->useGlobalTouchHook();
}

/**
 * Checks or unchecks the Hooks menu option quietly (no signals fired off).
 */
void TouchHooksMainWindow::quietlySetHooksMenuOption( bool useGlobalTouchHook )
{
    ui_.action_useGlobalTouchHook->blockSignals( true );
    ui_.action_useGlobalTouchHook->setChecked( useGlobalTouchHook );
    ui_.action_useGlobalTouchHook->blockSignals( false );
}

void TouchHooksMainWindow::closeEvent( QCloseEvent * event )
{
    setVisible( false );
    xmlSettings_->saveSettingsToXmlFile( this );
    QMainWindow::closeEvent( event );
}

void TouchHooksMainWindow::writeToGuiTextArea( const QString & message )
{
    ui_.plainText_guiTextArea->appendPlainText( message );
}

void TouchHooksMainWindow::writeScreenInfo()
{
    writeToGuiTextArea( touchMessageListener_->screenInfo() );
}

void TouchHooksMainWindow::writeServerInfo()
{
    writeToGuiTextArea( touchMessageListener_->serverInfo() );
}

bool TouchHooksMainWindow::nativeEvent( const QByteArray & eventType, void * message, long * result )
{
    bool retValue = false;

    if( eventType == "windows_generic_MSG") {
        retValue = touchMessageListener_->processWindowsGenericMessage( message );

        if( retValue ) {
            *result =  MA_NOACTIVATEANDEAT;
        }
    }
    return retValue;
}

void TouchHooksMainWindow::startTimer()
{
    touchMessageListener_->startTimer();
}
