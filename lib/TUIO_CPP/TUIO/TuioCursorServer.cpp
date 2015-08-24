/*
    TUIO Cursor Server Component - part of the reacTIVision project
    http://reactivision.sourceforge.net/

    Copyright (C) 2005-2009 Martin Kaltenbrunner <martin@tuio.org>

    Modified by J.R. Weber <joe.weber77@gmail.com> on 5/31/2015.
    Copied the TuioSever from the Touch2Tuio project and simplified to 
    a class sending just /tuio/2Dcur messages after receiving info from 
    Windows 8 touch events (WM_POINTERDOWN, WM_POINTERUPDATE, and 
    WM_POINTERUP).

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
#include "TuioCursorServer.h"
#include "UdpSender.h"
#include "FlashXmlTcpServer.h"
#include <sstream>

using namespace TUIO;
using namespace osc;

TuioCursorServer::TuioCursorServer( const char * host /*= "127.0.0.1"*/, 
                                    int udpPort1 /*= 3333*/, 
                                    int udpPort2 /*= 3334*/, 
                                    int flashXmlTcpPort /*= 3000*/ ) :
  firstUdpSender_( new UdpSender( host, udpPort1 ) ),
  secondUdpSender_( new UdpSender( host, udpPort2 ) ),
  flashXmlTcpSender_( new FlashXmlTcpServer() ),
  useFirstUdpSender_( true ),
  useSecondUdpSender_( true ),
  useFlashXmlTcpSender_( true ),
  flashXmlTcpPortStr_(),
  oscUdpBuffer_( nullptr ),
  oscUdpPacket_( nullptr ),
  updateInterval_( 1 ),
  fullUpdate_( false ),
  periodicUpdate_( false ),
  cursorUpdateTime_( TuioTime( currentFrameTime_ ) ),
  sourceName_( nullptr )
{
    initialize();

    flashXmlTcpPortStr_ = int2Str( flashXmlTcpPort );
    bool ok = flashXmlTcpSender_->setup( flashXmlTcpPort );

    if( ok ) {
        //std::cout << "\nflashXmlTcpSender_->setup(port) returned true.\n\n";
    }
    else {
         std::cout << "\nflashXmlTcpSender_->setup(port) returned false.\n\n";
    }
}

TuioCursorServer::~TuioCursorServer()
{
    initFrame( TuioTime::getSessionTime() );
    stopUntouchedMovingCursors();

    initFrame( TuioTime::getSessionTime() );
    sendEmptyUdpCursorBundle();
    sendEmptyFlashXmlTcpCursorBundle();

    if( sourceName_ ) delete [] sourceName_;
    delete oscUdpPacket_;
    delete [] oscUdpBuffer_;
    delete secondUdpSender_;
    delete firstUdpSender_;
    delete flashXmlTcpSender_;
}

bool TuioCursorServer::isFirstUdpSenderRunning()
{
    return firstUdpSender_->isConnected ();
}

bool TuioCursorServer::isSecondUdpSenderRunning()
{
    return secondUdpSender_->isConnected ();
}

bool TuioCursorServer::isFlashXmlTcpSenderRunning()
{
    return flashXmlTcpSender_->isConnected();
}

void TuioCursorServer::initialize() 
{
    int udpBufferSize = firstUdpSender_->getBufferSize();
    oscUdpBuffer_ = new char[udpBufferSize];
    oscUdpPacket_ = new osc::OutboundPacketStream( oscUdpBuffer_, udpBufferSize );

    initFrame( TuioTime::getSessionTime() );
    cursorUpdateTime_ = TuioTime( currentFrameTime_ );
    sendEmptyUdpCursorBundle();
    
    invert_x_ = false;
    invert_y_ = false;
    invert_a_ = false;
}

