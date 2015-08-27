#include <fused/type_a/microAppFactory.h>
#include <fused/type_a/microApplication.h>

TypeAMicroAppFactory::TypeAMicroAppFactory() : AppFactory() {
    return;
}

Application* TypeAMicroAppFactory::createApp() {
    TypeAMicroApplication *app = new TypeAMicroApplication(selectAppType());
    //app->setOS(os);
    return app;
}
