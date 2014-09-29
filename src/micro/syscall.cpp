#include <micro/syscall.h>

MicroSyscallSpec::MicroSyscallSpec(
        unsigned int _normalTicks,
        MicroOS::ServiceType *services,
        unsigned int nServices):
    SyscallSpec(),
    services(services),
    nServices(nServices)
{
    normalTicks = _normalTicks;
}
