#include <appFactory.h>

AppFactory::AppFactory():
    appTypePointer(0)
{
    //
}


Application* AppFactory::createApp() {
    return new Application(selectAppType());
}

unsigned int AppFactory::selectAppType() {
    if(appTypePointer >= Application::getNSpecs()) appTypePointer = 0;
    return appTypePointer++;
}
