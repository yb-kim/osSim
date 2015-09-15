#pragma once
#include <appFactory.h>
#include <fused/type_a/monoApplication.h>

class TypeAMonoAppFactory : public AppFactory {
public:
    TypeAMonoAppFactory();
    Application* createApp();
    TypeAMonoApplication *createSyscallApp(
            int syscallIndex, int targetApplicationCoreIndex);

private:
    //
};
