#include <core.h>

Core::Core():
    appRunning(NULL)
{
    //
}

Core::Core(unsigned int index):
    index(index), appRunning(NULL)
{
    //
}

Core::Core(unsigned int index, Environment *env):
    index(index), env(env), appRunning(NULL)
{
    //
}

void Core::loadApp(Application *app) {
    appRunning = app;
    app->setCoreIndex(index);
    app->setEnv(env);
}

void Core::run(unsigned int unitTick) {
    if(appRunning == NULL) return;
    appRunning->run(unitTick);
}
