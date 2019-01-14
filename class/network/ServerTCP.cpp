#include "ServerTCP.hpp"
#include <logger.h>
#include <KINU/World.hpp>


ServerTCP::ServerTCP(Univers &univers, unsigned int port)
		: port_(port),
		univers_(univers),
		  nu_(0),
		  acceptor_(io_service_, tcp::endpoint(tcp::v4(), port)),
		  mapSize(MAP_DEFAULT) {
	start_accept();
	thread = boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_));
	thread.detach();
}

void ServerTCP::start_accept() {
	acceptor_.async_accept([this](std::error_code ec, tcp::socket socket) {

		if (!ec) {
			TCPConnection::pointer new_connection =
					TCPConnection::create(snake_array[nu_],
										  acceptor_.get_io_service(), *this);
			new_connection->socket() = std::move(socket);

			int16_t first_id = 0;

			for (int i = 0; i < MAX_SNAKE; i++) {
				if (snake_array[i].id == -1) {
					first_id = i;
					break;
				}
			}

			snake_array[first_id] = Snake::randomSnake(first_id);
			new_connection->read_socket_header();
			pointers.push_back(new_connection);
			refresh_data_snake_array(new_connection, first_id);
			refresh_data_map_size(new_connection);
			++nu_;
			start_accept();
		} else {
			std::cerr << "Error accept in server" << std::endl;
		}
	});
}

void ServerTCP::erase_snake(Snake const &snake) {
	nu_--;
	int16_t id = snake.id;
	snake_array[id].id = -1;
	async_write(ClientTCP::add_prefix(eHeader::REMOVE_SNAKE, &id));
}


void ServerTCP::refresh_data_snake_array(
		TCPConnection::pointer &connection,
		int16_t id) {
	connection->write_socket(ClientTCP::add_prefix(eHeader::ID, &id));
	connection->write_socket(ClientTCP::add_prefix(eHeader::SNAKE_ARRAY, snake_array));
	async_write(ClientTCP::add_prefix(eHeader::SNAKE_ARRAY, &snake_array));
}

void ServerTCP::refresh_data_map_size(TCPConnection::pointer &connection) {
	async_write(ClientTCP::add_prefix(eHeader::RESIZE_MAP, &mapSize));
}

void ServerTCP::start_game() {
	for (int index = 0; index < MAX_SNAKE; ++index) {
		if (!snake_array[index].isReady && snake_array[index].id != -1) {
			log_trace("Nop index %d with id : ", index, snake_array[index].id );
			return;
		}
	}
	ClientTCP::StartInfo startInfo;
	startInfo.nu = nu_;
	startInfo.time_duration = boost::posix_time::microsec_clock::universal_time();
	async_write(ClientTCP::add_prefix(eHeader::START_GAME, &startInfo));
}

void ServerTCP::async_write(std::string message) {
	for (auto &pointer : pointers) {
		pointer->write_socket(message);
	}
}

void ServerTCP::async_write(void const *input, size_t len) {
	for (auto &pointer : pointers) {
		pointer->write_socket(input, len);
	}
}

void ServerTCP::parse_input(eHeader header, void const *input, size_t len) {
//	std::cout << "ServerTCP::lock()" << header << std::endl;
	mutex.lock();
	switch (header) {
		case eHeader::SNAKE_ARRAY: {
			std::memcpy(snake_array, input, len);
			break;
		}
		case eHeader::SNAKE: {
			Snake snake_temp;
			std::memcpy(&snake_temp, input, sizeof(Snake));
			assert(snake_temp.id >= 0 && snake_temp.id < MAX_SNAKE);
			snake_array[snake_temp.id] = snake_temp;
			if (!snake_array[snake_temp.id].isAlive)
				log_warn("Snake is die %d", snake_array[snake_temp.id].id);
			break;
		}
		case eHeader::START_GAME: {
			mutex.unlock();
			return;
		}
		case eHeader::FOOD: {
//			log_info("ServerTCP::FOOD");
			ClientTCP::FoodInfo foodInfo;
			std::memcpy(&foodInfo, input, len);
			foodInfoArray.push_back(foodInfo);
			mutex.unlock();
//			std::cout << "ServerTCP::unlock()" << std::endl;
			return;
		}
		case eHeader::RESIZE_MAP: {
			log_info("ServerTCP::RESIZE_MAP");
			std::memcpy(&mapSize, input, len);
			break;
		}
		case eHeader::INPUT: {
			ClientTCP::InputInfo inputInfo;
			std::memcpy(&inputInfo, input, len);
			snake_array[inputInfo.id].direction = inputInfo.dir;
			snake_array[inputInfo.id].isUpdate = true;
			for (int index = 0; index < nu_; ++index) {
				std::cout << "Snake id :" << snake_array[index].id << "color :" << snake_array[index].sprite << " is alive : " << snake_array[index].isAlive << " update : " << snake_array[index].isUpdate <<std::endl;
				if (snake_array[index].isAlive &&
					!snake_array[index].isUpdate) {
					mutex.unlock();
//					std::cout << "ServerTCP::unlock()" << std::endl;
					return;
				}
			}
			async_write(ClientTCP::add_prefix(eHeader::SNAKE_ARRAY, snake_array));
			for (auto infoArray : foodInfoArray) {
				async_write(ClientTCP::add_prefix(eHeader::FOOD, &infoArray));
			}
			foodInfoArray.clear();
			char data = '#';
			async_write(ClientTCP::add_prefix(eHeader::POCK, &data));
			for (int index = 0; index < nu_; ++index) {
				snake_array[index].isUpdate = false;
			}
			mutex.unlock();
//			std::cout << "ServerTCP::unlock()" << std::endl;
			return;
		}
		default: {
			break;
		}
	}
	async_write(ClientTCP::add_prefix(header, const_cast<void *>(input)));
	mutex.unlock();
//	std::cout << "ServerTCP::unlock()" << std::endl;
}

