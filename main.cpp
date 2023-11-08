#include "PC_Control.hpp"
int main()
{   
    PC_control controller(54000);
    controller.listen_for_mobile();
    controller.Accept_mobile();
    while(true){
        if(controller.recieve_order()=="-1"){
            controller.Accept_mobile();
        }
        else if(controller.recieve_order()=="close"){
            std::cout<<"Controller Turned Off"<<std::endl;
            break;
        }
    }

}