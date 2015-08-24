/*******************************************************************************
ValidatorException

Purpose: For reporting an error detected by the XmlParamsValidator class.
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
#ifndef HOOKSEXCEPTIONS_VALIDATOREXCEPTION_H
#define HOOKSEXCEPTIONS_VALIDATOREXCEPTION_H

#include "hooksExceptions/IOException.h"

namespace hooksExceptions
{
    /***********************************************************************//**
    For reporting an error detected by the XmlParamsValidator class.
    ***************************************************************************/
    class ValidatorException : public hooksExceptions::IOException
    {
        public:
            ValidatorException( const QString & message,
                                const QString & source ,
                                const QString & tagName, 
                                const QString & tagValue,
                                const QString & tagExpectedValue,
                                const QString & filename );
            virtual ~ValidatorException();

            QString getTagName();
            QString getTagValue();
            QString getTagExpectedValue();
            QString getFilename();

        private:
            QString tagName_,
                    tagValue_,
                    tagExpectedValue_,
                    filename_;
    };
}

#endif