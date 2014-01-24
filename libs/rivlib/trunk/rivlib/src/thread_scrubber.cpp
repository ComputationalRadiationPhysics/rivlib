/*
 * thread_scrubber.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "thread_scrubber.h"

using namespace eu_vicci::rivlib;
using namespace the::system::threading;


/*
 * thread_scrubber::thread_scrubber
 */
thread_scrubber& thread_scrubber::instance(void) {
    static thread_scrubber inst;
    return inst;
}


/*
 * thread_scrubber::run
 */
int thread_scrubber::run(void) {

    while (true) {

        this->lock_obj.lock();
        if (this->cnt == 0) {
            this->terminating = true;
            this->lock_obj.unlock();
            break;
        }

        if (this->queue.size() == 0) {
            this->lock_obj.unlock();
            thread::sleep(10);
            continue;
        }

        element_base* e = this->queue.front();
        this->queue.erase(this->queue.begin());
        this->lock_obj.unlock();

        while (e->is_running()) {
            thread::sleep(10);
        }

        the::safe_delete(e);

        this->lock_obj.lock();
        this->cnt--;
        this->lock_obj.unlock();

    }

    return 0;
}


/*
 * thread_scrubber::thread_scrubber
 */
thread_scrubber::thread_scrubber(void) : runnable(), lock_obj(), worker(nullptr),
        queue(), terminating(false) {
    this->worker = new thread(this);
}


/*
 * thread_scrubber::thread_scrubber
 */
thread_scrubber::~thread_scrubber(void) {
    if (this->worker->is_running()) {
        this->worker->join();
    }
    the::safe_delete(this->worker);
}
