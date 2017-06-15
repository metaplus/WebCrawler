#pragma once
#include "common/citeSTL.hpp"
#include "common/citeBoost.hpp"

const boost::filesystem::path root="/home/kse/Videos";

namespace net{

    class Connection{ //: public enable_shared_from_this<Connection>{
    private:
        class Handler{
        public:
            Handler(asio::io_service& _srv):socket(_srv){ }
            void process(boost::barrier& barrier,unique_future<vector<char>>& chunk){
                //   auto self(shared_from_this());

                asio::async_read_until(socket,buffer,"\r\n\r\n",[this,&chunk,&barrier](const error&,size_t){
                    istream in(&buffer);
                    in>>method>>url>>version;
                    string identifier,value;
                    while(getline(in>>ws,identifier,':')&&getline(in>>ws,value,'\r')){
                        request[identifier]=value;
                    }

                    vector<char> ts=chunk.get();
                    stringstream response;
                    response<<"HTTP/1.1 200 OK\r\n"
                            <<"Content-Length: "<<ts.size()<<"\r\n"
                            <<"Content-Type: video/mp2t\r\n\r\n";

                    if(url=="/"){
                        barrier.wait();
                        asio::write(socket,asio::buffer(response.str()));
                        asio::write(socket,asio::buffer(ts));
                        cout<<this_thread::get_id()<<et<<"finish"<<el;
                    }else{
                        string name;
                    }

                });
            }
        private:
            friend class Connection;
            tcp::socket socket;
            asio::streambuf buffer;
            string method,url,version;
            map<string,string> request;
        };

    public:
        Connection(short _port,boost::thread_group& _sender)
                :service(),work(service),
                 acceptor(service,tcp::endpoint(tcp::v4(),_port),true) {
            acceptor.listen();
            _sender.create_thread(boost::bind(&asio::io_service::run,&service));
        }
        void run(boost::barrier& barrier){
            auto current=make_shared<Handler>(service);
            acceptor.async_accept(current->socket,[&barrier,this,current](const error& error){
                if(!error){
                    sequence.push_back(current);
                    current->process(barrier,chunk);
                }
                run(barrier);
            });
        }
        Connection& source(shared_ptr<promise<vector<char>>> from){
            chunk=from->get_future();
            return *this;
        }
        ~Connection(){
            service.stop();
        }

    private:
        friend class Server;
        asio::io_service service;
        asio::io_service::work work;
        tcp::acceptor acceptor;
        vector<shared_ptr<Handler>> sequence;
        unique_future<vector<char>> chunk;
    };

}