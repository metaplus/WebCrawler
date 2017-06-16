#pragma once
#include "common/citeSTL.hpp"
#include "common/citeBoost.hpp"
#include "config.hpp"

namespace net{

    class session{ //: public enable_shared_from_this<session>{
    private:
        class protocal{
        public:
            explicit protocal(io_service& _srv):socket(_srv){ }
            void handle(barrier& bar,unique_future<vector<char>>& chunk){
                //   auto self(shared_from_this());

                async_read_until(socket,buffer,"\r\n\r\n",[&,this](const error&,size_t){
                    istream in(&buffer);
                    in>>method>>url>>version;
                    string identifier,value;
                    while(getline(in>>ws,identifier,':')&&getline(in>>ws,value,'\r')){
                        request[identifier]=value;
                    }

                    auto ts=chunk.get();
                    stringstream response;
                    response	<<"HTTP/1.1 200 OK\r\n"
                					<<"Content-Length: "<<ts.size()<<"\r\n"
									<<"Content-Type: video/mp2t\r\n\r\n";

                    if(url=="/"){
                        bar.wait();
                        asio::write(socket,asio::buffer(response.str()));
                        asio::write(socket,asio::buffer(ts));
                        cout<<this_thread::get_id()<<et<<"finish"<<en;
                    }else{
                        string name;
                    }

                });
            }
        private:
            friend class session;
            tcp::socket socket;
            asio::streambuf buffer;
            string method,url,version;
            map<string,string> request;
        };

    public:
        session(short _port,boost::thread_group& _sender)
                :service(),work(service),
                 acceptor(service,tcp::endpoint(tcp::v4(),_port),true) {
            acceptor.listen();
			//acceptor.set_option(asio::socket_base::reuse_address(true));
            _sender.create_thread(boost::bind(&asio::io_service::run,&service));
        }
        void run(barrier& barrier){
            auto current=make_shared<protocal>(service);
            acceptor.async_accept(current->socket,[&,this,current](const error& error){
                if(!error){
                    sequence.push_back(current);
                    current->handle(barrier,chunk);
                }
                run(barrier);
            });
        }
        session& source(shared_ptr<promise<vector<char>>> from){
            chunk=from->get_future();
            return *this;
        }
        ~session(){
            service.stop();
        }

    private:
        friend class server;
        io_service service;
        io_service::work work;
        tcp::acceptor acceptor;
        vector<shared_ptr<protocal>> sequence;
        unique_future<vector<char>> chunk;
    };

}