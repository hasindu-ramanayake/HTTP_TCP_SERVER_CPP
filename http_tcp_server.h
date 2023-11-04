#ifndef INCLUDE_HTTP_TCP_SERVER_H
#define INCLUDE_HTTP_TCP_SERVER_H

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string>

// struct in_addr{
//     unsigned long sAddr;
// };


// struct sockaddr_in {
//     short           sinFamily;
//     unsigned short  sinPort;    
//     struct in_addr  sinAddr;        
//     char            sinZero[8];
// };


namespace http {

    class TcpServer {

        public:
            TcpServer(std::string ipAdd_, int port_);
            ~TcpServer();
            int startListen();


        private:
            std::string _comIpAdd;
            int _comPort;
            int _comSocket;
            int _comNewSocket;
            long _comIncomeMessage;
            struct sockaddr_in _comSocketAdd;
            unsigned int _comSocketAddLen;
            std::string _comServerMessage;

            int setUpSocket();
            int closeUpSocket();
            int acceptConnection(int &newSocket_ );
            void sendResponse();
            std::string responce();
            
    };

} // end of http


#endif