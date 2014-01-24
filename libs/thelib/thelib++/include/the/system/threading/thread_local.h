/*
 * the/system/threading/thread_local.h
 *
 * Copyright (c) 2012, TheLib Team (http://www.thelib.org/license)
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

#ifndef THE_SYSTEM_THREADING_THREAD_LOCAL_H_INCLUDED
#define THE_SYSTEM_THREADING_THREAD_LOCAL_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#ifdef THE_LINUX
#include <pthread.h>
#endif /* THE_LINUX */
#ifdef THE_WINDOWS
#include <windows.h>
#endif /* THE_WINDOWS */

#include "the/system/system_exception.h"
#include "the/assert.h"
#include "the/invalid_operation_exception.h"
#include "the/not_copyable.h"
#include "the/not_implemented_exception.h"
#include "the/stack_trace.h"
#include "the/tchar.h"


namespace the {
namespace system {
namespace threading {


    /**
     * Encapsulates a thread-local variable.
     *
     * Note: __declspec(thread) on Windows could be slightly faster because the
     * compiler can optimise access to the TLS table. I assume GCC could do the
     * same for __thread.
     *
     * @param T The type of the variable, which must not be larger than a 
     *          pointer.
     */
    template<class T> class thread_local : public not_copyable {

    public:

        /** The native TLS index type. */
#if defined(THE_WINDOWS)
        typedef DWORD native_index_type;
#elif defined(THE_LINUX)
        typedef pthread_key_t native_index_type;
#else /* defined(THE_WINDOWS) */
#error "the::system::threading::thread_local must define native_index_type!"
#endif /* defined(THE_WINDOWS) */

        /** The type of data stored in TLS. */
        typedef T value_type;

        /**
         * Ctor.
         */
        thread_local(void) throw();

        /**
         * Dtor.
         */
        virtual ~thread_local(void);

        /**
         * Get the current value of the TLS variable.
         *
         * @return The current value of the TLS variable.
         *
         * @throws the::system_exception In case the variable could not be read.
         * @throws the::invalid_operation_exception In case the variable has
         *                                          never been written before.
         */
        inline const value_type get(void) const {
            THE_STACK_TRACE;
            return reinterpret_cast<T>(this->get0());
        }

        /**
         * Get the current value of the TLS variable.
         *
         * @return The current value of the TLS variable.
         *
         * @throws the::system_exception In case the variable could not be read.
         * @throws the::invalid_operation_exception In case the variable has
         *                                          never been written before.
         */
        inline value_type get(void) {
            THE_STACK_TRACE;
            return reinterpret_cast<T>(this->get0());
        }

        /** 
         * Answer whether the TLS variable has been successfully initialised, 
         * i.e. whether a TLS index was acquired.
         *
         * @return true if the variable is initialised, false otherwise.
         */
        inline bool is_initialised(void) const {
            THE_STACK_TRACE;
#if defined(THE_WINDOWS)
            return (this->index != TLS_OUT_OF_INDEXES);
#elif defined(THE_LINUX)
            return this->isInitialised;
#else /* defined(THE_WINDOWS) */
            THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
        }

        /**
         * Get the current value of the TLS variable.
         *
         * @return The current value of the TLS variable.
         *
         * @throws the::system_exception In case the variable could not be read.
         * @throws the::invalid_operation_exception In case the variable has
         *                                          never been written before.
         */
        inline operator const value_type(void) const {
            THE_STACK_TRACE;
            return reinterpret_cast<value_type>(this->get0());
        }

        /**
         * Get the current value of the TLS variable.
         *
         * @return The current value of the TLS variable.
         *
         * @throws the::system_exception In case the variable could not be read.
         * @throws the::invalid_operation_exception In case the variable has
         *                                          never been written before.
         */
        inline operator value_type(void) {
            THE_STACK_TRACE;
            return reinterpret_cast<value_type>(this->get0());
        }

        /**
         * Set the new value of the TLS variable.
         *
         * @param value The new value of the TLS variable.
         *
         * @throws the::system_exception In case the TLS index could not be
         *                               acquired lazily or in case that the
         *                               value could not be written.
         */
        thread_local& operator =(const value_type& value);

    protected:

        /** Super class typedef. */
        typedef not_copyable base;

    private:

        /**
         * Performs a lazy initialisation of 'index'. 
         *
         * @throws the::system_exception If the initialisation failed.
         */
        void initialise(void);

        /**
         * Retrieves the TLS value.
         *
         * @returns The value stored for 'index'.
         *
         * @throws the::system_exception In case the variable could not be read.
         * @throws the::invalid_operation_exception In case the variable has
         *                                          never been written before.
         */
        void *get0(void);

        /** The TLS index of the variable. */
        native_index_type index;

#ifdef THE_LINUX
        /** Remembers whether the variable was initialised. */
        bool isInitialised;
#endif /* THE_LINUX */

    };

} /* end namespace threading */
} /* end namespace system */
} /* end namespace the */


