/*
 * rivlib
 * encoder/image_encoder_base.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#pragma once

#include "rivlib/image_data_types.h"
#include "element_node.h"
#include "encoder/image_request.h"
#include "data/slot.h"
#include "data/buffer.h"
#include "the/system/threading/thread.h"
#include "the/system/threading/runnable.h"
#include "the/system/threading/runnable_thread.h"
#include "the/delegate.h"
#include "the/blob.h"
#include "the/system/threading/critical_section.h"
#include "the/collections/fast_forward_list.h"
#include "the/system/threading/event.h"


namespace eu_vicci {
namespace rivlib {
namespace encoder {

    /**
     * Base class for image encoders
     */
    class image_encoder_base : public element_node {
    public:

        /** ctor */
        image_encoder_base(void);

        /** dtor */
        virtual ~image_encoder_base(void);

        /**
         * Starts the encoding of new input data
         */
        void start_new_input_encoding(void);

        /**
         * Waits for the encoding of new input data to complete
         *
         * @param abort If set to true, the current encoding process will be aborted.
         */
        void wait_input_encoding(bool abort);

        /**
         * Answers whether or not the image encoder is currently working
         *
         * @return True if the encoder is currently working
         */
        bool is_input_encoding_running(void);

        /**
         * Requests encoded output data
         *
         * @param req The output request
         */
        void request_output(image_request::ptr req);

        /**
         * Removes all pending requests with the specified target
         *
         * @param cb The target callback function
         * @param ctxt The target callback context
         */
        void remove_pending_requests(image_request::output_callback cb, void* ctxt);

    protected:

        /**
         * Answer whether or not the encoder should terminate as fast as possible
         *
         * @return True if the encoder should terminate as fast as possible
         */
        inline bool encoder_should_terminate(void) const {
            return this->encoder_terminate;
        }

        /**
         * Performs the actual encoding
         *
         * @param data The raw input data
         *
         * @return The encoded input data
         */
        virtual data::buffer::shared_ptr encode(data::buffer::shared_ptr data) = 0;

    private:

        /** Utility runnable class */
        class runnable : public the::system::threading::runnable {
        public:

            /** Type for run delegates */
            typedef the::delegate<int> run_delegate;

            /** Ctor */
            runnable(void);

            /** Ctor */
            ~runnable(void);

            /**
             * Perform the work of a thread.
             *
             * @return The application dependent return code of the thread. This 
             *         must not be STILL_ACTIVE (259).
             */
            virtual int run(void);

            /**
             * Requests the runnable to be terminated
             *
             * @return graceful
             */
            virtual the::system::threading::thread::termination_behaviour on_thread_terminating(void) throw();

            /**
             * Sets the run delegate
             *
             * @param func The new run delegate
             */
            inline void set_run(run_delegate func) {
                this->run_func = func;
            }

            /**
             * Sets the termination event
             *
             * @param terminate_e The event to be set when the thread should be terminated
             */
            inline void set_terminate_event(the::system::threading::event *terminate_e) {
                this->terminate_e = terminate_e;
            }

            /**
             * Sets the termination flag
             *
             * @param terminate_b The bool to be set when the thread should be terminated
             */
            inline void set_terminate_flag(bool *terminate_b) {
                this->terminate_b = terminate_b;
            }

            /**
             * Gets the run delegate
             *
             * @return the run delegate
             */
            inline const run_delegate& get_run(void) const {
                return this->run_func;
            }

        private:

            /** The run delegate */
            run_delegate run_func;

            /** The event to be set when the thread should be terminated */
            the::system::threading::event *terminate_e;

            /** The bool to be set when the thread should be terminated */
            bool *terminate_b;

        };

        /**
         * Perform the work of the input encoding
         *
         * @return The application dependent return code of the thread. This 
         *         must not be STILL_ACTIVE (259).
         */
        int run_input_collector(void);

        /**
         * Signals that new input data is now available
         */
        inline void on_new_input_data(data::slot&, data::buffer::shared_ptr) {
            this->encoder_new_data_event.set();
#ifdef THE_LINUX
            the::system::threading::thread::yield();
            the::system::threading::thread::sleep(1);
#endif /* THE_LINUX */
        }

        /**
         * Perform the encoding of the input data
         *
         * @return The application dependent return code of the thread. This 
         *         must not be STILL_ACTIVE (259).
         */
        int run_encoder(void);

        /**
         * Signals that new encoded data is now available
         */
        inline void on_new_encoded_data(data::slot&, data::buffer::shared_ptr) {
            this->output_update_event.set();
#ifdef THE_LINUX
            the::system::threading::thread::yield();
            the::system::threading::thread::sleep(1);
#endif /* THE_LINUX */
        }

        /**
         * Performs the output management of encoded data
         *
         * @return The application dependent return code of the thread. This 
         *         must not be STILL_ACTIVE (259).
         */
        int run_output(void);

        /** The input data worker thread */
        the::system::threading::runnable_thread<runnable> input_worker;

        /** The event that new input data is available */
        the::system::threading::event input_new_data_event;

        /** The lock for the input data */
        the::system::threading::critical_section input_data_lock;

        /** Flag to abort the processing of the input_worker */
        bool input_worker_abort;

        /** Flag showing if the processing of the input_worker is computing */
        bool input_worker_running;

        /** Slot for the raw input data */
        data::slot raw_input;

        /** The worker performing the actual encoding */
        the::system::threading::runnable_thread<runnable> encoder_worker;

        /** Signal that new data is available for the encoder */
        the::system::threading::event encoder_new_data_event;

        /** flag to signal that the encoder should terminate */
        bool encoder_terminate;

        /** Slot for the encoded frame data */
        data::slot encoded_data;

        /** The worker sending the output data */
        the::system::threading::runnable_thread<runnable> output_worker;

        /** Event fired when new data for the output or new output requests are available */
        the::system::threading::event output_update_event;

        /** pending output requests */
        the::collections::fast_forward_list<image_request::ptr, the::system::threading::critical_section> out_reqs;

    };


} /* end namespace encoder */
} /* end namespace rivlib */
} /* end namespace eu_vicci */
