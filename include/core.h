#pragma once

#include <application.h>

class Core {
public:
    Core();
    void loadApp(Application *app);
    void run(unsigned int unitTick) { appRunning->run(unitTick); }
    Application* getAppRunning() { return appRunning; }
private:
    Application *appRunning;
};
