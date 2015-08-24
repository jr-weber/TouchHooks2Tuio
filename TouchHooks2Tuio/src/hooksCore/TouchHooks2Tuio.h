/*******************************************************************************
TouchHooks2Tuio

PURPOSE: Operates the TouchHook DLL code for attaching and detaching a Windows
         global hook that allows the DLL to monitor all Windows 8 touch events.
         See the TouchHook class in the TouchHook project directory for the
         code that establishes the global hook.

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
#ifndef HOOKSCORE_TOUCHHOOKS2TUIO_H
#define HOOKSCORE_TOUCHHOOKS2TUIO_H

#include <QObject>
#include <string>

namespace hooksGui { class GuiTextWriter; }

namespace hooksCore
{
    class TouchHooks2Tuio : public QObject
    {
        Q_OBJECT

    public:
        TouchHooks2Tuio();
        ~TouchHooks2Tuio();

        void initializeConsolePidForDebugging( int consolePid );
        bool initializeGlobalTouchHook();
        bool removeGlobalTouchHook();
        bool useGlobalTouchHook();

        unsigned int getUwmCustomPointerDown();
        unsigned int getUwmCustomPointerUpdate();
        unsigned int getUwmCustomPointerUp();
        unsigned int getUwmCustomTouched();
        unsigned int getUwmCustomTimer();

    private:
        bool useGlobalTouchHook_;

    };
}

#endif
