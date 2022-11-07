#ifndef __CONTROLLER_HANDLER_H__
#define __CONTROLLER_HANDLER_H__



#include <stdint.h>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include "threadsafe_mailbox.hpp"


typedef struct {

    int16_t lsx_ = 0;
    int16_t lsy_ = 0;
    int16_t lt_ = -32767;
    int16_t rsx_ = 0;
    int16_t rsy_ = 0;
    int16_t rt_ = -32767;
    int16_t dx_ = 0;
    int16_t dy_ = 0;
    uint8_t buttons_ = 0;



} controller_data;


class ControllerOpenException: public std::exception{

public:
    virtual const char* what() const throw()
    {
        return "cannot open controller!\n";
    }

};


class ControllerHandler{

public:


    ControllerHandler(const std::string dev_name);
    ~ControllerHandler();
    void run();

    MailboxInput<controller_data> getMailbox();

    static void printControllerData(const controller_data& data);

private:

    bool open_controller(const std::string dev_name);

    void do_controller2(controller_data* controller_data);

    void op();

    void printControllerData();

    std::thread handler_thread_;


    std::string dev_name_;
    int fd;
    fd_set set;


    controller_data cd = {0,0,-32767,0,0,-32767,0,0,0};

    controller_data prev_cd = {0,0,-32767,0,0,-32767,0,0,0};

    MailboxOutput<controller_data> output_mailbox_;


    uint16_t controller_retry_rate_ms_ = 1000U;


};


// end __CONTROLLER_HANDLER_H__
#endif