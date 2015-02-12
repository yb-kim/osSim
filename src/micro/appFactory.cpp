#include <micro/appFactory.h>
#include <micro/application.h>

MicroAppFactory::MicroAppFactory() : AppFactory() {
    nextNsCoreIndex = 0;
    //
}

Application* MicroAppFactory::createApp() {
    MicroApplication *app = new MicroApplication(selectAppType());
    app->setNsCoreIndex(getNsCoreIndex());
    app->setOS(os);
    return app;
}


int MicroAppFactory::getNsCoreIndex() {
    if(nextNsCoreIndex == MicroOS::nSet) {
        nextNsCoreIndex = 0;
    }
    return nextNsCoreIndex++;
}


void MicroAppFactory::init() {
    return;
}
