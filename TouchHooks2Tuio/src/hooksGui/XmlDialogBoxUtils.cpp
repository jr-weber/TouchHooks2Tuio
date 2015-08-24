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
#include "hooksGui/XmlDialogBoxUtils.h"
#include <QMessageBox>

using hooksGui::XmlDialogBoxUtils;

XmlDialogBoxUtils::XmlDialogBoxUtils()
{
}

XmlDialogBoxUtils::~XmlDialogBoxUtils()
{
}

void XmlDialogBoxUtils::warnUser( hooksExceptions::FileNotFoundException & e )
{
    QString title( "Warning" );
    QString message( e.getMessage() + "\nFile: " + e.filename() );
    message += "\n\nDefault settings will be used instead.";
    openWarningMessageBox( title, message );
}

void XmlDialogBoxUtils::warnUser( hooksExceptions::XmlReaderException & e )
{
    QString title( "Warning" );
    QString message( e.getMessage() +  "\n" + e.getXmlFilename() );
    message.append( "\n\nLine = " + QString::number( e.getLineNumber() ) );

    message.append( " and column = " + QString::number( e.getColNumber() ) + "\n" );
    message.append( e.getDocErrors() );
    message.append( "\n\nDefault configuration values will be used instead." );
    openWarningMessageBox( title, message );
}

void XmlDialogBoxUtils::warnUser( hooksExceptions::XmlWriterException & e )
{    
    QString title( "Warning" );
    QString message( e.getMessage() + "\n" + e.getXmlFilename() );
    openWarningMessageBox( title, message );
}

void XmlDialogBoxUtils::warnUser( hooksExceptions::UnknownXmlTagException & e )
{
    QString title( "Warning: UnknownXmlTagException" );
    QString message = e.getMessage() + "\n";
    message += "Unknown Tag = <" + e.getTagName() + ">\n";
    message += "File:  " + e.getFilename();
    openWarningMessageBox( title, message );
}

void XmlDialogBoxUtils::warnUser( hooksExceptions::ValidatorException & e )
{
    QString title( "Warning" );
    QString message = e.getMessage() + "\n";
    message += "Tag = <" + e.getTagName() + ">\n";
    message += "Expected value = " + e.getTagExpectedValue() + "\n";
    message += "Actual value = " + e.getTagValue() + "\n\n";
    message += "File:  " + e.getFilename();
    message += "\nA default configuration value will be used instead.";
    openWarningMessageBox( title, message );
}

void XmlDialogBoxUtils::warnUser( std::vector<hooksExceptions::UnknownXmlTagException> & errors )
{
    int length = errors.size();
    QString title( "Warning" );

    if( length > 0 ) {
        QString message = "Error reading file '";
        message += errors.at(0).getFilename() + "'\n";
        message += (length == 1) ? "Invalid XML tag:\n\n" : "Invalid XML tags:\n\n";

        for( int i = 0; i < length; ++i ) {
            message += "Unknown Tag = <" + errors.at(i).getTagName() + ">\n";
        }
        message += "Invalid tags are ignored.";
        openWarningMessageBox( title, message );
    }
}

void XmlDialogBoxUtils::warnUser( std::vector<hooksExceptions::ValidatorException> & errors )
{
    int length = errors.size();
    QString title( "Warning" );

    if( length > 0 ) {
        QString message = "Error reading file '";
        message += errors.at(0).getFilename() + "'\n";
        message += (length == 1) ? "Invalid tag value:\n\n" : "Invalid tag values:\n\n";

        for( int i = 0; i < length; ++i ) {
            message += "Tag = <" + errors.at(i).getTagName() + ">\n";
            message += "Expected value = " + errors.at(i).getTagExpectedValue() + "\n";
            message += "Actual value = " + errors.at(i).getTagValue() + "\n\n";
        }
        message += "A default value was used instead.";
        openWarningMessageBox( title, message );
    }
}

void XmlDialogBoxUtils::warnUser( const QString & message )
{
    QString title( "Warning" );
    openWarningMessageBox( title, message );
}

void XmlDialogBoxUtils::openWarningMessageBox( const QString & title, const QString & message )
{
    QMessageBox messageBox;
    messageBox.setWindowTitle( title );
    messageBox.setText( message );
    messageBox.setIcon( QMessageBox::Warning );
    messageBox.exec();
}
