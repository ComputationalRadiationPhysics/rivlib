/*
 * the/stack_trace.cpp
 *
 * Copyright (c) 2012, TheLib Team (http://www.thelib.org/license)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of TheLib, TheLib Team, nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THELIB TEAM AS IS AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THELIB TEAM BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * stack_trace.cpp
 *
 * Copyright (C) 2006 - 2008 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */
#include "the/stack_trace.h"
#if !defined(THELIB_SYMBOL_IMPORT)
#include "the/not_supported_exception.h"
#include "the/text/char_utility.h"
#include "the/trace.h"
#include <cmath>


///*
// * __thelib_stacktrace_manager
// */
//std::shared_ptr<the::stack_trace> THELIB_SYMBOLEXPORT __thelib_stacktrace_manager;
//
//// TODO/IDEA: Replace this with TLS variable


/*
 * the::stack_trace::manager
 */
std::shared_ptr<the::stack_trace>
    /*&*/the::stack_trace::manager/*(__thelib_stacktrace_manager)*/;


/*
 * the::stack_trace::marker::marker
 */
the::stack_trace::marker::marker(const char *functionName, const char *file,
        const int line) : id(0) {
    // TODO/IDEA: TLS is NULL if thread is new. If so, create the manager and set it.
    // Otherwise, just use the existing manager now.
    // From what I have read so far, it might be a problem using "normal" allocators
    // for this operation, because this might use TLS, too. One should think about 
    // whether that could be a problem for us. On Windows, one might be able to 
    // circumvent the problem by using something like VirtualAlloc.
    if (the::stack_trace::manager != NULL) {
        this->id = the::stack_trace::manager->push(functionName, file, line);
    }
}


/*
 * the::stack_trace::marker::~marker
 */
the::stack_trace::marker::~marker(void) {
    if ((this->id != 0) && (the::stack_trace::manager != NULL)) {
        the::stack_trace::manager->pop(this->id);
        // TODO/IDEA: Check whether stack is empty now. If so, delete the manager and
        // reset the TLS slot to NULL.
    }
}


/*
 * the::stack_trace::marker::marker
 */
the::stack_trace::marker::marker(const the::stack_trace::marker& src) {
    throw the::not_supported_exception("Forbidden copy ctor",
        __FILE__, __LINE__);
}


/*
 * the::stack_trace::marker::operator=
 */
the::stack_trace::marker& the::stack_trace::marker::operator=(
        const the::stack_trace::marker& rhs) {
    throw the::not_supported_exception(
        "Forbidden assignment operator", __FILE__, __LINE__);
}


/*
 * the::stack_trace::get_stack_string
 */
void the::stack_trace::get_stack_string(char *str, unsigned int &strSize) {
    if (the::stack_trace::manager == NULL) {
        if (str == NULL) {
            strSize = 1; // including the terminating zero
        } else {
            if (strSize > 0) {
                *str = '\0';
                strSize = 1;
            }
        }
    } else {
        the::stack_trace::manager->get_stack_string_impl(str, strSize);
    }
}


/*
 * the::stack_trace::get_stack_string
 */
void the::stack_trace::get_stack_string(wchar_t *str, unsigned int &strSize) {
    if (the::stack_trace::manager == NULL) {
        if (str == NULL) {
            strSize = 1; // including the terminating zero
        } else {
            if (strSize > 0) {
                *str = L'\0';
                strSize = 1;
            }
        }
    } else {
        the::stack_trace::manager->get_stack_string_impl(str, strSize);
    }
}


/*
 * the::stack_trace::initialise
 */
bool the::stack_trace::initialise(
        std::shared_ptr<the::stack_trace> manager, bool force) {
    if (the::stack_trace::manager != NULL) {
        if (the::stack_trace::manager == manager) {
            return true;
        }
        if (!force) {
            return false;
        }
    }
    if (!manager) {
        the::stack_trace::manager
            = std::shared_ptr<stack_trace>(new the::stack_trace());
    } else {
        the::stack_trace::manager = manager;
    }
    return true;
}


/*
 * the::stack_trace::Manager
 */
std::shared_ptr<the::stack_trace> the::stack_trace::Manager(void) {
    return the::stack_trace::manager;
}


/*
 * the::stack_trace::~stack_trace
 */
the::stack_trace::~stack_trace(void) {
    while (this->stack != NULL) {
        stack_element *e = this->stack;
        this->stack = e->next;
        e->func = NULL; // DO NOT DELETE
        e->file = NULL; // DO NOT DELETE
        e->next = NULL;
        delete e;
    }
}


/*
 * the::stack_trace::stack_trace
 */
