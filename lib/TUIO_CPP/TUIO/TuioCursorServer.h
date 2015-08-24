/*
 TUIO Cursor Server Component - part of the reacTIVision project
 http://reactivision.sourceforge.net/
 
 Copyright (C) 2005-2009 Martin Kaltenbrunner <martin@tuio.org>

 Modified by J.R. Weber <joe.weber77@gmail.com> on 5/31/2015.
 Copied the TuioSever from the Touch2Tuio project and simplified to 
 a class sending just /tuio/2Dcur messages after receiving info from 
 Windows touch messages.
 
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

#ifndef INCLUDED_TUIOCURSORSERVER_H
#define INCLUDED_TUIOCURSORSERVER_H

#include "TuioCursorManager.h"
#include "UdpSender.h"
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#ifndef WIN32
#include <netdb.h>
#include <arpa/inet.h>
#endif

class FlashXmlTcpServer;

namespace TUIO 
{
    /**
     * <p>The TuioCursorServer class is a simplfied TUIO protocol encoder 
     * intended for use with the TouchHooks2Tuio program, which intercepts
     * Windows 8 touch events (WM_POINTERDOWN, WM_POINTERUPDATE, WM_POINTERUP)
     * or  Windows 7 touch events (WM_TOUCH) and resends them as /tuio/2Dcur 
     * messages.</p>
     * 
     * <p>In order to encode and send TUIO messages an instance of TuioCursorServer
     * needs to be created. The TuioCursorServer instance then generates TUIO messages
     * which are deliverered by the provided OSCSender. The shown UDPSender send 
     * OSC to UDP port 3333 on localhost or to the configured host and port.</p> 
     *
     * <p>During runtime the each frame is marked with the initFrame and 
     * commitFrame methods, while the currently present TuioObjects are managed 
     * by the server with ADD, UPDATE and REMOVE methods in analogy to the 
     * TuioClient's TuioListener interface.</p>
     *
     *<p>See the SimpleSimulator example project for further hints on 
     * how to use a TuioServer class and its various methods.
     * <p><code>
     * TuioCursorServer *server = new TuioCursorServer();<br/>
     * server->setSourceName("MyTuioSource"); // optional for TUIO 1.1<br/>	 
     * ...<br/>
     * server->initFrame(TuioTime::getSessionTime());<br/>
     * TuioCursor *tcur = server->addTuioCursor(xpos,ypos);<br/>
     * server->commitFrame();<br/>
     * ...<br/>
     * server->initFrame(TuioTime::getSessionTime());<br/>
     * server->updateTuioCursor(tcur,xpos,ypos);<br/>
     * server->commitFrame();<br/>
     * ...<br/>
     * server->initFrame(TuioTime::getSessionTime());<br/>
     * server->removeTuioCursor(tcur);<br/>
     * server->commitFrame();<br/>
     * </code></p>
     *
     * @author Martin Kaltenbrunner
     * @version 1.5
     *
     * The original TuioServer class by Marin Kaltenbrunner was duplicated and 
     * modified for use with the TouchHooks2Tuio program, which is based on the 
     * Touch2Tuio project on GitHub.
     *     -J.R.Weber <joe.weber77@gmail.com>
     */ 
    class LIBDECL TuioCursorServer : public TuioCursorManager 
    { 
    public:
        /**
         * This constructor creates a TuioServer that uses internal UdpSender
         * and TcpSender objects for sending /tuio/2Dcur messages on ports 3333
         * and 3334 for TUIO UDP messages and port 3000 for Flash XML TCP 
         * messages.
         *
         * @param  host      the UDP and TCP host name (usually 127.0.0.1).
         * @param  udpPort1  the first port for sending TUIO UDP messages.
         * @param  udpPort2  the second port for sending TUIO UDP messages.
         * @param  flashXmlTcpPort  the port for TUIO Flash XML TCP messages.
         */
        TuioCursorServer( const char * host = "127.0.0.1", 
                          int udpPort1 = 3333, 
                          int udpPort2 = 3334, 
                          int flashXmlTcpPort = 3000 );

        /**
         * The destructor is doing nothing in particular.
         */
        virtual ~TuioCursorServer();
        
        /**
         * Enables the full update of all currently active and inactive TuioObjects, TuioCursors and TuioBlobs 
         *
         */
        void enableFullUpdate()  
        {
            fullUpdate_ = true;
        }
        
        /**
         * Disables the full update of all currently active and inactive TuioObjects, TuioCursors and TuioBlobs 
         */
        void disableFullUpdate() 
        {
            fullUpdate_ = false;
        }

        /**
         * Returns true if the full update of all currently active TuioObjects, TuioCursors and TuioBlobs is enabled.
         * @return	true if the full update of all currently active TuioObjects, TuioCursors and TuioBlobs is enabled
         */
        bool fullUpdateEnabled() 
        {
            return fullUpdate_;
        }

        /**
         * Disables the periodic full update of all currently active TuioObjects TuioObjects, TuioCursors and TuioBlobs
         *
         * @param	interval	update interval in seconds, defaults to one second
         */
        void enablePeriodicMessages( int interval = 1 ) 
        {
            periodicUpdate_ =  true;
            updateInterval_ = interval;
        }
        
        /**
         * Disables the periodic full update of all currently active and inactive TuioObjects, TuioCursors and TuioBlobs
         */
        void disablePeriodicMessages()
        {
            periodicUpdate_ = false;
        }
        
        /**
         * Returns true if the periodic update of all currently active TuioObjects, TuioCursors and TuioBlobs is enabled.
         * @return	true if the periodic update of all currently active TuioObjects, TuioCursors and TuioBlobs is enabled
         */
        bool periodicMessagesEnabled() 
        {
            return periodicUpdate_;
        }
        
        /**
         * Returns the periodic update interval in seconds.
         * @return	the periodic update interval in seconds
         */
        int getUpdateInterval()
        {
            return updateInterval_;
        }
        
        /**
         * Commits the current frame.
         * Generates and sends TUIO messages of all currently active and updated TuioObjects, TuioCursors and TuioBlobs.
         */
        void commitFrame();
        void commitFrame( int uniqueId ); // experimental
        
        /**
         * Defines the name of this TUIO source, which is transmitted within the /tuio/[profile] source message.
         *
         * @param	src	the desired name of this TUIO source
         */
        void setSourceName(const char *src);

        void useFirstUdpSender( bool b ) { useFirstUdpSender_ = b; }
        bool useFirstUdpSender() { return useFirstUdpSender_; }

        void useSecondUdpSender( bool b ) { useSecondUdpSender_ = b; }
        bool useSecondUdpSender() { return useSecondUdpSender_; }

        void useFlashXmlTcpSender( bool b ) { useFlashXmlTcpSender_ = b; }
        bool useFlashXmlTcpSender() { return useFlashXmlTcpSender_; }

        bool isFirstUdpSenderRunning();
        bool isSecondUdpSenderRunning();
        bool isFlashXmlTcpSenderRunning();
        
    private:
        void initialize();
        std::string int2Str( int n );

        void sendEmptyUdpCursorBundle();
        void deliverOscUdpPacket( osc::OutboundPacketStream  * packet );
        void sendEmptyFlashXmlTcpCursorBundle();

        void processTuioUdpMessages();
        void startUdpCursorBundle();
        void addUdpCursorMessage( TuioCursor * tcur );
        void sendUdpCursorBundle( long fseq );

        void processFlashXmlTcpMessages();
        void addFlashXml2DcurProfile( std::string & blobMessage, TuioCursor * tcur );

        UdpSender * firstUdpSender_,
                  * secondUdpSender_;
        FlashXmlTcpServer * flashXmlTcpSender_;
        bool useFirstUdpSender_,
             useSecondUdpSender_,
             useFlashXmlTcpSender_;
        std::string flashXmlTcpPortStr_;

        char * oscUdpBuffer_; 
        osc::OutboundPacketStream  * oscUdpPacket_;

        int updateInterval_;
        bool fullUpdate_,
             periodicUpdate_;
        TuioTime cursorUpdateTime_;	
        char * sourceName_;
    };
}
#endif /* INCLUDED_TuioCursorServer_H */
