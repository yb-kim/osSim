#pragma once

#include <syscall.h>

class MonoSyscall : public Syscall {
public:
    //
private:
    void initSyscalls();
};
