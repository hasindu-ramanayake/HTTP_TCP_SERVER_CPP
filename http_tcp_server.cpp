#include <iostream>
#include <sstream>
#include <unistd.h>

// custom headers
#include "http_tcp_server.h"

const int BUFFER_SIZE = 30720;

namespace serverLog {
    
    void printInfoLog( const std::string &log_ ){
        std::cout << "INFO: " << log_ << std::endl;
    }

    void printErrorLog( const std::string &log_ ){
        std::cout << "ERROR: " << log_ << std::endl;
    }  

}

/*
// easy to refer copy comment
    typedef struct {
        unsigned long sAddr;
    } in_addr;


    typedef struct SOCKADDR_IN {
        short           sinFamily;
        unsigned short  sinPort;    
        in_addr         sinAddr;        
        char            sinZero[8];
    } sockaddr;

*/

http::TcpServer::TcpServer( std::string ipAdd_, int port_ )
    :_comIpAdd(ipAdd_), 
    _comPort(port_),
    _comSocket(),
    _comNewSocket(),
    _comIncomeMessage(),
    _comSocketAdd(),
    _comSocketAddLen(sizeof(_comSocketAdd)),
    _comServerMessage("wellcome to the Server")
{

    serverLog::printInfoLog("Starting server..");
    _comSocketAdd.sinFamily = AF_INET;
    _comSocketAdd.sinPort= htons(_comPort);
    _comSocketAdd.sinAddr.sAddr = inet_addr( _comIpAdd.c_str() );

    if ( setUpSocket() != 0 ) {
        std::ostringstream ss;
        ss << "Fail to start the server port: " << ntohs(_comSocketAdd.sinPort);
        serverLog::printInfoLog(ss.str());
    }
}
http::TcpServer::~TcpServer() {
    serverLog::printInfoLog("Closing server..");
    closeUpSocket();

}

int http::TcpServer::setUpSocket() {
    _comSocket = socket( AF_INET, SOCK_STREAM, 0);
    if ( _comSocket < 0 ) {
        serverLog::printErrorLog("Unable to create a socket.");
        return 1;
    }

    if ( bind(_comSocket, (const http::sockaddr* )&_comSocketAdd, _comSocketAddLen ) < 0 ) {
        serverLog::printErrorLog("Cannot bind socket and address.");
        return 1;
    }
    return 0;
}

int http::TcpServer::startListen() {
    if ( listen(_comSocket, 20) < 0 ) {
        serverLog::printErrorLog("Socket Listen fail.");
    }

    std::ostringstream ss;
    ss << " connected address: " << inet_ntoa(_comSocketAdd.sinAddr) << " PORT: " << ntohs(_comSocketAdd.sinPort) << " ***\n\n";
    serverLog::printInfoLog(ss.str());

    int bytesReceived;

    while (true)
    {
        serverLog::printInfoLog("Waiting for a new connection ");
        // acceptConnection(_comNewSocket);

        char buffer[BUFFER_SIZE] = {0};
        bytesReceived = read(_comNewSocket, buffer, BUFFER_SIZE);
        if (bytesReceived < 0) {
            serverLog::printErrorLog("Failed to read bytes from client socket connection");
        }

        std::ostringstream ss;
        serverLog::printInfoLog ("Received Request from client");
        serverLog::printInfoLog(ss.str());

        // sendResponse();
        

        close(_comNewSocket);
    }

    return 0;
}


int http::TcpServer::closeUpSocket() {
    close(_comSocket);
    close( _comNewSocket);
    serverLog::printInfoLog("All sockets are closed. exit the server");

    exit(0);

}