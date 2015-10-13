#pragma once
#include <appFactory.h>

class MonoAppFactory : public AppFactory {
public:
    MonoAppFactory();
    MonoAppFactory(int *appWeights, int nWeights);
    Application* createApp();

private:
    //
};
