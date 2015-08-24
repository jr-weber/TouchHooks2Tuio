/*******************************************************************************
XmlDialogBoxUtils

PURPOSE: Extracts info from an XML-related exception and uses a modal dialog 
         box to warn the user. 
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
#ifndef HOOKSGUI_XMLDIALOGBOXUTILS_H
#define HOOKSGUI_XMLDIALOGBOXUTILS_H

#include "hooksExceptions/FileNotFoundException.h"
#include "hooksExceptions/XmlReaderException.h"
#include "hooksExceptions/XmlWriterException.h"
#include "hooksExceptions/UnknownXmlTagException.h"
#include "hooksExceptions/ValidatorException.h"
#include <QString>
#include <vector>

namespace hooksGui
{
    /***********************************************************************//**
    Extracts info from an XML-related exception and uses a modal dialog 
    box to warn the user.
    ***************************************************************************/
    class XmlDialogBoxUtils
    {

    public:
        XmlDialogBoxUtils();
        virtual ~XmlDialogBoxUtils();

        void warnUser( hooksExceptions::FileNotFoundException & e );
        void warnUser( hooksExceptions::XmlReaderException & e );
        void warnUser( hooksExceptions::XmlWriterException & e );
        void warnUser( hooksExceptions::UnknownXmlTagException & e );
        void warnUser( hooksExceptions::ValidatorException & e );
        void warnUser( std::vector<hooksExceptions::UnknownXmlTagException> & errors );
        void warnUser( std::vector<hooksExceptions::ValidatorException> & errors );
        void warnUser( const QString & message );

    private:
        void openWarningMessageBox( const QString & title, const QString & message );
    };
}

#endif
