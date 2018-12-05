#pragma  once

class Model {
public:
    
    Model();
    ~Model();

private:
    
    void	clean_();

    static bool				debug_;

    Model(Model const &Model) = delete;
    Model &operator=(Model const &Model) = delete;
};