// server-crawl.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include "common/citeSTL.hpp"
#include "common/citeBoost.hpp"
#include "common/utility/pool.hpp"
#include "common/utility/material.hpp"
#include "network/client.hpp"

// global configuration variable facilitating adjustment
string url_xiami{ "http://www.xiami.com/song/" };
string url_playcount{ "http://www.xiami.com/count/getplaycount?type=song&id=" };
string url_mp3{ "http://link.hhtjim.com/xiami/" };
auto root{ file::current_path().root_path()/"media" };
auto dir_mp3{ root / "mp3" };
auto dir_json{ root / "json" };
auto thread_count{ max<short>(thread::hardware_concurrency() - 2,2) };
using protocal = tcp;

int main()
{
	pool<thread> worker{ thread_count };
	// display thread pool deepness, regardless of the main thread
	cout << "concurrency" << et << thread_count << en;
	ptree tree;
	factory<net::client> factory{ worker,root,tree };
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
	randomizer<int> offset{ 0,500 };
	vector<unique_future<bool>> cluster(10);
	transform(cluster.begin(),cluster.end(),cluster.begin(),[&](unique_future<bool>& result)
	{
		packaged_task<bool()> task= [&]()
		{
			auto count{ 0 };
			while (count++<1000)
			{
				auto index = randomizer<int>{ 0,200000000 }() + offset();
				auto downloader = factory.create(index);
				downloader->crawl<mp3>(url_mp3 + lexical_cast<string>(index) + ".mp3");
			}
			return true;
		};
		result = task.get_future();
	});
	for_each(cluster.begin(), cluster.end(), bind(&unique_future<bool>::get));
	for(directory_iterator iter= directory_iterator{dir_mp3},end;iter!=end;++iter)
	{
		auto index{ iter->path().filename().replace_extension("").string() };
		auto client = factory.create(lexical_cast<size_t>(index));
		client->crawl<html>(url_xiami+index);
		client->crawl<json>(url_playcount + index);
		//client->crawl<mp3>(url_mp3 + index);
		this_thread::sleep_for(chrono::milliseconds(1200));
	}
	worker.finish();
	property::write_json((root / "json" / "all.json").string(), tree);
	return 0;
}

