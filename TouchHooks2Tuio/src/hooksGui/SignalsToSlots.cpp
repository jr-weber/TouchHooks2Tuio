/*******************************************************************************
SignalsToSlots

PURPOSE: Mostly used for connecting menu options to functions in the main 
         window.  Also used to connect the LocalServer to the main window.

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
#include "hooksGui/SignalsToSlots.h"
#include "hooksGui/TouchHooksMainWindow.h"
#include "hooksCore/TouchHooks2Tuio.h"
#include "hooksCore/TouchMessageListener.h"
#include "hooksServer/LocalServer.h"
#include "ui_TouchHooksMainWindow.h"
#include <QApplication>

using hooksGui::SignalsToSlots;

SignalsToSlots::SignalsToSlots( hooksGui::TouchHooksMainWindow * mainWindow, 
                                hooksCore::TouchHooks2Tuio * touchHooks2Tuio,
                                hooksCore::TouchMessageListener * touchMessageListener,
                                hooksServer::LocalServer * localServer,
                                Ui::TouchHooksMainWindowUi * ui ) :
  mainWindow_( mainWindow ),
  touchHooks2Tuio_( touchHooks2Tuio ),
  touchMessageListener_( touchMessageListener ),
  localServer_( localServer ),
  ui_( ui )
{
}

SignalsToSlots::~SignalsToSlots()
{
}

void SignalsToSlots::makeConnections()
{
    connectFileMenu();
    connectHooksMenu();
    connectNetworkMenu();
    connectLocalServer();
}

void SignalsToSlots::connectFileMenu()
{
    connect( ui_->action_quit,
             SIGNAL( triggered() ),
             QApplication::instance(),
             SLOT( closeAllWindows() ) );
}

void SignalsToSlots::connectHooksMenu()
{
    connect( ui_->action_useGlobalTouchHook,
             SIGNAL( toggled( bool ) ),
             mainWindow_,
             SLOT( useGlobalTouchHook( bool ) ) );
}

void SignalsToSlots::connectNetworkMenu()
{
    connect( ui_->action_tuioUdpChannelOne,
             SIGNAL( toggled( bool ) ),
             mainWindow_,
             SLOT( useTuioUdpChannelOne( bool ) ) );

    connect( ui_->action_tuioUdpChannelTwo,
             SIGNAL( toggled( bool ) ),
             mainWindow_,
             SLOT( useTuioUdpChannelTwo( bool ) ) );

    connect( ui_->action_flashXmlTcpChannel,
             SIGNAL( toggled( bool ) ),
             mainWindow_,
             SLOT( useFlashXmlTcpChannel( bool ) ) );
}

void SignalsToSlots::connectLocalServer()
{
    connect( localServer_,
             SIGNAL( clientRequestsGlobalHookRelease() ),
             mainWindow_,
             SLOT( removeGlobalTouchHook() ) );
}
