#pragma once

#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/unordered_map.hpp>
#include <boost/thread/future.hpp>

using namespace boost::filesystem;
using namespace boost::asio::ip;

namespace asio=boost::asio;
namespace file=boost::filesystem;
namespace this_thread = boost::this_thread;

using boost::lockfree::spsc_queue;
using boost::lockfree::capacity;
using boost::barrier;
using boost::thread;
using boost::unique_future;
using boost::regex;
using boost::promise;

using error = boost::system::error_code;
