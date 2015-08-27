#pragma once
#include <appFactory.h>

class TypeAMicroAppFactory : public AppFactory {
public:
    TypeAMicroAppFactory();
    Application* createApp();

private:
    //
};
