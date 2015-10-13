#include <mono/appFactory.h>
#include <mono/application.h>
#include <iostream>

using namespace std;

MonoAppFactory::MonoAppFactory() : AppFactory() {
    //
}

MonoAppFactory::MonoAppFactory(int *appWeights, int nWeights) :
    AppFactory(appWeights, nWeights) 
{
        //
}

Application* MonoAppFactory::createApp() {
    return new MonoApplication(selectAppType());
}
