/*
 * src\system\net\socket.cpp
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
 * Socket.cpp
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Mueller (christoph.mueller@vis.uni-stuttgart.de). All rights reserved.
 */

#include "the/system/net/socket.h"

#include <cstdlib>

// The TEMP_FAILURE_RETRY macro handles EINTR on Linux. On windows, we do not
// need this and just define an empty macro executing the statement.
#ifdef THE_WINDOWS
#define TEMP_FAILURE_RETRY(e) e
#endif /* THE_WINDOWS */

// Define the SOCKET_ERROR macro for Linux in order to use it like on Windows 
// instead of a magic number
#ifdef THE_LINUX
#define SOCKET_ERROR (-1)
#endif /* THE_LINUX */

#ifdef THE_LINUX
#include <poll.h>
#include <unistd.h>
#include <net/if.h>
#endif /* THE_LINUX */

#include "the/assert.h"
#include "the/stack_trace.h"

#include "the/system/net/debug.h"
#include "the/system/net/disconnected_exception.h"
#include "the/system/net/socket_exception.h"


#include "the/not_implemented_exception.h"


/*
 * the::system::net::socket::cleanup
 */
void the::system::net::socket::cleanup(void) {
#ifdef THE_WINDOWS
    if (::WSACleanup() != 0) {
        throw socket_exception(__FILE__, __LINE__);
    }
#endif /* THE_WINDOWS */
}


/*
 * the::system::net::socket::create
 */
the::system::net::socket the::system::net::socket::create(
            const the::system::net::protocol_family protocolFamily,
            const the::system::net::socket_type type,
            const the::system::net::protocol protocol) {
    THE_STACK_TRACE;
    socket retval;
    retval.open(protocolFamily, type, protocol);
    return std::move(retval);
}


/*
 * the::system::net::socket::startup
 */
void the::system::net::socket::startup(void) {
#ifdef THE_WINDOWS
    WSAData wsaData;

    // Note: Need Winsock 2 for timeouts.
    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw socket_exception(__FILE__, __LINE__);
    }
#endif /* THE_WINDOWS */
}


/*
 * the::system::net::socket::invalid_handle
 */
const the::system::net::socket::native_handle_type
the::system::net::socket::invalid_handle
#if defined(THE_WINDOWS)
    = INVALID_SOCKET;
#elif defined(THE_LINUX)
    = -1;
#else /* defined(THE_WINDOWS) */
#error "Specify the native invalid socket handle value!"
#endif /* defined(THE_WINDOWS) */


/*
 * the::system::net::socket::maximum_connections
 */
const int the::system::net::socket::maximum_connections = SOMAXCONN;


/*
 * the::system::net::socket::timeout_infinite
 */
const the::system::net::socket::timeout_type
the::system::net::socket::timeout_infinite = 0;


/*
 * the::system::net::socket::socket
 */
the::system::net::socket::socket(socket&& rhs) : handle(rhs.handle) {
    THE_STACK_TRACE;
    rhs.handle = socket::invalid_handle;
}


/*
 * the::system::net::socket::socket
 */
the::system::net::socket::socket(native_handle_type&& handle) : handle(handle) {
    THE_STACK_TRACE;
    handle = socket::invalid_handle;
}


/*
 * the::system::net::socket::~socket
 */
