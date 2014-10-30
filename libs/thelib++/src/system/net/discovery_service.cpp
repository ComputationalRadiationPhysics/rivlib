/*
 * src\system\net\discovery_service.cpp
 *
 * Copyright (C) 2013 TheLib Team (http://www.thelib.org/license)
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
 * ClusterDiscoveryService.cpp
 *
 * Copyright (C) 2006 - 2007 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */


#include "the/system/net/discovery_service.h"

#include "the/assert.h"
#include "the/invalid_operation_exception.h"
#include "the/stack_trace.h"

#include "the/system/net/debug.h"


/*
 * the::system::net::discovery_service::default_port
 */
const uint16_t the::system::net::discovery_service::default_port = 28181;


/*
 * the::system::net::discovery_service::discovery_service
 */
the::system::net::discovery_service::discovery_service(const std::string& name,
        const ip_end_point& responseAddr, const ip_address4& bcastAddr,
        const uint16_t port) : not_copyable(), discovery_observable_type(),
        error_observable_type(), bcastAddr(bcastAddr, port),
        bindAddr(ip_address4::any, port), name(name),
        responseAddr(responseAddr) {
    THE_STACK_TRACE;
}


/*
 * the::system::net::discovery_service::~discovery_service
 */
the::system::net::discovery_service::~discovery_service(void) {
    THE_STACK_TRACE;
    try {
        this->stop();
    } catch (...) {
        THE_NETWORKING_TRACE_WARN("An error occurred while destroying "
            "a discovery_service. This is normal in case that the "
            "discovery_service has already been stopped by the user "
            "before.\n");
    }
}


/*
 * the::system::net::discovery_service::advertise
 */
