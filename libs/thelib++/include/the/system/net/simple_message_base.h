/*
 * include\the\system\net\simple_message_base.h
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
* AbstractSimpleMessage.h
*
* Copyright (C) 2006 - 2010 by Visualisierungsinstitut Universitaet Stuttgart. 
* Alle Rechte vorbehalten.
*/

#ifndef THE_SYSTEM_NET_SIMPLE_MESSAGE_BASE_H_INCLUDED
#define THE_SYSTEM_NET_SIMPLE_MESSAGE_BASE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/equatable.h"
#include "the/memory.h"
#include "the/stack_trace.h"
#include "the/types.h"



namespace the {
namespace system {
namespace net {

    /**
     * This class defines the interface of a network message for use with the 
     * simple message protocol of TheLib. There are two child classes, which 
     * implement a message that manages its own memory and one that uses 
     * user-specified memory. Please note that operations that reallocate the
     * message storage might fail if user-given memory is used.
     *
     * Implementation note: For all implementations, it is assumed that the
     * message header and the message body form a continous memory range.
     */
    class simple_message_base : public equatable<simple_message_base> {

    public:

        /** Size type for simple messages. */
        typedef uint32_t body_size_type;

        /** ID type for simple messages. */
        typedef uint32_t message_id_type;

        /**
         * This is the message header that goes over the wire. It is not 
         * recommended using this structure directly, but rather than via 
         * the message classes. We use this structure to have a definitive 
         * layout of the data that go over the wire and that can be exchanged
         * with other programming languages.
         *
         * The format of this header is compatible with VISlib.
         */
        typedef struct header_value_type_t {
            message_id_type id;         ///< User-defined message ID.
            body_size_type body_size;   ///< Size of the body in bytes.
        } header_value_type;

        /** Pointer to a constant on-wire message header. */
        typedef const header_value_type *const_header_pointer_type;

        /** Pointer to a on-wire message header. */
        typedef header_value_type *header_pointer_type;

        /** Dtor. */
        virtual ~simple_message_base(void);

        /**
         * Ensure that the message body is big enough to hold the number of
         * bytes specified in the message header.
         *
         * @throws std::bad_alloc If the memory could not be allocated.
         * @throws the::invalid_operation_excecption If the operation is not
         *                                           possible for semantic
         *                                           reasons.
         */
        inline void assert_body_size(void) {
            THE_STACK_TRACE;
            this->assert_storage(this->header()->body_size);
        }

        /**
         * Get a pointer to the message body. The object remains owner of
         * the memory designated by the pointer returned.
         *
         * @return A pointer to the message body.
         */
        inline const void *body(void) const {
            THE_STACK_TRACE;
            return the::as_at<void>(this->get_storage(), this->header_size());
        }

        /**
         * Get a pointer to the message body. The object remains owner of
         * the memory designated by the pointer returned.
         *
         * @return A pointer to the message body.
         */
        inline void *body(void) {
            THE_STACK_TRACE;
            return the::as_at<void>(this->get_storage(), this->header_size());
        }

        /**
         * Answer the message body as pointer to T. The object remains
         * owner of the memory designated by the pointer returned.
         *
         * @tparam T The type of pointer to be returned.
         *
         * @return The message body.
         */
        template<class T> inline const T *body(void) const {
            THE_STACK_TRACE;
            return the::as_at<T>(this->get_storage(), this->header_size());
        }

        /**
         * Answer the message body as pointer to T. The object remains
         * owner of the memory designated by the pointer returned.
         *
         * @tparam T The type of pointer to be returned.
         *
         * @return The message body.
         */
        template<class T> inline T *body(void) {
            THE_STACK_TRACE;
            return the::as_at<T>(this->get_storage(), this->header_size());
        }

        /**
         * Answer the message body at an offset of 'offset' bytes.
         *
         * @param offset The offset into the body in bytes.
         *
         * @return The 'offset'th byte in the message body.
         *
         * @throws the::index_out_of_range_exception If the offset exceeds the
         *                                           total body size.
         */
        const void *body_at(const size_t offset) const;

        /**
         * Answer the message body at an offset of 'offset' bytes.
         *
         * @param offset The offset into the body in bytes.
         *
         * @return The 'offset'th byte in the message body.
         *
         * @throws the::index_out_of_range_exception If the offset exceeds the
         *                                           total body size.
         */
        void *body_at(const size_t offset);

        /**
         * Answer the message body at an offset of 'offset' bytes as pointer 
         * to T. The object remains owner of the memory designated by the 
         * pointer returned.
         *
         * @tparam T The type of pointer to be returned.
         *
         * @return A pointer to the begin of the message body + 'offset' bytes.
         *
         * @throws the::index_out_of_range_exception If the offset exceeds the
         *                                           total body size.
         */
        template<class T> 
        inline const T *body_at(const size_t offset) const {
            THE_STACK_TRACE;
            return static_cast<const T *>(this->body_at(offset));
        }

        /**
         * Answer the message body at an offset of 'offset' bytes as pointer 
         * to T. The object remains owner of the memory designated by the 
         * pointer returned.
         *
         * @tparam T The type of pointer to be returned.
         *
         * @return A pointer to the begin of the message body + 'offset' bytes.
         *
         * @throws the::index_out_of_range_exception If the offset exceeds the
         *                                           total body size.
         */
        template<class T> 
        inline T *body_at(const size_t offset) {
            THE_STACK_TRACE;
            return static_cast<T *>(this->body_at(offset));
        }

