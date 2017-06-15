#pragma once

#include <iostream>
#include <algorithm>
#include <numeric>
#include <utility>
#include <valarray>
#include <string>
#include <cassert>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <fstream>
#include <cmath>
#include <functional>
#include <chrono>
#include <sstream>
#include <thread>
#include <limits>
#include <memory>
#include <future>
#include <random>
#include <regex>
#include <queue>
#include <iterator>
#include <atomic>
#include <ratio>
#include <streambuf>

namespace chrono = std::chrono;
namespace place = std::placeholders;

using std::string;
using std::vector;
using std::set;
using std::map;
using std::unordered_map;
using std::istringstream;
using std::istream;
using std::ostream;
using std::istream_iterator;
using std::ostream_iterator;
using std::istreambuf_iterator;
using std::ostreambuf_iterator;
using std::conditional;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::ratio;
using std::numeric_limits;
using std::valarray;
using std::function;
using std::pair;
using std::tuple;
using std::shared_ptr;
using std::unique_ptr;
using std::stringstream;
using std::default_random_engine;

using std::accumulate;
using std::transform;
using std::bind;
using std::getline;
using std::ws;
using std::make_shared;
using std::make_unique;		// unsuppoted by old version compiler

using std::cout;
using std::cin;
using std::cerr;

enum symbol:char{
    el='\n',et='\t',er='\r',sp=' ',dot=','
};

inline
string operator""_str (const char* c){
    return string{c};
}

inline
ostream& crlf(ostream& os){
    return  os<<'\r'<<'\n';
}

template <typename T,typename Di=typename conditional<numeric_limits<T>().is_integer,
        uniform_int_distribution<>,uniform_real_distribution<>>::type>
class Random{
public:
    using value_type =T;
    using distribution_type=Di;
    Random(value_type low,value_type high){
        rd=bind(distribution_type{low,high},default_random_engine{});
    }
    value_type operator()(){
        return rd();
    }
private:
    function<value_type()> rd;
};

inline
ostream& operator<<(ostream& os,const symbol e){
    return os<<char{e};
}


template <typename T> 
ostream& operator<<(ostream& os,const valarray<T>& va){
    auto last=va.size();
    for(const auto iter:va){
        os<<iter;
        if(--last>0) os<<' ';
    }
    return os;
}
template<typename T,typename U>
ostream& operator<<(ostream& os,pair<T,U> pr){
    return os<<pr.first<<' '<<pr.second<<el;
}
template <typename T>
ostream& operator<<(ostream& os,vector<T> vec){
    for(const auto& v:vec ){
        os<<v<<et;
    }
    return os;
}

inline
double sigmoid(double t){
    return 1/(1+exp(-t));
}

inline
double derived_sigmoid(double t){
    auto tmp=sigmoid(t);
    return tmp*(1-tmp);
}

inline
int hardlim(double d,double threshold=0.5){
    return d>threshold?1:0;
}


