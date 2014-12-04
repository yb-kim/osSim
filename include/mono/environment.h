#pragma once

#include <environment.h>
#include <mono/bus.h>

class MonoEnvironment : public Environment {
public:
    MonoEnvironment(EnvConfig *cfg, OS *os);
private:
    Bus *bus;
    OS* os;
};
