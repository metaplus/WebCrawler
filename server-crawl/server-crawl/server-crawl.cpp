// server-crawl.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "common/citeSTL.hpp"
#include "common/citeBoost.hpp"
#include "network/connection.hpp"

string test_url{ "http://xiamirun.avosapps.com/run?song=http://www.xiami.com/song/1795467286" };

int main()
{
	cout << "test initialization" << en;
	cout << count_url(5324) << en;
	return 0;
}

