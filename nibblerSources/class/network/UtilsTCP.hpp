#pragma once

#include <boost/asio/ip/tcp.hpp>

using boost::asio::ip::tcp;

class UtilsTCP {
public:
	UtilsTCP() = default;
	~UtilsTCP() = default;
	UtilsTCP &operator=(const EntitiesManager &) = delete;
	UtilsTCP(const EntitiesManager &) = delete;
};


