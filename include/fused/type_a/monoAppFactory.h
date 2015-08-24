#pragma once
#include <appFactory.h>

class TypeAMonoAppFactory : public AppFactory {
public:
    TypeAMonoAppFactory();
    Application* createApp();

private:
    //
};