the::stack_trace::stack_trace(void) : stack(NULL) {
    // Intentionally empty
}


/*
 * the::stack_trace::get_stack_string_impl
 */
void the::stack_trace::get_stack_string_impl(char *str,
        unsigned int &strSize) {
    stack_element *stck = this->start_use_stack();
    stack_element *e = stck;
    unsigned int s = 1; // terminating zero
    unsigned int l1, l2, l3;
    char buf[1024];
    while (e != NULL) {
        s += static_cast<unsigned int>(::strlen(e->func));
        s += static_cast<unsigned int>(::strlen(e->file));
        s += static_cast<int>(ceilf(log10f(static_cast<float>(e->line + 1))));
        if (e->line == 0) s++;
        s += 5; // '%func [%file:%line]\n' => five additional characters
        e = e->next;
    }

    if (str == NULL) {
        strSize = s;
        this->stop_use_stack(stck);
        return;
    }

    if (strSize == 0) {
        this->stop_use_stack(stck);
        return;
    }

    if (s > strSize) {
        s = strSize - 1; // for the terminating zero
    }
    strSize = 0;

    e = stck;
    while (e != NULL) {

        l1 = static_cast<unsigned int>(::strlen(e->func));
        l2 = static_cast<unsigned int>(::strlen(e->file));
        l3 = static_cast<int>(ceilf(log10f(static_cast<float>(e->line + 1))));
        if (e->line == 0) l3++;

        if ((l1 + l2 + l3 + 5) > (s - 1)) break; // skip rest of stack

        ::memcpy(str, e->func, l1);
        str += l1;
        strSize += l1;
        s -= l1;

        ::memcpy(str, " [", 2);
        str += 2;
        strSize += 2;
        s -= 2;

        ::memcpy(str, e->file, l2);
        str += l2;
        strSize += l2;
        s -= l2;

        ::memcpy(str, ":", 1);
        str += 1;
        strSize += 1;
        s -= 1;

        l1 = e->line;
        for (int l = l3 - 1; l >= 0; l--) { // itoa (argl!)
            buf[l] = static_cast<char>('0' + (l1 % 10));
            l1 = l1 / 10;
        }
        ::memcpy(str, buf, l3);
        str += l3;
        strSize += l3;
        s -= l3;

        ::memcpy(str, "]\n", 2);
        str += 2;
        strSize += 2;
        s -= 2;

        e = e->next;
    }

    *str = '\0';
    strSize += 1;

    this->stop_use_stack(stck);
}


/*
 * the::stack_trace::get_stack_string_impl
 */
void the::stack_trace::get_stack_string_impl(wchar_t *str,
        unsigned int &strSize) {
    if (str != NULL) {
        unsigned int i;
        char *t = new char[strSize];    // not soo good, but should work as
                                        // we can assume 7-bit ASCII here
        this->get_stack_string_impl(t, strSize);

        for (i = 0; (i < strSize) && (t[i] != '\0'); i++) {
            str[i] = the::text::char_utility::to_unicode(t[i]);
        }
        if (i < strSize) str[i] = L'\0';

        delete[] t;

    } else {
        this->get_stack_string_impl((char*)NULL, strSize);
    }
}


/*
 * the::stack_trace::pop
 */
void the::stack_trace::pop(int id) {
    stack_element *e = this->stack;
    if (e == NULL) {
        THE_TRACE(THE_TRCCHL_THELIB_COMMON, THE_TRCLVL_ERROR,
            "Unable to Pop: Trace Stack is empty.\n");
        return;
    }

    this->stack = e->next;
    e->next = NULL;

    if (id != static_cast<int>((size_t)e)) {
        THE_TRACE(THE_TRCCHL_THELIB_COMMON, THE_TRCLVL_WARN,
            "stack_trace::pop: Stack seems corrupted.\n");
    }

    e->func = NULL; // DO NOT DELETE
    e->file = NULL; // DO NOT DELETE
    e->next = NULL;
    delete e;
}


/*
 * the::stack_trace::push
 */
int the::stack_trace::push(const char* func, const char* file,
         const int line) {
    stack_element *e = new stack_element;
    e->func = func;
    e->file = file;
    e->line = line;
    e->next = this->stack;
    this->stack = e;
    return static_cast<int>((size_t)e);
}


/*
 * the::stack_trace::start_use_stack
 */
the::stack_trace::stack_element* the::stack_trace::start_use_stack(void) {
    return this->stack;
}


/*
 * the::stack_trace::stop_use_stack
 */
void the::stack_trace::stop_use_stack(
        the::stack_trace::stack_element* stack) {
    // intentionally empty
}

#endif /* !defined(THELIB_SYMBOL_IMPORT) */
