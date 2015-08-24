#include <fused/type_a/monoAppFactory.h>
#include <fused/type_a/monoApplication.h>

TypeAMonoAppFactory::TypeAMonoAppFactory() : AppFactory() {
    return;
}

Application* TypeAMonoAppFactory::createApp() {
    return new TypeAMonoApplication(-1);
}
