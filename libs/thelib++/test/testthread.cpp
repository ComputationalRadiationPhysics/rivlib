/*
 * testthread.cpp
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

#include "the/assert.h"
#include "the/trace.h"

#include "the/system/threading/auto_lock.h"
#include "the/system/threading/critical_section.h"
#include "the/system/threading/debug.h"
#include "the/system/threading/runnable.h"
#include "the/system/threading/runnable_thread.h"
#include "the/system/threading/thread.h"

using namespace the::system::threading;


std::atomic<int> cntStarted;
std::atomic<int> cntStarting;
std::atomic<int> cntTerminated;
std::atomic<int> cntTerminating;


////////////////////////////////////////////////////////////////////////////////
#pragma region

class StatusTestRunnable : public runnable {

public:
    inline StatusTestRunnable(void) : critSect(NULL) { }
    virtual ~StatusTestRunnable(void);
    virtual void on_thread_started(const thread::id& id) throw();
    virtual void on_thread_starting(void *userData) throw();
    virtual void on_thread_terminated(const thread::exit_reason reason) throw();
    virtual thread::termination_behaviour on_thread_terminating(void) throw();
    virtual int run(void);
private:
    critical_section *critSect;
};


/*
 * StatusTestRunnable::~StatusTestRunnable
 */
StatusTestRunnable::~StatusTestRunnable(void) {
}


/*
 * StatusTestRunnable::on_thread_started
 */
void StatusTestRunnable::on_thread_started(const thread::id& id) throw() {
    std::cout << "StatusTestRunnable started with ID " << id << std::endl;
    ++::cntStarted;
}


/*
 * StatusTestRunnable::on_thread_starting
 */
void StatusTestRunnable::on_thread_starting(void *userData) throw() {
    std::cout << "StatusTestRunnable is starting... " << std::endl;
    ++::cntStarting;
    THE_ASSERT(userData != NULL);
    this->critSect = static_cast<critical_section *>(userData);
}


/*
 * StatusTestRunnable::on_thread_terminated
 */
void StatusTestRunnable::on_thread_terminated(const thread::exit_reason reason) throw() {
    std::cout << "StatusTestRunnable terminated with reason " << reason << std::endl;
    ++::cntTerminated;
}


/*
 * StatusTestRunnable::on_thread_terminating
 */
thread::termination_behaviour StatusTestRunnable::on_thread_terminating(void) throw() {
    std::cout << "StatusTestRunnable is requested to terminate." << std::endl;
    ++::cntTerminating;
    return thread::termination_behaviour::forceful;
}

/*
 * StatusTestRunnable::run
 */
int StatusTestRunnable::run(void) {
    THE_ASSERT(this->critSect != NULL);
    this->critSect->lock();
    this->critSect->unlock();
    return 0;
}

#pragma endregion
////////////////////////////////////////////////////////////////////////////////


/*
 * ::TestStatus
 */
void TestStatus(void) {
    StatusTestRunnable r;
    thread t(&r);
    critical_section critSect;

    ::cntStarted = 0;
    ::cntStarting = 0;
    ::cntTerminated = 0;
    ::cntTerminated = 0;
    critSect.lock();

    AssertFalse("Thread not running", t.is_running());
    AssertNoException("Starting thread", t.start(&critSect));
    AssertTrue("Thread running", t.is_running());
    critSect.unlock();
    AssertNoException("Waiting for thread", t.join());
    AssertFalse("Thread not running", t.is_running());
}


/*
 * ::TestRunnableThread
 */
void TestRunnableThread(void) {
    runnable_thread<StatusTestRunnable> t;
    critical_section critSect;

    critSect.lock();

    AssertFalse("runnable_thread not running", t.is_running());
    AssertNoException("Starting runnable_thread", t.start(&critSect));
    AssertTrue("runnable_thread running", t.is_running());
    critSect.unlock();
    AssertNoException("Waiting for runnable_thread", t.join());
    AssertFalse("runnable_thread not running", t.is_running());
}


////////////////////////////////////////////////////////////////////////////////
#pragma region

class PrintIntRunnable : public runnable {

public:
    inline PrintIntRunnable(const int cntReps) : cntReps(cntReps) { }
    virtual ~PrintIntRunnable(void);
    virtual void on_thread_started(const thread::id& id) throw();
    virtual void on_thread_starting(void *userData) throw();
    virtual void on_thread_terminated(const thread::exit_reason reason) throw();
    virtual thread::termination_behaviour on_thread_terminating(void) throw();
    virtual int run(void);
    inline int GetExpectedExitCode(void) const {
        return this->cntReps;
    }
private:
    int cntReps;
};


