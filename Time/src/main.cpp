#include <chrono>
#include <iostream>
#include <unistd.h>
#include <ctime>

typedef struct      t_TimeEvent {
    std::chrono::milliseconds   _deltaUpdate;
}                   TimeEvent;

class               Time {
public:

private:
    std::chrono::nanoseconds                            _lag;
    std::chrono::milliseconds                           _time;
    std::chrono::high_resolution_clock::time_point      _currentTime;
    std::chrono::high_resolution_clock::time_point      _pastTime;
}

using namespace std::chrono_literals;

// we use a fixed timestep of 1 / (60 fps) = 16 milliseconds
constexpr std::chrono::nanoseconds timestep(16ms);

bool handle_events() {
  // poll for events

  return false; // true if the user wants to quit the game
}

long update(void)
{
    long i;
    for (i = 0; i < 1000000000; i+=2)
        i--;
    return (i);
}

void render() {
  // render stuff here
}

int main()
{
    std::chrono::nanoseconds lag(0);
    std::chrono::high_resolution_clock::time_point time_start = std::chrono::high_resolution_clock::now();
    bool quit_game = false;
    int loops;

    while(!quit_game)
    {
        std::chrono::duration<double> delta_time = std::chrono::high_resolution_clock::now() - time_start;
        time_start = std::chrono::high_resolution_clock::now();
        lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

        quit_game = handle_events();

        // update game logic as lag permits
        loops = 0;
        while(lag >= timestep && loops < 5)
        {
            std::cout << lag.count() / timestep.count() << std::endl;
            lag -= timestep;
            update(); // update at a fixed rate each time
            loops++;
        }
        render();
    }
}
