#pragma once
class Shader {
public:
    Shader();
    ~Shader();

private:
    Shader(Shader const &shader);
    Shader &operator=(Shader const &shader);
};