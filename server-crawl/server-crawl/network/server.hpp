#pragma once
#include "connection.hpp"


namespace net{

    class Server{
    public:
        Server(boost::thread_group& _sender,int _amount,ushort _port=8080)
                :amount(_amount),port(_port),sender(_sender){
            for(int i=0;i<amount;i++){
                session.push_back(make_shared<connection>(port+i,sender));
            }
        }

        // Stale version, regardless of data source
        void run(boost::barrier& barrier){
            for_each(session.begin(),session.end(),
                     std::bind(&connection::run,placeholders::_1,std::ref(barrier)));
        }

    private:
        int amount;
        ushort port;
        boost::thread_group& sender;
        vector<shared_ptr<connection>> session;
    };

}

