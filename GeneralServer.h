//
// Created by yuval on 07/01/19.
//

#ifndef EX2_TCPSERVER_H
#define EX2_TCPSERVER_H


#include "Server.h"
/*
 * this class represent a general sever that have the basic methods ,its heir from Server
 */
namespace server_side {
    class GeneralServer : public Server {

    private:
        int serverSocket;
        void start(int serverSocket, ClientHandler &c) override;

    public:
        GeneralServer();
        void stop() override;
        void open(int port, ClientHandler &c) override;
    };
}

#endif //EX2_TCPSERVER_H
