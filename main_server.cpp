#include <iostream>

#include "http_tcp_server.h"

using namespace std;

int main ( ) {

    http::TcpServer myServer("127.0.0.1", 8080);

    myServer.startListen();
    // cout << "Hello Server.! " << endl; 
    // cout << "Hi There " << endl; 

    return 0;
}