#include "ServerTCP.hpp"


ServerTCP::ServerTCP(Univers &univers, unsigned int port)
		: number_clients_(0),
		  port_(port),
		  univers_(univers),
		  acceptor_(io_service_, tcp::endpoint(tcp::v4(), port)),
		  mapSize(MAP_DEFAULT) {
	start_accept();
	thread = boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_));
	thread.detach();
}

void ServerTCP::start_accept() {
	acceptor_.async_accept([this](std::error_code ec, tcp::socket socket) {

		if (!ec) {

			int16_t id = -1;

			for (int index = 0; index < MAX_SNAKE; ++index) {
				if (snake_array_[index].id == -1) {
					id = index;
					break;
				}
			}
			if (id == -1) {
				start_accept();
				return;
			}
			log_success("New id on server %d", id);
			snake_array_[id] = Snake::randomSnake(id);

			TCPConnection::pointer new_connection =
					TCPConnection::create(snake_array_[id],
										  acceptor_.get_io_service(), *this);
			new_connection->socket() = std::move(socket);
			number_clients_++;
			pointers.push_back(new_connection);
			new_connection->readSocketHeader();
			new_connection->writeSocket(ClientTCP::add_prefix(eHeader::ID, &id));
			writeToClientTCP(ClientTCP::add_prefix(eHeader::RESIZE_MAP, &mapSize));
			sendSnakeArray();
			start_accept();
		} else {
			std::cerr << "Error accept in server" << std::endl;
		}
	});
}

void ServerTCP::eraseSnake(Snake const &snake) {
	int16_t id = snake.id;
	assert(id >= 0 && id < MAX_SNAKE);
	snake_array_[id].reset();
	auto it = std::find_if(pointers.begin(), pointers.end(),
			[id](TCPConnection::pointer p) { return p->getId() == id; });
	if (it != pointers.end())
		pointers.erase(it);
	writeToClientTCP(ClientTCP::add_prefix(eHeader::REMOVE_SNAKE, &id));
}


void ServerTCP::sendSnakeArray() {
	std::for_each(snake_array_.begin(), snake_array_.end(),
				  [this](auto snake){
						if (snake.id != -1) {
//							log_warn("SendSnake > id : %d alive : %d", snake.id, snake.isAlive);
							writeToClientTCP(ClientTCP::add_prefix(eHeader::SNAKE, &snake));
						}
				  });
}

void ServerTCP::startGame() {
	assert(isReady());
	ClientTCP::StartInfo startInfo;
	std::for_each(snake_array_.begin(), snake_array_.end(),
				  [this](auto snake){ snake.isAlive = true; });
	startInfo.nu = ServerTCP::number_clients_;
	startInfo.time_duration = boost::posix_time::microsec_clock::universal_time();
	for (auto &snakeArray : snake_array_) {
		log_error("SnakeArrayInSERVERTCP [%d][%d]", snakeArray.id, snakeArray.isAlive);
	}
	writeToClientTCP(ClientTCP::add_prefix(eHeader::START_GAME, &startInfo));
}

void ServerTCP::writeToClientTCP(std::string message) {
	for (auto &pointer : pointers) {
		pointer->writeSocket(message);
	}
}

