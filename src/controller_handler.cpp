#include "controller_handler.h"
#include <fcntl.h>

#include <unistd.h>

#include <linux/joystick.h>

#include <errno.h>

#include <iostream>
#include "utilities.h"



ControllerHandler::ControllerHandler(const std::string dev_name){

    dev_name_ = dev_name;
}


ControllerHandler::~ControllerHandler(){

}


MailboxInput<controller_data> ControllerHandler::getMailbox(){
    return output_mailbox_.getConnection();
}

bool ControllerHandler::open_controller(const std::string dev_name){

    bool res = false;
    fd = open(dev_name.c_str(), O_RDONLY);
    if (fd != -1)
    {
        res = true;
        struct js_event event;
        for(int i = 0; i < 20; i++){

        if (read(fd, &event, sizeof(struct js_event)) == -1 && errno != EAGAIN)
        {

            throw ControllerOpenException();
        }

        }
    }
    
    FD_ZERO(&set);
    FD_SET(fd, &set);
    
    return res;

}


void ControllerHandler::run(){

    std::cout << "control handler thread start\n";

    handler_thread_ = std::move(std::thread([this] (){
        for(;;){

            if(open_controller(dev_name_)){

                try{

                    op();
                }
                catch( ControllerOpenException& c){

                    std::cout << "ERR: " << c.what();
                }

            }
            else{
                //sleep for a bit and retry opening the controller
                std::cout << "couldn't open controller :(\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(controller_retry_rate_ms_));
            }

        }
    }));

}


void zero_buffer(int16_t* buff, uint16_t size){

    for(int i=0; i<size; i++){
        buff[i] = 0U;
    }

}


void ControllerHandler::op(){


    for(;;){

        do_controller2(&cd);
        // std::string sendstr;
        
        output_mailbox_.put(cd);
        
    }

}


void ControllerHandler::do_controller2(controller_data* controller_data){

    struct js_event event;
    if (FD_ISSET(fd, &set))
    {
        if (read(fd, &event, sizeof(struct js_event)) == -1 && errno != EAGAIN)
        {

            throw ControllerOpenException();
        }

        switch (event.type)
        {
            case JS_EVENT_BUTTON:

                // controller_data[event.number + 8] = event.value;

                if(event.value){
                    controller_data->buttons_ |= (1 << event.number);
                }
                else{
                    controller_data->buttons_ &= ~(1 << event.number);
                }
                
                break;

    
            case JS_EVENT_AXIS:

                // printf("axis! %i %i\n", event.number, event.value);
                switch(event.number){

                    case 0:
                        controller_data->lsy_ = event.value;
                        break;

                    case 1:
                        controller_data->lsx_ = event.value;
                        break;


                    case 2:
                        controller_data->lt_ = event.value;
                        break;

                    case 3:
                        controller_data->rsy_ = event.value;
                        break;

                    case 4:
                        controller_data->rsx_ = event.value;
                        break;


                    case 5:
                        controller_data->rt_ = event.value;
                        break;

                    case 6:
                        controller_data->dx_ = event.value;
                        break;


                    case 7:
                        controller_data->dy_ = event.value;
                        break;
                        
                }


            default:

                break;
        }

    }

}


void ControllerHandler::printControllerData(const controller_data& data){

    std::cout << "cd: " << std::to_string(data.lsx_) << " " 
                        << std::to_string(data.lsy_) << " " 
                        << std::to_string(data.lt_) << " " 
                        << std::to_string(data.rsx_) << " " 
                        << std::to_string(data.rsy_) << " " 
                        << std::to_string(data.rt_) << " " 
                        << std::to_string(data.buttons_) << "\n";


}

