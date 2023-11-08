#include "PC_Control.hpp"
PC_control::PC_control(int port,std::string ip):Port(port),IP(ip){
            listening =socket(AF_INET,SOCK_STREAM,0);
            if(listening == -1)
            {
                std::cout<<"can't create socket"<<std::endl;
                throw std::runtime_error("can't create socket");
            }
            //bind            
            hint.sin_family=AF_INET;
            hint.sin_port=htons(Port);
            inet_pton(AF_INET,ip.c_str(),&hint.sin_addr);
            if(bind(listening,(sockaddr*)&hint,sizeof(hint))==-1)
            {
                std::cout<<"can't Bind"<<std::endl;
                throw std::runtime_error("can't Bind");
            }
            memset(host ,0,NI_MAXHOST);
            memset(svc,0,NI_MAXSERV);
            clientSize=sizeof(client);
        }
        
int PC_control::listen_for_mobile(){
            int val=listen(listening, SOMAXCONN);
            if(val==-1){
            std::cout<<"can't listen"<<std::endl;
            throw std::runtime_error("can't listen");
            return val;
            }
            return 1;
        }
void PC_control::Accept_mobile(){
            clientSocket =accept(listening,(sockaddr*)&client,&clientSize);
            if (clientSocket==-1){
            std::cout<<"client couldnot connect"<<std::endl;
            throw std::runtime_error("client couldnot connect");}
            // close(listening);
            int result= getnameinfo((sockaddr*)&client,sizeof(client),host,NI_MAXHOST,svc,NI_MAXSERV,0);

            if(result)
            {
                std::cout<<host<<" Connected on "<<svc<<std::endl;
            }
            else{
                inet_ntop(AF_INET,&client.sin_addr,host,NI_MAXHOST);
                std::cout<<host<<"Connectedd on"<<ntohs(client.sin_port)<<std::endl;
            }
        }
std::string PC_control::recieve_order(){
            message_to_client="Choose one of the following actions:\n1-term to open terminal\n2-code to open vscode\n3-fire to open firefox\n4-calc to open calculator\n";
            send_to_mobile(message_to_client,message_to_client.length());
            memset(buf,0,4096);
            int bytesrecved=recv(clientSocket,buf,4096,0);
            
            if(bytesrecved==-1){
                std::cout<<"Connection issue"<<std::endl;
                close(clientSocket);
                return "-1";
            }
            if(bytesrecved==0 ){
                std::cout<<"client disconnected"<<std::endl;
                close(clientSocket);
                return "0";
            }
            std::string message(buf,0,bytesrecved);
            // Remove leading whitespaces and newlines
            message.erase(message.begin(), std::find_if(message.begin(), message.end(), [](int ch) {
                return !(std::isspace(ch) || ch == '\n');
            }));

            // Remove trailing whitespaces and newlines
            message.erase(std::find_if(message.rbegin(), message.rend(), [](int ch) {
                return !(std::isspace(ch) || ch == '\n');
            }).base(), message.end());

            // Convert to lowercase
            std::transform(message.begin(), message.end(), message.begin(), [](unsigned char c) {
                return std::tolower(c);
            });
            std::cout<<"Recieved: "<<message<<bytesrecved<<std::endl;
            if(message=="calc"){
                system("gnome-calculator");
                message_to_client="Opened Calculator\n"; 
            }else if(message=="fire"){
                system("firefox");
                message_to_client="Opened Firefox\n";
            }else if(message=="code"){
                system("code");
                message_to_client="Opened VSCode\n";
            }else if(message=="term"){
                system("gnome-terminal");
                message_to_client="Opened new Terminal\n";
            }else{
                message_to_client="Order not supported try again\n";
            }
            send_to_mobile(message_to_client,message_to_client.length());
            return message;
        }
void PC_control::send_to_mobile(std::string message,int N_bytes){
            
            send(clientSocket,message.c_str(),N_bytes,0);

        }
PC_control::~PC_control(){
            close(listening);
            close(clientSocket);
            delete[] host;
            delete[] svc;
            delete[] buf;
        }
