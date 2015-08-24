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
#include "hooksCore/TouchMessageListener.h"
#include "hooksCore/TouchHooks2Tuio.h"
#include "TuioCursorServer.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <iostream>

using hooksCore::TouchMessageListener;

const unsigned int TouchMessageListener::TIMER_CALL_TIME = 100,
                   TouchMessageListener::MAX_CURSOR_IDLE_TIME = 300;

TouchMessageListener::TouchMessageListener() :
  cursorMap_(),
  tuioCursorServer_( 0 ),
  host_( "127.0.0.1" ),
  serverUdpPortOne_( 3333 ), 
  serverUdpPortTwo_( 3334 ), 
  serverFlashTcpPort_( 3000 ),
  useTuioUdpChannelOne_( true ),
  useTuioUdpChannelTwo_( true ),
  useFlashXmlTcpChannel_( true ),
  uwmCustomPointerdown_( 0 ),
  uwmCustomPointerUpdate_( 0 ),
  uwmCustomPointerUp_( 0 ),
  uwmCustomTouch_( 0 ),
  timer_( new QTimer( this ) ),
  screenOffsetX_( 0 ),
  screenOffsetY_( 0 ),
  screenWidth_( 1920 ),
  screenHeight_( 1080 ),
  mirroredMonitors_( true )
{
    QRect screenRect_ = QApplication::desktop()->screenGeometry();
    setScreenDimensions( screenRect_.x(), 
                         screenRect_.y(), 
                         screenRect_.width(), 
                         screenRect_.height() );
}

TouchMessageListener::~TouchMessageListener()
{
    delete timer_;
}

void TouchMessageListener::setServerInfo( const QString & host,
                                          int udpPortOne,
                                          int udpPortTwo,
                                          int flashXmlPort )
{
    host_ = host;
    serverUdpPortOne_ = udpPortOne;
    serverUdpPortTwo_ = udpPortTwo;
    serverFlashTcpPort_ = flashXmlPort;
}

void TouchMessageListener::useTuioUdpChannelOne( bool b )
{
    useTuioUdpChannelOne_ = b;
    tuioCursorServer_->useFirstUdpSender( useTuioUdpChannelOne_ );
}

void TouchMessageListener::useTuioUdpChannelTwo( bool b )
{
    useTuioUdpChannelTwo_ = b;
    tuioCursorServer_->useSecondUdpSender( useTuioUdpChannelTwo_ );
}

void TouchMessageListener::useFlashXmlTcpChannel( bool b )
{
    useFlashXmlTcpChannel_ = b;
    tuioCursorServer_->useFlashXmlTcpSender( useFlashXmlTcpChannel_ );
}

QString TouchMessageListener::host()
{
    return host_;
}

int TouchMessageListener::tuioUdpChannelOnePort()
{
    return serverUdpPortOne_;
}

int TouchMessageListener::tuioUdpChannelTwoPort()
{
    return serverUdpPortTwo_;
}

int TouchMessageListener::flashXmlChannelPort()
{
    return serverFlashTcpPort_;
}

bool TouchMessageListener::useTuioUdpChannelOne()
{
    return useTuioUdpChannelOne_;
}

bool TouchMessageListener::useTuioUdpChannelTwo()
{
    return useTuioUdpChannelTwo_;
}

bool TouchMessageListener::useFlashXmlTcpChannel()
{
    return useFlashXmlTcpChannel_;
}

void TouchMessageListener::initializeTuioServers()
{
    std::string hostName = host_.toStdString();
    tuioCursorServer_.reset( new TUIO::TuioCursorServer( hostName.c_str(), 
                                                         serverUdpPortOne_, 
                                                         serverUdpPortTwo_, 
                                                         serverFlashTcpPort_ ) );

    tuioCursorServer_->useFirstUdpSender( useTuioUdpChannelOne_ );
    tuioCursorServer_->useSecondUdpSender( useTuioUdpChannelTwo_ );
    tuioCursorServer_->useFlashXmlTcpSender( useFlashXmlTcpChannel_ );
}

void TouchMessageListener::startTimer()
{
    connect( timer_, SIGNAL( timeout() ), this, SLOT( processTimer() ) );
    timer_->start( TIMER_CALL_TIME );
}

void TouchMessageListener::setScreenDimensions( int x, int y, int width, int height )
{
    screenOffsetX_ = x;
    screenOffsetY_ = y;
    screenWidth_ = width;
    screenHeight_ = height;
}

