#pragma once

#include <application.h>

class Core {
public:
    Core();
    Core(unsigned int index);
    void loadApp(Application *app);
    void run(unsigned int unitTick) { appRunning->run(unitTick); }
    Application* getAppRunning() { return appRunning; }
    unsigned int getIndex() { return index; }
private:
    Application *appRunning;
    unsigned int index;
};
