/*
 * connection_base_impl.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_CONNECTION_BASE_IMPL_H_INCLUDED
#define VICCI_RIVLIB_CONNECTION_BASE_IMPL_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */


#include "rivlib/control_connection.h"
#include "the/string.h"
#include "the/system/threading/critical_section.h"
#include "the/system/threading/thread.h"
#include "the/system/threading/runnable.h"
#include "vislib/SmartRef.h"
#include "vislib/TcpCommChannel.h"
#include <vector>
#include <algorithm>
#include "rivlib/ip_communicator.h"
#include "uri_utility.h"
#include "rivlib/ip_utilities.h"
#include "the/math/functions.h"
#include "the/text/string_builder.h"
#include "vislib/PeerDisconnectedException.h"
#include "vislib/IPCommEndPoint.h"
#include "vislib/Trace.h"


namespace eu_vicci {
namespace rivlib {

    /**
     * Implementation of the core class
     *
     * @param T The class of the owning object
     */
    template<class T>
    class connection_base_impl : public the::system::threading::runnable {
    public:

        /** dtor */
        virtual ~connection_base_impl(void);

        /**
         * Sets the owner of this object
         *
         * @param owner The owner of this object
         */
        void set_owner(T* owner);

        /**
         * Registers a listener at this object. The memory ownership of the
         * listener object is not taken by this control_connection object,
         * i.e. the caller must ensure that the provided pointer to the
         * listener remains valid as long as it is registered with this
         * control_connection object.
         *
         * @param l The listener to be registered.
         */
        virtual void add_listener(typename T::listener* l);

        /**
         * Removes a listener from this object.
         *
         * @param l The listener to be removed
         */
        virtual void remove_listener(typename T::listener* l);

        /**
         * Removes all listeners from this object
         */
        virtual void remove_all_listeners(void);

        /**
         * Connects to a riv provider service
         *
         * @param uri The uri to connect to (using riv-schema)
         */
        virtual void connect(const char *uri);

        /**
         * Connects to a riv provider service
         *
         * @param uri The uri to connect to (using riv-schema)
         */
        virtual void connect(const wchar_t *uri);

        /**
         * Disconnects from the currently connected riv provider service
         *
         * @param wait If true, the method will return only after the
         *            connection has been closed
         */
        virtual void disconnect(bool wait = false);

        /**
         * Answer the connection status of this channel
         *
         * @return The connection status
         */
        virtual connection_base::status get_status(void) const;

        /**
         * Answer the uri of the connected riv provider service
         *
         * @return The uri of the connected riv provider service
         */
        virtual const char *get_uri_astr(void) const;

        /**
         * Answer the uri of the connected riv provider service
         *
         * @return The uri of the connected riv provider service
         */
        virtual const wchar_t *get_uri_wstr(void) const;

        /**
         * Locks this node
         */
        virtual void lock(void);

        /**
         * Unlocks this node
         */
        virtual void unlock(void);

        /**
         * The thread will call this method if it was requested to terminate by
         * the user.
         *
         * @return termination_behaviour::graceful or termination_behaviour::forceful.
         */
        virtual the::system::threading::thread::termination_behaviour on_thread_terminating(void) throw();

        /**
         * Perform the work of a thread.
         *
         * @return The application dependent return code of the thread. This 
         *         must not be STILL_ACTIVE (259).
         */
        virtual int run(void);

    protected:

        /** ctor */
        connection_base_impl(void);

        /**
         * Sends some data to the server
         *
         * @param data The data to send
         * @param size The size of 'data' in bytes
         *
         * @return The number of bytes sent
         */
        size_t send(const void *data, size_t size);

        /**
         * Receives some data from the server
         *
         * @param data Points to the memory to receive the data
         * @param size The size of 'data' in bytes
         *
         * @return The number of bytes actually received
         */
        size_t receive(void *data, size_t size);

        /**
         * Checks if the uri conforms to the specific channel type
         *
         * @param scheme The scheme
         * @param username The user name
         * @param host The host name or address
         * @param is_host_v6 True if the host address is IPv6
         * @param host_port The port
         * @param path The request path
         * @param query The request query
         * @param fragment The request fragment
         *
         * @throw the::exception if uri does not conform to the specific channel type
         */
        virtual void check_uri(std::string &scheme,
            std::string& username, std::string& host,
            bool &is_host_v6, unsigned short& host_port,
            std::string& path, std::string& query,
            std::string& fragment) = 0;

        /**
         * Evaluates the answer code and throws an exception if the connection
         * is not accepted.
         *
         * @param code The answer code sent from the server
         *
         * @throw the::exception if the connection is not accepted.
         */
        virtual void evaluate_answer(unsigned short code) = 0;

