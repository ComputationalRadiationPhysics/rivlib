// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CLIENT_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CLIENT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CLIENT_EXPORTS
#define CLIENT_API __declspec(dllexport)
#else
#define CLIENT_API __declspec(dllimport)
#endif

#include "the/stack_trace.h"

// This class is exported from the Client.dll
class CLIENT_API CClient {
public:
    CClient(void);
    // TODO: add your methods here.
    void func1(void);

    template <class func> void func2(func f) {
        THE_STACK_TRACE;
        f();
    }

    void func3(void (*f)(void));

};

extern CLIENT_API int nClient;

CLIENT_API int print_stack(void);