void the::system::net::discovery_service::advertise(void) {
    THE_STACK_TRACE;

    // TODO

//    Message request;                // The UDP datagram we send.
//    ClusterDiscoveryService *cds    // The discovery service we work for.
//        = static_cast<ClusterDiscoveryService *>(discSvc);
//                /** The socket used for the broadcast. */
//    Socket socket;                  // Socket used for broadcast.
//    
//    ASSERT(cds != NULL);
//
//    // Assert expected memory layout of messages.
//    ASSERT(sizeof(request) == MAX_USER_DATA + 2 * sizeof(UINT32));
//    ASSERT(reinterpret_cast<BYTE *>(&(request.senderBody)) 
//       == reinterpret_cast<BYTE *>(&request) + 2 * sizeof(UINT32));
//
//    /* Prepare the socket. */
//    try {
//        Socket::Startup();
//        socket.Create(cds->bindAddr, Socket::TYPE_DGRAM, Socket::PROTOCOL_UDP);
//        socket.SetBroadcast(true);
//        //socket.SetLinger(true, 0);  // Force hard close.
//    } catch (SocketException e) {
//        VLTRACE(Trace::LEVEL_VL_ERROR, "Discovery sender thread could not "
//            "create its. The error code is %d (\"%s\").\n", e.GetErrorCode(),
//            e.GetMsgA());
//        return e.GetErrorCode();
//    }
//
//    /* Prepare our request for initial broadcasting. */
//    request.magicNumber = MAGIC_NUMBER;
//    request.msgType = MSG_TYPE_IAMHERE;
//    request.senderBody.sockAddr = cds->responseAddr;
//#if (_MSC_VER >= 1400)
//    ::strncpy_s(request.senderBody.name, MAX_NAME_LEN, cds->name, 
//        MAX_NAME_LEN);
//#else /* (_MSC_VER >= 1400) */
//    ::strncpy(request.senderBody.name, cds->name.PeekBuffer(), MAX_NAME_LEN);
//#endif /* (_MSC_VER >= 1400) */
//
//    VLTRACE(Trace::LEVEL_VL_INFO, "The discovery sender thread is starting ...\n");
//
//    /* Send the initial "immediate alive request" message. */
//    try {
//        socket.Send(cds->bcastAddr, &request, sizeof(Message));
//        VLTRACE(Trace::LEVEL_VL_INFO, "Discovery service sent MSG_TYPE_IAMHERE "
//            "to %s.\n", cds->bcastAddr.ToStringA().PeekBuffer());
//
//        /*
//         * If the discovery service is configured not to be member of the 
//         * cluster, but to only search other members, the sender thread can 
//         * leave after the first request sent above.
//         * Otherwise, the thread should wait for the normal request interval
//         * time before really starting.
//         */
//        if (cds->isObserver) {
//            VLTRACE(Trace::LEVEL_VL_INFO, "Discovery service is leaving request "
//                "thread as it is only discovering other nodes.\n");
//            return 0;
//        } else {
//            sys::Thread::Sleep(cds->requestInterval);
//        }
//
//    } catch (SocketException e) {
//        VLTRACE(Trace::LEVEL_VL_WARN, "A socket error occurred in the "
//            "discovery sender thread. The error code is %d (\"%s\").\n",
//            e.GetErrorCode(), e.GetMsgA());
//    } catch (...) {
//        VLTRACE(Trace::LEVEL_VL_ERROR, "The discovery sender caught an "
//            "unexpected exception.\n");
//        return -1;
//    }
//
//    /* Change our request for alive broadcasting. */
//    request.msgType = MSG_TYPE_IAMALIVE;
//
//    while (this->isRunning) {
//        try {
//            /* Broadcast request. */
//            cds->prepareRequest();
//            socket.Send(cds->bcastAddr, &request, sizeof(Message));
//            VLTRACE(Trace::LEVEL_VL_INFO, "Discovery service sent "
//                "MSG_TYPE_IAMALIVE to %s.\n", 
//                cds->bcastAddr.ToStringA().PeekBuffer());
//
//            sys::Thread::Sleep(cds->requestInterval);
//        } catch (SocketException e) {
//            VLTRACE(Trace::LEVEL_VL_WARN, "A socket error occurred in the "
//                "discovery sender thread. The error code is %d (\"%s\").\n",
//                e.GetErrorCode(), e.GetMsgA());
//        } catch (...) {
//            VLTRACE(Trace::LEVEL_VL_ERROR, "The discovery sender caught an "
//                "unexpected exception.\n");
//            return -1;
//        }
//    } /* end while (this->isRunning) */
//
//    /* Clean up. */
//    try {
//        /* Now inform all other nodes, that we are out. */
//        request.msgType = MSG_TYPE_SAYONARA;
//        socket.Send(cds->bcastAddr, &request, sizeof(Message));
//        VLTRACE(Trace::LEVEL_VL_INFO, "Discovery service sent MSG_TYPE_SAYONARA to "
//            "%s.\n", cds->bcastAddr.ToStringA().PeekBuffer());
//        
//        Socket::Cleanup();
//    } catch (SocketException e) {
//        VLTRACE(Trace::LEVEL_VL_ERROR, "Socket cleanup failed in the discovery "
//            "request thread. The error code is %d (\"%s\").\n", 
//            e.GetErrorCode(), e.GetMsgA());
//        return e.GetErrorCode();
//    }
//
//    return 0;
}


/*
 * the::system::net::discovery_service::start
 */
void the::system::net::discovery_service::start(void) {
    THE_STACK_TRACE;
    if (this->receiverThread.is_running()) {
        THROW_THE_EXCEPTION(invalid_operation_exception,
            THE_SYSTEM_STRING_CONSTANT("The discovery_service is already ")
            THE_SYSTEM_STRING_CONSTANT("running."));
    }

    socket::startup();
    this->receiverThread.start(this);
    this->advertise();
}


/*
 * the::system::net::discovery_service::stop
 */
void the::system::net::discovery_service::stop(void) {
    THE_STACK_TRACE;
    try {
        this->receiverThread.terminate(true);
    } catch (...) {
        THE_NETWORKING_TRACE_WARN("An error occurred while stopping the "
            "discovery worker thread. This may be harmless if you e.g. "
            "try to stop a discovery_service that is not running.\n");
    }

    socket::cleanup();
}


