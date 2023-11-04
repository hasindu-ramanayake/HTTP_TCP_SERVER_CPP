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
    _comServerMessage(responce())
{

    serverLog::printInfoLog("Starting server..");
    _comSocketAdd.sin_family = AF_INET;
    _comSocketAdd.sin_port= htons(_comPort);
    _comSocketAdd.sin_addr.s_addr = inet_addr( _comIpAdd.c_str() );

    if ( setUpSocket() != 0 ) {
        std::ostringstream ss;
        ss << "Fail to start the server port: " << ntohs(_comSocketAdd.sin_port);
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

    if ( bind(_comSocket, (sockaddr* )&_comSocketAdd, _comSocketAddLen ) < 0 ) {
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
    ss << " connected address: " << inet_ntoa(_comSocketAdd.sin_addr) << " PORT: " << ntohs(_comSocketAdd.sin_port) << " ***\n\n";
    serverLog::printInfoLog(ss.str());

    int bytesReceived;

    while (true)
    {
        serverLog::printInfoLog("Waiting for a new connection ");
        acceptConnection(_comNewSocket);

        char buffer[BUFFER_SIZE] = {0};
        bytesReceived = read(_comNewSocket, buffer, BUFFER_SIZE);
        if (bytesReceived < 0) {
            serverLog::printErrorLog("Failed to read bytes from client socket connection");
        }

        std::ostringstream ss;
        serverLog::printInfoLog ("Received Request from client");
        serverLog::printInfoLog(ss.str());

        sendResponse();
        

        close(_comNewSocket);
    }

    return 0;
}

int http::TcpServer::acceptConnection(int &newSocket_ ) {
    newSocket_ = accept( _comSocket, (sockaddr*)&_comSocketAdd, &_comSocketAddLen );

    if ( newSocket_ < 0 ) {
        std::ostringstream ss;
        ss << " server fail to accept incomming stream ADD: " << inet_ntoa(_comSocketAdd.sin_addr) << " PORT: " << ntohs(_comSocketAdd.sin_port);
        serverLog::printErrorLog(ss.str());
    }

    return 0;
}

std::string http::TcpServer::responce() {
    std::string html = "<!DOCTYPE html><html lanf=\"en\"><body><h1>Welcome to C++ server<h1></body>";
    std::ostringstream ss;
    ss << "HTTP/1.1 200 OK\n";
    ss << "Content-type:text/html\n";
    ss << "Content-Lenth: " << html.size() << std::endl << std::endl;
    ss << html;

    return ss.str();
}


void http::TcpServer::sendResponse() {
    long sendByte;
    sendByte = write( _comNewSocket, _comServerMessage.c_str(), _comServerMessage.size());
    if ( sendByte == _comServerMessage.size() ) {
        serverLog::printInfoLog("Server responce to the client");
    } else {
        serverLog::printErrorLog("Error sending response to client");
    }

}



int http::TcpServer::closeUpSocket() {
    close(_comSocket);
    close( _comNewSocket);
    serverLog::printInfoLog("All sockets are closed. exit the server");

    exit(0);

}