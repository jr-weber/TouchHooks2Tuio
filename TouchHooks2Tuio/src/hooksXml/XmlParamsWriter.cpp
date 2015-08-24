/*******************************************************************************
XmlParamsWriter

PURPOSE: Write program settings to an XML file.
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
#include "hooksXml/XmlParamsWriter.h"
#include "hooksXml/XmlParamsValidator.h"
#include "hooksExceptions/XmlWriterException.h"
#include <QFile>
#include <QTextStream>
#include <iostream>

using hooksXml::XmlParamsWriter;
using hooksExceptions::XmlWriterException;

XmlParamsWriter::XmlParamsWriter() :
  debug_( false )
{
}

XmlParamsWriter::~XmlParamsWriter()
{
}

void XmlParamsWriter::write( hooksXml::XmlParamsValidator * validator, 
                             const QString & xmlFile )
{
    QFile file( xmlFile );

    if( file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) ) {
        QTextStream stream( &file );
        stream << getSettingsAsXml( validator );
        file.close();
        debugPrintLn( xmlFile + " was written out." );
    }
    else {
        QString msg( "" );
        throw XmlWriterException( msg, "XmlParamsWriter::write()", xmlFile );
    }
}

void XmlParamsWriter::printXmlToStandardOut( hooksXml::XmlParamsValidator * validator )
{
    std::cout << getSettingsAsXml( validator ).toStdString();
}

QString XmlParamsWriter::getSettingsAsXml( hooksXml::XmlParamsValidator * validator )
{
    QString xml( "<TouchHooks2Tuio>\n\n" );
    xml.append( getHooksXml( validator ) );
    xml.append( getNetworkXml( validator ) );
    xml.append( "</TouchHooks2Tuio>\n" );
    return xml;
}

QString XmlParamsWriter::getHooksXml( hooksXml::XmlParamsValidator * validator )
{
    QString xml( "    <Hooks>\n" );
    xml.append( createXmlFromBool( "useGlobalHook", validator->useGlobalHook() ) );
    xml.append( "    </Hooks>\n\n" );
    return xml;
}

QString XmlParamsWriter::getNetworkXml( hooksXml::XmlParamsValidator * validator )
{
    QString xml( "    <Network>\n" );
    xml.append( createXmlFromString( "localHost", validator->getLocalHost() ) );
    xml.append( createXmlFromInt( "tuioUdpChannelOnePort", validator->getTuioUdpChannelOnePort() ) );
    xml.append( createXmlFromInt( "tuioUdpChannelTwoPort", validator->getTuioUdpChannelTwoPort() ) );
    xml.append( createXmlFromInt( "flashXmlChannelPort", validator->getFlashXmlChannelPort() ) );
    xml.append( createXmlFromBool( "useTuioUdpChannelOne", validator->useTuioUdpChannelOne() ) );
    xml.append( createXmlFromBool( "useTuioUdpChannelTwo", validator->useTuioUdpChannelTwo() ) );
    xml.append( createXmlFromBool( "useFlashXmlChannel", validator->useFlashXmlChannel() ) );
    xml.append( "    </Network>\n\n" );
    return xml;
}

QString XmlParamsWriter::createXmlFromBool( QString tag, bool b )
{
    return createXmlFromString( tag, (b ? "true" : "false") );
}

QString XmlParamsWriter::createXmlFromInt( QString tag, int n )
{
    return createXmlFromString( tag, QString::number( n ) );
}

QString XmlParamsWriter::createXmlFromDouble( QString tag, double n )
{
    return createXmlFromString( tag, QString::number( n, 'g', 3 ) );
}

QString XmlParamsWriter::createXmlFromString( QString tag, QString theValue )
{
    return QString( "        <" + tag + "> " + theValue + " </" + tag + ">\n" );
}

void XmlParamsWriter::debugPrintLn( const QString & msg )
{
    if( debug_ ) {
        std::cout << msg.toStdString() << "\n";
    }
}
