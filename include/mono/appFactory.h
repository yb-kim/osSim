#pragma once
#include <appFactory.h>

class MonoAppFactory : public AppFactory {
public:
    MonoAppFactory();
    Application* createApp();

private:
    //
};
