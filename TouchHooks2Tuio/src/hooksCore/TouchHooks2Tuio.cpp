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
#include "hooksCore/TouchHooks2Tuio.h"
#include "TouchHook.h"

using hooksCore::TouchHooks2Tuio;

TouchHooks2Tuio::TouchHooks2Tuio() :
  useGlobalTouchHook_( false )
{
}

TouchHooks2Tuio::~TouchHooks2Tuio()
{
}

void TouchHooks2Tuio::initializeConsolePidForDebugging( int consolePid )
{
    SetConsoleId( (DWORD)consolePid );
}

bool TouchHooks2Tuio::initializeGlobalTouchHook()
{
    useGlobalTouchHook_ = true;
    return InstallGlobalTouchHook();
}

bool TouchHooks2Tuio::removeGlobalTouchHook()
{
    useGlobalTouchHook_ = false;
    return RemoveGlobalTouchHook();
}

bool TouchHooks2Tuio::useGlobalTouchHook()
{
    return useGlobalTouchHook_;
}

unsigned int TouchHooks2Tuio::getUwmCustomPointerDown()
{
    return UwmCustomPointerDown();
}

unsigned int TouchHooks2Tuio::getUwmCustomPointerUpdate()
{
    return UwmCustomPointerUpdate();
}

unsigned int TouchHooks2Tuio::getUwmCustomPointerUp()
{
    return UwmCustomPointerUp();
}

unsigned int TouchHooks2Tuio::getUwmCustomTouched()
{
    return UwmCustomTouch();
}

unsigned int TouchHooks2Tuio::getUwmCustomTimer()
{
    return UwmCustomTimer();
}
