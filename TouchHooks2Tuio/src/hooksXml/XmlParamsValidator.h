/*******************************************************************************
XmlParamsValidator

PURPOSE: Basic validation of values from an XML configuration file.
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
#ifndef HOOKSXML_XMLPARAMSVALIDATOR_H
#define HOOKSXML_XMLPARAMSVALIDATOR_H

#include <QString>

namespace hooksXml
{
    class XmlParamsValidator
    {
    public:
        XmlParamsValidator();
        virtual ~XmlParamsValidator();

        void initialize();
        void setXmlConfigFilename( const QString & filename );
        QString getXmlConfigFilename();

        // checked setters
        void useGlobalHook( const QString & s );
        void setLocalHost( const QString & s );
        void setTuioUdpChannelOnePort( const QString & s );
        void setTuioUdpChannelTwoPort( const QString & s );
        void setFlashXmlChannelPort( const QString & s );
        void useTuioUdpChannelOne( const QString & s );
        void useTuioUdpChannelTwo( const QString & s );
        void useFlashXmlChannel( const QString & s );

        // getters
        bool useGlobalHook();
        QString getLocalHost();
        int getTuioUdpChannelOnePort();
        int getTuioUdpChannelTwoPort();
        int getFlashXmlChannelPort();
        bool useTuioUdpChannelOne();
        bool useTuioUdpChannelTwo();
        bool useFlashXmlChannel();

        // unchecked setters
        void useGlobalHook( bool b );
        void setLocalHost( const std::string & host );
        void setTuioUdpChannelOnePort( int port );
        void setTuioUdpChannelTwoPort( int port );
        void setFlashXmlChannelPort( int port );
        void useTuioUdpChannelOne( bool b );
        void useTuioUdpChannelTwo( bool b );
        void useFlashXmlChannel( bool b );

    private:
        QString xmlConfigFilename_;
        bool useGlobalHook_;
        QString localHost_;
        int tuioUdpChannelOnePort_,
            tuioUdpChannelTwoPort_,
            flashXmlChannelPort_;
        bool useTuioUdpChannelOne_,
             useTuioUdpChannelTwo_,
             useFlashXmlChannel_;
    };
}

#endif
