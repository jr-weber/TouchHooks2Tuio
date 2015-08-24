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
#ifndef HOOKSXML_XMLPARAMSWRITER_H
#define HOOKSXML_XMLPARAMSWRITER_H

#include <QString>

namespace hooksXml { class XmlParamsValidator; }

namespace hooksXml
{
    /***********************************************************************//**
    Knows how to read program settings from a SettingsValidator object and 
    write them to an XML file.
    ***************************************************************************/
    class XmlParamsWriter
    {
    public:
        XmlParamsWriter();
        virtual ~XmlParamsWriter();

        void write( hooksXml::XmlParamsValidator * validator, const QString & xmlFile );
        void printXmlToStandardOut( hooksXml::XmlParamsValidator * validator );

    private:
        QString getSettingsAsXml( hooksXml::XmlParamsValidator * validator );
        QString getHooksXml( hooksXml::XmlParamsValidator * validator );
        QString getNetworkXml( hooksXml::XmlParamsValidator * validator );
        QString createXmlFromBool( QString tag, bool b );
        QString createXmlFromInt( QString tag, int n );
        QString createXmlFromDouble( QString tag, double n );
        QString createXmlFromString( QString tag, QString theValue );

        void debugPrintLn( const QString & msg );
        bool debug_;
    };
}

#endif