/*
 * PrintIntRunnable::~PrintIntRunnable
 */
PrintIntRunnable::~PrintIntRunnable(void) {
}


/*
 * PrintIntRunnable::on_thread_started
 */
void PrintIntRunnable::on_thread_started(const thread::id& id) throw() {
    std::cout << "PrintIntRunnable started with ID " << id << std::endl;
    ++::cntStarted;
}


/*
 * PrintIntRunnable::on_thread_starting
 */
void PrintIntRunnable::on_thread_starting(void *userData) throw() {
    std::cout << "PrintIntRunnable is starting... " << std::endl;
    ++::cntStarting;
}


/*
 * PrintIntRunnable::on_thread_terminated
 */
void PrintIntRunnable::on_thread_terminated(const thread::exit_reason reason) throw() {
    std::cout << "PrintIntRunnable terminated with reason " << reason << std::endl;
    ++::cntTerminated;
}


/*
 * PrintIntRunnable::on_thread_terminating
 */
thread::termination_behaviour PrintIntRunnable::on_thread_terminating(void) throw() {
    std::cout << "PrintIntRunnable is requested to terminate." << std::endl;
    ++::cntTerminating;
    return thread::termination_behaviour::forceful;
}


/*
 * PrintIntRunnable::run
 */
int PrintIntRunnable::run(void) {
    int i = 0;

    for (i = 0; i < this->cntReps; ++i) {
        std::cout << "[" << thread::get_current_id() << "] " << i << std::endl;
        thread::yield();
    }

    return i;
}

#pragma endregion
////////////////////////////////////////////////////////////////////////////////

/*
 * ::TestPrintIntRunnable
 */
void TestPrintIntRunnable(void) {
    PrintIntRunnable r1(100);
    PrintIntRunnable r2(50);
    thread t1(&r1);
    thread t2(&r2);

    ::cntStarted = 0;
    ::cntStarting = 0;
    ::cntTerminated = 0;
    ::cntTerminated = 0;

    AssertNoException("Starting thread #1", t1.start());
    AssertNoException("Starting thread #2", t2.start());

    AssertNoException("Waiting for thread #1", t1.join());
    AssertNoException("Waiting for thread #2", t2.join());

    AssertEqual("Return value of thread #1", int(t1.get_exit_code()), r1.GetExpectedExitCode());
    AssertEqual("Return value of thread #2", int(t2.get_exit_code()), r2.GetExpectedExitCode());

    AssertEqual("on_thread_started called", int(::cntStarted), 2);
    AssertEqual("on_thread_starting called", int(::cntStarting), 2);
    AssertEqual("on_thread_terminated called", int(::cntTerminated), 2);
    AssertEqual("on_thread_terminating not called", int(::cntTerminating), 0);
}


////////////////////////////////////////////////////////////////////////////////
#pragma region

class SyncedPrintIntRunnable : public runnable {

public:
    inline SyncedPrintIntRunnable(const int cntReps) : cntReps(cntReps), critSect(NULL) { }
    virtual ~SyncedPrintIntRunnable(void);
    virtual void on_thread_starting(void *userData) throw();
    virtual int run(void);
    inline int GetExpectedExitCode(void) const {
        return this->cntReps;
    }
private:
    int cntReps;
    critical_section *critSect;
};


/*
 * SyncedPrintIntRunnable::~SyncedPrintIntRunnable
 */
SyncedPrintIntRunnable::~SyncedPrintIntRunnable(void) {
}


/*
 * SyncedPrintIntRunnable::on_thread_starting
 */
void SyncedPrintIntRunnable::on_thread_starting(void *userData) throw() {
    std::cout << "SyncedPrintIntRunnable is starting... " << std::endl;
    this->critSect = static_cast<critical_section *>(userData);
}


/*
 * SyncedPrintIntRunnable::run
 */
int SyncedPrintIntRunnable::run(void) {
    int i = 0;
    THE_ASSERT(this->critSect != NULL);

    for (i = 0; i < this->cntReps; ++i) {
        auto_lock<critical_section> l(*this->critSect);
        std::cout << "[" << thread::get_current_id() << "] " << i << std::endl;
    }

    return i;
}

