// Host.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Client.h"
#include "the/stack_trace.h"
#include "the/trace.h"


void host_func(void) {
    THE_STACK_TRACE;
    nClient = 8;
    print_stack();

    CClient cx;

    cx.func2(&print_stack);
}


int _tmain(int argc, _TCHAR* argv[]) {
    THE_STACK_TRACE;
    the::stack_trace::initialise();

    the::trace::get_instance().set_default_level(THE_TRCLVL_WARN);
    the::trace::get_instance().set_level(THE_TRCCHL_DEFAULT, THE_TRCLVL_WARN);

    THE_TRACE(THE_TRCCHL_DEFAULT, THE_TRCLVL_INFO,   "Info from HOST %d\n", __LINE__); // should not be printed
    THE_TRACE(THE_TRCCHL_DEFAULT, THE_TRCLVL_WARN,   "Warn from HOST %d\n", __LINE__); // should be printed
    THE_TRACE(THE_TRCCHL_DEFAULT, THE_TRCLVL_ERROR, "Error from HOST %d\n", __LINE__); // should be printed

    print_stack();

    CClient c;

    nClient = 7;

    c.func1();

    host_func();
    c.func3(&host_func);

    return 0;
}

