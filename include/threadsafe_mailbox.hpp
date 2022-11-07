#pragma once
#include <mutex>

template<typename T>
class MailboxOutput;


template<typename T>
class DataContainer{

public:
    DataContainer(T init): item_(init){

    }
    DataContainer() = default;
    ~DataContainer() = default;

    T get(){
        std::lock_guard<std::mutex> l(m_);
        return item_;
    }



private:

    friend MailboxOutput<T>;

    void put(T item){
        std::lock_guard<std::mutex> l(m_);
        item_ = item;
    }

    T item_;

    std::mutex m_;

}; 


//read side of the mailbox
template<typename T>
class MailboxInput{


public:

    MailboxInput() = default;
    MailboxInput(DataContainer<T>* init): item_(init) { }
    ~MailboxInput() { }


    T get(){

        return item_->get();
    }


private:
    //this can end up as a dangler... need to make this a weak_ptr
    DataContainer<T>* item_;

};


//write side of the mailbox
template<typename T>
class MailboxOutput{

public:

    MailboxOutput() = default;
    MailboxOutput(T init): data_container_(init) { }
    ~MailboxOutput() { }


    void put(T item){
        data_container_.put(item);
    }

    MailboxInput<T> getConnection(){
        return MailboxInput<T>(&data_container_);
    }


private:

    DataContainer<T> data_container_;

};