void TuioCursorServer::sendEmptyUdpCursorBundle()
{
    oscUdpPacket_->Clear();	
    (*oscUdpPacket_) << osc::BeginBundleImmediate;

    if( sourceName_ ) {
        (*oscUdpPacket_) << osc::BeginMessage( "/tuio/2Dcur") 
                         << "source" << sourceName_ << osc::EndMessage;
    }
    (*oscUdpPacket_) << osc::BeginMessage( "/tuio/2Dcur") << "alive" << osc::EndMessage;	
    (*oscUdpPacket_) << osc::BeginMessage( "/tuio/2Dcur") << "fseq" << -1 << osc::EndMessage;
    (*oscUdpPacket_) << osc::EndBundle;
    deliverOscUdpPacket( oscUdpPacket_ );
}

void TuioCursorServer::deliverOscUdpPacket( osc::OutboundPacketStream  * packet )
{
    if( useFirstUdpSender_ ) {  
        firstUdpSender_->sendOscPacket( packet ); 
    }
    if( useSecondUdpSender_ ) { 
        secondUdpSender_->sendOscPacket( packet );
    }
}

void TuioCursorServer::sendEmptyFlashXmlTcpCursorBundle()
{
    std::stringstream message;
    message << "<OSCPACKET ADDRESS=\"127.0.0.1\" PORT=\"" << flashXmlTcpPortStr_
            << "\" TIME=\"" << (currentFrameTime_.getTotalMilliseconds() / 1000.0f) << "\">"

               "<MESSAGE NAME=\"/tuio/2Dcur\">"
               "<ARGUMENT TYPE=\"s\" VALUE=\"alive\"/>"
               "</MESSAGE>"

               "<MESSAGE NAME=\"/tuio/2Dcur\">"
               "<ARGUMENT TYPE=\"s\" VALUE=\"fseq\"/>"
               "<ARGUMENT TYPE=\"i\" VALUE=\"" << currentFrame_  << "\"/>" 
               "</MESSAGE>"

               "</OSCPACKET>";

    std::string msg = message.str();
    bool ok = flashXmlTcpSender_->sendtoAll( msg );

    //if( ok ) {  std::cout << "\nok = true\n"; }
    //else     {  std::cout << "\nok = false\n"; }
    //std::cout << "\n\n" << msg.c_str() << "\n\n";
}

void TuioCursorServer::commitFrame() 
{
    // Update any listeners by calling on the superclass.
    TuioCursorManager::commitFrame();

    if( updateCursor_ ) {
        if( useFirstUdpSender_ || useSecondUdpSender_ ) {
            processTuioUdpMessages();
        }
        if( useFlashXmlTcpSender_ ) {
            processFlashXmlTcpMessages();
        }
    }
    else if( periodicUpdate_ ) {
        TuioTime timeCheck = currentFrameTime_ - cursorUpdateTime_;

        if( timeCheck.getSeconds() >= updateInterval_ ) {
            cursorUpdateTime_ = TuioTime( currentFrameTime_ );
            startUdpCursorBundle();

            if( fullUpdate_ ) {
                for( auto tuioCursor = cursorList_.begin(); tuioCursor != cursorList_.end(); ++tuioCursor ) {
                    // start a new packet if we exceed the packet capacity
                    if( (oscUdpPacket_->Capacity() - oscUdpPacket_->Size()) < CUR_MESSAGE_SIZE ) {
                        sendUdpCursorBundle( currentFrame_ );
                        startUdpCursorBundle();
                    }
                    addUdpCursorMessage( *tuioCursor );
                }
            }
            sendUdpCursorBundle( currentFrame_ );
        }
    }
    updateCursor_ = false;
}

void TuioCursorServer::processTuioUdpMessages()
{
    startUdpCursorBundle();

    for( auto tuioCursor = cursorList_.begin(); tuioCursor != cursorList_.end(); ++tuioCursor ) {
        // Start a new packet if we exceed the packet capacity.
        if( (oscUdpPacket_->Capacity() - oscUdpPacket_->Size()) < CUR_MESSAGE_SIZE ) {
            sendUdpCursorBundle( currentFrame_ );
            startUdpCursorBundle();
        }
        TuioCursor * tcur = (*tuioCursor);

        if( fullUpdate_ || (tcur->getTuioTime() == currentFrameTime_) ) {
            addUdpCursorMessage( tcur );
        }
    }
    cursorUpdateTime_ = TuioTime( currentFrameTime_ );
    sendUdpCursorBundle( currentFrame_ );
}

