/*
 * the/system/threading/runnable.cpp
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
 * Runnable.cpp  15.08.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#include "the/system/threading/runnable.h"

#include "the/stack_trace.h"


/*
 * the::system::threading::runnable::~runnable
 */
the::system::threading::runnable::~runnable(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::threading::runnable::on_thread_started
 */
void the::system::threading::runnable::on_thread_started(
        const thread::id& id) throw() {
    THE_STACK_TRACE;
}


/*
 * the::system::threading::runnable::on_thread_starting
 */
void the::system::threading::runnable::on_thread_starting(
        void *userData) throw() {
    THE_STACK_TRACE;
}


/*
 * the::system::threading::runnable::on_thread_terminated
 */
void the::system::threading::runnable::on_thread_terminated(
        const thread::exit_reason reason) throw() {
    THE_STACK_TRACE;
}


/*
 * the::system::threading::runnable::on_thread_terminating
 */
the::system::threading::thread::termination_behaviour 
the::system::threading::runnable::on_thread_terminating(void) throw() {
    THE_STACK_TRACE;
    return thread::unsupported;
}


/*
 * the::system::threading::runnable::runnable
 */
the::system::threading::runnable::runnable(void) {
    THE_STACK_TRACE;
}