        /**
         * Answer the size of the message body in bytes.
         *
         * @return The size of the message body.
         */
        inline size_t body_size(void) const {
            THE_STACK_TRACE;
            return this->header()->body_size;
        }

        /**
         * Determines whether this object and 'rhs' are equal.
         *
         * @param rhs The object to be compared.
         *
         * @return true if this object and 'rhs' are equal, false otherwise.
         */
        virtual bool equals(const simple_message_base& rhs) const;

        /**
         * Answer the size of the message header in bytes.
         *
         * @return The size of the message header.
         */
        inline size_t header_size(void) const {
            THE_STACK_TRACE;
            return sizeof(header_value_type);
        }

        /**
         * Answer the message ID from the header.
         *
         * @return The message ID.
         */
        inline message_id_type id(void) const {
            THE_STACK_TRACE;
            return this->header()->id;
        }

        /**
         * Update the message body with data from 'body'.
         *
         * Please note that for shallow_simple_messages, passing a body that is
         * too large to fit the existing memory block fill fail with an
         * exception.
         *
         * @param body     Pointer to the body data of at least 'bodySize' or
         *                 this->body_size() bytes, depending on the value of
         *                 'bodySize'.
         *                 If this pointer is nullptr, the message body will 
         *                 be erased regardless of the value of 'bodySize'.
         *                 The caller remains owner of the memory designated by
         *                 'body'. The object creates a deep copy in this method.
         * @param bodySize The size of the data to be copied to the message
         *                 body in bytes. If this parameter is 0, the value
         *                 from the current message header is used. If 'body'
         *                 is nullptr, this parameter is ignored and the new 
         *                 body size is 0.
         *
         * @throws std::bad_alloc If the memory could not be allocated.
         * @throws the::invalid_operation_excecption If the operation is not
         *                                           possible for semantic
         *                                           reasons.
         */
        void set_body(const void *body, const body_size_type bodySize = 0);

        /**
         * Updates the ID of the message.
         *
         * @param id The new ID of the message.
         */
        inline void set_id(const message_id_type id) {
            THE_STACK_TRACE;
            this->header()->id = id;
        }

        /**
         * Answer the overall message size in bytes.
         *
         * @return The combined size of the header and message body.
         */
        inline size_t size(void) const {
            THE_STACK_TRACE;
            return (sizeof(header_value_type) + this->body_size());
        }

        /**
         * Assignment operator.
         *
         * Note for subclass implementors: This implementation allocates
         * sufficient memory for 'rhs' in this message using assert_storage
         * and then memcopies the whole message. A check of the storage pointers
         * is done before copying, i.e. the method will not copy from an
         * aliased object (if this is recognisable).
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        simple_message_base& operator =(const simple_message_base& rhs);

        /**
         * Get the message data starting at the header. This is what you want to
         * send over the network.
         *
         * The ownership of the memory returned does not change.
         *
         * @return Pointer to the memory of the message (including header).
         */
        inline operator const void *(void) const {
            THE_STACK_TRACE;
            return this->get_storage();
        }

        /**
         * Get the message data starting at the header. This is what you want to
         * send over the network.
         *
         * The ownership of the memory returned does not change.
         *
         * @return Pointer to the memory of the message (including header).
         */
        inline operator void *(void) {
            THE_STACK_TRACE;
            return this->get_storage();
        }

    protected:

        /**
         * Initialises a new instance.
         */
        simple_message_base(void);

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
        virtual bool assert_storage(void *& outStorage, const size_t size) = 0;

        /**
         * Ensure that there is enough storage for the message header and a body
         * of the specified size. If the storage is reallocated, the header is
         * automatically preserved (not updated!) by this method. A pointer to 
         * the header is returned.
         *
         * Implementation note: This method uses 
         * simple_message_base::assert_storage(void *& outStorage, 
         * const size_t size) to allocate the storage. Additional logic is 
         * wrapped around this call. The method will add the required amount
         * of memory for the header to the body size specified.
         *
         * @param bodySize The size of the message body in bytes.
         *
         * @return The pointer to the begin of the storage (i.e. header).
         *
         * @throws std::bad_alloc If the memory could not be allocated.
         * @throws the::invalid_operation_excecption If the operation is not
         *                                           possible for semantic
         *                                           reasons.
         */
        void *assert_storage(const size_t size);

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
        virtual const void *get_storage(void) const throw() = 0;

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
        virtual void *get_storage(void) throw() = 0;

        /**
         * Provides typed access to the message header, which is the begin of the
         * memory range of the message.
         *
         * @return A pointer to the message header.
         */
        inline const_header_pointer_type header(void) const {
            THE_STACK_TRACE;
            return static_cast<const_header_pointer_type>(this->get_storage());
        }

        /**
         * Provides typed access to the message header, which is the begin of the
         * memory range of the message.
         *
         * @return A pointer to the message header.
         */
        inline header_pointer_type header(void) {
            THE_STACK_TRACE;
            return static_cast<header_pointer_type>(this->get_storage());
        }

    private:

        /** Super class typedef. */
        typedef equatable<simple_message_base> base;

    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_SIMPLE_MESSAGE_BASE_H_INCLUDED */
