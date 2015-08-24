/*******************************************************************************
XmlSettings

PURPOSE: Reads/writes program settings from/to an XML file.
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
#include "hooksXml/XmlSettings.h"
#include "hooksXml/XmlParamsValidator.h"
#include "hooksXml/XmlParamsReader.h"
#include "hooksXml/XmlParamsWriter.h"
#include "hooksGui/TouchHooksMainWindow.h"
#include "hooksGui/XmlDialogBoxUtils.h"
#include "hooksCore/TouchMessageListener.h"
#include "hooksExceptions/FileNotFoundException.h"
#include "hooksExceptions/XmlReaderException.h"
#include "hooksExceptions/XmlWriterException.h"
#include "hooksExceptions/UnknownXmlTagException.h"
#include "hooksExceptions/ValidatorException.h"
#include <iostream>

using hooksXml::XmlSettings;
using hooksExceptions::FileNotFoundException;
using hooksExceptions::XmlReaderException;
using hooksExceptions::XmlWriterException;
using hooksExceptions::UnknownXmlTagException;
using hooksExceptions::ValidatorException;

const QString XmlSettings::DEFAULT_CONFIG_FILE = "Data/Settings/TouchHooks2TuioSettings.xml";

XmlSettings::XmlSettings() :
  validator_( std::make_shared<hooksXml::XmlParamsValidator>() ),
  reader_( std::make_unique<hooksXml::XmlParamsReader>() ),
  writer_( std::make_unique<hooksXml::XmlParamsWriter>() ),
  dialogBox_( std::make_unique<hooksGui::XmlDialogBoxUtils>() )
{
}

XmlSettings::~XmlSettings()
{
}

void XmlSettings::readXmlConfigFile()
{
    useXmlFileToUpdateValidator();
}

void XmlSettings::updateServerHostAndPorts( hooksGui::TouchHooksMainWindow * mainWindow )
{
    std::shared_ptr<hooksCore::TouchMessageListener>
    touchMessageListener = mainWindow->touchMessageListener();

    touchMessageListener->setServerInfo( validator_->getLocalHost(), 
                                         validator_->getTuioUdpChannelOnePort(),
                                         validator_->getTuioUdpChannelTwoPort(),
                                         validator_->getFlashXmlChannelPort() );
}

void XmlSettings::setTuioChannelsOnOrOff( hooksGui::TouchHooksMainWindow * mainWindow )
{
    std::shared_ptr<hooksCore::TouchMessageListener>
    touchMessageListener = mainWindow->touchMessageListener();

    touchMessageListener->useTuioUdpChannelOne( validator_->useTuioUdpChannelOne() );
    touchMessageListener->useTuioUdpChannelTwo( validator_->useTuioUdpChannelTwo() );
    touchMessageListener->useFlashXmlTcpChannel( validator_->useFlashXmlChannel() );

    mainWindow->setNetworkMenuCheckboxes( validator_->useTuioUdpChannelOne(),
                                          validator_->useTuioUdpChannelTwo(),
                                          validator_->useFlashXmlChannel() );
}

void XmlSettings::initializeGlobalTouchHook( hooksGui::TouchHooksMainWindow * mainWindow )
{
    if( validator_->useGlobalHook() ) {
        mainWindow->useGlobalTouchHook( true ); // Sets hook and writes status to gui.
        mainWindow->quietlySetHooksMenuOption( true );
    }
    else {
        // Do not call 'mainWindow->useGlobalTouchHook( false )' here.  That would
        // try to remove a non-existant hook and report the failure to the gui.
        mainWindow->quietlySetHooksMenuOption( false );
    }
}

void XmlSettings::saveSettingsToXmlFile( hooksGui::TouchHooksMainWindow * mainWindow )
{
    std::shared_ptr<hooksCore::TouchMessageListener>
    touchMessageListener = mainWindow->touchMessageListener();

    validator_->useGlobalHook( mainWindow->useGlobalTouchHook() );
    validator_->setLocalHost( touchMessageListener->host().toStdString() );
    validator_->setTuioUdpChannelOnePort( touchMessageListener->tuioUdpChannelOnePort() );
    validator_->setTuioUdpChannelTwoPort( touchMessageListener->tuioUdpChannelTwoPort() );
    validator_->setFlashXmlChannelPort( touchMessageListener->flashXmlChannelPort() );
    validator_->useTuioUdpChannelOne( touchMessageListener->useTuioUdpChannelOne() );
    validator_->useTuioUdpChannelTwo( touchMessageListener->useTuioUdpChannelTwo() );
    validator_->useFlashXmlChannel( touchMessageListener->useFlashXmlTcpChannel() );

    useValidatorToUpdateXmlFile();
}

void XmlSettings::useXmlFileToUpdateValidator()
{
    try {
        reader_->read( DEFAULT_CONFIG_FILE, validator_.get() );
    }
    catch( FileNotFoundException e ) {
        dialogBox_->warnUser( e );
    }
    catch( XmlReaderException e ) {
        dialogBox_->warnUser( e );
    }
    catch( UnknownXmlTagException e ) {
        dialogBox_->warnUser( e );
    }
    catch( ValidatorException e ) {
        dialogBox_->warnUser( e );
    }
    catch( ... ) {
        QString msg = "An unexpected error occurred while trying to read \n'";
        dialogBox_->warnUser( msg + DEFAULT_CONFIG_FILE + "'." );
    }
    if( reader_->hasUnknownXmlTagExceptions() ) {
        dialogBox_->warnUser( reader_->unknownXmlTagExceptions() );
    }
    if( reader_->hasValidatorExceptions() ) {
        dialogBox_->warnUser( reader_->validatorExceptions() );
    }
}

void XmlSettings::useValidatorToUpdateXmlFile()
{
    try {
        writer_->write( validator_.get(), DEFAULT_CONFIG_FILE );
    }
    catch( XmlWriterException e ) {
        dialogBox_->warnUser( e );
    }
    catch( ... ) {
        QString msg( "An unexpected error occurred while trying to write \n" );
        dialogBox_->warnUser( msg + DEFAULT_CONFIG_FILE );
    }
}

void XmlSettings::useGlobalHook( bool b )
{
    validator_->useGlobalHook( b );
}

void XmlSettings::useTuioUdpChannelOne( bool b )
{
    validator_->useTuioUdpChannelOne( b );
}

void XmlSettings::useTuioUdpChannelTwo( bool b )
{
    validator_->useTuioUdpChannelTwo( b );
}

void XmlSettings::useFlashXmlChannel( bool b )
{
    validator_->useFlashXmlChannel( b );
}
