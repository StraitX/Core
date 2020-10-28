#ifndef STRAITX_ENGINE_HPP
#define STRAITX_ENGINE_HPP


namespace StraitX{

class Application;

class Engine{
private:
    Application *mApplication;
public:
    Engine();
    ~Engine();

    int Run();
private:
    int Initialize();
    int Finalize();
    void MainLoop();
};

}; // namespace StraitX::


#endif // STRAITX_ENGINE_HPP