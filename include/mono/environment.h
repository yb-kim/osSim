#pragma once

#include <environment.h>
#include <mono/bus.h>

class MonoEnvironment : public Environment {
public:
    MonoEnvironment(EnvConfig *cfg, OS *os);
    Bus* getBus() { return bus; }
    OS* getOS() { return os; }
private:
    Bus *bus;
    OS* os;
};
