#include <micro/syscall.h>

MicroSyscallSpec::MicroSyscallSpec(
        unsigned int _normalTicks,
        MicroOS::ServiceType *services,
        unsigned int nServices,
        std::string name,
        unsigned int _index):
    SyscallSpec(),
    services(services),
    nServices(nServices),
    name(name)
{
    normalTicks = _normalTicks;
    index = _index;
}
