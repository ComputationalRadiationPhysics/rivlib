/*
 * rivlib
 * data/slot.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#pragma once
#include "data/buffer.h"
#include "the/system/threading/auto_lock.h"
#include "the/system/threading/critical_section.h"
#include "the/delegate.h"
#include "the/multicast_delegate.h"


namespace eu_vicci {
namespace rivlib {
namespace data {


    /*
     * Slot holding a buffer
     */
    class slot {
    public:

        /** Delegate for update callbacks */
        typedef the::delegate<void, slot&, buffer::shared_ptr> update_delegate;

        /** Multicast-delegate for update callbacks */
        typedef the::multicast_delegate<update_delegate> update_multicast_delegate;

        /** Delegate for close callbacks */
        typedef the::delegate<void, slot&> close_delegate;

        /** Multicast-delegate for update callbacks */
        typedef the::multicast_delegate<close_delegate> close_multicast_delegate;

        /** Ctor */
        slot(void);

        /** Dtor */
        ~slot(void);

        /**
         * Gets the buffer hold by the slot
         *
         * @return The buffer hold by the slot
         */
        inline const buffer::shared_ptr get_buffer(void) const {
            auto_lock lock(this->lock_obj);
            return this->buf;
        }

        /**
         * Access to the on_update event
         *
         * @return Reference to the on_update event
         */
        inline update_multicast_delegate& on_update(void) {
            auto_lock lock(this->lock_obj);
            return this->on_update_event;
        }

        /**
         * Access to the on_update event
         *
         * @return Reference to the on_update event
         */
        inline const update_multicast_delegate& on_update(void) const {
            auto_lock lock(this->lock_obj);
            return this->on_update_event;
        }

        /**
         * Access to the on_close event
         *
         * @return Reference to the on_close event
         */
        inline close_multicast_delegate& on_close(void) {
            auto_lock lock(this->lock_obj);
            return this->on_close_event;
        }

        /**
         * Access to the on_close event
         *
         * @return Reference to the on_close event
         */
        inline const close_multicast_delegate& on_close(void) const {
            auto_lock lock(this->lock_obj);
            return this->on_close_event;
        }

        /**
         * Sets the buffer to a new object
         *
         * @remarks Also fired the 'on_update' event if b is not the currently
         *          set buffer.
         *
         * @param b The new buffer object
         */
        void set_buffer(buffer::shared_ptr b);

    private:

        /** The type for auto locks */
        typedef the::system::threading::auto_lock<the::system::threading::critical_section> auto_lock;

        /** the buffer */
        buffer::shared_ptr buf;

        /** the locking object */
        mutable the::system::threading::critical_section lock_obj;

        /** event fired whenever the buffer is updated */
        update_multicast_delegate on_update_event;

        /** event fired when the slot is closed */
        close_multicast_delegate on_close_event;

    };


} /* end namespace data */
} /* end namespace rivlib */
} /* end namespace eu_vicci */
