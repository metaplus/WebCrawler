#pragma once
#include "common/citeSTL.hpp"
#include "common/citeBoost.hpp"

// thread pool prototype powered by asio
// memory pool might be specified in the future

template<typename T,typename = enable_if_t<is_same<T,thread>::value>>
class pool:noncopyable{
public:
	explicit pool(short thread_count)
		:count{thread_count},service{},work{make_unique<io_service::work>(ref(service))}
	{
		for(auto i=0;i<count;i++)
		{
			worker.create_thread(boost::bind(&io_service::run,&service));
		}
	}
	io_service& get()
	{
		return service;
	}
	template<typename U> typename
	enable_if<is_function<U>::value>::type post(U task)
	{
		service.post(task);
	}

	void wait()
	{
		call_once(token,[this]
		{
			work.reset();
			worker.join_all();
		});
	}
	~pool()
	{
		wait();
		service.stop();
	}
private:
	short count;
	once_flag token;
	io_service service;
	unique_ptr<io_service::work> work;
	thread_group worker;
};