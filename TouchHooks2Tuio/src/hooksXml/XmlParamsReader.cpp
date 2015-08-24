/*******************************************************************************
XmlParamsReader

PURPOSE: Reads program settings from an XML file.
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
#include "hooksXml/XmlParamsReader.h"
#include "hooksXml/XmlParamsValidator.h"
#include "hooksExceptions/FileNotFoundException.h"
#include "hooksExceptions/XmlReaderException.h"
#include <QFile>
#include <QIODevice>
#include <QByteArray>
#include <QtXml/QDomDocument>
#include <QtXml/QDomDocumentType>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
#include <QtXml/QDomAttr>
#include <QtXml/QDomText>
#include <iostream>

using hooksXml::XmlParamsReader;
using hooksExceptions::ValidatorException;
using hooksExceptions::UnknownXmlTagException;
using hooksExceptions::FileNotFoundException;
using hooksExceptions::XmlReaderException;

XmlParamsReader::XmlParamsReader() :
  unknownXmlTagExceptions_(),
  validatorExceptions_(),
  xmlFile_( "unknown" ),
  debugPrint_( false )
{
}

XmlParamsReader::~XmlParamsReader()
{
}

void XmlParamsReader::read( const QString & xmlFile, 
                            hooksXml::XmlParamsValidator * validator )
{
    xmlFile_ = xmlFile;
    validator->setXmlConfigFilename( xmlFile_ );
    unknownXmlTagExceptions_.clear();
    validatorExceptions_.clear();
    debugPrintLn( "\nSettingsXmlReader::readXmlFile( " + xmlFile_ + " ) called..." );

    QFile file( xmlFile_ );
    QDomDocument doc;
    QString err;
    int line = 0,
        col = 0;

    if( !file.open( QIODevice::ReadOnly ) ) {
        QString msg( "The XML configuration file could not be found." );
        throw FileNotFoundException( msg, "XmlParamsReader::read()", xmlFile );
    }
    if( !doc.setContent( file.readAll(), &err, &line, &col ) ) {
        QString msg( "An error occurred while reading the XML configuration file." );
        throw XmlReaderException( msg, "XmlParamsReader::read()",
                                  err, line, col, xmlFile_ );
    }
    extractParams( doc, validator );
}

void XmlParamsReader::extractParams( QDomDocument & doc, 
                                     hooksXml::XmlParamsValidator * validator )
{
    QDomElement root = doc.documentElement();
    QDomNode childNode = root.firstChild();
    QString tag = root.tagName().trimmed();
    debugPrintLn( "root = <" + tag + ">" );

    if( tag.compare( "TouchHooks2Tuio", Qt::CaseInsensitive ) != 0 ) {
        QString msg( "Unrecognized XML tag found." );
        throw UnknownXmlTagException( msg, "XmlParamsReader::extractParams()",
                                      tag, xmlFile_ );
    }
    while( !childNode.isNull() ) {
        if( childNode.isElement() ) {
            storeParams( childNode.toElement(), validator );
        }
        childNode = childNode.nextSibling();
    }
}

void XmlParamsReader::storeParams( QDomElement element, 
                                     hooksXml::XmlParamsValidator * validator )
{
    QString tag = element.tagName().trimmed();
    QDomNode childNode = element.firstChild();
    debugPrintLn( "   Element = <" + tag + ">" );
    tag = tag.toLower();

   
    if( tag == "hooks" ) {
        storeHooksParams( childNode, validator );
    }
    else if( tag == "network" ) {
        storeNetworkParams( childNode, validator );
    }
    else { 
        if( tag.size() == 0 ) { tag = "NO VALUE GIVEN"; }
        QString msg( "Unrecognized XML tag found." );
        UnknownXmlTagException e( msg, "XmlParamsReader::storeParams()",
                                  tag, xmlFile_ );
        unknownXmlTagExceptions_.push_back( e );
    }
}

void XmlParamsReader::storeHooksParams( QDomNode & node, 
                                        hooksXml::XmlParamsValidator * validator )
{
    while( !node.isNull() ) {
        if( node.isElement() ) {
            QDomElement subelement = node.toElement();
            QString tag = subelement.tagName().trimmed(),
                    text = subelement.text().trimmed();
            debugPrintLn( "        XML tag: " + tag + " = " + text );
            tag = tag.toLower();

            try {
                if( tag == "useglobalhook" ) {
                    validator->useGlobalHook( text );
                }
                else { 
                    if( tag.size() == 0 ) { tag = "NO VALUE GIVEN"; }
                    QString msg( "Unrecognized XML tag found." );
                    UnknownXmlTagException e( msg, "XmlParamsReader::storeHooksParams()",
                                              tag, xmlFile_ );
                    unknownXmlTagExceptions_.push_back( e );
                }
            }
            catch( ValidatorException e ) {
                validatorExceptions_.push_back( e );
            }
        }
        node = node.nextSibling();
    }
}

void XmlParamsReader::storeNetworkParams( QDomNode & node, 
                                          hooksXml::XmlParamsValidator * validator )
{
    while( !node.isNull() ) {
        if( node.isElement() ) {
            QDomElement subelement = node.toElement();
            QString tag = subelement.tagName().trimmed(),
                    text = subelement.text().trimmed();
            debugPrintLn( "        XML tag: " + tag + " = " + text );
            tag = tag.toLower();

            try {
                if( tag == "localhost" ) {
                    validator->setLocalHost( text );
                }
                else if( tag == "tuioudpchanneloneport" ) { 
                    validator->setTuioUdpChannelOnePort( text ); 
                }
                else if( tag == "tuioudpchanneltwoport" ) { 
                    validator->setTuioUdpChannelTwoPort( text ); 
                }
                else if( tag == "flashxmlchannelport" ) { 
                    validator->setFlashXmlChannelPort( text ); 
                }
                else if( tag == "usetuioudpchannelone" ) { 
                    validator->useTuioUdpChannelOne( text ); 
                }
                else if( tag == "usetuioudpchanneltwo" ) { 
                    validator->useTuioUdpChannelTwo( text ); 
                }
                else if( tag == "useflashxmlchannel" ) { 
                    validator->useFlashXmlChannel( text ); 
                }
                else { 
                    if( tag.size() == 0 ) { tag = "NO VALUE GIVEN"; }
                    QString msg( "Unrecognized XML tag found." );
                    UnknownXmlTagException e( msg, "XmlParamsReader::storeNetworkParams()",
                                              tag, xmlFile_ );
                    unknownXmlTagExceptions_.push_back( e );
                }
            }
            catch( ValidatorException e ) {
                validatorExceptions_.push_back( e );
            }
        }
        node = node.nextSibling();
    }
}

bool XmlParamsReader::hasUnknownXmlTagExceptions()
{
    return (unknownXmlTagExceptions_.size() > 0);
}

std::vector<hooksExceptions::UnknownXmlTagException> XmlParamsReader::unknownXmlTagExceptions()
{
    return unknownXmlTagExceptions_;
}

bool XmlParamsReader::hasValidatorExceptions()
{
    return (validatorExceptions_.size() > 0);
}

std::vector<hooksExceptions::ValidatorException> XmlParamsReader::validatorExceptions()
{
    return validatorExceptions_;
}

void XmlParamsReader::debugPrintLn( const QString & msg )
{
    if( debugPrint_ ) {
        std::cout << msg.toStdString() << "\n";
    }
}
