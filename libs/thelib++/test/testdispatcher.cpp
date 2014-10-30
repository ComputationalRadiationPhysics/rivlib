/*
 * testdispatcher.cpp
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
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "stdafx.h"
#include "testdispatcher.h"
#include "testhelper.h"

#include "the/system/net/message_dispatcher.h"

#include "the/system/threading/runnable_thread.h"
#include "the/system/threading/semaphore.h"


static const long CNT_MSGS = 10;
static long cntReceived = 0;

static the::system::threading::semaphore semWaitRecv(0l, CNT_MSGS);


/*
 * ::OnError
 */
static void OnError(
        the::system::net::socket_simple_message_dispatcher::error_observable_type& src,
        the::system::net::socket_simple_message_dispatcher::error_event_args_type& ea) {
    try {
        ea.rethrow();
    } catch (the::system::net::socket_exception e) {
        std::cout << "Socket error " << e.get_msg_astr() << "occurred." << std::endl;
    } catch (...) {
        std::cout << "Unexpected error occurred." << std::endl;
    }
}

/*
 * ::OnMessage
 */
static void OnMessage(
        the::system::net::socket_simple_message_dispatcher::message_observable_type& src,
        the::system::net::socket_simple_message_dispatcher::message_type& ea) {
    std::cout << "Received message " << ea.id() << std::endl;
    ++::cntReceived;
    ::semWaitRecv.unlock();
}


/*
 * ::TestDispatcher
 */
void TestDispatcher(void) {
    using namespace the::system::net;
    const uint16_t port = 12345;

    AssertNoException("Socket startup in main thread", socket::startup());

    socket_simple_message_dispatcher::message_type msg;
    ip_end_point remoteAddr;
    the::system::threading::runnable_thread<socket_simple_message_dispatcher> dispatcher;
    the::system::net::socket scktRecv = the::system::net::socket::create_tcp(protocol_family::inter_network4);
    the::system::net::socket scktServer = the::system::net::socket::create_tcp(protocol_family::inter_network4);
    the::system::net::socket scktSend;

    dispatcher.add_observer(::OnError);
    dispatcher.add_observer(::OnMessage);
    ::cntReceived = 0;

    /* Establish the connection. */
    scktServer.bind(ip_end_point(address_family::inter_network4, port));
    scktServer.listen();
    scktRecv.connect(ip_end_point(ip_address4::loopback, port));
    scktSend = scktServer.accept(remoteAddr);
    scktServer.close();

    /* Start the dispatcher thread. */
    socket_simple_message_dispatcher::configuration_type config(scktRecv);
    dispatcher.start(&config);

    /* Configure the message. */
    msg.set_body(nullptr);

    /* Send messages. */
    for (int i = 0; i < CNT_MSGS; ++i) {
        msg.set_id(i);
        AssertNoException("Sending message", scktSend.send(static_cast<void *>(msg), msg.size(), true));
    }

    /* Wait for messages. */
    for (int i = 0; i < CNT_MSGS; ++i) {
        ::semWaitRecv.lock();
    }

    dispatcher.terminate(true);

    AssertEqual("Received the expected number of messages using dispatcher", CNT_MSGS, cntReceived);
    AssertNoException("Socket cleanup in main thread", socket::cleanup());
}
