#pragma once
#include "common/citeSTL.hpp"
// music source is xiami.com

auto query_max{ 2000000000 };

inline
string query_url(int id)
{
	stringstream ss;
	ss << "http://xiamirun.avosapps.com/run?song=http://www.xiami.com/song/" << id;
	return ss.str();
};

inline
string count_url(int id)
{
	stringstream ss;
	ss << "http://www.xiami.com/count/getplaycount?id=" << id << "&type=song";
	return ss.str();
}