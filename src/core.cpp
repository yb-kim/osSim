#include <core.h>

Core::Core():
    appRunning(NULL)
{
    //
}

Core::Core(unsigned int index):
    index(index)
{
    //
}

void Core::loadApp(Application *app) {
    appRunning = app;
    app->setCoreIndex(index);
}