////////////////////////////////////////////////////////////////////////////////

/*
 * the::system::net::discovery_service::discovery_worker::discovery_worker
 */
the::system::net::discovery_service::discovery_worker::discovery_worker(void) 
        : owner(nullptr) {
    THE_STACK_TRACE;
}

/*
 * ...::net::discovery_service::discovery_worker::~discovery_worker
 */
the::system::net::discovery_service::discovery_worker::~discovery_worker(void) {
    THE_STACK_TRACE;
}

/*
 * the::system::net::discovery_service::discovery_worker::on_thread_starting
 */
void the::system::net::discovery_service::discovery_worker::on_thread_starting(
        void *userData) throw() {
    THE_STACK_TRACE;
    this->owner = static_cast<discovery_service *>(userData);
}


/*
 * ...net::discovery_service::discovery_worker::on_thread_terminating
 */
the::system::threading::thread::termination_behaviour
the::system::net::discovery_service::discovery_worker::on_thread_terminating(
        void) throw() {
    THE_STACK_TRACE;
    try {
        this->receiverSocket.close();
    } catch (...) {
        THE_NETWORKING_TRACE_WARN("An error occurred while closing the "
            "discovery receiver socket.\n");
    }
    return threading::thread::termination_behaviour::graceful;
}


/*
 * the::system::net::discovery_service::discovery_worker::run
 */