void TuioCursorServer::startUdpCursorBundle()
{
    oscUdpPacket_->Clear();
    (*oscUdpPacket_) << osc::BeginBundleImmediate;

    if( sourceName_ ) {
        (*oscUdpPacket_) << osc::BeginMessage( "/tuio/2Dcur" ) 
                         << "source" << sourceName_ 
                         << osc::EndMessage;
    }
    (*oscUdpPacket_) << osc::BeginMessage( "/tuio/2Dcur" ) << "alive";

    for( auto tuioCursor = cursorList_.begin(); tuioCursor != cursorList_.end(); ++tuioCursor ) {
        (*oscUdpPacket_) << (int32)((*tuioCursor)->getSessionID());
    }
    (*oscUdpPacket_) << osc::EndMessage;
}

void TuioCursorServer::addUdpCursorMessage( TuioCursor * tcur )
{
    float xpos = tcur->getX();
    float xvel = tcur->getXSpeed();

    if( invert_x_ ) {
        xpos = 1 - xpos;
        xvel = -1 * xvel;
    }
    float ypos = tcur->getY();
    float yvel = tcur->getYSpeed();

    if( invert_y_ ) {
        ypos = 1 - ypos;
        yvel = -1 * yvel;
    }
    (*oscUdpPacket_) << osc::BeginMessage( "/tuio/2Dcur" ) << "set";
    (*oscUdpPacket_) << (int32)(tcur->getSessionID()) << xpos << ypos;
    (*oscUdpPacket_) << xvel << yvel << tcur->getMotionAccel();
    (*oscUdpPacket_) << osc::EndMessage;
}

void TuioCursorServer::sendUdpCursorBundle(long fseq) 
{
    (*oscUdpPacket_) << osc::BeginMessage( "/tuio/2Dcur") << "fseq" << (int32)fseq << osc::EndMessage;
    (*oscUdpPacket_) << osc::EndBundle;
    deliverOscUdpPacket( oscUdpPacket_ );
}

void TuioCursorServer::processFlashXmlTcpMessages()
{
    std::string setBlobsMsg;
    std::string aliveBlobsMsg;

    for( auto tuioCursor = cursorList_.begin(); tuioCursor != cursorList_.end(); ++tuioCursor ) {
        TuioCursor * tcur = (*tuioCursor);

        if( fullUpdate_ || (tcur->getTuioTime() == currentFrameTime_) ) {
            addFlashXml2DcurProfile( setBlobsMsg, tcur );
        }
        aliveBlobsMsg += "<ARGUMENT TYPE=\"i\" VALUE=\"" + int2Str( tcur->getSessionID() ) + "\"/>";
    }

    std::stringstream message;
    message << "<OSCPACKET ADDRESS=\"127.0.0.1\" PORT=\"" << flashXmlTcpPortStr_
            << "\" TIME=\"" << (currentFrameTime_.getTotalMilliseconds() / 1000.0f) << "\">"

            << setBlobsMsg 

            << "<MESSAGE NAME=\"/tuio/2Dcur\">"
               "<ARGUMENT TYPE=\"s\" VALUE=\"alive\"/>"
            << aliveBlobsMsg 
            << "</MESSAGE>"

               "<MESSAGE NAME=\"/tuio/2Dcur\">"
               "<ARGUMENT TYPE=\"s\" VALUE=\"fseq\"/>"
               "<ARGUMENT TYPE=\"i\" VALUE=\"" << currentFrame_ << "\"/>"
               "</MESSAGE>" 

               "</OSCPACKET>";

    cursorUpdateTime_ = TuioTime( currentFrameTime_ );

    std::string msg = message.str();
    bool ok = flashXmlTcpSender_->sendtoAll( msg );

    //if( ok ) {  std::cout << "\nok = true\n"; }
    //else     {  std::cout << "\nok = false\n"; }
    //std::cout << "\n\n" << msg.c_str() ;
}

