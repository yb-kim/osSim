#include <micro/syscall.h>

MicroSyscallSpec::MicroSyscallSpec(
        unsigned int _normalTicks,
        MicroOS::Services *services,
        unsigned int nServices):
    SyscallSpec(),
    services(services),
    nServices(nServices)
{
    normalTicks = _normalTicks;
}
