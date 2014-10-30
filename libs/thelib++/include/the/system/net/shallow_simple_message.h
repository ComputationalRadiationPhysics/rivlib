/*
 * include\the\system\net\shallow_simple_message.h
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

#ifndef THE_SYSTEM_NET_SHALLOW_SIMPLE_MESSAGE_H_INCLUDED
#define THE_SYSTEM_NET_SHALLOW_SIMPLE_MESSAGE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/system/net/simple_message_base.h"



namespace the {
namespace system {
namespace net {

    /**
     * This class can be used to apply the behaviour of a TheLib simple message
     * on a user-supplied memory range. This means that the class creates alias
     * pointers, but does not take ownership of the memory. The user is 
     * responsible for ensuring that the memory lives as long as the message
     * object and that the memory bounds specified are correct.
     *
     * It is illegal to specify nullptr. The behaviour of the class is
     * undefined if you do so.
     *
     * It is illegal to specify a memory block that has not at least a size
     * of sizeof(simple_message_base::header_value_type) bytes. The behaivour
     * of the class is undefined if you do so.
     *
     * The class will never release the user-defined memory.
     */
    class shallow_simple_message : public simple_message_base {

    public:

        /** 
         * Create a new message using the storage provided by 'storage'. 
         * 'cntStorage' specifies the amount of memory available starting
         * at 'storage'. If this is 0, it is assumed that there is a valid
         * message header at the start of 'storage' which contains the size
         * of the message. This message size is then used as size of the
         * memory block.
         *
         * Be aware of that it is illegal providing storage that is 
         * insufficient for storing a message header. This will cause an
         * unpredictable behaviour.
         *
         * It is illegal providing nullptr pointer for storage.
         *
         * @param storage    Memory to be used for the message. The caller
         *                   remains owner of this memory and must ensure
         *                   that the memory block lives as long as the
         *                   object.
         * @param cntStorage The size of the memory block provided for
         *                   'storage' in bytes. This must be the actual
         *                   size for at least the header or 0 for making
         *                   the ctor interpret the message data.
         *
         * @throw the::argument_null_exception If 'storage' is nullptr.
         * @throw the::argument_exception If 'cntStorage' is not zero, but
         *                                less than the size of a message
         *                                header.
         */
        inline shallow_simple_message(void *storage, 
                const size_t cntStorage = 0) : base(), cntStorage(0),
                storage(nullptr) {
            THE_STACK_TRACE;
            this->set_storage(storage, cntStorage);
        }

        /** Dtor. */
        virtual ~shallow_simple_message(void);

        /**
         * Update the storage memory.
         *
         * Note that this will not affect the current storage, i. e. there
         * is no ownership change. The caller is responsible for releasing
         * this memory after the method returns.
         *
         * Be aware of that it is illegal providing storage that is 
         * insufficient for storing a message header. This will cause an
         * unpredictable behaviour.
         *
         * It is illegal providing a NULL pointer for storage.
         *
         * @param storage    Memory to be used for the message. The caller
         *                   remains owner of this memory and must ensure
         *                   that the memory block lives as long as the
         *                   object.
         * @param cntStorage The size of the memory block provided for
         *                   'storage' in bytes. This must be the actual
         *                   size for at least the header or 0 for making
         *                   the ctor interpret the message data.
         *
         * @throw the::argument_null_exception If 'storage' is nullptr.
         * @throw the::argument_exception If 'cntStorage' is not zero, but
         *                                less than the size of a message
         *                                header.
         */
        void set_storage(void *storage, const size_t cntStorage = 0);

        /**
         * Assignment operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline shallow_simple_message& operator =(
                const simple_message_base& rhs) {
            base::operator =(rhs);
            return *this;
        }

    protected:

        /**
         * Ensure that whatever type of storage is used has enough memory to 
         * store a message (including header) with the specified size. The caller
         * must add the size of the header.
         *
         * This implementation does NOTHING! The owner of the external storage
         * used by this object is responsible for the memory requirement.
         * 
         * @param outStorage This variable receives the pointer to the begin of
         *                   the storage.
         * @param size       The size of the memory to be allocated in bytes.
         *
         * @return false as it remains the same 
         *         (i. e. the pointer has not been changed).
         *
         * @throws the::invalid_operation_excecption If the memory needs to be
         *                                           grown to meet the request.
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

        /** User-specified size of the memory allocated for the message. */
        size_t cntStorage;

        /** Storage for the message and its header. */
        void *storage;

    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_SHALLOW_SIMPLE_MESSAGE_H_INCLUDED */
