/*
 * include\the\system\net\socket.h
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
 * Socket.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Mueller (christoph.mueller@vis.uni-stuttgart.de). All rights reserved.
 */

#ifndef THE_SYSTEM_NET_SOCKET_H_INCLUDED
#define THE_SYSTEM_NET_SOCKET_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include <memory>

#ifdef THE_WINDOWS
#include <WinSock2.h>
#include <Windows.h>
#elif THE_LINUX
#include <arpa/inet.h>
#include <netinet/tcp.h>
#endif /* THE_WINDOWS */

#include "the/equatable.h"
#include "the/not_copyable.h"
#include "the/stack_trace.h"

#include "the/system/net/ip_end_point.h"
#include "the/system/net/protocol.h"
#include "the/system/net/protocol_family.h"
#include "the/system/net/socket_exception.h"
#include "the/system/net/socket_options.h"
#include "the/system/net/socket_type.h"
#include "the/system/net/shutdown_manifest.h"


namespace the {
namespace system {
namespace net {

    /**
     * This class implements a wrapper around Berkeley-style sockets.
     */
    class socket : public equatable<socket>, public not_copyable {

    public:

        /**
         * Type of the socket that is used for client end points.
         *
         * This typedef is part of the socket behaviour.
         */
        typedef socket client_type;

        /** Shared pointer to a constant socket. */
        typedef std::shared_ptr<const socket> const_shared_pointer_type;

        /**
         * The type of end point addresses used by the socket.
         *
         * This typedef is part of the socket behaviour.
         */
        typedef the::system::net::ip_end_point end_point_type;

        /** The type of the native socket handle. */
#if defined(THE_WINDOWS)
        typedef SOCKET native_handle_type;
#elif defined(THE_LINUX)
        typedef int native_handle_type;
#else /* defined(THE_WINDOWS) */
#error "the::system::net::socket must define native_handle_type!"
#endif /* defined(THE_WINDOWS) */

        /** This enumeration determines which events socket::select checks. */
        typedef enum select_mask_t {
            none = 0x0,         //< Nothing will be/is selected.
            read = 0x1,         //< Check/result for reading.
            write = 0x2,        //< Check/result for writing.
            exception = 0x4     //< Check/result for errors.
        } select_mask;

        /**
         * Type of the socket that is used for server end points.
         *
         * This typedef is part of the socket behaviour.
         */
        typedef socket server_type;

        /** Shared pointer to socket. */
        typedef std::shared_ptr<socket> shared_pointer_type;

        /** The type to specify timeouts. */
        typedef unsigned int timeout_type;

        /**
         * Cleanup after use of sockets.
         * 
         * This method does nothing on Linux. On  Windows, it calls 
         * WSACleanup(). Call this method after you finished using sockets in
         * the calling thread for releasing the resources associated with the
         * Winsock-DLL.
         *
         * It is safe to call cleanup() multiple times as long as startup() has
         * been called multiple times before.
         *
         * This method is part of the socket behaviour.
         *
         * @throws the::system::net::socket_exception In case of an error.
         */
        static void cleanup(void);

        static socket create(
            const the::system::net::protocol_family protocolFamily,
            const the::system::net::socket_type type,
            const the::system::net::protocol protocol);

        static inline socket create_tcp(
                const the::system::net::protocol_family protocolFamily) {
            THE_STACK_TRACE;
            return socket::create(protocolFamily, socket_type::stream,
                protocol::tcp);
        }

        static inline socket create_udp(
                const the::system::net::protocol_family protocolFamily) {
            THE_STACK_TRACE;
            return socket::create(protocolFamily, socket_type::datagram,
                protocol::udp);
        }

        /**
         * Initialise the use of sockets.
         *
         * This method does nothing on Linux. On Windows, this method calls
         * WSAStartup()  and initialises the Winsock-DLL for the calling thread.
         * You must call this method once before using any sockets.  Call
         * cleanup() after you finished the use of sockets.
         *
         * It is safe to call startup() multiple times as long as the same
         * number of cleanup() calls is made afterwards.
         *
         * This method is part of the socket behaviour.
         *
         * @throws the::system::net::socket_exception In case of an error.
         */
        static void startup(void);

