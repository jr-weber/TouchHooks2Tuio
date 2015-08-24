/*******************************************************************************
main.cpp

PURPOSE: Reads the Data/Settings/TouchHooks2TuioSettings.xml configuration file
         before starting up the main window of the GUI.  Most program settings
         are intended to be changed through the Hooks or Network menu above the
         text area of the GUI, but port number changes have to be done through
         the configuration file.  TUIO-aware clients usually expect TUIO UDP 
         messages on port 3333, whereas the older Flash XML TCP messages are
         usually expected on port 3000.  There is an option to give a few 
         command line arguments, but that is mostly intended for use by another
         program, the PlaysurfaceLauncher (http://playsurface.org).

AUTHOR:  J.R. Weber <joe.weber77@gmail.com>
CREATED: 6/16/2015
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
#include "hooksGui/TouchHooksMainWindow.h"
#include "hooksXml/XmlSettings.h"
#include <QtWidgets/QApplication>
#include <QString>
#include <iostream>
#include <memory>
#include <Windows.h>

// function prototypes
QString localServerName( int, char * [] );
void processBooleanCmdLineArgs( int, char * [], std::shared_ptr<hooksXml::XmlSettings>  );
void processBooleanArg( const QString &, std::shared_ptr<hooksXml::XmlSettings> );

int main( int argc, char * argv [] )
{
    QApplication a( argc, argv ); 

    hooksGui::TouchHooksMainWindow mainWindow;
    mainWindow.readXmlConfigFile();
    processBooleanCmdLineArgs( argc, argv, mainWindow.xmlSettings() );

    mainWindow.updateServerHostAndPorts();
    mainWindow.initializeConsolePidForDebugging( (int)GetCurrentProcessId() );
    mainWindow.initializeCustomMessagesForHook();
    mainWindow.initializeTuioServers();
    mainWindow.setTuioChannelsOnOrOff();
    mainWindow.initializeLocalServer( localServerName( argc, argv ) );
    mainWindow.writeScreenInfo();
    mainWindow.writeServerInfo();
    mainWindow.initializeGlobalTouchHook();
    mainWindow.show();
    mainWindow.startTimer();

    return a.exec();
}

/*******************************************************************************
If a command line arg of the form localServerName=someName is found, 
then someName will be returned.  Otherwise, returns an empty string.
*******************************************************************************/
QString localServerName( int argc, char * argv[] )
{
    QString name = "";
    QString key = "localServerName=";

    for( int i = 1; i < argc; ++i ) {
        QString arg( argv[i] );
        int numCharsInValue = arg.size() - key.size();

        if( numCharsInValue > 0 && arg.startsWith( key, Qt::CaseInsensitive )  ) {
            name = arg.right( numCharsInValue );
            break;
        }
    }
    return name;
}

/*******************************************************************************
Uses command line args to modify the XML settings that were read in from the
Data/Settings/TouchHooks2TuioSettings.xml file.  See the processBooleanArg()
function below for what boolean params can be modified.
*******************************************************************************/
void processBooleanCmdLineArgs( int argc, char * argv [], 
                                std::shared_ptr<hooksXml::XmlSettings> xmlSettings )
{
    for( int i = 1; i < argc; ++i ) {
        processBooleanArg( argv[i], xmlSettings );
    }
}

void processBooleanArg( const QString & arg,
                        std::shared_ptr<hooksXml::XmlSettings> xmlSettings )
{
    QString argLower = arg.toLower();

    if(      argLower == "useglobalhook=true" )       { xmlSettings->useGlobalHook( true ); }
    else if( argLower == "useglobalhook=false" )      { xmlSettings->useGlobalHook( false ); }

    else if( argLower == "usetuioudpchannelone=true" )  { xmlSettings->useTuioUdpChannelOne( true ); }
    else if( argLower == "usetuioudpchannelone=false" ) { xmlSettings->useTuioUdpChannelOne( false ); }

    else if( argLower == "usetuioudpchanneltwo=true" )  { xmlSettings->useTuioUdpChannelTwo( true ); }
    else if( argLower == "usetuioudpchanneltwo=false" ) { xmlSettings->useTuioUdpChannelTwo( false ); }

    else if( argLower == "useflashxmlchannel=true" )    { xmlSettings->useFlashXmlChannel( true ); }
    else if( argLower == "useflashxmlchannel=false" )   { xmlSettings->useFlashXmlChannel( false ); }
}
