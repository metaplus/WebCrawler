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


namespace asio=boost::asio;
namespace file=boost::filesystem;
namespace this_thread = boost::this_thread;
namespace lockfree = boost::lockfree;

using boost::barrier;
using boost::thread;
using boost::thread_group;
using boost::unique_future;
using boost::shared_future;
using boost::regex;
using boost::promise;
using boost::lockfree::spsc_queue;
using boost::lockfree::capacity;
using boost::asio::ip::address;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using boost::asio::io_service;

using boost::asio::async_read_until;
using boost::asio::async_read;
using boost::asio::async_write;
using boost::asio::read;
using boost::asio::write;

using error = boost::system::error_code;