        /** The invalid native handle value. */
        static const native_handle_type invalid_handle;

        /**
         * The maximum number of connections that can be accepted in listen().
         * This is the value of the SOMAXCONN constant.
         */
        static const int maximum_connections;

        /** Constant for specifying an infinite timeout. */
        static const timeout_type timeout_infinite;

        /**
         * Initialises a new instance.
         */
        inline socket(void) : equatable<socket>(), not_copyable(),
                handle(invalid_handle) {
            THE_STACK_TRACE;
        }

        /**
         * Move 'rhs' to this object.
         *
         * @param rhs The object to be moved.
         */
        socket(socket&& rhs);

        /**
         * Create socket wrapper from an existing handle.
         *
         * The existing handle will be invalidated as the object takes ownership
         * of it.
         *
         * @param handle The socket handle.
         */
        socket(native_handle_type&& handle);

        /** Dtor. */
        virtual ~socket(void);

        /**
         * Permit incoming connection attempt on the socket.
         *
         * Note for Linux: The implementation handles EINTR by itself and
         * retries the operation until it succeeds or fails with another
         * error.
         *
         * This method is part of the socket behaviour.
         *
         * @param outPeerAddress A pointer that will receive the address of the 
         *                       connecting entity, as known to the 
         *                       communications layer. The exact format of
         *                       the address is determined by the address 
         *                       family established when the socket was created.
         *
         * @return The accepted socket.
         *
         * @throws the::system::net::socket_exception If the operation fails.
         */
        socket accept(end_point_type& outPeerAddress);

        /**
         * Bind the socket to the specified address.
         *
         * This method is part of the socket behaviour.
         *
         * @param address The address to bind.
         *
         * @throws std::bad_cast If the end point is not compatible with the
         *                       socket.
         * @throws the::system::net::socket_exception If the operation fails.
         */
        void bind(const end_point_type& address);

        /**
         * This is a Linux-only method. It is not required nor supported on 
         * Windows. On Windows, the method has no effect.
         *
         * Bind this socket to a particular device like "eth0", as specified 
         * in 'name'.
         *
         * @param name The name of the device to bind to, e. g. "eth0".
         *
         * @throws SocketException If the operation fails.
         */
        //virtual void BindToDevice(const StringA& name);

        /**
         * Close the socket.
         *
         * If the socket is not open, i.e. not valid, this method must succeed,
         * too.
         *
         * This method is part of the socket behaviour.
         *
         * @throws the::system::net::socket_exception If the operation failed.
         */
        void close(void);

        /**
         * Connect to the specified socket address using this socket.
         *
         * Note for Linux: The implementation handles EINTR by itself and
         * retries the operation until it succeeds or fails with another
         * error.
         *
         * This method is part of the socket behaviour.
         *
         * @param address The address to connect to.
         *
         * @throws std::bad_cast If the end point is not compatible with the
         *                       socket.
         * @throws the::system::net::socket_exception If the operation fails.
         */
        void connect(const end_point_type& address);

        /**
         * Determines whether this object and 'rhs' are equal.
         *
         * @param rhs The object to be compared.
         *
         * @return true if this object and 'rhs' are equal, false otherwise.
         */
        virtual bool equals(const socket& rhs) const;

        /**
         * Set a socket option.
         *
         * @param opt The option to be set. The actual value of the option is
         *            irrelevant and will be overwritten by the method.
         *
         * @return 'opt'.
         *
         * @throws the::system::net::socket_exception If the operation fails.
         */
        template<int L, int N, class T>
        socket_option<L, N, T>& get_option(socket_option<L, N, T>& opt) const;

