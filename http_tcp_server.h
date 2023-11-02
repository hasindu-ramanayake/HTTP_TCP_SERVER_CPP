#ifndef INCLUDE_HTTP_TCP_SERVER_H
#define INCLUDE_HTTP_TCP_SERVER_H

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string>

namespace http {

    typedef struct {
        unsigned long sAddr;
    } in_addr;


    typedef struct SOCKADDR_IN {
        short           sinFamily;
        unsigned short  sinPort;    
        in_addr         sinAddr;        
        char            sinZero[8];
    } sockaddr;

    
    class TcpServer {

        public:
            TcpServer(std::string ipAdd_, int port_);
            ~TcpServer();


        private:
            std::string _comIpAdd;
            int _comPort;
            int _comSocket;
            int _comNewSocket;
            long _comIncomeMessage;
            sockaddr _comSocketAdd;
            unsigned int _comSocketAddLen;
            std::string _comServerMessage;

            int setUpSocket();
            int closeUpSocket();
            int startListen();
            
    };

} // end of http


#endif