#include <micro/appFactory.h>
#include <micro/application.h>

MicroAppFactory::MicroAppFactory() : AppFactory() {
    //
}

Application* MicroAppFactory::createApp() {
    MicroApplication *app = new MicroApplication(selectAppType());
    app->setOS(os);
    return app;
}