void TouchMessageListener::setCustomMessageToListenFor( std::shared_ptr<hooksCore::TouchHooks2Tuio> touchHooks2Tuio )
{
    uwmCustomPointerdown_ = touchHooks2Tuio->getUwmCustomPointerDown();
    uwmCustomPointerUpdate_ = touchHooks2Tuio->getUwmCustomPointerUpdate();
    uwmCustomPointerUp_ = touchHooks2Tuio->getUwmCustomPointerUp();
    uwmCustomTouch_ = touchHooks2Tuio->getUwmCustomTouched();
}

bool TouchMessageListener::processWindowsGenericMessage( void * message )
{
    bool retValue = true;
    const MSG * msg = reinterpret_cast<MSG *>(message);

    if( msg->message == uwmCustomPointerdown_ ) {
        processPointerDown( msg );
    }
    else if( msg->message == uwmCustomPointerUpdate_ ) {
        processPointerUpdate( msg );
    }
    else if( msg->message == uwmCustomPointerUp_ ) {
        processPointerUp( msg );
    }
    else if( msg->message == uwmCustomTouch_ ) {
        processTouch( msg );
    }
    else {
        retValue = false;
    }
    return retValue;
}

/**
 * For Windows 8 touch WM_POINTERDOWN message.
 */
void TouchMessageListener::processPointerDown( const MSG * msg )
{
    UINT32 id = GET_POINTERID_WPARAM( msg->wParam );
    POINTS p = MAKEPOINTS( msg->lParam );
    float x = scaledX( p.x ),
          y = scaledY( p.y );

    tuioCursorServer_->initFrame( TUIO::TuioTime::getSessionTime() );
    cursorMap_[id] = tuioCursorServer_->addTuioCursor( x, y );
    tuioCursorServer_->commitFrame();

    //printPointerDownMsg( id, p.x, p.y );
    //printPointerDownScaledMsg( id, x, y );
    //printTuioCursor( cursorMap_[id] );
}

float TouchMessageListener::scaledX( int x )
{
    int mirroredX = x + screenOffsetX_,
        sideBySideX = (x - screenWidth_) + screenOffsetX_,
        screenX = mirroredMonitors_ ? mirroredX : sideBySideX,
        screenWidth = screenWidth_ + screenOffsetX_;

    return (float)screenX / screenWidth;
}

float TouchMessageListener::scaledY( int y )
{
    int screenY = y + screenOffsetY_,
        screenHeight = screenHeight_ + screenOffsetY_;

    return (float)screenY / screenHeight;
}

/**
 * For Windows 8 touch WM_POINTERUPDATE message.
 */
void TouchMessageListener::processPointerUpdate( const MSG * msg )
{
    UINT32 id = GET_POINTERID_WPARAM( msg->wParam );
    POINTS p = MAKEPOINTS( msg->lParam );   
    float x = scaledX( p.x ),
          y = scaledY( p.y );

    tuioCursorServer_->initFrame( TUIO::TuioTime::getSessionTime() );
    tuioCursorServer_->updateTuioCursor( cursorMap_[id], x, y );
    tuioCursorServer_->commitFrame();

    //printPointerUpdateMsg( id, p.x, p.y );
    //printPointerUpdateScaledMsg( id, x, y );
    //printTuioCursor( cursorMap_[id] );
}

/**
 * For Windows 8 touch WM_POINTERUP message.
 */
void TouchMessageListener::processPointerUp( const MSG * msg )
{
    UINT32 id = GET_POINTERID_WPARAM( msg->wParam );

    tuioCursorServer_->initFrame( TUIO::TuioTime::getSessionTime() );
    tuioCursorServer_->removeTuioCursor( cursorMap_[id] );
    tuioCursorServer_->commitFrame();
    cursorMap_.erase( cursorMap_.find( id ) );

    //printPointerUpMsg( id );
}

/**
 * Windows 7 touch is not supported.
 */
void TouchMessageListener::processTouch( const MSG * msg )
{
    //std::cout << "TouchMessageListener: processTouch() called...\n";
}

void TouchMessageListener::processTimer()
{
    bool frameOpen = false;
    std::map<DWORD, TUIO::TuioCursor *>::iterator iter = cursorMap_.begin();

    while( iter != cursorMap_.end() ) {
        long delta = TUIO::TuioTime::getSessionTime().getTotalMilliseconds()
                   - iter->second->getTuioTime().getTotalMilliseconds();
        //std::cout << "delta = " << delta << "\n";

        if( delta > MAX_CURSOR_IDLE_TIME ) {
            if( !frameOpen ) {
                tuioCursorServer_->initFrame( TUIO::TuioTime::getSessionTime() );
                frameOpen = true;
            }
            tuioCursorServer_->removeTuioCursor( iter->second );
            std::map<DWORD, TUIO::TuioCursor*>::iterator tmp = iter++;
            cursorMap_.erase( tmp );
            //std::cout << "Removed cursor due to time limit\n";
        }
        else  {
            ++iter;
        }
    }
    if( frameOpen ) {
        tuioCursorServer_->commitFrame();
    }
}

