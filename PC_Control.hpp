#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

class PC_control{
    private:
        int listening;
        int Port;
        std::string IP;
        sockaddr_in hint;
        sockaddr_in client;
        socklen_t clientSize;
        char host[NI_MAXHOST]; 
        char svc[NI_MAXSERV];
        int clientSocket;
        char buf[4096];
        std::string message_to_client;
        public:
        PC_control(int port,std::string ip="0.0.0.0");
        int listen_for_mobile();
        void Accept_mobile();
        std::string recieve_order();
        void send_to_mobile(std::string message,int N_bytes);
        ~PC_control();
};