        /**
         * Performs a graceful TCP client disconnect sequence. 
         *
         * This method must only be called on TCP sockets! It is intended
         * for sockets that have the linger option not enabled.
         * It performs a shutdown of the send channel and waits for any pending 
         * data to receive. The caller should close the socket afterwards if
         * 'isClose' is not set. Note that this method is blocking.
         *
         * See http://msdn.microsoft.com/en-us/library/ms738547(VS.85).aspx for
         * more information.
         *
         * In case the operation fails and 'isClose' is set, it is guaranteed 
         * that Close() is called in any case. However, the Close() call may
         * fail itself.
         *
         * @param isClose If true, the socket will be closed once FD_CLOSE was
         *                signaled. Otherwise, the method will return without
         *                closing the socket after it was signaled.
         *
         * @throws SocketException In case the operation fails.
         */
        //void GracefulDisconnect(const bool isClose);

        ///**
        // * Send an I/O Control message to the socket (Windows only).
        // *
        // * @param ioControlCode    The I/O control code. See WSAIoctl function
        // *                         in winsock2 documentation for possible codes.
        // * @param inBuffer         Pointer to the input buffer.
        // * @param cntInBuffer      Size of 'inBuffer' in bytes.
        // * @param outBuffer        Pointer to the output buffer.
        // * @param cntOutBuffer     Size of 'outBuffer' in bytes.
        // * @param outBytesReturned Bytes that have actually been returned into
        // *                         'outBuffer'.
        // *
        // * @throws SocketException If the operation fails.
        // */
        //// TODO: Linux IOCTLs?
        //void IOControl(const DWORD ioControlCode, void *inBuffer, 
        //    const DWORD cntInBuffer, void *outBuffer, const DWORD cntOutBuffer,
        //    DWORD& outBytesReturned);

        /**
         * Answer whether the socket is valid. Only use sockets that return true
         * in this method.
         *
         * @return true, if the socket is valid, false otherwise.
         */
        inline bool is_valid(void) const {
            THE_STACK_TRACE;
            return (this->handle != invalid_handle);
        }

        /**
         * Place the socket in a state in which it is listening for an incoming
         * connection.
         *
         * This method is part of the socket behaviour.
         *
         * @param backlog Maximum length of the queue of pending connections.
         *
         * @throws the::system::net::socket_exception If the operation fails.
         */
        void listen(const int backlog = maximum_connections);

        /**
         * Answer the address the socket is locally bound to (this equals to the
         * socket API function getsockname()).
         *
         * This method is part of the socket behaviour.
         *
         * @return The address of the local end point.
         *
         * @throws the::system::net::socket_exception If the operation fails.
         */
        end_point_type local_end_point(void) const;

        /**
         * Answer the address the socket is locally bound to (this equals to the
         * socket API function getsockname()).
         *
         * This method is part of the socket behaviour.
         *
         * @return The address of the local end point.
         *
         * @throws the::system::net::socket_exception If the operation fails.
         */
        end_point_type local_ip_end_point(void) const;

        /**
         * Open a new socket.
         *
         * If the socket is already open, it will be closed before it is 
         * re-opened.
         *
         * This method is part of the socket behaviour.
         *
         * @param protocolFamily The protocol family.
         * @param type           The type of the socket.
         * @param protocol       The protocol to use.
         *
         * @throws the::system::net::socket_exception If the operation fails.
         */
        void open(const the::system::net::protocol_family protocolFamily,
            const the::system::net::socket_type type,
            const the::system::net::protocol protocol);


        /**
         * Receives 'cntBytes' from the socket and saves them to the memory
         * designated by 'outData'.
         *
         * 'outData' must be large enough to receive at least 'cntBytes'.
         *
         * Note for Linux: The implementation handles EINTR by itself and 
         * retries the operation until it succeeds or fails with another
         * error.
         *
         * @param outData      The buffer to receive the data. The caller must
         *                     allocate this memory and remains owner.
         * @param cntBytes     The number of bytes to receive.
         * @param timeout      A timeout in milliseconds. A value less than 1 
         *                     specifies an infinite timeout. If the operation 
         *                     timeouts, an exception will be thrown.
         * @param flags        The flags that specify the way in which the call 
         *                     is made.
         *
         * @return The number of bytes actually received.
         *
         * @throws the::system::net::socket_exception If the operation fails.
         * @throws the::system::net::disconnected_exception If the peer node
         *                                                  disconnected
         *                                                  gracefully (no data
         *                                                  have been received
         *                                                  in this case).
         */
        size_t receive(void *outData, const size_t cntBytes,
            const timeout_type timeout = timeout_infinite,
            const int flags = 0);