QString TouchMessageListener::screenInfo()
{
    return "Screen offset (x, y) = ("
           + QString::number( screenOffsetX_ ) + ", " 
           + QString::number( screenOffsetY_ ) + ")\n"
           + "Screen width x height = "
           + QString::number( screenWidth_ ) + " x "
           + QString::number( screenHeight_ );
}

QString TouchMessageListener::serverInfo()
{
    return "\nHost: " + host_ + "\n"
           + tuioUdpServerOneStatus()
           + tuioUdpServerTwoStatus()
           + flashXmlTcpServerStatus()
           + "\n"
           + tuioUdpChannelOneStatus() + "\n"
           + tuioUdpChannelTwoStatus() + "\n"
           + flashXmlChannelStatus() + "\n";
}

QString TouchMessageListener::tuioUdpServerOneStatus()
{
    bool ok = tuioCursorServer_->isFirstUdpSenderRunning();
    return "TUIO UDP channel 1 on port " 
           + QString::number( serverUdpPortOne_ )
           + (ok ? ": Server started ok.\n" : ": Server failed to start.\n" );
}

QString TouchMessageListener::tuioUdpServerTwoStatus()
{
    bool ok = tuioCursorServer_->isSecondUdpSenderRunning();
    return "TUIO UDP channel 2 on port "
           + QString::number( serverUdpPortTwo_ )
           + (ok ? ": Server started ok.\n" : ": Server failed to start.\n" );
}

QString TouchMessageListener::flashXmlTcpServerStatus()
{
    bool ok = tuioCursorServer_->isFlashXmlTcpSenderRunning();
    return "Flash XML channel on port " 
           + QString::number( serverFlashTcpPort_ )
           + (ok ? ": Server started ok.\n" : ": Server failed to start.\n" );
}

QString TouchMessageListener::tuioUdpChannelOneStatus()
{
    QString msg = "Network menu: TUIO UDP channel 1 is ";
    msg += useTuioUdpChannelOne_ ? "ON" : "OFF";
    return msg;
}

QString TouchMessageListener::tuioUdpChannelTwoStatus()
{
    QString msg = "Network menu: TUIO UDP channel 2 is ";
    msg += useTuioUdpChannelTwo_ ? "ON" : "OFF";
    return msg;
}

QString TouchMessageListener::flashXmlChannelStatus()
{
    QString msg = "Network menu: Flash XML channel is ";
    msg += useFlashXmlTcpChannel_ ? "ON" : "OFF";
    return msg;
}

void TouchMessageListener::printPointerDownMsg( unsigned int id, int x, int y )
{
    std::cout << "TouchMessageListener: uwmCustomPointerdown_ message received; " 
              << "(x, y) = (" << x << ", " << y << "); "
              << "id = " << id << "\n";
}

void TouchMessageListener::printPointerDownScaledMsg( unsigned int id, float x, float y )
{
    std::cout << "\nTouchMessageListener: uwmCustomPointerdown_ message received; " 
              << "(x, y) = (" << x << ", " << y << "); "
              << "id = " << id << "\n";
}

void TouchMessageListener::printPointerUpdateMsg( unsigned int id, int x, int y )
{
    std::cout << "TouchMessageListener: uwmCustomPointerUpdate_ message received; " 
              << "(x, y) = (" << x << ", " << y << "); "
              << "id = " << id << "\n";
}

void TouchMessageListener::printPointerUpdateScaledMsg( unsigned int id, float x, float y )
{
    std::cout << "TouchMessageListener: uwmCustomPointerUpdate_ message received; " 
              << "(x, y) = (" << x << ", " << y << "); "
              << "id = " << id << "\n\n";
}

void TouchMessageListener::printPointerUpMsg( unsigned int id )
{
    std::cout << "TouchMessageListener: uwmCustomPointerUp_ message received; "
              << "id = " << id << "\n";
}

void TouchMessageListener::printTuioCursor( TUIO::TuioCursor * tuioCursor )
{
    if( tuioCursor != NULL ) {
        std::cout << "sessionId = " << tuioCursor->getSessionID() << "\n"
                  << "x = " <<  tuioCursor->getX() << "\n"
                  << "y = " <<  tuioCursor->getY() << "\n"
                  << "xSpeed = " <<  tuioCursor->getXSpeed() << "\n"
                  << "ySpeed = " <<   tuioCursor->getYSpeed() << "\n"
                  << "motionAccel = " <<   tuioCursor->getMotionAccel() << "\n";
    }
    else {
        std::cout << "tuioCursor == NULL\n";
    }
}
