#if defined(__x86_64__)
    #if defined(__linux__)
        #include "arch/linux/x86_64/x86_64.h"
    /*
    #elif defined(__APPLE__)
        #include "arch/maccos/x86_64/x86_64.h"
    #elif defined(_WIN32)
        #include "arch/windows/x86_64/x86_64.h"
    */
    #endif
#endif
