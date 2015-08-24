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
#ifndef HOOKSXML_XMLPARAMSREADER_H
#define HOOKSXML_XMLPARAMSREADER_H

#include "hooksExceptions/ValidatorException.h"
#include "hooksExceptions/UnknownXmlTagException.h"
#include <QString>
#include <QtCore/QByteArray>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <vector>

namespace hooksXml { class XmlParamsValidator; }

namespace hooksXml
{
    class XmlParamsReader
    {
    public:
        XmlParamsReader();
        virtual ~XmlParamsReader();

        void read( const QString & xmlFile, hooksXml::XmlParamsValidator * validator );

        bool hasUnknownXmlTagExceptions();
        std::vector<hooksExceptions::UnknownXmlTagException> unknownXmlTagExceptions();
        bool hasValidatorExceptions();
        std::vector<hooksExceptions::ValidatorException> validatorExceptions();

    private:
        void extractParams( QDomDocument & doc, hooksXml::XmlParamsValidator * validator );
        void storeParams( QDomElement element, hooksXml::XmlParamsValidator * validator );
        void storeHooksParams( QDomNode & node, hooksXml::XmlParamsValidator * validator );
        void storeNetworkParams( QDomNode & node, hooksXml::XmlParamsValidator * validator );
        void debugPrintLn( const QString & msg );

        std::vector<hooksExceptions::UnknownXmlTagException> unknownXmlTagExceptions_;
        std::vector<hooksExceptions::ValidatorException> validatorExceptions_;
        QString xmlFile_;
        bool debugPrint_;
    };
}

#endif
