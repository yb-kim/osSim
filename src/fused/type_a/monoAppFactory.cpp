#include <fused/type_a/monoAppFactory.h>

TypeAMonoAppFactory::TypeAMonoAppFactory() : AppFactory() {
    return;
}

TypeAMonoAppFactory::TypeAMonoAppFactory(int *appWeights, int nWeights) : 
    AppFactory(appWeights, nWeights)
{
    return;
}

Application* TypeAMonoAppFactory::createApp() {
    return new TypeAMonoApplication(-1, -1);
}

TypeAMonoApplication* TypeAMonoAppFactory::createSyscallApp(
        int syscallIndex, int targetApplicationCoreIndex) {
    int syscallProcessIndex = Application::getSyscallProcessIndex(syscallIndex);
    TypeAMonoApplication *app = new TypeAMonoApplication(
            syscallProcessIndex, targetApplicationCoreIndex);
    return app;
}