        /**
         * The implementation of the communication core functionality
         */
        virtual void communication_core(void) = 0;


        /**
         * Gets the registered listeners
         *
         * @return The registered listeners
         */
        inline const std::vector<typename T::listener*>& get_listeners(void) {
            return this->listeners;
        }

        /**
         * Answer the owner of the object
         *
         * @return The owner of the object (Never nullptr)
         */
        inline T *get_owner(void) const {
            THE_ASSERT(this->owner != nullptr);
            return this->owner;
        }

    private:

        /**
         * Closes the comm channel
         */
        void close_comm(void);

        /**
         * Informs listeners that the comm channel is established
         */
        void on_comm_connected(void);

        /**
         * Informs the listeners about an error
         *
         * @param msg The error message
         */
        void on_comm_error(const char *msg);

        /** The owning connection */
        T *owner;

        /** The uri */
        the::astring uri_astr;

        /** The uri */
        the::wstring uri_wstr;

        /** The connection status */
        connection_base::status stat;

        /** The thread lock object */
        the::system::threading::critical_section lock_obj;

        /** The registered listeners */
        std::vector<typename T::listener*> listeners;

        /** The thread object */
        the::system::threading::thread *worker_thread;

        /** The communcation channel */
        vislib::SmartRef<vislib::net::TcpCommChannel> comm;

    };



    /*
     * connection_base_impl<T>::~connection_base_impl
     */
    template<class T>
    connection_base_impl<T>::~connection_base_impl(void) {
        if (this->stat != connection_base::status::not_connected) {
            this->disconnect(true);
        }

        this->listeners.clear(); // do not delete entries

        THE_ASSERT(!this->worker_thread->is_running());
        the::safe_delete(this->worker_thread);

        THE_ASSERT(this->comm.IsNull());

        this->owner = nullptr; // do not delete

        vislib::net::Socket::Cleanup();
    }


    /*
     * connection_base_impl<T>::set_owner
     */
    template<class T>
    void connection_base_impl<T>::set_owner(T* owner) {
        THE_ASSERT(this->owner == nullptr);
        THE_ASSERT(owner != nullptr);
        this->owner = owner;
    }


    /*
     * connection_base_impl<T>::add_listener
     */
    template<class T>
    void connection_base_impl<T>::add_listener(typename T::listener* l) {
        using namespace the::system::threading;
        auto_lock<critical_section>(this->lock_obj);
        typename std::vector<typename T::listener *>::iterator it
            = std::find(this->listeners.begin(), this->listeners.end(), l);
        if (it == this->listeners.end()) {
            this->listeners.push_back(l);
        }
    }


    /*
     * connection_base_impl<T>::remove_listener
     */
    template<class T>
    void connection_base_impl<T>::remove_listener(typename T::listener* l) {
        using namespace the::system::threading;
        auto_lock<critical_section>(this->lock_obj);
        typename std::vector<typename T::listener *>::iterator it
            = std::find(this->listeners.begin(), this->listeners.end(), l);
        if (it != this->listeners.end()) {
            this->listeners.erase(it);
        }
    }


    /*
     * connection_base_impl<T>::remove_all_listeners
     */
    template<class T>
    void connection_base_impl<T>::remove_all_listeners(void) {
        using namespace the::system::threading;
        auto_lock<critical_section>(this->lock_obj);
        this->listeners.clear();
    }


    /*
     * connection_base_impl<T>::connect
     */
    template<class T>
    void connection_base_impl<T>::connect(const char *uri) {
        this->lock_obj.lock();
        if (this->stat != connection_base::status::not_connected) {
            this->lock_obj.unlock();
            this->disconnect(true);
            this->lock_obj.lock();
        }
        THE_ASSERT(!this->worker_thread->is_running());
        this->uri_astr = uri;
        the::text::string_converter::convert(this->uri_wstr, this->uri_astr);
        this->stat = connection_base::status::connecting;
        this->worker_thread->start();
        this->lock_obj.unlock();
    }


    /*
     * connection_base_impl<T>::connect
     */
    template<class T>
    void connection_base_impl<T>::connect(const wchar_t *uri) {
        this->lock_obj.lock();
        if (this->stat != connection_base::status::not_connected) {
            this->lock_obj.unlock();
            this->disconnect(true);
            this->lock_obj.lock();
        }
        THE_ASSERT(!this->worker_thread->is_running());
        this->uri_wstr = uri;
        the::text::string_converter::convert(this->uri_astr, this->uri_wstr);
        this->stat = connection_base::status::connecting;
        this->worker_thread->start();
        this->lock_obj.unlock();
    }


