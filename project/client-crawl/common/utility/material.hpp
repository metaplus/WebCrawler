#pragma once
#include "common/citeSTL.hpp"
#include "common/citeBoost.hpp"

struct element
{
	virtual void process(streambuf&, tcp::socket&, size_t) = 0;
};


struct json :element
{
	using arg_type = ptree;
	ptree& tree;
	string str;
	json(size_t id, ptree& tr, const file::path& catalog) :tree{ tr }
	{
		str = lexical_cast<string>(id);
	}
	void process(streambuf& response, tcp::socket& socket, size_t length) override
	{
		auto json_stream = make_shared<iostream>(&response);
		auto node = make_shared<ptree>();
		streambuf rest;
		asio::read(socket, rest, transfer_exactly(length));
		*json_stream << &rest;
		static io_service::strand strand{ socket.get_io_service() };
		property::read_json(*json_stream, *node);
		if (!node->get_optional<char>("error"))
		{
			strand.post(bind(&ptree::add_child, &tree, str, *node));
		}
	}
};


struct mp3 :element
{
	file::path catalog;
	ptree& tree;
	size_t id;
	mp3(size_t id, ptree& tr, const file::path& dir) :catalog{ dir }, tree{ tr }, id{ id }
	{
		static auto suffix = "mp3";
		catalog.append(suffix);
		static once_flag flag;
		call_once(flag,bind(file::create_directory,catalog));
		catalog.append(lexical_cast<string>(id)).replace_extension(suffix);
	}
	void process(streambuf& response, tcp::socket& socket, size_t length)
	{
		if (length > 1024 * 1024 * 20 || length < 1024)
			return;
		tree.push_back(ptree::value_type{ lexical_cast<string>(id),ptree{} });
		auto ofptr = make_shared<file::ofstream>(catalog, ios::binary | ios::trunc);
		*ofptr << &response;
		async_read(socket, response, transfer_exactly(length),
			[&,ofptr](const error& ec, size_t transferred)
		{
			*ofptr << &response;
		});
	}
};