#pragma once
#include <appFactory.h>

class MicroAppFactory : public AppFactory {
public:
    MicroAppFactory();
    Application* createApp();

private:
    //
};
