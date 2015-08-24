#include <ofxTCPServer.h>
#include <iostream>
#include <memory>

int main( )
{
    std::cout << "Hello World!\n";

    std::unique_ptr <ofxTCPServer> tcpServer( new ofxTCPServer() );
    bool ok = tcpServer->setup( 3000 );

    if( ok ) {
        std::cout << "Server setup on port 3000 ok.\n";
    }
    else {
        std::cout << "Server setup on port 3000 failed.\n";
    }
    char c = getchar();
    std::cout << "Goodbye World!\n";

    tcpServer.reset( nullptr );
}