void ServerTCP::remove(TCPConnection::pointer remove) {
//	std::cout << "Romve a Snake + Connection" << std::endl;

	pointers.erase(std::remove_if(pointers.begin(), pointers.end(),
								  [remove](TCPConnection::pointer pointer) {
									  return remove == pointer;
								  }));
}

void ServerTCP::close_acceptor() {
	acceptor_.cancel();
	acceptor_.close();
}

ServerTCP::~ServerTCP() {
	std::for_each(pointers.begin(), pointers.end(), [](TCPConnection::pointer connection){
		connection->close();
	});
	io_service_.stop();
	close_acceptor();
	thread.interrupt();
	std::cout << "ServerTCP::close" << std::endl;

}

bool ServerTCP::isFull() const {
	for (int index = 0; index < MAX_SNAKE; ++index) {
		if (snake_array[index].id == -1)
			return false;
	}
	return true;
}

unsigned int ServerTCP::getPort_() const {
	return port_;
}

/** ---------------------- TCPConnection ---------------------- **/

TCPConnection::TCPConnection(
		Snake const &snake,
		boost::asio::io_service &io_service,
		ServerTCP &serverTCP)
		: snake_(snake),
		  socket_(io_service),
		  serverTCP_(serverTCP) {
}

void TCPConnection::checkError_(boost::system::error_code const &error_code) {

	if ((boost::asio::error::eof == error_code) ||
		(boost::asio::error::connection_reset == error_code)) {
		//Need to reset the Snake;
//		std::cout << "Disconnected" << std::endl;
		serverTCP_.erase_snake(snake_);
		serverTCP_.remove(shared_from_this());
	}
}

void
TCPConnection::handle_read_data(eHeader header,
								boost::system::error_code const &error_code,
								size_t len) {
	//std::cout << "TCPConnection::handle_read_data [" << len << "]" << std::endl;
	checkError_(error_code);

	if (error_code.value() == 0 && len > 0) {
		serverTCP_.parse_input(header, buffer_data.data(), len);
		read_socket_header();
	}
}

void TCPConnection::handle_write(const boost::system::error_code &error_code,
								 size_t len) {
	checkError_(error_code);
}

void
TCPConnection::handle_read_header(const boost::system::error_code &error_code,
								  size_t len) {
	checkError_(error_code);
	if (error_code.value() == 0) {
		assert(len == sizeof(eHeader));
		eHeader header;
		std::memcpy(&header, buffer_data.data(), sizeof(eHeader));
		read_socket_data(header);
	}
}


void TCPConnection::read_socket_header() {
	//std::cout << "TCPConnection::read_socket_header" << std::endl;
	boost::asio::async_read(socket_,
							boost::asio::buffer(buffer_data,
												ClientTCP::size_header[static_cast<int>(eHeader::HEADER)]),
							boost::bind(&TCPConnection::handle_read_header,
										shared_from_this(),
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::read_socket_data(eHeader header) {
	//std::cout << "TCPConnection::read_socket_data" << std::endl;
	boost::asio::async_read(socket_, boost::asio::buffer(buffer_data,
														 ClientTCP::size_header[static_cast<int>(header)]),
							boost::bind(&TCPConnection::handle_read_data,
										shared_from_this(),
										header,
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::write_socket(void const *data, size_t len) {

	boost::asio::async_write(socket_, boost::asio::buffer(data, len),
							 boost::bind(&TCPConnection::handle_write,
										 shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::write_socket(std::string message) {
	boost::asio::async_write(socket_, boost::asio::buffer(message),
							 boost::bind(&TCPConnection::handle_write,
										 shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}


TCPConnection::pointer
TCPConnection::create(Snake const &snake, boost::asio::io_service &io_service,
					  ServerTCP &serverTCP) {
	return pointer(new TCPConnection(snake, io_service, serverTCP));
}


tcp::socket &TCPConnection::socket() {
	return socket_;
}

void TCPConnection::close() {
	std::cout << "Close socket" << std::endl;
	socket_.close();
}
