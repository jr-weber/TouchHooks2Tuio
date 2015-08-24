ofxTCPServer_v0.8.4_VS2013 is derived from the ofxNetwork module of 
openFrameworks version 0.8.4 and includes POCO and OpenSSL static
libraries.

openFrameworks: http://openframeworks.cc/download
POCO: http://pocoproject.org/index.html
OpenSSL: http://www.openssl.org/source

openFrameworks 0.8.4 and the static libraries that come with it were
built for Visual Studio 2012, which uses version 11.0 of the Microsoft
C/C++ compiler.  Surprisingly, those static libraries will not link
with Visual Studio 2013, which use version 12.0 of the Microsoft
C/C++ compiler.  Therefore, I used Visual Studio 2013 to build the
POCO and OpenSSL static libraries needed by ofxTCPServer, but 
excluded the rest of openFrameworks because it has a large number
of static libraries, all of which would need to be rebuilt with
Visual Studio 2013 (and that would be a fairly major undertaking).

The ofxTCPServer class is used by the FlashXmlTcpServer.h class
that can be found by opening the TouchHooks2Tuio.sln file with
Visual Studio 2013 and looking in the TuioServer project.  There 
are some notes there explaining why I used ofxTCPServer for the
Flash XML TCP server, even though the TUIO_CPP library from
http://tuio.org/?software is used for the TUIO UDP server.

The MIT License for openFrameworks should be found in the same
directory as this README.txt file.  Otherwise, it can be obtained
by going to the openFrameworks site at 
http://openframeworks.cc/download

-J.R.Weber <joe.weber77@gmail.com>
8/23/2015
