#pragma once


#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/array.hpp>
#include "ServerTCP.hpp"

using boost::asio::ip::tcp;

class ClientTCP {
public:
	ClientTCP(boost::asio::io_service &io, std::string &hostname);


	void read_socket();


private:
	boost::asio::deadline_timer timer;
	tcp::socket socket;
	tcp::resolver resolver;
	tcp::resolver::query query;
};


