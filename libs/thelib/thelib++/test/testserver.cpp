/*
 * testserver.cpp
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
#include "testserver.h"
#include "testhelper.h"

#include "the/system/net/server.h"

#include "the/system/threading/runnable_thread.h"


static int cntConnects = 2;


/*
 * ::OnError
 */
static void OnError(
        the::system::net::socket_server::error_observable_type& src,
        the::system::net::socket_server::error_event_args_type& ea) {
    try {
        ea.rethrow();
    } catch (the::system::net::socket_exception e) {
        std::cout << "Socket error " << e.get_msg_astr() << "occurred." << std::endl;
    } catch (...) {
        std::cout << "Unexpected error occurred." << std::endl;
    }
}


/*
 * ::OnNewConnection
 */
static void OnNewConnection(
        the::system::net::socket_server::client_connected_observable_type& src,
        the::system::net::socket_server::client_connected_event_args_type& ea) {
    std::cout << "Accepting client " << ea.client_address().to_astring() << std::endl;
    the::system::net::socket s = ea.accept();
    s.close();

    if (--::cntConnects <= 0) {
        dynamic_cast<the::system::net::socket_server&>(src).on_thread_terminating();
    }
}


/*
 * ::OnStateChange
 */
static void OnStateChange(
        the::system::net::socket_server::state_observable_type& src,
        the::system::net::socket_server::state_event_args_type& ea) {
    std::cout << "Server went from " << ea.old_state() << " to " << ea.new_state() << std::endl;
}


/*
 * ::TestServer
 */
void TestServer(void) {
    using namespace the::system::net;
    using namespace the::system::threading;

    AssertNoException("Starting socket API", socket::startup());

    the::system::net::socket s = socket::create(protocol_family::inter_network4, socket_type::stream, protocol::tcp);
    ip_end_point ep(ip_address4::any, 12345);

    socket_server::configuration_type config(s, ep);
    runnable_thread<socket_server> server;
    socket_server::error_handler_type he(::OnError);

    server.add_observer(he);
    server.add_observer(socket_server::client_connected_handler_type(::OnNewConnection));
    server.add_observer(socket_server::state_handler_type(::OnStateChange));
    server.start(&config);
    server.join();

    AssertNoException("Stopping socket API", socket::cleanup());
}