    /*
     * connection_base_impl<T>::disconnect
     */
    template<class T>
    void connection_base_impl<T>::disconnect(bool wait) {
        this->lock_obj.lock();
        if (this->worker_thread->is_running()) {
            this->stat = connection_base::status::disconnecting;
            this->lock_obj.unlock();
            this->worker_thread->terminate(wait);
        } else {
            this->lock_obj.unlock();
        }
    }


    /*
     * connection_base_impl<T>::get_status
     */
    template<class T>
    connection_base::status connection_base_impl<T>::get_status(void) const {
        return this->stat;
    }


    /*
     * connection_base_impl<T>::get_uri_astr
     */
    template<class T>
    const char *connection_base_impl<T>::get_uri_astr(void) const {
        return this->uri_astr.c_str();
    }


    /*
     * connection_base_impl<T>::get_uri_wstr
     */
    template<class T>
    const wchar_t *connection_base_impl<T>::get_uri_wstr(void) const {
        return this->uri_wstr.c_str();
    }


    /*
     * connection_base_impl<T>::lock
     */
    template<class T>
    void connection_base_impl<T>::lock(void) {
        this->lock_obj.lock();
    }


    /*
     * connection_base_impl<T>::unlock
     */
    template<class T>
    void connection_base_impl<T>::unlock(void) {
        this->lock_obj.unlock();
    }


    /*
     * connection_base_impl<T>::on_thread_terminating
     */
    template<class T>
    the::system::threading::thread::termination_behaviour connection_base_impl<T>::on_thread_terminating(void) throw() {
        try {
            if (!this->comm.IsNull()) {
                this->comm->Close();
            }
        } catch(...) {
            return the::system::threading::thread::termination_behaviour::forceful;
        }

        return the::system::threading::thread::termination_behaviour::graceful;
    }


    /*
     * connection_base_impl<T>::run
     */
    template<class T>
    int connection_base_impl<T>::run(void) {
        using namespace vislib::net;

        std::string uri_scheme;
        std::string uri_username;
        std::string uri_host;
        bool uri_is_host_v6 = false;
        unsigned short uri_host_port;
        std::string uri_path;
        std::string uri_query;
        std::string uri_fragment;
        bool uri_is_port_set = false;

        uri_utility::parse_uri(this->uri_astr, uri_scheme, uri_username, uri_host, uri_is_host_v6, uri_host_port, uri_is_port_set, uri_path, uri_query, uri_fragment);
        if (uri_scheme.empty()) {
            uri_scheme = "RIV";
        }
        if (uri_host.empty()) {
            uri_host = "localhost";
        }
        if (!uri_is_port_set) {
            uri_host_port = ip_communicator::DEFAULT_PORT;
        }

        try {
            this->check_uri(uri_scheme, uri_username, uri_host, uri_is_host_v6, uri_host_port, uri_path, uri_query, uri_fragment);
        } catch(the::exception ex) {
            std::string msg("exception: ");
            msg += ex.get_msg_astr();
            this->on_comm_error(msg.c_str());
            return -1;
        } catch(...) {
            this->on_comm_error("Unexpected exception");
            return -1;
        }

        // std::string host;
        // the::text::astring_builder::format_to(host, "%s:%u", uri_host.c_str(), uri_host_port);

        this->comm = TcpCommChannel::Create(TcpCommChannel::FLAG_NODELAY);
        try {

//fprintf(stderr, "Arglhfitz: %s:%d (%d)\n", uri_host.c_str(), (int)uri_host_port, (int)uri_is_host_v6);
//vislib::Trace::GetInstance().SetLevel(vislib::Trace::LEVEL_ALL);

            this->comm->Connect(IPCommEndPoint::Create(
                uri_is_host_v6 ? IPCommEndPoint::IPV6 : IPCommEndPoint::IPV4,
                uri_host.c_str(), uri_host_port));

//fprintf(stderr, "hugo\n");

            rivlib::ip_handshake_id id;
            rivlib::ip_handshake_id id_ref;

            uri_scheme += "\x13\x57\x9B\xDF";

            ::memset(id_ref.id_str, 0, 8);
            ::memcpy(id_ref.id_str, uri_scheme.c_str(), the::math::minimum<size_t>(8, uri_scheme.length()));
            id_ref.tst_dword = 0x12345678;
            id_ref.tst_float = 2.71828175f;

            THE_ASSERT(sizeof(rivlib::ip_handshake_id) == 16);

            if (this->comm->Receive(&id, sizeof(rivlib::ip_handshake_id)) != sizeof(rivlib::ip_handshake_id)) {
                throw the::exception("handshake id truncated", __FILE__, __LINE__);
            }

            if (::memcmp(&id, &id_ref, sizeof(rivlib::ip_handshake_id)) != 0) {
                throw the::exception("Connection handshake failed on magic_id", __FILE__, __LINE__);
            }

            std::string request = uri_path; // syntax: "username@path?query#fragment"
            if (!uri_username.empty()) {
                request = uri_username + "@" + request;
            }
            if (!uri_query.empty()) {
                request += "?";
                request += uri_query;
            }
            if (!uri_fragment.empty()) {
                request += "#";
                request += uri_fragment;
            }

            unsigned int requestLen = static_cast<unsigned int>(request.length());
            this->comm->Send(&requestLen, 4);
            this->comm->Send(request.c_str(), requestLen);

            unsigned short answer;
            if (this->comm->Receive(&answer, 2) != 2) {
                throw the::exception("Request answer truncated", __FILE__, __LINE__);
            }

            this->evaluate_answer(answer);

            // now connection is established!
            this->on_comm_connected();

            this->communication_core();

            this->close_comm();

        } catch(the::exception ex) {
            std::string msg("exception: ");
            msg += ex.get_msg_astr();
            this->on_comm_error(msg.c_str());
            this->close_comm();
            return -1;

        } catch(vislib::net::PeerDisconnectedException) {
            // expected behaviour on shutdown, so this could be ok
            this->close_comm();

        } catch(vislib::Exception ex) {
            std::string msg("vislib exception: ");
            the::text::astring_builder::format_to(msg, "vislib exception: %s (at %s:%d)\n",
                ex.GetMsgA(), ex.GetFile(), ex.GetLine());
            if (ex.HasStack()) {
                msg += "Call stack: ";
                msg += ex.GetStack();
            }
            this->on_comm_error(msg.c_str());
            this->close_comm();
            return -1;

        } catch(...) {
            this->on_comm_error("unkown internal exception");
            this->close_comm();
            return -1;

        }

        return 0;
    }


