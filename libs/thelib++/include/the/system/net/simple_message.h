/*
 * include\the\system\net\simple_message.h
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

#ifndef THE_SYSTEM_NET_SIMPLE_MESSAGE_H_INCLUDED
#define THE_SYSTEM_NET_SIMPLE_MESSAGE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/blob.h"
#include "the/stack_trace.h"

#include "the/system/net/simple_message_base.h"


namespace the {
namespace system {
namespace net {

    /**
     * This class implements a message that manages the storage for the
     * header and the message body. It is normally recommended using this
     * message class.
     */
    class simple_message : public simple_message_base {

    public:

        /** 
         * Create a new message with the specified number of bytes for the
         * message body.
         *
         * The storage for the message header is added automatically to the
         * body size specified. The body size is automatically set in the
         * message header.
         *
         * The message ID will be initialise to zero. The message body will
         * remain uninitialised.
         *
         * @param bodySize The size of the message body to be allocated in
         *                 bytes. This defaults to zero.
         *
         * @throws std::bad_alloc If the memory for the message could not be
         *                        allocated.
         */
        simple_message(const body_size_type bodySize = 0);

        /** 
         * Create a new message with the specified ID and message body.
         *
         * @param id       The ID of the message.
         * @param body     The body content of the message.
         * @param bodySize The size of the memory designated by 'body' in bytes.
         *
         * @throws std::bad_alloc If the memory for the message could not be
         *                        allocated.
         */
        simple_message(const message_id_type id, const void *body,
            const body_size_type bodySize);

        /**
         * Clone 'rhs'.
         *
         * @param rhs The object to be cloned.
         *
         * @throws std::bad_alloc If the memory for the message could not be
         *                        allocated.
         */
        inline simple_message(const simple_message& rhs) : base() {
            THE_STACK_TRACE;
            *this = rhs;
        }

        /**
         * Move 'rhs' into this object.
         *
         * 'rhs' will be a message without body after that. The message ID of
         * 'rhs' will be cleared to zero.
         *
         * @param rhs The object to be moved.
         */
        inline simple_message(simple_message&& rhs) : base() {
            THE_STACK_TRACE;
            *this = std::move(rhs);
        }

        /** Dtor. */
        virtual ~simple_message(void);

        /**
         * Assignment operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline simple_message& operator =(const simple_message& rhs) {
            base::operator =(rhs);
            return *this;
        }

        /**
         * Move 'rhs' into this object.
         *
         * 'rhs' will be a message without body after that. The message ID of
         * 'rhs' will be cleared to zero.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        simple_message& operator =(simple_message&& rhs);

    protected:

        /**
         * Ensure that whatever type of storage is used has enough memory to
         * store a message (including header) with the specified size. The
         * caller must add the size of the header, i.e. implementors should
         * ensure that there are at least 'size' bytes in 'outStorage' when
         * the method returns.
         *
         * Child classes must implement this method in order to allow the logic
         * of this parent class to work. The parent class will care for all 
         * intermediate pointers to be updated and for header data to be 
         * restored in case of a reallocation.
         *
         * Child classes that want to (re-)allocate storage should use the
         * other overload of simple_message_base::assert_storage() in order
         * to correctly update the message header.
         *
         * @param outStorage This variable receives the pointer to the begin of
         *                   the storage.
         * @param size       The size of the memory to be allocated in bytes.
         *
         * @return true if the storage has been reallocated, false if it remains
         *         the same (i.e. the pointer has not been changed).
         *
         * @throws std::bad_alloc If the memory could not be allocated.
         * @throws the::invalid_operation_excecption If the operation is not
         *                                           possible for semantic
         *                                           reasons.
         */
        virtual bool assert_storage(void *& outStorage, const size_t size);

        /**
         * Get the message data starting at the header.
         *
         * Subclasses must return the pointer to the message data here,
         * regardless how it was obtained.
         *
         * The ownership of the memory returned does not change.
         *
         * @return Pointer to the whole memory of the message.
         */
        virtual const void *get_storage(void) const throw();

        /**
         * Get the message data starting at the header.
         *
         * Subclasses must return the pointer to the message data here,
         * regardless how it was obtained.
         *
         * The ownership of the memory returned does not change.
         *
         * @return Pointer to the whole memory of the message.
         */
        virtual void *get_storage(void) throw();

    private:

        /** Super class typedef. */
        typedef simple_message_base base;

        /** Storage for the message and its header. */
        the::blob storage;

    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_SIMPLE_MESSAGE_H_INCLUDED */
