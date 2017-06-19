// server-crawl.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include "common/citeSTL.hpp"
#include "common/citeBoost.hpp"
#include "common/utility/pool.hpp"
#include "common/utility/material.hpp"
#include "network/client.hpp"

#define _MP3_DOWNLOAD_PHASE_
#define _INFO_CRAWL_PHASE_
// global configuration variable facilitating adjustment
string url_prefix{ "http://xiamirun.avosapps.com/run?song=http://www.xiami.com/song/" };
string url_json{ "http://xiamirun.avosapps.com/run?song=http://www.xiami.com/song/1795747926" };
string url_mp3{ "http://om5.alicdn.com/754/2110193754/2102680677/1795467286_1494729308421.mp3?auth_key=c86f660796d9f57308b95b55c05d8891-1498273200-0-null" };
string url_jpg{ "http://img.xiami.net/images/album/img20/482520/4825201484482520.jpg" };
string url_reborn{ "http://link.hhtjim.com/xiami/" };
string url_mp32{ "http://link.hhtjim.com/xiami/1795747926" }; 
string url_mp33{ "http://link.hhtjim.com/xiami/1770409076.mp3" };

auto root{ file::current_path().root_path()/"media" };
auto thread_count{ max<short>(thread::hardware_concurrency() - 2,2) };
using protocal = tcp;

int main()
{
	pool<thread> worker{ thread_count };
	// display thread pool deepness, regardless of the main thread
	cout << "concurrency" << et << thread_count << en;
	ptree tree;
	factory<net::client> factory{ worker,root,tree };
	cout << boolalpha;
	//	creat media folder in the root directory and erase its previous sub-existence
	if(file::exists(root))
	{
		for(directory_iterator iter{root},end;iter!=end;++iter)
		{
			file::remove_all(iter->path());
		}
	}else
	{
		file::create_directory(root);
	}


	size_t init{ 1795747926 };
	randomizer<int> seed{ 0,5000 };
	for (auto i = 0; i < 2000000000; i += seed())
	{
		auto client = factory.create(i);
		cerr << i << en;
		client->crawl<mp3>(url_reborn + lexical_cast<string>(i) + ".mp3");
	}

	worker.finish();
	cout << "-----------------------------------------------" << en;
	cout << tree.size() << en;
	
	property::write_json((root/"validity.json").string(), tree);
	return 0;
}

