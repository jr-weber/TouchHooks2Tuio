/*
    TUIO Server Component - part of the reacTIVision project
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
#include "TuioCursorManager.h"
using namespace TUIO;

TuioCursorManager::TuioCursorManager() : 
  currentFrameTime_( TuioTime::getSessionTime() ), 
  currentFrame_( 0 ), 
  maxCursorID_( -1 ), 
  sessionID_( -1 ), 
  updateCursor_( false ), 
  verbose_( false ), 
  invert_x_( false ), 
  invert_y_( false ), 
  invert_a_( false )
{

}

TuioCursorManager::~TuioCursorManager() 
{
}

TuioCursor * TuioCursorManager::addTuioCursor( float x, float y )
{
    sessionID_++;

    int cursorID = (int)cursorList_.size();

    if( (int)(cursorList_.size()) <= maxCursorID_ ) {
        std::list<TuioCursor*>::iterator closestCursor = freeCursorList_.begin();

        for( std::list<TuioCursor*>::iterator iter = freeCursorList_.begin(); iter != freeCursorList_.end(); iter++ ) {
            if( (*iter)->getDistance( x, y ) < (*closestCursor)->getDistance( x, y ) ) closestCursor = iter;
        }

        TuioCursor *freeCursor = (*closestCursor);
        cursorID = (*closestCursor)->getCursorID();
        freeCursorList_.erase( closestCursor );
        delete freeCursor;
    }
    else maxCursorID_ = cursorID;

    TuioCursor *tcur = new TuioCursor( currentFrameTime_, sessionID_, cursorID, x, y );
    cursorList_.push_back( tcur );
    updateCursor_ = true;

    for( std::list<TuioListener*>::iterator listener = listenerList_.begin(); listener != listenerList_.end(); listener++ )
        (*listener)->addTuioCursor( tcur );

    if( verbose_ )
        std::cout << "add cur " << tcur->getCursorID() << " (" << tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY() << std::endl;

    return tcur;
}

void TuioCursorManager::updateTuioCursor( TuioCursor *tcur, float x, float y ) 
{
    if( tcur == NULL ) return;
    if( tcur->getTuioTime() == currentFrameTime_ ) return;
    tcur->update( currentFrameTime_, x, y );
    updateCursor_ = true;

    if( tcur->isMoving() ) {
        for( std::list<TuioListener*>::iterator listener = listenerList_.begin(); listener != listenerList_.end(); listener++ )
            (*listener)->updateTuioCursor( tcur );

        if( verbose_ )
            std::cout << "set cur " << tcur->getCursorID() << " (" << tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY()
            << " " << tcur->getXSpeed() << " " << tcur->getYSpeed() << " " << tcur->getMotionAccel() << " " << std::endl;
    }
}

void TuioCursorManager::removeTuioCursor( TuioCursor *tcur )
{
    if( tcur == NULL ) return;

    cursorList_.remove( tcur );
    tcur->remove( currentFrameTime_ );
    updateCursor_ = true;

    for( std::list<TuioListener*>::iterator listener = listenerList_.begin(); listener != listenerList_.end(); listener++ )
        (*listener)->removeTuioCursor( tcur );

    if( verbose_ )
        std::cout << "del cur " << tcur->getCursorID() << " (" << tcur->getSessionID() << ")" << std::endl;

    if( tcur->getCursorID() == maxCursorID_ ) {
        maxCursorID_ = -1;
        delete tcur;

        if( cursorList_.size() > 0 ) {
            std::list<TuioCursor*>::iterator clist;
            for( clist = cursorList_.begin(); clist != cursorList_.end(); clist++ ) {
                int cursorID = (*clist)->getCursorID();
                if( cursorID > maxCursorID_ ) maxCursorID_ = cursorID;
            }

            freeCursorBuffer_.clear();
            for( std::list<TuioCursor*>::iterator flist = freeCursorList_.begin(); flist != freeCursorList_.end(); flist++ ) {
                TuioCursor *freeCursor = (*flist);
                if( freeCursor->getCursorID() > maxCursorID_ ) delete freeCursor;
                else freeCursorBuffer_.push_back( freeCursor );
            }

            freeCursorList_ = freeCursorBuffer_;

        }
        else {
            for( std::list<TuioCursor*>::iterator flist = freeCursorList_.begin(); flist != freeCursorList_.end(); flist++ ) {
                TuioCursor *freeCursor = (*flist);
                delete freeCursor;
            }
            freeCursorList_.clear();
        }
    }
    else if( tcur->getCursorID() < maxCursorID_ ) {
        freeCursorList_.push_back( tcur );
    }
}

long TuioCursorManager::getSessionID()
{
    sessionID_++;
    return sessionID_;
}

long TuioCursorManager::getFrameID() 
{
    return currentFrame_;
}

TuioTime TuioCursorManager::getFrameTime() 
{
    return currentFrameTime_;
}

void TuioCursorManager::initFrame( TuioTime ttime )
{
    currentFrameTime_ = TuioTime( ttime );
    ++currentFrame_;
}

void TuioCursorManager::commitFrame()
{
    for( std::list<TuioListener*>::iterator listener = listenerList_.begin(); listener != listenerList_.end(); listener++ )
        (*listener)->refresh( currentFrameTime_ );
}

TuioCursor* TuioCursorManager::getClosestTuioCursor( float xp, float yp ) 
{
    TuioCursor *closestCursor = NULL;
    float closestDistance = 1.0f;

    for( std::list<TuioCursor*>::iterator iter = cursorList_.begin(); iter != cursorList_.end(); iter++ ) {
        float distance = (*iter)->getDistance( xp, yp );
        if( distance < closestDistance ) {
            closestCursor = (*iter);
            closestDistance = distance;
        }
    }

    return closestCursor;
}

std::list<TuioCursor*> TuioCursorManager::getUntouchedCursors()
{
    std::list<TuioCursor*> untouched;
    for( std::list<TuioCursor*>::iterator tuioCursor = cursorList_.begin(); tuioCursor != cursorList_.end(); tuioCursor++ ) {
        TuioCursor *tcur = (*tuioCursor);
        if( tcur->getTuioTime() != currentFrameTime_ ) untouched.push_back( tcur );
    }
    return untouched;
}

void TuioCursorManager::stopUntouchedMovingCursors()
{
    std::list<TuioCursor*> untouched;
    for( std::list<TuioCursor*>::iterator tuioCursor = cursorList_.begin(); tuioCursor != cursorList_.end(); tuioCursor++ ) {
        TuioCursor *tcur = (*tuioCursor);
        if( (tcur->getTuioTime() != currentFrameTime_) && (tcur->isMoving()) ) {
            tcur->stop( currentFrameTime_ );
            updateCursor_ = true;
            if( verbose_ )
                std::cout << "set cur " << tcur->getCursorID() << " (" << tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY()
                << " " << tcur->getXSpeed() << " " << tcur->getYSpeed() << " " << tcur->getMotionAccel() << " " << std::endl;
        }
    }
}

void TuioCursorManager::removeUntouchedStoppedCursors()
{
    if( cursorList_.size() == 0 ) return;
    std::list<TuioCursor*>::iterator tuioCursor = cursorList_.begin();
    while( tuioCursor != cursorList_.end() ) {
        TuioCursor *tcur = (*tuioCursor);
        if( (tcur->getTuioTime() != currentFrameTime_) && (!tcur->isMoving()) ) {
            removeTuioCursor( tcur );
            tuioCursor = cursorList_.begin();
        }
        else tuioCursor++;
    }
}

void TuioCursorManager::resetTuioCursors() 
{
    std::list<TuioCursor*>::iterator tuioCursor = cursorList_.begin();
    while( tuioCursor != cursorList_.end() ) {
        removeTuioCursor( (*tuioCursor) );
        tuioCursor = cursorList_.begin();
    }
}

/*******************************************************************************
The hash map-based functions below are a work in-progress and are not ready for
use: the updateTuioCursor( uniqueId, xp, yp ) function will crash the program
receiving the /tuio/2Dcur message.
*******************************************************************************/
TuioCursor * TuioCursorManager::addTuioCursor( int uniqueId, float xp, float yp )
{
    TuioCursor * tcur = new TuioCursor( currentFrameTime_, uniqueId, uniqueId, xp, yp );
    cursorMap_[uniqueId] = tcur;
    updateCursor_ = true;
    return tcur;
}

TuioCursor * TuioCursorManager::updateTuioCursor( int uniqueId, float xp, float yp )
{
    TuioCursor * tcur = cursorMap_[uniqueId];

    if( tcur != NULL && tcur->getTuioTime() != currentFrameTime_ ) {
        tcur->update( currentFrameTime_, xp, yp );
        updateCursor_ = true;

        if( tcur->isMoving() ) {
            for( auto listener = listenerList_.begin(); listener != listenerList_.end(); ++listener ) {
                (*listener)->updateTuioCursor( tcur );
            }
        }
    }
    return tcur;
}

void TuioCursorManager::removeTuioCursor( int uniqueId )
{
    TuioCursor * tcur = cursorMap_[uniqueId];

    if( tcur != NULL ) {
        cursorMap_.erase( uniqueId );
        tcur->remove( currentFrameTime_ );
        updateCursor_ = true;

        for( auto listener = listenerList_.begin(); listener != listenerList_.end(); ++listener ) {
            (*listener)->removeTuioCursor( tcur );
        }
        delete tcur;
    }
}
