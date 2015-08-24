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

#ifndef INCLUDED_TUIOCURSORDISPATCHER_H
#define INCLUDED_TUIOCURSORDISPATCHER_H

#include "TuioListener.h"
#include <map>

#ifndef WIN32
#include <pthread.h>
#else
#include <windows.h>
#endif

namespace TUIO
{	
    /**
     * <p>The TuioCursorDispatcher generates TUIO events which are broadcasted to all 
     * registered classes that implement the {@link TuioListener} interface.</p> 
     *
     * @author Martin Kaltenbrunner
     * @version 1.5
     */ 
    class LIBDECL TuioCursorDispatcher
    {
    public:
        /**
         * This constructor creates a TuioCursorDispatcher
         *
         */
        TuioCursorDispatcher();

        /**
         * The destructor is doing nothing in particular.
         */
        virtual ~TuioCursorDispatcher();

        /**
         * Adds the provided TuioListener to the list of registered TUIO event listeners
         *
         * @param  listener  the TuioListener to add
         */
        void addTuioListener( TuioListener * listener );

        /**
         * Removes the provided TuioListener from the list of registered TUIO event listeners
         *
         * @param  listener  the TuioListener to remove
         */
        void removeTuioListener( TuioListener * listener );

        /**
         * Removes all TuioListener from the list of registered TUIO event listeners
         */
        void removeAllTuioListeners();

        /**
         * Returns a List of all currently active TuioCursors
         *
         * @return  a List of all currently active TuioCursors
         */
        std::list<TuioCursor *> getTuioCursors();
        std::map<long, TuioCursor *> getTuioCursorsFromMap();

        /**
         * Returns a List with a copy of currently active TuioCursors
         *
         * @return  a List with a copy of all currently active TuioCursors
         */
        std::list<TuioCursor> copyTuioCursors();

        /**
         * Returns the TuioCursor corresponding to the provided Session ID
         * or NULL if the Session ID does not refer to an active TuioCursor
         *
         * @return  an active TuioCursor corresponding to the provided Session ID or NULL
         */
        TuioCursor * getTuioCursor( long s_id );
        TuioCursor * getTuioCursorFromMap( long s_id );

        /**
         * Locks the TuioCursor list in order to avoid updates during access
         */
        void lockCursorList();
        void lockCursorMap();

        /**
         * Releases the lock of the TuioCursor list
         */
        void unlockCursorList();
        void unlockCursorMap();

    protected:
        std::list<TuioListener *> listenerList_;
        std::list<TuioCursor *> cursorList_;
        std::map<long, TuioCursor *> cursorMap_;
        
#ifndef WIN32
        pthread_mutex_t cursorMutex_;
        pthread_mutex_t cursorMapMutex_;
#else
        HANDLE cursorMutex_;
        HANDLE cursorMapMutex_;
#endif	
                
    };
}
#endif /* INCLUDED_TUIOCURSORDISPATCHER_H */