/*
 * the::system::threading::thread_local<T>::thread_local
 */
template<class T> the::system::threading::thread_local<T>::thread_local(void)
        throw() :
#if defined(THE_WINDOWS)
        index(TLS_OUT_OF_INDEXES) 
#elif defined(THE_LINUX)
        isInitialised(false)
#endif /* defined(THE_WINDOWS) */
        {
    THE_STACK_TRACE;
    ASSERT(sizeof(T) <= sizeof(void *));
}


/*
 * the::system::threading::thread_local<T>::~thread_local
 */
template<class T> the::system::threading::thread_local<T>::~thread_local(void) {
    THE_STACK_TRACE;
    if (this->is_initialised()) {
#if defined(THE_WINDOWS)
        ::TlsFree(this->index);
        this->index = TLS_OUT_OF_INDEXES;
#elif defined(THE_LINUX)
        ::pthread_key_delete(this->index);
        this->isInitialised = false;
#endif /* defined(THE_WINDOWS) */
    }
}


/*
 * the::system::threading::thread_local<T>::operator =
 */
template<class T> the::system::threading::thread_local<T>& 
the::system::threading::thread_local<T>::operator =(const value_type& value) {
    THE_STACK_TRACE;
    void *v = const_cast<void *>(reinterpret_cast<const void *>(value));

    this->initialise();
    
#if defined(THE_WINDOWS)
    if (::TlsSetValue(this->index, v) == FALSE) {
        throw system_exception(__FILE__, __LINE__);
    }
#elif defined(THE_LINUX)
    int error = ::pthread_setspecific(this->index, v);
    if (error != 0) {
        throw system_exception(error, __FILE__, __LINE__);
    }
#else /* defined(THE_WINDOWS) */
    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
    return *this;
}


/*
 * the::system::threading::thread_local<T>::initialise
 */
template<class T> 
void the::system::threading::thread_local<T>::initialise(void) {
    THE_STACK_TRACE;
    if (!this->is_initialised()) {
#if defined(THE_WINDOWS)
        this->index = ::TlsAlloc();
#elif defined(THE_LINUX)
        this->isInitialised = (::pthread_key_create(&this->index, NULL) == 0);
#else /* defined(THE_WINDOWS) */
        THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
        if (!this->is_initialised()) {
            throw system_exception(__FILE__, __LINE__);
        }
    }
}


/*
 * the::system::threading::thread_local<T>::get0
 */
template<class T> void *the::system::threading::thread_local<T>::get0(void) {
    THE_STACK_TRACE;
    if (!this->is_initialised()) {
        throw invalid_operation_exception(_T("A thread_local variable cannot ")
            _T("be read unless it has been initialised."), __FILE__, __LINE__);
    }

    void *value 
#if defined(THE_WINDOWS)
        = ::TlsGetValue(this->index);
    DWORD error = ::GetLastError();
    if ((value == NULL) && (error != ERROR_SUCCESS)) {
        throw system_exception(error, __FILE__, __LINE__);
    }
#elif defined(THE_LINUX)
        = ::pthread_getspecific(this->index);
#else /* defined(THE_WINDOWS) */
        = NULL;
    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
    
    return value;
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_THREADING_THREAD_LOCAL_H_INCLUDED */