    /*
     * connection_base_impl<T>::connection_base_impl
     */
    template<class T>
    connection_base_impl<T>::connection_base_impl(void) : runnable(), owner(nullptr),
            uri_astr(), uri_wstr(), stat(connection_base::status::not_connected),
            lock_obj(), listeners(), worker_thread(nullptr), comm() {
        vislib::net::Socket::Startup();
        this->worker_thread = new the::system::threading::thread(this);

    #if defined(DEBUG) || defined(_DEBUG)
        vislib::Trace::GetInstance().SetLevel(vislib::Trace::LEVEL_NONE);
    #endif /* defined(DEBUG) || defined(_DEBUG) */

    }


    /*
     * connection_base_impl<T>::send
     */
    template<class T>
    size_t connection_base_impl<T>::send(const void *data, size_t size) {
        return static_cast<size_t>(this->comm->Send(data, size));
    }


    /*
     * connection_base_impl<T>::receive
     */
    template<class T>
    size_t connection_base_impl<T>::receive(void *data, size_t size) {
        return static_cast<size_t>(this->comm->Receive(data, size));
    }


    /*
     * connection_base_impl<T>::close_comm
     */
    template<class T>
    void connection_base_impl<T>::close_comm(void) {
        using namespace the::system::threading;
        auto_lock<critical_section>(this->lock_obj);

        if (!this->comm.IsNull()) {
            try {
                this->comm->Close();
            } catch(...) {
            }
            this->comm.Release();

            this->stat = connection_base::status::not_connected;
            size_t s = this->listeners.size();
            for (size_t i = 0; i < s; i++) {
                THE_ASSERT(this->owner != nullptr);
                this->listeners[i]->on_disconnected(this->owner);
            }
        }
    }


    /*
     * connection_base_impl<T>::on_comm_connected
     */
    template<class T>
    void connection_base_impl<T>::on_comm_connected(void) {
        using namespace the::system::threading;
        auto_lock<critical_section>(this->lock_obj);

        this->stat = connection_base::status::connected;
        size_t s = this->listeners.size();
        for (size_t i = 0; i < s; i++) {
            THE_ASSERT(this->owner != nullptr);
            this->listeners[i]->on_connected(this->owner);
        }
    }


    /*
     * connection_base_impl<T>::on_comm_error
     */
    template<class T>
    void connection_base_impl<T>::on_comm_error(const char *msg) {
        using namespace the::system::threading;
        auto_lock<critical_section>(this->lock_obj);

        if (this->stat != connection_base::status::disconnecting) {
            this->stat = connection_base::status::not_connected;
        }
        size_t s = this->listeners.size();
        for (size_t i = 0; i < s; i++) {
            THE_ASSERT(this->owner != nullptr);
            this->listeners[i]->on_error(this->owner, msg);
        }
    }


} /* end namespace rivlib */
} /* end namespace eu_vicci */


#endif /* VICCI_RIVLIB_CONNECTION_BASE_IMPL_H_INCLUDED */
