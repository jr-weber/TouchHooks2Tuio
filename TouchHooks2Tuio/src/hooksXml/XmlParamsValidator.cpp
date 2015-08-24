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
#include "hooksXml/XmlParamsValidator.h"
#include "hooksExceptions/ValidatorException.h"

using hooksXml::XmlParamsValidator;
using hooksExceptions::ValidatorException;

XmlParamsValidator::XmlParamsValidator()
{
    initialize();
}

XmlParamsValidator::~XmlParamsValidator()
{
}

void XmlParamsValidator::initialize()
{
    xmlConfigFilename_ = "unknown";
    useGlobalHook_ = true;
    localHost_ = "127.0.0.1";
    tuioUdpChannelOnePort_ = 3333;
    tuioUdpChannelTwoPort_ = 3334;
    flashXmlChannelPort_ = 3000;
    useTuioUdpChannelOne_ = true;
    useTuioUdpChannelTwo_ = true;
    useFlashXmlChannel_ = true; 
}

void XmlParamsValidator::setXmlConfigFilename( const QString & filename )
{
    xmlConfigFilename_ = filename;
}

QString XmlParamsValidator::getXmlConfigFilename()
{
    return xmlConfigFilename_;
}

void XmlParamsValidator::useGlobalHook( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useGlobalHook_ = true;
    }
    else if( b == "false" ) {
        useGlobalHook_ = false;
    }
    else {
        throw ValidatorException( "Invalid startup setting detected.",
                                  "XmlParamsValidator::useGlobalHook()",
                                  "useGlobalHook",
                                  s,
                                  "true or false",
                                  xmlConfigFilename_ );
    }
}

void XmlParamsValidator::setLocalHost( const QString & s )
{
    QString hostname( s.trimmed() );

    if( hostname.size() < 1 ) {
        throw ValidatorException( "Invalid startup setting detected.",
                                  "XmlParamsValidator::setLocalHost()",
                                  "localHost",
                                  s,
                                  "valid IP address such as 127.0.0.1",
                                  xmlConfigFilename_ );
    }
    localHost_ = hostname;
}

void XmlParamsValidator::setTuioUdpChannelOnePort( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 1 ) {
        throw ValidatorException( "Invalid startup setting detected.",
                                  "XmlParamsValidator::setTuioUdpChannelOnePort()",
                                  "tuioUdpChannelOnePort",
                                  s,
                                  "a positive integer",
                                  xmlConfigFilename_ );
    }
    tuioUdpChannelOnePort_ = n;
}

void XmlParamsValidator::setTuioUdpChannelTwoPort( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 1 ) {
        throw ValidatorException( "Invalid startup setting detected.",
                                  "XmlParamsValidator::setTuioUdpChannelTwoPort()",
                                  "tuioUdpChannelTwoPort",
                                  s,
                                  "a positive integer",
                                  xmlConfigFilename_ );
    }
    tuioUdpChannelTwoPort_ = n;
}

void XmlParamsValidator::setFlashXmlChannelPort( const QString & s )
{
    bool ok = false;
    int n = s.toInt( &ok );

    if( !ok || n < 1 ) {
        throw ValidatorException( "Invalid startup setting detected.",
                                  "XmlParamsValidator::setFlashXmlChannelPort()",
                                  "flashXmlChannelPort",
                                  s,
                                  "a positive integer",
                                  xmlConfigFilename_ );
    }
    flashXmlChannelPort_ = n;
}

void XmlParamsValidator::useTuioUdpChannelOne( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useTuioUdpChannelOne_ = true;
    }
    else if( b == "false" ) {
        useTuioUdpChannelOne_ = false;
    }
    else {
        throw ValidatorException( "Invalid startup setting detected.",
                                  "XmlParamsValidator::useTuioUdpChannelOne()",
                                  "useTuioUdpChannelOne",
                                  s,
                                  "true or false",
                                  xmlConfigFilename_ );
    }
}

void XmlParamsValidator::useTuioUdpChannelTwo( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useTuioUdpChannelTwo_ = true;
    }
    else if( b == "false" ) {
        useTuioUdpChannelTwo_ = false;
    }
    else {
        throw ValidatorException( "Invalid startup setting detected.",
                                  "XmlParamsValidator::useTuioUdpChannelTwo()",
                                  "useTuioUdpChannelTwo",
                                  s,
                                  "true or false",
                                  xmlConfigFilename_ );
    }
}

void XmlParamsValidator::useFlashXmlChannel( const QString & s )
{
    QString b = s.trimmed().toLower();

    if( b == "true" ) {
        useFlashXmlChannel_ = true;
    }
    else if( b == "false" ) {
        useFlashXmlChannel_ = false;
    }
    else {
        throw ValidatorException( "Invalid startup setting detected.",
                                  "XmlParamsValidator::useFlashXmlChannel()",
                                  "useFlashXmlChannel",
                                  s,
                                  "true or false",
                                  xmlConfigFilename_ );
    }
}

// getters
bool XmlParamsValidator::useGlobalHook() { return useGlobalHook_; }
QString XmlParamsValidator::getLocalHost() { return localHost_; }
int XmlParamsValidator::getTuioUdpChannelOnePort() { return tuioUdpChannelOnePort_; }
int XmlParamsValidator::getTuioUdpChannelTwoPort() { return tuioUdpChannelTwoPort_; }
int XmlParamsValidator::getFlashXmlChannelPort()   { return flashXmlChannelPort_; }
bool XmlParamsValidator::useTuioUdpChannelOne() { return useTuioUdpChannelOne_; }
bool XmlParamsValidator::useTuioUdpChannelTwo() { return useTuioUdpChannelTwo_; }
bool XmlParamsValidator::useFlashXmlChannel()   { return useFlashXmlChannel_; }

// unchecked setters
void XmlParamsValidator::useGlobalHook( bool b )   { useGlobalHook_ = b; }
void XmlParamsValidator::setLocalHost(const std::string & host) { localHost_ = host.c_str(); }
void XmlParamsValidator::setTuioUdpChannelOnePort( int port ) { tuioUdpChannelOnePort_ = port; }
void XmlParamsValidator::setTuioUdpChannelTwoPort( int port ) { tuioUdpChannelTwoPort_ = port; }
void XmlParamsValidator::setFlashXmlChannelPort( int port )   { flashXmlChannelPort_ = port; }
void XmlParamsValidator::useTuioUdpChannelOne( bool b ) { useTuioUdpChannelOne_ = b; }
void XmlParamsValidator::useTuioUdpChannelTwo( bool b ) { useTuioUdpChannelTwo_ = b; }
void XmlParamsValidator::useFlashXmlChannel( bool b )   { useFlashXmlChannel_ = b; }