int the::system::net::discovery_service::discovery_worker::run(void) {
    THE_STACK_TRACE;
    THE_ASSERT(this->owner != nullptr);

    ip_end_point peerAddr;            // Receives address of communication peer.


    // TODO
    //IPEndPoint peerAddr;            // Receives address of communication peer.
    //PeerNode peerNode;              // The peer node to register in our list.
    //Message msg;                    // Receives the request messages.
    //ClusterDiscoveryService *cds    // The discovery service we work for.
    //    = static_cast<ClusterDiscoveryService *>(discSvc);

    //ASSERT(cds != NULL);

    //// Assert expected message memory layout.
    //ASSERT(sizeof(msg) == MAX_USER_DATA + 2 * sizeof(UINT32));
    //ASSERT(reinterpret_cast<BYTE *>(&(msg.senderBody)) 
    //   == reinterpret_cast<BYTE *>(&msg) + 2 * sizeof(UINT32));

    /* 
     * Prepare a datagram socket listening for requests on the specified
     * adapter and port. 
     */
    try {
        socket::startup();
        this->receiverSocket.open(static_cast<protocol_family>(
            this->owner->bindAddr.address_family()), socket_type::datagram,
            protocol::udp);
        this->receiverSocket.set_option(socket_options::broadcast(true));
        // TODO: Make shared socket use configurable (security issue!).
#ifdef THE_WINDOWS
	// TODO: Just removed for Linux support. Think about what I have done here.
        this->receiverSocket.set_option(
            socket_options::exclusive_address_use(false));
#endif /* THE_WINDOWS */
        this->receiverSocket.set_option(socket_options::reuse_address(true));
        this->receiverSocket.bind(this->owner->bindAddr);
    //    //socket.SetLinger(false, 0);     // Force hard close.
    } catch (socket_exception e) {
//        VLTRACE(Trace::LEVEL_VL_ERROR, "Discovery receiver thread could not "
  //          "create its socket and bind it to the requested address. The "
    //        "error code is %d (\"%s\").\n", e.GetErrorCode(), e.GetMsgA());
        return e.get_error().native_error();
    }

    THE_NETWORKING_TRACE_INFO("The discovery receiver thread is starting...\n");
    //while (this->isRunning) {
    //    try {

    //        /* Wait for next message. */
    //        this->socket.Receive(peerAddr, &msg, sizeof(Message));

    //        if (msg.magicNumber == MAGIC_NUMBER) {
    //            /* Message OK, look for its content. */

    //            if ((msg.msgType == MSG_TYPE_IAMALIVE) 
    //                    && (cds->name.Equals(msg.senderBody.name))) {
    //                /* Got a discovery request for own cluster. */
    //                VLTRACE(Trace::LEVEL_VL_INFO, "Discovery service received "
    //                    "MSG_TYPE_IAMALIVE from %s.\n", 
    //                    peerAddr.ToStringA().PeekBuffer());
    //                
    //                /* Add peer to local list, if not yet known. */
    //                cds->addPeerNode(peerAddr, IPEndPoint(
    //                    msg.senderBody.sockAddr));

    //            } else if ((msg.msgType == MSG_TYPE_IAMHERE) 
    //                    && (cds->name.Equals(msg.senderBody.name))) {
    //                /* 
    //                 * Get an initial discovery request. This triggers an 
    //                 * immediate alive message, but without adding the sender to
    //                 * the cluster.
    //                 *
    //                 * Note: Nodes sending the MSG_TYPE_IAMHERE message must
    //                 * be added to the list of known nodes, because nodes in 
    //                 * observer mode also send MSG_TYPE_IAMHERE to request an
    //                 * immediate response of all running nodes.
    //                 */
    //                VLTRACE(Trace::LEVEL_VL_INFO, "Discovery service received "
    //                    "MSG_TYPE_IAMHERE from %s.\n", 
    //                    peerAddr.ToStringA().PeekBuffer());

    //                if (!cds->isObserver) {
    //                    /* Observers must not send alive messages. */
    //                    peerAddr.SetPort(cds->bindAddr.GetPort());
    //                    ASSERT(msg.magicNumber == MAGIC_NUMBER);
    //                    msg.msgType = MSG_TYPE_IAMALIVE;
    //                    msg.senderBody.sockAddr = cds->responseAddr;
    //                    this->socket.Send(peerAddr, &msg, sizeof(Message));
    //                    VLTRACE(Trace::LEVEL_VL_INFO, "Discovery service sent "
    //                        "immediate MSG_TYPE_IAMALIVE answer to %s.\n",
    //                        peerAddr.ToStringA().PeekBuffer());
    //                }

    //            } else if ((msg.msgType == MSG_TYPE_SAYONARA)
    //                    && (cds->name.Equals(msg.senderBody.name))) {
    //                /* Got an explicit disconnect. */
    //                VLTRACE(Trace::LEVEL_VL_INFO, "Discovery service received "
    //                    "MSG_TYPE_SAYONARA from %s.\n",
    //                    peerAddr.ToStringA().PeekBuffer());

    //                cds->removePeerNode(IPEndPoint(msg.senderBody.sockAddr));

    //            } else if (msg.msgType >= MSG_TYPE_USER) {
    //                /* Received user message. */
    //                cds->fireUserMessage(peerAddr, msg.msgType, msg.userData);
    //            }
    //        } /* end if (response.magicNumber == MAGIC_NUMBER) */

    //    } catch (SocketException e) {
    //        VLTRACE(Trace::LEVEL_VL_WARN, "A socket error occurred in the "
    //            "discovery receiver thread. The error code is %d (\"%s\").\n",
    //            e.GetErrorCode(), e.GetMsgA());
    //    } catch (...) {
    //        VLTRACE(Trace::LEVEL_VL_ERROR, "The discovery receiver caught an "
    //            "unexpected exception.\n");
    //        return -1;
    //    }

    //} /* end while (this->isRunning) */

    //try {
    //    Socket::Cleanup();
    //} catch (SocketException e) {
    //    VLTRACE(Trace::LEVEL_VL_ERROR, "Socket cleanup failed in the discovery "
    //        "receiver thread. The error code is %d (\"%s\").\n", 
    //        e.GetErrorCode(), e.GetMsgA());
    //    return e.GetErrorCode();
    //}

    //return 0;

    return 0;
}
