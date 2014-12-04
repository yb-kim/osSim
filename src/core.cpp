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

Core::Core(unsigned int index, Environment *env):
    index(index), env(env)
{
    //
}

void Core::loadApp(Application *app) {
    appRunning = app;
    app->setCoreIndex(index);
}
