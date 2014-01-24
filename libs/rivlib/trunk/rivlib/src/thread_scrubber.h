/*
 * thread_scrubber.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_THREAD_SCRUBBER_H_INCLUDED
#define VICCI_RIVLIB_THREAD_SCRUBBER_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include <memory>
#include <vector>
#include "rivlib/node_base.h"
#include "the/assert.h"
#include "the/system/threading/auto_lock.h"
#include "the/system/threading/thread.h"
#include "the/system/threading/runnable.h"
#include "the/system/threading/critical_section.h"


namespace eu_vicci {
namespace rivlib {

    /*
     * Utility class to clean up thread-related objects asynchronously
     */
    class thread_scrubber : the::system::threading::runnable {
    public:

        /**
         * Answer the only instance of the class
         */
        static thread_scrubber& instance(void);

        /**
         * Registers an object on creation
         *
         * @param obj The pointer to the object
         * @param T The real class type of the object
         */
        template<class T>
        static void register_object(std::shared_ptr<node_base> obj) {
            the::system::threading::auto_lock<the::system::threading::critical_section>(instance().lock_obj);
            instance().cnt++;
        }

        /**
         * Enqueues an object for cleanup
         *
         * @param obj The pointer to the object
         * @param T The real class type of the object
         */
        template<class T>
        static void cleanup_object(std::shared_ptr<node_base> obj) {
            the::system::threading::auto_lock<the::system::threading::critical_section>(instance().lock_obj);
            THE_ASSERT(instance().worker != nullptr);

            instance().queue.push_back(new element<T>(obj));
            if (instance().terminating) {
                if (instance().worker->is_running()) {
                    instance().worker->join();
                }
                instance().terminating = false;
            }
            if (!instance().worker->is_running()) {
                instance().worker->start();
            }
        }

        /**
         * The thread worker function
         *
         * @return 0
         */
        virtual int run(void);

    private:

        /**
         * List element base class
         */
        class element_base {
        public:

            /** dtor */
            virtual ~element_base(void) {
                this->obj.reset();
            }

            /**
             * Answer whether the thread of this object is still running
             *
             * @return True if the thread of this object is still running
             */
            virtual bool is_running(void) const throw() = 0;

        protected:

            /**
             * Ctor
             *
             * @param obj The pointer to the object
             */
            element_base(std::shared_ptr<node_base> obj) : obj(obj) {
                // intentionally empty
            }

            /** The pointer to the object */
            std::shared_ptr<node_base> obj;

        };

        template<class T>
        class element: public element_base {
        public:

            /**
             * Ctor
             *
             * @param obj The pointer to the object
             */
            element(std::shared_ptr<node_base> obj) : element_base(obj) {
                // intentionally empty
            }

            /** dtor */
            virtual ~element(void) {
                this->obj.reset();
            }

            /**
             * Answer whether the thread of this object is still running
             *
             * @return True if the thread of this object is still running
             */
            virtual bool is_running(void) const throw() {
                T* o = dynamic_cast<T*>(this->obj.get());
                THE_ASSERT(o != nullptr);
                try {
                    return o->is_running();
                } catch(...) {
                    return false;
                }
            }

        };

        /** ctor */
        thread_scrubber(void);

        /** dtor */
        virtual ~thread_scrubber(void);

        /** The lock object */
        the::system::threading::critical_section lock_obj;

        /** The worker thread */
        the::system::threading::thread *worker;

        /** The number of registered threads */
        int64_t cnt;

        /** The element queue */
        std::vector<element_base*> queue;

        /** utility flag to aid the thread restart */
        bool terminating;

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */


#endif /* VICCI_RIVLIB_THREAD_SCRUBBER_H_INCLUDED */
