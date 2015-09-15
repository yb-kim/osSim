#pragma once

#include <mono/application.h>
#include <fused/type_a/mono.h>

class TypeAMonoApplication : public MonoApplication {
public:
    TypeAMonoApplication(unsigned int appSpecIndex, int targetApplicationCoreIndex);
    TypeAMonoApplication(unsigned int appSpecIndex,
            int syscallIndex, int targetApplicationCoreIndex);
    void setSyscallIndex(int i) { syscallIndex = i; }
    int getSyscallIndex() { return syscallIndex; }
    int getTargetApplicationCoreIndex() { return targetApplicationCoreIndex; }

protected:
    void processFinish();
    int syscallIndex;
    int targetApplicationCoreIndex;
};
