#pragma once
#include "common/citeSTL.hpp"
#include "common/citeBoost.hpp"
#include "common/utility/pool.hpp"
#include "common/utility/material.hpp"
#include "network/session.hpp"
#include <cassert>
#include <future>



template<typename T>	//, typename = enable_if_t<is_same<T, net::client>::value>>
class factory :noncopyable {
public:
	factory(pool<thread>& po, const file::path& pa, ptree& tr) :
		pool{ po }, path{ pa }, tree{ tr }
	{

	}
	auto create(size_t id)
	{
		auto ptr{ std::make_shared<T>(pool, path, id, tree,*this) };
		manager.push_back(ptr);
		return ptr;
	}
	void push(shared_ptr<T> ptr)
	{
		manager.push_back(ptr);
	}
private:
	pool<thread>& pool;
	const file::path& path;
	property::ptree& tree;
	vector<shared_ptr<T>> manager;
};


namespace net
{
	class handler
	{
	public:
		explicit handler(const file::path& dir, size_t id, ptree& tr) : catalog{ dir }, id{ id }, tree{ tr }
		{
		}
		template<typename U, typename = enable_if_t<is_base_of<element, U>::value>>
		void resolve(const error& err, resolver::iterator iter, shared_ptr<U> ptr)
		{
			async_connect(*socket, iter, bind(&handler::connect, this, place::error));
			elem = move(ptr);
		}

	private:
		void connect(const error& err)
		{
			ostringstream oss;			// potential hazard ? thread_local
			oss << "GET " << directory << " HTTP/1.1" << crlf
				<< "Host: " << host << crlf
				<< "Accept: */*" << crlf
				<< "Connection: close" << crlf << crlf;
			async_write(*socket, asio::buffer(oss.str().data(), oss.str().size()), bind(&handler::write, this, place::error));
		}
		void write(const error& err)
		{
			async_read_until(*socket, response, "\r\n\r\n",
				bind(&handler::read, this, place::error));
		}

		void read(const error& err)
		{		
			if (err)
			{
				cerr << "zzzzzzzzzzzzzzzzzzz1111111111111111111111" << en;
				return;
			}
			istream response_stream{ &response };
			string line;
			size_t length{ 0 };
			while (getline(response_stream, line, '\r') && !line.empty())
			{
			//	cout << line << en;
				if(line.find("Location: ")!=string::npos)
				{
					callback.set_value(line.substr(line.find(':') + 2));
					return;
				}
				if (line.find("Content-Length: ") != string::npos)
				{
					cout << line << en;
					length = lexical_cast<size_t>(line.substr(line.find(':') + 2));
				}
				response_stream.ignore();
			}
			response_stream >> ws;
			callback.set_value({});
			elem->process(response, *socket, length - response.size());
		}
	public:
		shared_ptr<element> elem;
		unique_ptr<tcp::resolver> dns;
		unique_ptr<tcp::socket> socket;
		streambuf response;
		string host;
		string directory;
		size_t id;
		ptree& tree;
		const file::path& catalog;
		boost::promise<string> callback;
	};




	class client:public enable_shared_from_this<client>
	{
	public:
		explicit client(pool<thread>& p, const file::path& dir, size_t id, ptree& tr, factory<client>& fac) :
			pool{ p }, catalog{ dir }, id{ id }, tree{ tr }, service{ pool.get() }, factory{ fac }
		{
		}
		template<typename U>
		enable_if_t<std::is_base_of_v<element,U>> crawl(const string& url)
		{
			auto task = new handler(catalog, id, tree);
			task->dns = make_unique<tcp::resolver>(service);
			task->socket = make_unique<tcp::socket>(service);
			smatch part;
			regex pattern{ "http:\/\/([^\/]+)(.*)" };
			regex_match(url, part, pattern);
			task->host = part[1].str();
			task->directory = part[2].str();

			task->dns->async_resolve(resolver::query(task->host, "http"),
				bind(&handler::resolve<U>, task, 
						place::error, place::iterator,make_shared<U>(id,tree,catalog)));
			auto value = task->callback.get_future().get();
			if(!value.empty())
			{
				auto task2 = std::make_shared<client>(pool, catalog, id, tree, factory);
				factory.push(task2);
				task2->crawl<mp3>(value);
			}
		}


	private:
		pool<thread>& pool;
		io_service& service;
		const file::path& catalog;
		size_t id;
		ptree& tree;
		factory<client>& factory;
	};
}



