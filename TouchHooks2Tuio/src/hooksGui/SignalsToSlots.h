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
#ifndef HOOKSGUI_SIGNALSTOSLOTS_H
#define HOOKSGUI_SIGNALSTOSLOTS_H

#include <QObject>

namespace hooksGui { class TouchHooksMainWindow; }
namespace hooksCore { class TouchHooks2Tuio; }
namespace hooksCore { class TouchMessageListener; }
namespace hooksServer { class LocalServer; }
namespace Ui { class TouchHooksMainWindowUi; }

namespace hooksGui
{
    class SignalsToSlots : public QObject
    {
        Q_OBJECT

    public:
        SignalsToSlots( hooksGui::TouchHooksMainWindow * mainWindow, 
                        hooksCore::TouchHooks2Tuio * touchHooks2Tuio,
                        hooksCore::TouchMessageListener * touchMessageListener,
                        hooksServer::LocalServer * localServer,
                        Ui::TouchHooksMainWindowUi * ui );
        virtual ~SignalsToSlots();

        void makeConnections();

    private:
        void connectFileMenu();
        void connectHooksMenu();
        void connectNetworkMenu();
        void connectLocalServer();

        hooksGui::TouchHooksMainWindow * mainWindow_;
        hooksCore::TouchHooks2Tuio * touchHooks2Tuio_;
        hooksCore::TouchMessageListener * touchMessageListener_;
        hooksServer::LocalServer * localServer_;
        Ui::TouchHooksMainWindowUi * ui_;
    };
}

#endif
