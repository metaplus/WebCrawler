#pragma once
#include "common/citeSTL.hpp"
#include "common/citeBoost.hpp"
#include "common/utility/pool.hpp"

namespace net
{
	class client;

	struct handler
	{
		void read(const error& err,shared_ptr<streambuf> response)
		{
			cout << en;
			istream response_stream{ response.get() };
			cout << response_stream.rdbuf() << en;
		}

		void write(const error& err)
		{
			auto response = make_shared<streambuf>();
			async_read_until(*socket, *response, "\r\n",
				bind(&handler::read, this, place::error, response));
			bufptr.push_back(response);

		}
		void connect(const error& err)
		{
			BOOST_ASSERT_MSG(!err, "connection error");
			cout << "remote" << et << socket->remote_endpoint() << en
				<< "local" << et << socket->local_endpoint() << en;
			auto request = make_shared<streambuf>();
			ostream request_stream{ request.get() };
			request_stream << "GET " << directory << " HTTP/1.0" << crlf
				<< "Host: " << host << crlf
				<< "Accept: */*" << crlf
				<< "Connection: close" << crlf << crlf;
			async_write(*socket, *request, bind(&handler::write, this, place::error));
			bufptr.push_back(request);
		}
		void resolve (const error& err, resolver::iterator iter)
		{
			BOOST_ASSERT_MSG(!err, "DNS error");
			async_connect(*socket, iter, bind(&handler::connect, this, place::error));
		}
		unique_ptr<tcp::resolver> dns;
		unique_ptr<tcp::socket> socket;
		vector<shared_ptr<streambuf>> bufptr;
		string host;
		string directory;
	};
	class client:public handler
	{
	public:
		explicit client(pool<thread>& p):
			pool{p},service{pool.get()}
		{
			dns = make_unique<tcp::resolver>(service);
			socket = make_unique<tcp::socket>(service);
		}
		void remote(string url)
		{
			smatch part;
			regex pattern{ "http:\/\/([^\/]+)(.*)" };
			regex_match(url, part, pattern);
			host = part[1].str();
			directory = part[2].str();
			cout << "host" << et << host << en
				<< "catlog" << et << directory << en;
		}
		void run() 
		{
			dns->async_resolve(resolver::query(host, "http"),
				bind(&handler::resolve, this, place::error, place::iterator));
	//		this_thread::sleep_for(chrono::seconds{ 1 });
			pool.wait();
		}
	private:
		pool<thread>& pool;
		io_service& service;
	};
}

