#include <iostream>

#include "controller_handler.h"


int main(){

    ControllerHandler ch("/dev/input/js0");

    auto mailbox = ch.getMailbox();

    ch.run();

    while(true){
        ControllerHandler::printControllerData(mailbox.get());

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;


}