#pragma once

#include <micro/os.h>
#include <micro/application.h>

class TypeAMicroApplication : public MicroApplication {
public:
    TypeAMicroApplication(unsigned int appSpecIndex);

protected:
    virtual void doIpc();
};
