#include <core.h>

Core::Core():
    appRunning(NULL)
{
    //
}

void Core::loadApp(Application *app) {
    appRunning = app;
}
