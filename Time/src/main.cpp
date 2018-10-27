#include <chrono>
#include <iostream>
#include <unistd.h>
#include <ctime>

using namespace std::chrono_literals;

typedef struct      t_TimeEvent {
	std::chrono::milliseconds   _deltaUpdate;
}                   TimeEvent;

class               Time {
public:

	Time() :
	_lag(0),
	_currentTime(std::chrono::high_resolution_clock::now()),
	_loops(0) {}
	~Time() {}

	void							update(void)
	{
		this->_deltaTime = std::chrono::high_resolution_clock::now() - this->_currentTime;
		this->_currentTime = std::chrono::high_resolution_clock::now();
		this->_lag += std::chrono::duration_cast<std::chrono::nanoseconds>(this->_deltaTime);
		this->_time += std::chrono::duration_cast<std::chrono::milliseconds>(this->_deltaTime);
	}
	bool							haveLag(void) const
	{
		return (this->_lag >= Time::timestep);
	}
	void							updateLag(void)
	{
		this->_lag -= Time::timestep;
		this->_loops++;
		if (this->_loops > 5)
			this->_lag = 0ns;
	}

	static std::chrono::nanoseconds const				timestep;
private:
	std::chrono::nanoseconds							_lag;
	std::chrono::milliseconds							_time;
	std::chrono::high_resolution_clock::time_point		_currentTime;
	std::chrono::high_resolution_clock::time_point		_pastTime;
	std::chrono::duration<double>						_deltaTime;
	int													_loops;


};
std::chrono::nanoseconds const Time::timestep = 16ms;


// we use a fixed timestep of 1 / (60 fps) = 16 milliseconds
bool handle_events() {
	return false; // true if the user wants to quit the game
}

long update(void)
{
	long i;
	for (i = 0; i < 1000000; i+=2)
		i--;
	return (i);
}

void render() {
	// render stuff here
}

int main()
{
	bool quit_game = false;

	Time time;

	while(!quit_game)
	{
		time.update();
		quit_game = handle_events();

		// update game logic as lag permits
		while(time.haveLag())
		{
			update(); // update at a fixed rate each time
			time.updateLag();
		//	std::cout << "Update" << std::endl;
		}
		//std::cout << "Render" << std::endl;
		render();
	}
}