the::system::net::socket::~socket(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::net::socket::accept
 */
the::system::net::socket the::system::net::socket::accept(
        socket::end_point_type& outPeerAddress) {
    THE_STACK_TRACE;
    native_handle_type retval = socket::invalid_handle;

#if defined(THE_WINDOWS)
    INT addrLen = static_cast<int>(outPeerAddress.size());

    if ((retval = ::WSAAccept(this->handle,
            static_cast<sockaddr *>(outPeerAddress), &addrLen, NULL,
            0)) == socket::invalid_handle) {
        THROW_THE_SOCKET_EXCEPTION;
    }

#else /* defined(THE_WINDOWS) */
    unsigned int addrLen = static_cast<unsigned int>(outPeerAddress.size());

    TEMP_FAILURE_RETRY(retval = ::accept(this->handle,
        static_cast<sockaddr *>(outPeerAddress), &addrLen));
    if (retval == socket::invalid_handle) {
        THROW_THE_SOCKET_EXCEPTION;
    }

#endif /* defined(THE_WINDOWS) */

    return socket(std::move(retval));
}


/*
 * the::system::net::socket::bind
 */
void the::system::net::socket::bind(const socket::end_point_type& address) {
    THE_STACK_TRACE;
    const ip_end_point& a = dynamic_cast<const ip_end_point& >(address);

    if (TEMP_FAILURE_RETRY(::bind(this->handle,
            static_cast<const struct sockaddr *>(a),
            sizeof(struct sockaddr_storage))) == SOCKET_ERROR) {
        THROW_THE_SOCKET_EXCEPTION;
    }
}

///*
// * vislib::net::Socket::BindToDevice
// */
//void vislib::net::Socket::BindToDevice(const StringA& name) {
//#ifndef _WIN32
//    struct ifreq interface;
//
//    ::strncpy(interface.ifr_ifrn.ifrn_name, name.PeekBuffer(), 
//        name.Length() + 1);
//
//    if (::setsockopt(this->handle, SOL_SOCKET, SO_BINDTODEVICE, &interface,
//            sizeof(interface)) == -1) {
//        throw SocketException(__FILE__, __LINE__);
//    }
//#endif /* !_WIN32 */
//}


/*
 * the::system::net::socket::close
 */
void the::system::net::socket::close(void) {
    THE_STACK_TRACE;

    if (this->is_valid()) {
#if defined(THE_WINDOWS)
        if (::closesocket(this->handle) == SOCKET_ERROR) {
#else /* defined(THE_WINDOWS) */
        if (TEMP_FAILURE_RETRY(::close(this->handle)) == SOCKET_ERROR) {
#endif /* defined(THE_WINDOWS) */
            THROW_THE_SOCKET_EXCEPTION;
        }

        this->handle = socket::invalid_handle;
    } /* end if (this->is_valid()) */
}


/*
 * the::system::net::socket::connect
 */
void the::system::net::socket::connect(const socket::end_point_type& address) {
    THE_STACK_TRACE;
    const ip_end_point& a = dynamic_cast<const ip_end_point& >(address);

#if defined(THE_WINDOWS)
    if (::WSAConnect(this->handle, static_cast<const struct sockaddr *>(a),
            sizeof(struct sockaddr_storage), NULL, NULL, NULL, NULL)
            == SOCKET_ERROR) {
        THROW_THE_SOCKET_EXCEPTION;
    }

#else /* defined(THE_WINDOWS) */
    // mueller: Linux of kaukerdl showed behaviour as described in 
    // http://www.madore.org/~david/computers/connect-intr.html. I took the
    // fix from there, including the inspiration for the variable naming...
    const struct sockaddr *sa = static_cast<const struct sockaddr *>(a);
    const size_t sal = sizeof(struct sockaddr_storage);
    if (::connect(this->handle, sa, sal) == SOCKET_ERROR) {
        struct pollfd linuxReallySucks;
        int someMoreJunk = 0;
        size_t yetMoreUselessJunk = sizeof(someMoreJunk);

        if (errno != EINTR /* && errno != EINPROGRESS */) {
            THROW_THE_SOCKET_EXCEPTION;
        }
    
        linuxReallySucks.fd = this->handle;
        linuxReallySucks.events = POLLOUT;
        while (::poll(&linuxReallySucks, 1, -1) == -1) {
            if (errno != EINTR) {
                THROW_THE_SOCKET_EXCEPTION;
            }
        }

        // >> THIS DOES NOT WORK >>
        //this->GetOption(SOL_SOCKET, SO_ERROR, &someMoreJunk, 
        //    yetMoreUselessJunk);
        //if (someMoreJunk != 0) {
        //    THROW_THE_EXCEPTION(socket_exception, someMoreJunk);
        //}
    } /* end if (::connect(this->handle, sa, sal) == SOCKET_ERROR) */
#endif /* defined(THE_WINDOWS) */
}


///*
// * vislib::net::Socket::Create
// */
//void vislib::net::Socket::Create(const IPEndPoint& familySpecAddr, 
//            const Type type, const Protocol protocol) {
//    switch (familySpecAddr.GetAddressFamily()) {
//        case IPEndPoint::FAMILY_INET:
//            this->Create(FAMILY_INET, type, protocol);
//            break;
//
//        case IPEndPoint::FAMILY_INET6:
//            this->Create(FAMILY_INET6, type, protocol);
//            break;
//
//        default:
//            throw IllegalParamException("familySpecAddr", __FILE__, __LINE__);
//    }
//}
//


/*
 * the::system::net::socket::equals
 */
bool the::system::net::socket::equals(const socket& rhs) const {
    THE_STACK_TRACE;
    return (this->handle == rhs.handle);
}


///*
// * vislib::net::Socket::GracefulDisconnect
// */
//void vislib::net::Socket::GracefulDisconnect(const bool isClose) {
//    BYTE buffer[4];             // Buffer for receiving remaining data.
//    
//    try {
//        /* Signal to server that we will not send anything else. */
//        this->Shutdown(SEND);
//
//        /* Receive all pending data from server. */
//        while (this->Receive(&buffer, sizeof(buffer)) > 0);
//
//    } catch (...) {
//        /* Ensure that Close() is called in any case if requested. */
//        if (isClose) {
//            this->Close();
//        }
//        throw;
//    }
//
//    /* Close socket if requested. */
//    if (isClose) {
//        this->Close();
//    }
//}


///*
// * vislib::net::Socket::IOControl
// */
//void vislib::net::Socket::IOControl(const DWORD ioControlCode, void *inBuffer,
//        const DWORD cntInBuffer, void *outBuffer, const DWORD cntOutBuffer,
//        DWORD& outBytesReturned) {
//#ifdef _WIN32
//    if (::WSAIoctl(this->handle, ioControlCode, inBuffer, cntInBuffer, 
//            outBuffer, cntOutBuffer, &outBytesReturned, NULL, NULL) 
//            == SOCKET_ERROR) {
//        throw SocketException(__FILE__, __LINE__);
//    }
//#else /* _WIN32 */
//    // TODO
//#endif /* _WIN32 */
//}


/*
 * the::system::net::socket::listen
 */
void the::system::net::socket::listen(const int backlog) {
    THE_STACK_TRACE;
    if (TEMP_FAILURE_RETRY(::listen(this->handle, backlog)) == SOCKET_ERROR) {
        THROW_THE_SOCKET_EXCEPTION;
    }
}


/*
 * the::system::net::socket::local_ip_end_point
 */
the::system::net::socket::end_point_type
the::system::net::socket::local_end_point(void) const {
    THE_STACK_TRACE;
    ip_end_point retval;
#if defined(THE_WINDOWS)
    int len = static_cast<int>(retval.size());
#else /* defined(THE_WINDOWS) */
    socklen_t len = static_cast<socklen_t>(retval.size());
#endif /* defined(THE_WINDOWS) */

    if (::getsockname(this->handle, static_cast<sockaddr *>(retval), &len)
            == SOCKET_ERROR) {
        THROW_THE_SOCKET_EXCEPTION;
    }

    return retval;
}


/*
 * the::system::net::socket::open
 */
void the::system::net::socket::open(
        const the::system::net::protocol_family protocolFamily,
        const the::system::net::socket_type type, 
        const the::system::net::protocol protocol) {
    THE_STACK_TRACE;

    this->close();

#if defined(THE_WINDOWS)
    this->handle = ::WSASocket(static_cast<const int>(protocolFamily),
        static_cast<const int>(type), static_cast<const int>(protocol),
        NULL, 0, WSA_FLAG_OVERLAPPED);
#else /* defined(THE_WINDOWS) */
    this->handle = ::socket(static_cast<const int>(protocolFamily), 
        static_cast<const int>(type), static_cast<const int>(protocol));
#endif /* defined(THE_WINDOWS) */

    if (this->handle == socket::invalid_handle) {
        THROW_THE_SOCKET_EXCEPTION;
    }
}


/*
 * the::system::net::socket::receive
 */
size_t the::system::net::socket::receive(void *outData, const size_t cntBytes,
        const timeout_type timeout, const int flags) {
    THE_STACK_TRACE;
    this->check_read(timeout);
    return this->receive(outData, cntBytes, false, flags);
}


/*
 * the::system::net::socket::receive
 */
size_t the::system::net::socket::receive(void *outData, const size_t cntBytes,
            const bool forceReceive, const int flags) {
    THE_STACK_TRACE;
    size_t totalReceived = 0;   // # of bytes totally received.
    int lastReceived = 0;       // # of bytes received during last recv() call.
#ifdef _WIN32
    WSAOVERLAPPED overlapped;   // Overlap structure for asynchronous recv().
    WSABUF wsaBuf;              // Buffer for WSA output.
    DWORD errorCode = 0;        // WSA error during last operation.
    DWORD inOutFlags = flags;   // Flags for WSA.

    the::zero_memory(&wsaBuf);
    wsaBuf.buf = static_cast<char *>(outData);
    wsaBuf.len = static_cast<u_long>(cntBytes);

    if ((overlapped.hEvent = ::WSACreateEvent()) == WSA_INVALID_EVENT) {
        THROW_THE_SOCKET_EXCEPTION;
    }
#endif /* _WIN32 */

    do {
#ifdef _WIN32
        if (::WSARecv(this->handle, &wsaBuf, 1, reinterpret_cast<DWORD *>(
                &lastReceived), &inOutFlags, &overlapped, NULL) != 0) {
            if ((errorCode = ::WSAGetLastError()) != WSA_IO_PENDING) {
                ::WSACloseEvent(overlapped.hEvent);
                THROW_THE_EXCEPTION(socket_exception, errorCode);
            }
            THE_NETWORKING_TRACE_DEBUG("Overlapped socket I/O pending...\n");
            if (!::WSAGetOverlappedResult(this->handle, &overlapped, 
                    reinterpret_cast<DWORD *>(&lastReceived), TRUE, 
                    &inOutFlags)) {
                ::WSACloseEvent(overlapped.hEvent);
                THROW_THE_SOCKET_EXCEPTION;
            }
        }
        totalReceived += static_cast<size_t>(lastReceived);
        wsaBuf.buf += lastReceived;
        wsaBuf.len -= lastReceived;

#else /* _WIN32 */
        TEMP_FAILURE_RETRY(lastReceived = ::recv(this->handle, 
            static_cast<char *>(outData) + totalReceived, 
            static_cast<int>(cntBytes - totalReceived), flags));

        if ((lastReceived >= 0) && (lastReceived != SOCKET_ERROR)) {
            /* Successfully received new package. */
            totalReceived += static_cast<size_t>(lastReceived);
        } else {
            /* Communication failed. */
            THROW_THE_SOCKET_EXCEPTION;
        }

#endif /* _WIN32 */
    } while (forceReceive && (totalReceived < cntBytes) && (lastReceived > 0));

#ifdef _WIN32
    if (!::WSACloseEvent(overlapped.hEvent)) {
        THROW_THE_SOCKET_EXCEPTION;
    }
#endif /*_WIN32 */

    // Raise special exception if peer node disconnected gracefully.
    if (lastReceived == 0) {
        throw disconnected_exception(__FILE__, __LINE__);
    }

    return totalReceived;
}


/*
 * the::system::net::socket::receive
 */
size_t the::system::net::socket::receive(end_point_type& outFromAddr,
            void *outData, const size_t cntBytes,
            const timeout_type timeout, const int flags) {
    THE_STACK_TRACE;
    size_t lastReceived = 0;    // The number of bytes actually received.
#ifdef THE_WINDOWS
    WSAOVERLAPPED overlapped;   // Overlap structure for asynchronous recv().
    WSABUF wsaBuf;              // Buffer for WSA output.
    DWORD errorCode = 0;        // WSA error during last operation.
    DWORD inOutFlags = flags;   // Flags for WSA.
    DWORD size = cntBytes;      // Number of bytes to receive/received.
    INT fromLen = static_cast<int>(outFromAddr.size());

    the::zero_memory(&wsaBuf);
    wsaBuf.buf = static_cast<char *>(outData);
    wsaBuf.len = static_cast<u_long>(cntBytes);
#else /* THE_WINDOWS */
    socklen_t fromLen = static_cast<socklen_t>(outFromAddr.size());
#endif /* THE_WINDOWS */

    /* Process the timeout if any. */
    this->check_read(timeout);

#ifdef THE_WINDOWS
    if ((overlapped.hEvent = ::WSACreateEvent()) == WSA_INVALID_EVENT) {
        THROW_THE_SOCKET_EXCEPTION;
    }

    if (::WSARecvFrom(this->handle, &wsaBuf, 1, &size, &inOutFlags, 
            static_cast<sockaddr *>(outFromAddr), &fromLen, &overlapped, NULL) 
            != 0) {
        if ((errorCode = ::WSAGetLastError()) != WSA_IO_PENDING) {
            ::WSACloseEvent(overlapped.hEvent);
            THROW_THE_EXCEPTION(socket_exception, errorCode);
        }
        THE_NETWORKING_TRACE_DEBUG("Overlapped socket I/O pending...\n");
        if (!::WSAGetOverlappedResult(this->handle, &overlapped, &size, TRUE,
                &inOutFlags)) {
            ::WSACloseEvent(overlapped.hEvent);
            THROW_THE_SOCKET_EXCEPTION;
        }
        lastReceived = static_cast<size_t>(size);
    }

    if (!::WSACloseEvent(overlapped.hEvent)) {
        THROW_THE_SOCKET_EXCEPTION;
    }

#else /* THE_WINDOWS */
    TEMP_FAILURE_RETRY(lastReceived = ::recvfrom(this->handle,
        outData, static_cast<int>(cntBytes), flags,
        static_cast<sockaddr *>(outFromAddr), &fromLen));
    if (lastReceived == SOCKET_ERROR) {
        THROW_THE_SOCKET_EXCEPTION;
    }

#endif /* THE_WINDOWS */

    // Raise special exception if peer node disconnected gracefully.
    if (lastReceived == 0) {
        throw disconnected_exception(__FILE__, __LINE__);
    }

    return lastReceived;
}


/*
 * the::system::net::socket::select
 */
the::system::net::socket::select_mask the::system::net::socket::select(
        const select_mask mask, const timeout_type timeout) {
    THE_STACK_TRACE;
    int n = 0;              // Highest descriptor in sets + 1.
    fd_set readSet;         // Set of sockets to check for readability.
    fd_set writeSet;        // Set of sockets to check for writability.
    fd_set exceptSet;       // Set of sockets to check for error states.
    fd_set *rs = nullptr;   // Pointer to 'readSet' if enabled.
    fd_set *ws = nullptr;   // Pointer to 'writeSet' if enabled.
    fd_set *es = nullptr;   // Pointer to 'exceptSet' if enabled.
    struct timeval to;      // Timeout for readability check.
    int retval = 0;         // Holds bits of events that have been raised.

    /* Initialise socket sets and timeout structure. */
    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);
    FD_ZERO(&exceptSet);

    if ((mask & select_mask::read) != 0) {
        rs = &readSet;
        FD_SET(this->handle, rs);
    }
    if ((mask & select_mask::write) != 0) {
        ws = &writeSet;
        FD_SET(this->handle, ws);
    }
    if ((mask & select_mask::exception) != 0) {
        es = &exceptSet;
        FD_SET(this->handle, es);
    }

    to.tv_sec = timeout / 1000;
    to.tv_usec = (timeout % 1000) * 1000;

    /* Wait for the socket to become readable. */
#ifndef THE_WINDOWS
    n = this->handle + 1;  // Windows does not need 'n' and will ignore it.
#endif /* !THE_WINDOWS*/
    if (TEMP_FAILURE_RETRY(::select(n, rs, ws, es, &to)) == SOCKET_ERROR) {
        THROW_THE_SOCKET_EXCEPTION;
    }

    /* Collect the results. */
    if ((rs != nullptr) && FD_ISSET(this->handle, rs)) {
        retval |= select_mask::read;
    }
    if ((ws != nullptr) && FD_ISSET(this->handle, ws)) {
        retval |= select_mask::write;
    }
    if ((es != nullptr) && FD_ISSET(this->handle, es)) {
        retval |= select_mask::exception;
    }

    return static_cast<select_mask>(retval);
}


/*
 * the::system::net::socket::send
 */
size_t the::system::net::socket::send(const void *data, const size_t cntBytes,
    const timeout_type timeout, const int flags) {
    THE_STACK_TRACE;
    this->check_write(timeout);
    return this->send(data, cntBytes, false, flags);
}


/*
 * the::system::net::socket::send
 */
size_t the::system::net::socket::send(const void *data, const size_t cntBytes,
        const bool forceSend, const int flags) {
    THE_STACK_TRACE;
    size_t totalSent = 0;       // # of bytes totally sent.
    int lastSent = 0;           // # of bytes sent during last send() call.
#ifdef _WIN32
    WSAOVERLAPPED overlapped;   // Overlap structure for asynchronous recv().
    WSABUF wsaBuf;              // Buffer for WSA output.
    DWORD errorCode = 0;        // WSA error during last operation.
    DWORD inOutFlags = flags;   // Flags for WSA.

    the::zero_memory(&wsaBuf);
    wsaBuf.buf = const_cast<char *>(static_cast<const char *>(data));
    wsaBuf.len = static_cast<u_long>(cntBytes);

    if ((overlapped.hEvent = ::WSACreateEvent()) == WSA_INVALID_EVENT) {
        THROW_THE_SOCKET_EXCEPTION;
    }
#endif /* _WIN32 */

    do {
#ifdef _WIN32
        if (::WSASend(this->handle, &wsaBuf, 1, reinterpret_cast<DWORD *>(
                &lastSent), flags, &overlapped, NULL) != 0) {
            if ((errorCode = ::WSAGetLastError()) != WSA_IO_PENDING) {
                ::WSACloseEvent(overlapped.hEvent);
                THROW_THE_EXCEPTION(socket_exception, errorCode);
            }
            THE_NETWORKING_TRACE_DEBUG("Overlapped socket I/O pending...\n");
            if (!::WSAGetOverlappedResult(this->handle, &overlapped,
                    reinterpret_cast<DWORD *>(&lastSent), TRUE, &inOutFlags)) {
                ::WSACloseEvent(overlapped.hEvent);
                THROW_THE_SOCKET_EXCEPTION;
            }
        }
        totalSent += static_cast<size_t>(lastSent);
        wsaBuf.buf += lastSent;
        wsaBuf.len -= lastSent;

#else /* _WIN32 */
        TEMP_FAILURE_RETRY(lastSent = ::send(this->handle,
            static_cast<const char *>(data), 
            static_cast<int>(cntBytes - totalSent), flags));

        if ((lastSent >= 0) && (lastSent != SOCKET_ERROR)) {
            totalSent += static_cast<size_t>(lastSent);
        } else {
            THROW_THE_SOCKET_EXCEPTION;
        }
#endif /* _WIN32 */

    } while (forceSend && (totalSent < cntBytes) && (lastSent > 0));

#ifdef _WIN32
    if (!::WSACloseEvent(overlapped.hEvent)) {
        THROW_THE_SOCKET_EXCEPTION;
    }
#endif /*_WIN32 */

    // Raise special exception if peer node disconnected gracefully.
    if (lastSent == 0) {
        throw disconnected_exception(__FILE__, __LINE__);
    }

    return totalSent;
}



/*
 * the::system::net::socket::send
 */
size_t the::system::net::socket::send(const end_point_type& toAddr,
        const void *data, const size_t cntBytes,
        const timeout_type timeout, const int flags) {
    THE_STACK_TRACE;
    size_t lastSent = 0;        // The number of bytes actually sent.
    const sockaddr *to = static_cast<const sockaddr *>(toAddr);
#ifdef THE_WINDOWS
    WSAOVERLAPPED overlapped;   // Overlap structure for asynchronous sendto().
    WSABUF wsaBuf;              // Buffer for WSA output.
    DWORD errorCode = 0;        // WSA error during last operation.
    DWORD inOutFlags = flags;   // Flags for WSA.
    DWORD size = cntBytes;      // Number of bytes to receive/received.
    INT toLen = static_cast<INT>(toAddr.size());

    the::zero_memory(&wsaBuf);
    wsaBuf.buf = const_cast<char *>(static_cast<const char *>(data));
    wsaBuf.len = static_cast<u_long>(cntBytes);
#else /* THE_WINDOWS */
    socklen_t toLen = static_cast<socklen_t>(toAddr.size());
#endif /* THE_WINDOWS */

    /* Process the timeout if any. */
    this->check_write(timeout);

#ifdef THE_WINDOWS
    if ((overlapped.hEvent = ::WSACreateEvent()) == WSA_INVALID_EVENT) {
        THROW_THE_SOCKET_EXCEPTION;
    }

    if (::WSASendTo(this->handle, &wsaBuf, 1, reinterpret_cast<DWORD *>(
        &lastSent), flags, to, toLen, &overlapped, NULL) != 0) {
        if ((errorCode = ::WSAGetLastError()) != WSA_IO_PENDING) {
            ::WSACloseEvent(overlapped.hEvent);
            THROW_THE_EXCEPTION(socket_exception, errorCode);
        }
        THE_NETWORKING_TRACE_DEBUG("Overlapped socket I/O pending...\n");
        if (!::WSAGetOverlappedResult(this->handle, &overlapped, &size, TRUE,
                &inOutFlags)) {
            ::WSACloseEvent(overlapped.hEvent);
            THROW_THE_SOCKET_EXCEPTION;
        }
        lastSent = static_cast<size_t>(size);
    }

    if (!::WSACloseEvent(overlapped.hEvent)) {
        THROW_THE_SOCKET_EXCEPTION;
    }

#else /* THE_WINDOWS */
    TEMP_FAILURE_RETRY(lastSent = ::sendto(this->handle,
        static_cast<const char *>(data), static_cast<int>(cntBytes), flags,
        to, toLen));
    if (lastSent == SOCKET_ERROR) {
        THROW_THE_SOCKET_EXCEPTION;
    }

#endif /* THE_WINDOWS */

    // Raise special exception if peer node disconnected gracefully.
    if (lastSent == 0) {
        throw disconnected_exception(__FILE__, __LINE__);
    }

    return lastSent;
}


/*
 * the::system::net::socket::shutdown
 */
void the::system::net::socket::shutdown(const shutdown_manifest how) {
    THE_STACK_TRACE;
    if (TEMP_FAILURE_RETRY(::shutdown(this->handle, how)) == SOCKET_ERROR) {
        THROW_THE_SOCKET_EXCEPTION;
    }
}


/*
 * the::system::net::socket::operator =
 */
the::system::net::socket& the::system::net::socket::operator =(
        socket&& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        this->handle = rhs.handle;
        rhs.handle = socket::invalid_handle;
    }
    return *this;
}


/*
 * the::system::net::socket::check_read
 */
void the::system::net::socket::check_read(const timeout_type timeout) {
    THE_STACK_TRACE;
    THE_ASSERT(this->is_valid());
    if (timeout != socket::timeout_infinite) {
        THE_NETWORKING_TRACE_DEBUG("Waiting for %d ms for socket to become "
            "ready for reading...\n", timeout);
        if (this->select(select_mask::read, timeout) != select_mask::read) {
            THROW_THE_EXCEPTION(socket_exception,
                socket_exception::error_timeout);
        }
    }
}


/*
 * the::system::net::socket::check_write
 */
void the::system::net::socket::check_write(const timeout_type timeout) {
    THE_STACK_TRACE;
    THE_ASSERT(this->is_valid());
    if (timeout != socket::timeout_infinite) {
        THE_NETWORKING_TRACE_DEBUG("Waiting for %d ms for socket to become "
            "ready for writing...\n", timeout);
        if (this->select(select_mask::write, timeout) != select_mask::write) {
            THROW_THE_EXCEPTION(socket_exception,
                socket_exception::error_timeout);
        }
    }
}
