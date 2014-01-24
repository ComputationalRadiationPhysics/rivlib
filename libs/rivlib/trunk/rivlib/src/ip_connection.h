/*
 * rivlib
 * ip_connection.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_IP_CONNECTION_H_INCLUDED
#define VICCI_RIVLIB_IP_CONNECTION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "element_node.h"
#include "encoder/image_encoder_base.h"
#include "the/system/threading/runnable.h"
#include "the/system/threading/thread.h"
#include "vislib/SmartRef.h"
#include "vislib/TcpCommChannel.h"


namespace eu_vicci {
namespace rivlib {


    /**
     * Class of (generic) ip connections
     */
    class ip_connection : public element_node, public the::system::threading::runnable {
    public:

        /** the used comm_channel type */
        typedef vislib::SmartRef<vislib::net::TcpCommChannel> comm_channel_type;

        /**
         * ctor
         *
         * @param comm The comm channel
         */
        ip_connection(comm_channel_type comm);

        /** dtor */
        virtual ~ip_connection(void);

        /**
         * The thread will call this method if it was requested to terminate by
         * the user.
         *
         * @return termination_behaviour::graceful or termination_behaviour::forceful.
         */
        virtual the::system::threading::thread::termination_behaviour on_thread_terminating(void) throw();

        /**
         * removes the object when the thread has terminated
         *
         * @param reason Not used
         */
        virtual void on_thread_terminated(const the::system::threading::thread::exit_reason reason) throw();

        /**
         * Perform the work of a thread.
         *
         * @return The application dependent return code of the thread. This 
         *         must not be STILL_ACTIVE (259).
         */
        virtual int run(void);

        /**
         * Answer wether or not the thread is running
         *
         * @return True if the thread is running
         */
        inline bool is_running(void) const {
            return (this->worker_thread != nullptr) ? this->worker_thread->is_running() : false;
        }

        /**
         * Sends a user message
         *
         * @param id The message ID. Should be at least RIVLIB_USERMSG
         * @param size The size of the message data
         * @param data The message data
         */
        void send_message(unsigned int id, unsigned int size, const char* data);

    protected:

        /**
         * Called to fire the 'core_discovered' event
         */
        virtual void on_core_discovered(void);

        /**
         * Called to fire the 'core_lost' event
         */
        virtual void on_core_lost(void);

    private:

        /**
         * Callback type used to asynchronously request encoder output
         *
         * @param data The encoded data
         * @param ctxt The user-defined context (this-ptr)
         */
        static void send_image_data(const data::buffer::shared_ptr data, void *ctxt);

        /**
         * Finds the requested provider object
         *
         * @param path The path to the object
         *
         * @return The found object or nullptr
         */
        api_ptr_base find_provider(const std::string& path);

        /**
         * The receiver message loop for control_channel connections
         *
         * @param path The path to the object
         */
        void ctrl_chan_receiver(const std::string& path);

        /**
         * The receiver message loop for data_channel connections
         *
         * @param path The path to the object
         * @param query The query
         */
        void data_chan_receiver(const std::string& path, const std::string& query);

        /**
         * The receiver message loop for image_data_channel connections
         *
         * @param encoder The image data encoder
         */
        void image_encoder_receiver(encoder::image_encoder_base* encoder);

        /** The comm channel */
        comm_channel_type comm;

        /** The thread object */
        the::system::threading::thread *worker_thread;

        /** flag to aid the cleanup process */
        bool is_terminating;

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */


#endif /* VICCI_RIVLIB_ELEMENT_NODE_H_INCLUDED */
