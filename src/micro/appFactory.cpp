#include <micro/appFactory.h>
#include <micro/application.h>

MicroAppFactory::MicroAppFactory() : AppFactory() {
    //
}

Application* MicroAppFactory::createApp() {
    return new MicroApplication(selectAppType());
}