        // TODO: Documentation
        size_t receive(void *outData, const size_t cntBytes,
            const bool forceReceive, const int flags = 0);

        // TODO: Documentation
        size_t receive(end_point_type& outFromAddr,
            void *outData, const size_t cntBytes,
            const timeout_type timeout = timeout_infinite,
            const int flags = 0);

        // TODO: Documentation
        select_mask select(const select_mask mask,
            const timeout_type timeout = timeout_infinite);

        /**
         * Send 'cntBytes' from the location designated by 'data' using this
         * socket.
         *
         * Note for Linux: The implementation handles EINTR by itself and
         * retries the operation until it succeeds or fails with another
         * error.
         *
         * @param data      The data to be sent. The caller remains owner of the
         *                  memory.
         * @param cntBytes  The number of bytes to be sent. 'data' must contain
         *                  at least this number of bytes.
         * @param timeout   A timeout in milliseconds. A value less than 1 
         *                  specifies an infinite timeout. If the operation 
         *                  timeouts, an exception will be thrown.
         * @param flags     The flags that specify the way in which the call is 
         *                  made.
         *
         * @return The number of bytes acutally sent.
         *
         * @throws the::system::net::socket_exception If the operation fails.
         */
        size_t send(const void *data, const size_t cntBytes,
            const timeout_type timeout = timeout_infinite,
            const int flags = 0);

        // TODO: Documentation
        size_t send(const void *data, const size_t cntBytes,
            const bool forceSend, const int flags = 0);

        // TODO: Documentation
        size_t send(const end_point_type& toAddr,
            const void *data, const size_t cntBytes,
            const timeout_type timeout = timeout_infinite,
            const int flags = 0);

        /**
         * Set a socket option.
         *
         * @param opt The option to be set.
         *
         * @throws the::system::net::socket_exception If the operation fails.
         */
        template<int L, int N, class T>
        void set_option(const socket_option<L, N, T>& opt) const;

        /**
         * Disable send or receive operations on the socket.
         *
         * This method is part of the socket behaviour.
         *
         * @param how What to shutdown.
         *
         * @throws the::system::net::socket_exception If the operation fails.
         */
        void shutdown(const shutdown_manifest how = shutdown_manifest::both);

        /**
         * Move values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        socket& operator =(socket&& rhs);

    private:

        // TODO: Documentation
        void check_read(const timeout_type timeout);

        // TODO: Documentation
        void check_write(const timeout_type timeout);

        //// MSVC11 generates C2248 and does not support function delete so far.
        //inline socket& operator =(const socket& rhs) {
        //    THROW_THE_INVALID_OPERATION_EXCEPTION;
        //}

        /** The native socket handle. */
        native_handle_type handle;
    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */


/*
 * the::system::net::socket::get_option
 */
template<int L, int N, class T>
the::system::net::socket_option<L, N, T>& the::system::net::socket::get_option(
        socket_option<L, N, T>& opt) const {
    THE_STACK_TRACE;
    typename the::system::net::socket_option<L, N, T>::option_type o;
    typename the::system::net::socket_option<L, N, T>::size_type s = opt.size;
    if (::getsockopt(this->handle, opt.level, opt.name,
            reinterpret_cast<char *>(&o), &s) == 0) {
        opt = o;
        return opt;
    } else {
        THROW_THE_SOCKET_EXCEPTION;
    }
}


/*
 * the::system::net::socket::set_option
 */
template<int L, int N, class T>
void the::system::net::socket::set_option(
        const socket_option<L, N, T>& opt) const {
    THE_STACK_TRACE;
    typename the::system::net::socket_option<L, N, T>::option_type o;
    typename the::system::net::socket_option<L, N, T>::size_type s = opt.size;
    if (::setsockopt(this->handle, opt.level, opt.name,
            reinterpret_cast<const char *>(&o), s) != 0) {
        THROW_THE_SOCKET_EXCEPTION;
    }
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_SOCKET_H_INCLUDED */
