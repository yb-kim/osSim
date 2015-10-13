#include <fused/type_a/microAppFactory.h>
#include <fused/type_a/microApplication.h>

TypeAMicroAppFactory::TypeAMicroAppFactory() : AppFactory() {
    return;
}

TypeAMicroAppFactory::TypeAMicroAppFactory(int *appWeights, int nWeights) :
    AppFactory(appWeights, nWeights)
{
    return;
}

Application* TypeAMicroAppFactory::createApp() {
    TypeAMicroApplication *app = new TypeAMicroApplication(selectAppType());
    app->setOS(os);
    return app;
}

/*
unsigned int TypeAMicroAppFactory::selectAppType() {
    int r = rand() % 810;
    cout << "rand: " << r << endl;
    if(0 <= r && r < 750) return 0;
    if(r < 760) return 1;
    if(r < 790) return 2;
    else return 3;
}
*/
