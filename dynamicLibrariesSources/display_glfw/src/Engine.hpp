#pragma once

class ActModel;

class Engine {
public:

    Engine() = default;
    ~Engine() = default;

protected:


    Engine(Engine const &shader) = delete;
    Engine &operator=(Engine const &shader) = delete;
};