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
string url_xiami{ "http://www.xiami.com/song/" };
string url_xiamia{ "http://www.xiami.com/song/1768946249" };
string url_playcount{ "http://www.xiami.com/count/getplaycount?type=song&id=" };
string dir_media{ "G:/media/mp3/" };
 

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
	//		file::remove_all(iter->path());
		}
	}else
	{
		file::create_directory(root);
	}

	/*
	size_t init{ 1770409076 };
	randomizer<int> seed{ 0,10000 };
	for (auto i = 1794000000; i < 3000409076; i += seed())
	{
		auto client = factory.create(i);
		cerr << i << en;
		client->crawl<mp3>(url_reborn + lexical_cast<string>(i) + ".mp3");
	}
	randomizer<int> seed2{ 0,2130409076 };
	while(true)
	{
		auto i = seed2();
		auto client = factory.create(i);
		cerr << i << en;
		client->crawl<mp3>(url_reborn + lexical_cast<string>(i) + ".mp3");
	}
*/
	auto count{ 0 };
	/*
	auto init{ 10000 };
	auto init2{ init };
	auto init3{ init };
	for(directory_iterator iter= directory_iterator{dir_media},end;iter!=end;++iter,++count)
	{
		if(init-->0)
		{
			continue;
		}
		static auto ccc = 0;
		if (ccc++ >= 1000) break;
		auto index{ iter->path().filename().replace_extension("").string() };
		cout << index <<et<<count<< en;
		auto client = factory.create(lexical_cast<size_t>(index));
		client->crawl<html>(url_xiami+index);
		this_thread::sleep_for(chrono::milliseconds(1200));
	}
	//	this_thread::sleep_for(chrono::seconds(2));
	*/
	count = 0;
	/*
	for (directory_iterator iter{ dir_media }, end; iter != end; ++iter)
	{
		if (init2-->0)
		{
			continue;
		}
		static auto ccc = 0;
		if (ccc++ >= 1000) break;
		auto index{ iter->path().filename().replace_extension("").string() };
		cout << index << et << count << en;
		//tree.put(ptree::path_type{ index }/"Comments", "xxxxx");
		auto client = factory.create(lexical_cast<size_t>(index));
		client->crawl<json>(url_playcount + index);
		this_thread::sleep_for(chrono::milliseconds	(800));
	}
	*/
	auto xxxx = 9000;
	auto name2{ (root / "json" / "all.json").string() };


	ptree all;


	for(auto x=0;x<=10000;x+=1000)
	{
		auto name1{ (root / "json" / (lexical_cast<string>(x) + ".json")).string() };
		property::read_json(name1, tree);
		cout << x << et << tree.size() << en;
		for (auto& y : tree)
		{
			all.push_back(y);
		}
	}
	cout << all.size() << en;
	worker.finish();
	cout << "-----------------------------------------------" << en;
	property::write_json(name2, all);
	cout << "count" << et << count << en;
	
	//property::write_json((root / (lexical_cast<string>(init3)+".json")).string(), tree);
	return 0;
}

