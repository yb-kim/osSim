#pragma once

#include <application.h>

class Environment;
class Application;

class Core {
public:
    Core();
    Core(unsigned int index);
    Core(unsigned int index, Environment *env);
    void loadApp(Application *app);
    void run(unsigned int unitTick);
    Application* getAppRunning() { return appRunning; }
    unsigned int getIndex() { return index; }
    Environment* getEnv() { return env; }
private:
    Application *appRunning;
    Environment *env;
    unsigned int index;
};