void TuioCursorServer::addFlashXml2DcurProfile( std::string & blobMessage, TuioCursor * tcur )
{
    int id = tcur->getSessionID();
    float xpos = tcur->getX(),
          xvel = tcur->getXSpeed(),
          ypos = tcur->getY(),
          yvel = tcur->getYSpeed(),
          maccel = tcur->getMotionAccel();

    if( invert_x_ ) {
        xpos = 1 - xpos;
        xvel = -1 * xvel;
    }
    if( invert_y_ ) {
        ypos = 1 - ypos;
        yvel = -1 * yvel;
    }

    std::stringstream ss;
    ss << "<MESSAGE NAME=\"/tuio/2Dcur\">"
          "<ARGUMENT TYPE=\"s\" VALUE=\"set\"/>"
          "<ARGUMENT TYPE=\"i\" VALUE=\"" << id     << "\"/>"  // id (session ID)
          "<ARGUMENT TYPE=\"f\" VALUE=\"" << xpos   << "\"/>"  // x  (position)
          "<ARGUMENT TYPE=\"f\" VALUE=\"" << ypos   << "\"/>"  // y  (position)
          "<ARGUMENT TYPE=\"f\" VALUE=\"" << xvel   << "\"/>"  // dX (velocity vector)
          "<ARGUMENT TYPE=\"f\" VALUE=\"" << yvel   << "\"/>"  // dY (velocity vector)
          "<ARGUMENT TYPE=\"f\" VALUE=\"" << maccel << "\"/>"  // m  (motion acceleration)
          "</MESSAGE>";

    blobMessage += ss.str();
}

void TuioCursorServer::setSourceName( const char * src ) 
{
    if( !sourceName_ ) sourceName_ = new char[256];

    if( firstUdpSender_->isLocal() ) {
        sprintf( sourceName_, "%s", src );
    } 
    else { 
        char hostname[64];
        char * source_addr = NULL;
        struct hostent * hp = NULL;
        struct in_addr * addr = NULL;
        
        gethostname(hostname, 64);
        hp = gethostbyname(hostname);
        
        if( hp == NULL ) {
            sprintf( hostname, "%s.local", hostname );
            hp = gethostbyname( hostname );
        }
        
        if( hp != NULL ) {
            for( int i = 0; hp->h_addr_list[i] != 0; ++i ) {
                addr = (struct in_addr *)(hp->h_addr_list[i]);
                //std::cout << inet_ntoa(*addr) << std::endl;
                source_addr = inet_ntoa( *addr );
            }
        }
        else {
            //generate a random internet address
            srand( (unsigned int)time( NULL ) );
            int32 r = rand();
            addr = (struct in_addr*)&r;
            source_addr = inet_ntoa( *addr );
        }
        sprintf( sourceName_, "%s@%s", src, source_addr );
    } 
    //std::cout << "source: " << sourceName_ << std::endl;
}

std::string TuioCursorServer::int2Str( int n )
{
    std::stringstream out;
    out << n;
    return out.str();
}

/*******************************************************************************
The hash map-based functions below is a work in-progress and not ready for
use.  If will crash the program receiving the /tuio/2Dcur message.
*******************************************************************************/
void TuioCursorServer::commitFrame( int uniqueId )
{
    TuioCursorManager::commitFrame();

    if( updateCursor_ ) {
        // TODO: Need version that cranks through cursor map, not cursor list.
        startUdpCursorBundle();
        TuioCursor * tcur = cursorMap_[uniqueId];

        if( tcur != NULL && (tcur->getTuioTime() == currentFrameTime_) ) {
            addUdpCursorMessage( tcur );
            cursorUpdateTime_ = TuioTime( currentFrameTime_ );
        }
        sendUdpCursorBundle( currentFrame_ );
        updateCursor_ = false;
    }
}
