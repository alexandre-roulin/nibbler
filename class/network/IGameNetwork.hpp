#ifndef NIBBLER_IGAMENETWORK_HPP
#define NIBBLER_IGAMENETWORK_HPP

#include <cstdint>
#include <nibbler.hpp>

class IGameNetwork {
public:
	virtual void addScore(uint16_t) = 0;

	virtual void deliverEvents() = 0;

	virtual void refreshMySnake(void) = 0;

	virtual void send_host_open_game(void) = 0;

	virtual void send_borderless(bool borderless) = 0;

	virtual void change_name(char const *name) = 0;

	virtual void change_sprite(eSprite snakeSprite) = 0;

	virtual void change_state_ready(void) = 0;

	virtual void change_map_size(unsigned int) = 0;

	virtual bool all_snake_is_dead() = 0;

	virtual const Snake *getSnakes() const = 0;

	virtual Snake const &getSnake(void) const = 0;

	virtual int16_t getId(void) const = 0;

	virtual void killSnake(uint16_t) = 0;

	virtual void connect(std::string dns, std::string port) = 0;

	virtual void write_socket(std::string message) = 0;

	virtual bool isOpenGame() const = 0;

	virtual bool isConnect() const = 0;

	virtual bool isSwitchingLibrary() const = 0;

	~IGameNetwork() {};

};

#endif //NIBBLER_IGAMENETWORK_HPP
