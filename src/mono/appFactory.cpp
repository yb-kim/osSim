#include <mono/appFactory.h>
#include <mono/application.h>
#include <iostream>

using namespace std;

MonoAppFactory::MonoAppFactory() : AppFactory() {
    //
}

Application* MonoAppFactory::createApp() {
    return new MonoApplication(selectAppType());
}