void ServerTCP::parseInput(eHeader header, void const *input, size_t len) {
//	std::cout << "ServerTCP::lock()" << header << std::endl;
	mutex.lock();
	switch (header) {
		case eHeader::SNAKE: {
			Snake snake_temp;
			std::memcpy(&snake_temp, input, sizeof(Snake));
			assert(snake_temp.id >= 0 && snake_temp.id < MAX_SNAKE);
			snake_array_[snake_temp.id] = snake_temp;
			break;
		}
		case eHeader::START_GAME: {
			mutex.unlock();
			return;
		}
		case eHeader::FOOD: {
			log_info("ServerTCP::FOOD");
			ClientTCP::FoodInfo foodInfo;
			std::memcpy(&foodInfo, input, len);
			foodInfoArray.push_back(foodInfo);
			mutex.unlock();
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
			snake_array_[inputInfo.id].direction = inputInfo.dir;
			snake_array_[inputInfo.id].isUpdate = true;
			updateInput();
			mutex.unlock();
//			std::cout << "ServerTCP::unlock()" << std::endl;
			return;
		}
		case eHeader::kPause : {
			log_info("ServerTCP::kPause");
			if (std::any_of(snake_array_.begin(), snake_array_.end(),
					[](auto snake){ return snake.isSwitchingLibrary; })) {
				mutex.unlock();
				return;
			}
			pause_ = !pause_;
			if (!pause_)
				updateInput();
//			log_warn("Pause is %s", pause_ ? "true" : "false");
			break;
		}
		case eHeader::kForcePause : {
			log_info("ServerTCP::kForcePause");

			int16_t id;
			std::memcpy(&id, input, len);
			snake_array_[id].isSwitchingLibrary = !snake_array_[id].isSwitchingLibrary;
			pause_ = std::any_of(snake_array_.begin(), snake_array_.end(), [](auto snake){
				return snake.isSwitchingLibrary;
			});
			writeToClientTCP(ClientTCP::add_prefix(eHeader::SNAKE, &(snake_array_.__elems_[id])));
//			log_warn("kPause is %s", pause_ ? "true" : "false");
			if (!pause_) {
				eAction pause = eAction::kPause;
				writeToClientTCP(ClientTCP::add_prefix(eHeader::kPause, &pause));
				updateInput();
			}
			mutex.unlock();
			return;
		}
		default: {
			break;
		}
	}
	writeToClientTCP(ClientTCP::add_prefix(header, const_cast<void *>(input)));
	mutex.unlock();
//	std::cout << "ServerTCP::unlock()" << std::endl;
}

void ServerTCP::updateInput() {
//	log_warn("Pause is %s", pause_ ? "true" : "false");
	if (pause_ || std::any_of(snake_array_.begin(), snake_array_.end(),
			[](auto snake){ return snake.isAlive && !snake.isUpdate;} )) return;
	sendSnakeArray();
	for (auto infoArray : foodInfoArray) {
		writeToClientTCP(ClientTCP::add_prefix(eHeader::FOOD, &infoArray));
	}
	foodInfoArray.clear();
	char data = '#';
	writeToClientTCP(ClientTCP::add_prefix(eHeader::POCK, &data));
	std::for_each(snake_array_.begin(), snake_array_.end(), [](auto snake){ snake.isUpdate = false;});
}

void ServerTCP::remove(TCPConnection::pointer remove) {
//	std::cout << "Romve a Snake + Connection" << std::endl;

	pointers.erase(std::remove_if(pointers.begin(), pointers.end(),
								  [remove](TCPConnection::pointer pointer) {
									  return remove == pointer;
								  }));
}

ServerTCP::~ServerTCP() {
	io_service_.stop();
	acceptor_.cancel();
	acceptor_.close();
	thread.interrupt();
	std::cout << "ServerTCP::close" << std::endl;

}

bool ServerTCP::isFull() const {
	return std::none_of(snake_array_.begin(), snake_array_.end(), [](auto snake){ return snake.id == -1; });
}

unsigned int ServerTCP::getPort_() const {
	return port_;
}

bool ServerTCP::isReady() const {
	return std::none_of(snake_array_.begin(), snake_array_.end(), [](auto snake){ return snake.id != -1 && !snake.isReady; });
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
		std::cout << "Disconnected" << std::endl;

		serverTCP_.eraseSnake(snake_);
		serverTCP_.remove(shared_from_this());
		serverTCP_.number_clients_--;

	}
}

void
TCPConnection::handle_read_data(eHeader header,
								boost::system::error_code const &error_code,
								size_t len) {
	//std::cout << "TCPConnection::handle_read_data [" << len << "]" << std::endl;
	checkError_(error_code);

	if (error_code.value() == 0 && len > 0) {
		serverTCP_.parseInput(header, buffer_data.data(), len);
		readSocketHeader();
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
		readSocketData(header);
	}
}


void TCPConnection::readSocketHeader() {
	//std::cout << "TCPConnection::read_socket_header" << std::endl;
	boost::asio::async_read(socket_,
							boost::asio::buffer(buffer_data,
												ClientTCP::size_header[static_cast<int>(eHeader::HEADER)]),
							boost::bind(&TCPConnection::handle_read_header,
										shared_from_this(),
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::readSocketData(eHeader header) {
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

void TCPConnection::writeSocket(std::string message) {
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

int16_t TCPConnection::getId() const {
	return snake_.id;
}

TCPConnection::~TCPConnection() {
	if (socket_.is_open())
	socket_.close();
}