#pragma endregion
////////////////////////////////////////////////////////////////////////////////


/*
 * ::TestSyncedPrintIntRunnable
 */
void TestSyncedPrintIntRunnable(void)  {
    SyncedPrintIntRunnable r1(100);
    SyncedPrintIntRunnable r2(30);
    thread t1(&r1);
    thread t2(&r2);
    critical_section critSect;

    AssertNoException("Starting thread #1", t1.start(&critSect));
    AssertNoException("Starting thread #2", t2.start(&critSect));

    AssertNoException("Waiting for thread #1", t1.join());
    AssertNoException("Waiting for thread #2", t2.join());

    AssertEqual("Return value of thread #1", int(t1.get_exit_code()), r1.GetExpectedExitCode());
    AssertEqual("Return value of thread #2", int(t2.get_exit_code()), r2.GetExpectedExitCode());
}


////////////////////////////////////////////////////////////////////////////////
#pragma region

class PrintIdRunnable : public runnable {

public:
    inline PrintIdRunnable(void) { }
    virtual ~PrintIdRunnable(void);
    virtual int run(void);
};


/*
 * PrintIdRunnable::~PrintIdRunnable
 */
PrintIdRunnable::~PrintIdRunnable(void) { }

/*
 * PrintIdRunnable::run
 */
int PrintIdRunnable::run(void) {
    std::cout << "[" << thread::get_current_id() << "] " << std::endl;
    return 0;
}

#pragma endregion
////////////////////////////////////////////////////////////////////////////////


/*
 * ::TestManyThreads
 */
void TestManyThreads(void) {
    const int CNT_THREADS = 10;
    const int CNT_REPEATS = 1000;
    PrintIdRunnable rs[CNT_THREADS];
    thread *ts[CNT_THREADS];

    for (int i = 0; i < CNT_REPEATS; ++i) {
        for (int t = 0; t < CNT_THREADS; ++t) {
            ts[t] = new thread(rs + t);
            ts[t]->start();
        }
        for (int t = 0; t < CNT_THREADS; ++t) {
            ts[t]->join();
            delete ts[t];
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
#pragma region

class DetachTestRunnable : public runnable {

public:
    typedef struct Locks_t {
        critical_section critSectStart;
        critical_section critSectStop;
    } Locks;

    inline DetachTestRunnable(void) : locks(nullptr) { }
    virtual ~DetachTestRunnable(void);
    virtual void on_thread_starting(void *userData) throw();
    virtual int run(void);
private:
    Locks *locks;
};


/*
 * DetachTestRunnable::~DetachTestRunnable
 */
DetachTestRunnable::~DetachTestRunnable(void) {
}


/*
 * DetachTestRunnable::on_thread_starting
 */
void DetachTestRunnable::on_thread_starting(void *userData) throw() {
    std::cout << "DetachTestRunnable is starting... " << std::endl;
    THE_ASSERT(userData != nullptr);
    this->locks = static_cast<Locks *>(userData);
}


/*
 * DetachTestRunnable::run
 */
int DetachTestRunnable::run(void) {
    THE_ASSERT(this->locks != nullptr);
    this->locks->critSectStop.lock();
    this->locks->critSectStart.lock();
    this->locks->critSectStart.unlock();
    this->locks->critSectStop.unlock();
    return 0;
}

#pragma endregion
////////////////////////////////////////////////////////////////////////////////


/*
 * ::TestDetach
 */
void TestDetach(void) {
    DetachTestRunnable r;
    thread t(&r);
    DetachTestRunnable::Locks locks;

    locks.critSectStart.lock();

    AssertFalse("Thread is initally not running", t.is_running());
    AssertNoException("Start thread", t.start(&locks));
    AssertTrue("Thread is running", t.is_running());
    AssertNoException("Detach thread", t.detach());
    // Note: This would deadlock (as critSect is hold by the caller) if detach 
    // did not work, but I cannot assert this:
    AssertNoException("Join falls through", t.join());
    locks.critSectStart.unlock();
    // Need other means to "join", because 't' is detached.
    locks.critSectStop.lock();
    locks.critSectStop.unlock();
}


/*
 * ::TestThread
 */
void TestThread(void) {
    SET_THE_THREADING_TRACE_ALL(),
    ::TestStatus();
    ::TestPrintIntRunnable();
    ::TestSyncedPrintIntRunnable();
    ::TestRunnableThread();
    ::TestDetach();
    ::TestManyThreads();
}
