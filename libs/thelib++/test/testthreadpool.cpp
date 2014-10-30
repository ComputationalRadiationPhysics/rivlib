/*
 * testthreadpool.cpp
 *
 * Copyright (c) 2013, TheLib Team (http://www.thelib.org/license)
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
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OFll
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "stdafx.h"
#include "testhelper.h"

#include <atomic>
#include <iostream>

#include "the/assert.h"
#include "the/stack_trace.h"
#include "the/trace.h"

#include "the/system/threading/auto_lock.h"
#include "the/system/threading/critical_section.h"
#include "the/system/threading/debug.h"
#include "the/system/threading/runnable.h"
#include "the/system/threading/thread_pool.h"

using namespace the::system::threading;


////////////////////////////////////////////////////////////////////////////////
#pragma region

std::atomic<int> cntRunnablesExecuted;

class Runnable : public runnable {
public:
    inline Runnable(void) {
        THE_STACK_TRACE; 
    }
    virtual ~Runnable(void);
    virtual int run(void);
};


/*
 * Runnable::~Runnable
 */
Runnable::~Runnable(void) { 
    THE_STACK_TRACE;
}


/*
 * Runnable::run
 */
int Runnable::run(void) {
    THE_STACK_TRACE;
    std::cout << "Runnable " << ::cntRunnablesExecuted 
        << " is being executed by thread [" 
        << the::system::threading::thread::get_current_id()
        << "]" << std::endl;
    return ++::cntRunnablesExecuted;
}

/*
 * ::TestRunnable
 */
void TestRunnable(void) {
    Runnable r[2];
    thread_pool tp;

    ::cntRunnablesExecuted = 0;
    for (size_t i = 0; i < sizeof(r) / sizeof(*r); ++i) {
        thread_pool::work_item_type wi = tp.create_work_item(r + i);
        tp.submit(wi);
        if (i == 0) {
            wi->wait();
        }
    }

    tp.wait();

    AssertEqual("All work items processed", int(::cntRunnablesExecuted), int(sizeof(r) / sizeof(*r)));
}

#pragma endregion
////////////////////////////////////////////////////////////////////////////////


/*
 * ::TestThreadPool
 */
void TestThreadPool(void) {
    SET_THE_THREADING_TRACE_ALL();
    ::TestRunnable();
}