// server-crawl.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include "common/citeSTL.hpp"
#include "common/citeBoost.hpp"
#include "common/utility/pool.hpp"
#include "network/session.hpp"

// global configuration variable facilitating adjustment
string test_url{ "http://xiamirun.avosapps.com/run?song=http://www.xiami.com/song/1795467286" };
auto thread_count{ max<short>(thread::hardware_concurrency() - 2,2) };
using protocal = tcp;

int main()
{
	pool<thread> pool{ thread_count };
	auto& service = pool.get();
	tcp::resolver resolver{ service };
	tcp::socket socket{ service };
	resolver.async_resolve(resolver::query("xiamirun.avosapps.com", "http"),
		[&](const error& err,resolver::iterator iter)
	{
		BOOST_ASSERT_MSG(!err, "DNS failure");

	});
	pool.wait();
	return 0;
}

