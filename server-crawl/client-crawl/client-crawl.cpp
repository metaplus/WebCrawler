// server-crawl.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "common/citeSTL.hpp"
#include "common/citeBoost.hpp"
#include "common/utility/pool.hpp"
#include "network/session.hpp"

string test_url{ "http://xiamirun.avosapps.com/run?song=http://www.xiami.com/song/1795467286" };

int main()
{
	pool<thread> pool{4};
	cout << sizeof(io_service)<<et<<sizeof(io_service::work)<<en;
	return 0;
}

