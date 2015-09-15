class ApplicationSpec {
public:
    enum ApplicationType { 
        NORMAL,
        SYSCALL_PROCESS
    };

    ApplicationSpec(ApplicationType _type, int *_syscallIndex, 
            unsigned int _nSyscalls):
        type(_type),
        syscallIndex(_syscallIndex),
        nSyscalls(_nSyscalls)
    {
        //
    }

    unsigned int getNSyscalls() { return nSyscalls; }
    int* getSyscallIndex() { return syscallIndex; }
    
private:
    ApplicationType type;
    int *syscallIndex;
    unsigned int nSyscalls;
};

