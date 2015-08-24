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
#ifndef INCLUDED_TUIOCURSORMANAGER_H
#define INCLUDED_TUIOCURSORMANAGER_H

#include "TuioCursorDispatcher.h"

#include <iostream>
#include <list>
#include <algorithm>

#define CUR_MESSAGE_SIZE 88  // setMessage + fseqMessage size

namespace TUIO {
    /**
     * <p>The TuioCursorManager class is the central TUIO session management component.</p> 
     * <p>During runtime the each frame is marked with the initFrame and commitFrame methods, 
     * while the currently present TuioCursor are managed by the server with 
     * ADD, UPDATE and REMOVE methods in analogy to the TuioClient's TuioListener interface.</p> 
     * <p><code>
     * TuioCursorManager *manager = new TuioCursorManager();<br/>
     * ...<br/>
     * server->initFrame(TuioTime::getSessionTime());<br/>
     * server->updateTuioCursor(tcur, xpos,ypos);<br/>
     * server->commitFrame();<br/>
     * ...<br/>
     * server->initFrame(TuioTime::getSessionTime());<br/>
     * server->removeTuioCursor(tcur);<br/>
     * server->commitFrame();<br/>
     * </code></p>
     *
     * @author Martin Kaltenbrunner
     * @version 1.5
     */ 
    class LIBDECL TuioCursorManager : public TuioCursorDispatcher { 
    
    public:

        /**
         * The default constructor creates a TuioCursorManager
         */
        TuioCursorManager();

        /**
         * The destructor is doing nothing in particular. 
         */
        virtual ~TuioCursorManager();
        
        /**
         * Creates a new TuioCursor based on the given arguments.
         * The new TuioCursor is added to the TuioServer's internal list of active TuioCursors 
         * and a reference is returned to the caller.
         *
         * @param	xp	the X coordinate to assign
         * @param	yp	the Y coordinate to assign
         * @return	reference to the created TuioCursor
         */
        TuioCursor * addTuioCursor(float xp, float yp);
        TuioCursor * addTuioCursor( int uniqueId, float xp, float yp );

        /**
         * Updates the referenced TuioCursor based on the given arguments.
         *
         * @param	tcur	the TuioCursor to update
         * @param	xp	the X coordinate to assign
         * @param	yp	the Y coordinate to assign
         */
        void updateTuioCursor(TuioCursor *tcur, float xp, float yp);
        TuioCursor * updateTuioCursor( int uniqueId, float xp, float yp );

        /**
         * Removes the referenced TuioCursor from the TuioServer's internal list of TuioCursors
         * and deletes the referenced TuioCursor afterwards
         *
         * @param	tcur	the TuioCursor to remove
         */
        void removeTuioCursor(TuioCursor *tcur);
        void removeTuioCursor( int uniqueId );
        
        /**
         * Initializes a new frame with the given TuioTime
         *
         * @param	ttime	the frame time
         */
        void initFrame(TuioTime ttime);
        
        /**
         * Commits the current frame.
         * Generates and sends TUIO messages of all currently active and updated TuioCursors.
         */
        void commitFrame();

        /**
         * Returns the next available Session ID for external use.
         * @return	the next available Session ID for external use
         */
        long getSessionID();

        /**
         * Returns the current frame ID for external use.
         * @return	the current frame ID for external use
         */
        long getFrameID();
        
        /**
         * Returns the current frame ID for external use.
         * @return	the current frame ID for external use
         */
        TuioTime getFrameTime();

        /**
         * Returns a List of all currently inactive TuioCursors
         *
         * @return  a List of all currently inactive TuioCursors
         */
        std::list<TuioCursor*> getUntouchedCursors();

        /**
         * Calculates speed and acceleration values for all currently inactive TuioCursors
         */
        void stopUntouchedMovingCursors();

        /**
         * Removes all currently inactive TuioCursors from the TuioServer's internal list of TuioCursors
         */
        void removeUntouchedStoppedCursors();
        
        /**
         * Returns the TuioCursor closest to the provided coordinates
         * or NULL if there isn't any active TuioCursor
         *
         * @return  the closest TuioCursor corresponding to the provided coordinates or NULL
         */
        TuioCursor* getClosestTuioCursor(float xp, float yp);
        
        /**
         * The TuioServer prints verbose TUIO event messages to the console if set to true.
         * @param	verbose	print verbose messages if set to true
         */
        void setVerbose(bool verbose) { verbose_ = verbose; }

        void setInversion(bool ix, bool iy, bool ia) { 
            invert_x_ = ix; 
            invert_y_ = iy; 
            invert_a_ = ia; 
        };

        void setInvertXpos(bool ix) { invert_x_ = ix; };
        void setInvertYpos(bool iy) { invert_y_ = iy; };
        void setInvertAngle(bool ia) { invert_a_ = ia; };
        bool getInvertXpos() { return invert_x_; };
        bool getInvertYpos() { return invert_y_; };
        bool getInvertAngle() { return invert_a_; };
        void resetTuioCursors();	
        
    protected:
        std::list<TuioCursor*> freeCursorList_;
        std::list<TuioCursor*> freeCursorBuffer_;

        TuioTime currentFrameTime_;
        long currentFrame_;
        int maxCursorID_;
        long sessionID_;

        bool updateCursor_;
        bool verbose_;

        bool invert_x_;
        bool invert_y_;
        bool invert_a_;
    };
}
#endif /* INCLUDED_TuioCursorManager_H */
