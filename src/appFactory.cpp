#include <appFactory.h>

AppFactory::AppFactory():
    appTypePointer(0)
{
    //
}

unsigned int AppFactory::selectAppType() {
    if(appTypePointer >= Application::getNSpecs()) appTypePointer = 0;
    return appTypePointer++;
}
