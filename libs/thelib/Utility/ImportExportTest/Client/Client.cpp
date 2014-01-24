// Client.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Client.h"
#include "the/stack_trace.h"
#include "cstdio"
#include "the/trace.h"


// This is an example of an exported variable
CLIENT_API int nClient = 0;

// This is an example of an exported function.
CLIENT_API int print_stack(void) {
    THE_STACK_TRACE;

    char *str = nullptr;
    unsigned int len;

    the::stack_trace::get_stack_string(str, len);
    str = new char[len + 1];
    ::ZeroMemory(str, len + 1);
    the::stack_trace::get_stack_string(str, len);

    printf("Stack (%d):\n%s\n", nClient, str);

    delete[] str;

    return 42;
}

// This is the constructor of a class that has been exported.
// see Client.h for the class definition
CClient::CClient() {
    THE_STACK_TRACE;

    nClient = 1;

    print_stack();

    return;
}


/*
 * 
 */
void CClient::func1(void) {
    THE_STACK_TRACE;
    print_stack();
}


/*
 * 
 */
void CClient::func3(void (*f)(void)) {
    THE_STACK_TRACE;
    f();

    THE_TRACE(THE_TRCCHL_DEFAULT, THE_TRCLVL_INFO,   "Info from Client %d\n", __LINE__); // should not be printed
    THE_TRACE(THE_TRCCHL_DEFAULT, THE_TRCLVL_WARN,   "Warn from Client %d\n", __LINE__); // should be printed
    THE_TRACE(THE_TRCCHL_DEFAULT, THE_TRCLVL_ERROR, "Error from Client %d\n", __LINE__); // should be printed

}
