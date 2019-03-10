#pragma once

class ActModel;

class Engine {
public:

    Engine() = default;
    ~Engine() = default;
	Engine(Engine const &shader) = delete;
	Engine &operator=(Engine const &shader) = delete;

protected:
};