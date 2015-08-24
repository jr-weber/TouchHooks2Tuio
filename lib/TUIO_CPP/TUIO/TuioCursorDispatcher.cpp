/*
 TUIO C++ Library - part of the reacTIVision project
 http://reactivision.sourceforge.net/
 
 Copyright (C) 2005-2009 Martin Kaltenbrunner <martin@tuio.org>
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "TuioCursorDispatcher.h"

#include <iostream>
#include <list>
#include <algorithm>
#include <cstring>

using namespace TUIO;

TuioCursorDispatcher::TuioCursorDispatcher() 
{
#ifndef WIN32	
    pthread_mutex_init(&cursorMutex_,NULL);
    pthread_mutex_init(&cursorMapMutex_,NULL);
#else
    cursorMutex_ = CreateMutex(NULL,FALSE,"cursorMutex_");
    cursorMapMutex_ = CreateMutex(NULL,FALSE,"cursorMapMutex_");
#endif	
}

TuioCursorDispatcher::~TuioCursorDispatcher()
{
#ifndef WIN32	
    pthread_mutex_destroy(&cursorMutex_);
    pthread_mutex_destroy(&cursorMapMutex_);
#else
    CloseHandle(cursorMutex_);
    CloseHandle(cursorMapMutex_);
#endif		
}

void TuioCursorDispatcher::lockCursorList() 
{
#ifndef WIN32	
    pthread_mutex_lock(&cursorMutex_);
#else
    WaitForSingleObject(cursorMutex_, INFINITE);
#endif		
}

void TuioCursorDispatcher::lockCursorMap()
{
#ifndef WIN32	
    pthread_mutex_lock(&cursorMapMutex_);
#else
    WaitForSingleObject(cursorMapMutex_, INFINITE);
#endif	
}

void TuioCursorDispatcher::unlockCursorList() 
{
#ifndef WIN32	
    pthread_mutex_unlock(&cursorMutex_);
#else
    ReleaseMutex(cursorMutex_);
#endif		
}

void TuioCursorDispatcher::unlockCursorMap()
{
#ifndef WIN32	
    pthread_mutex_unlock(&cursorMapMutex_);
#else
    ReleaseMutex(cursorMapMutex_);
#endif	
}

void TuioCursorDispatcher::addTuioListener(TuioListener *listener) 
{
    listenerList_.push_back(listener);
}

void TuioCursorDispatcher::removeTuioListener(TuioListener *listener) 
{
    std::list<TuioListener*>::iterator result = find(listenerList_.begin(),listenerList_.end(),listener);
    if (result!=listenerList_.end()) listenerList_.remove(listener);
}

void TuioCursorDispatcher::removeAllTuioListeners()
{	
    listenerList_.clear();
}

TuioCursor * TuioCursorDispatcher::getTuioCursor( long s_id ) 
{
    lockCursorList();

    for (std::list<TuioCursor*>::iterator iter=cursorList_.begin(); iter != cursorList_.end(); iter++) {
        if((*iter)->getSessionID()==s_id) {
            unlockCursorList();
            return (*iter);
        }
    }	
    unlockCursorList();
    return NULL;
}

TuioCursor * TuioCursorDispatcher::getTuioCursorFromMap( long s_id )
{
    lockCursorMap();
    TuioCursor * tcur = cursorMap_[s_id];
    unlockCursorMap();
    return tcur;
}

std::list<TuioCursor *> TuioCursorDispatcher::getTuioCursors() 
{
    lockCursorList();
    std::list<TuioCursor*> listBuffer = cursorList_;
    unlockCursorList();
    return listBuffer;
}

std::map<long, TuioCursor *> TuioCursorDispatcher::getTuioCursorsFromMap()
{
    lockCursorMap();
    std::map<long, TuioCursor *> mapBuffer = cursorMap_;
    unlockCursorMap();
    return mapBuffer;
}

std::list<TuioCursor> TuioCursorDispatcher::copyTuioCursors() 
{
    lockCursorList();
    std::list<TuioCursor> listBuffer;
    for (std::list<TuioCursor*>::iterator iter=cursorList_.begin(); iter != cursorList_.end(); iter++) {
        TuioCursor *tcur = (*iter);
        listBuffer.push_back(*tcur);
    }
    unlockCursorList();

    return listBuffer;
}
