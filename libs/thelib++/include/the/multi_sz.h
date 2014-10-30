/*
 * include\the\multi_sz.h
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
 * multi_sz.h
 *
 * Copyright (C) 2006 - 2007 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_MULTI_SZ_H_INCLUDED
#define THE_MULTI_SZ_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include <cstdarg>

#include "the/assert.h"
#include "the/equatable.h"
#include "the/index_out_of_range_exception.h"
#include "the/memory.h"
#include "the/stack_trace.h"
#include "the/string.h"
#include "the/tchar.h"

#include <list>

namespace the {

    /**
     * Represents a set of zero-terminated strings terminated by an additional
     * zero. The class can be instantiated for std::char_traits.
     *
     * Note that the set cannot contain empty strings as this would corrupt the
     * data structure.
     *
     * @tparam T The type of the characters (char or wchar_t).
     */
    template<class T> class multi_sz {

    public:

        /** An iterator for the strings in a multi_sz. */
        class iterator : public std::iterator<std::forward_iterator_tag,
                const T *, ptrdiff_t, const T *, const T *>,    // TODO: I am really unsure whether this is harmful...
                public the::equatable<iterator> {

        public:

            /**
             * Clone 'rhs'.
             *
             * @param rhs The object to be cloned.
             */
            iterator(const iterator& rhs) {
                THE_STACK_TRACE;
                *this = rhs;
            }

            /**
             * Dtor.
             */
            virtual ~iterator(void);

            /**
             * Determines whether this iterator and 'rhs' are equal.
             *
             * @param rhs The object to be compared.
             *
             * @return true if this object and 'rhs' are equal, false otherwise.
             */
            virtual bool equals(const iterator& rhs) const;

            /**
             * Assignment.
             *
             * @param rhs The right hand side operand.
             *
             * @return *this.
             */
            inline iterator& operator =(const iterator& rhs) {
                THE_STACK_TRACE;
                this->current = rhs.current;
                this->multiSz = rhs.multiSz;
                return *this;
            }

            /**
             * Dereferences the iterator, which returns the current string.
             *
             * @return The current string.
             */
            reference operator *(void) const;

            /**
             * Answers the current string.
             *
             * @return The current string.
             */
            pointer operator ->(void) const;

            /**
             * Prefix increment.
             *
             * @return *this.
             */
            iterator& operator ++(void);

            /**
             * Postfix increment.
             *
             * @return The previous iterator position.
             */
            iterator operator ++(int);

        private:

            /**
             * Initialises a new instance.
             *
             * @param current Pointer to the first character of the current
             *                string.
             */
            inline iterator(value_type current) : current(current) {
                THE_STACK_TRACE;
            }

            /** Pointer to the begin of the current string. */
            value_type current;

            /** Enable multi_sz to create instances. */
            friend class multi_sz<T>;
        }; /* end class iterator */

        /** Define a local name for the character type. */
        typedef T char_type;

        /** Define a local name for the string size. */
        typedef size_t size_type;

        /** Define a local name for the string type. */
        typedef std::basic_string<char_type> string_type;

        /** Define a local name for the character traits. */
        typedef std::char_traits<T> traits_type;

        /**
         * Answer the number of entries in the set of zero-terminated strings
         * 'multi_sz'. Note that is is indispensable that 'multi_sz' is terminated
         * using TWO zeros. It is safe to pass nullptr.
         *
         * @param multiSz A set of zero-terminated strings, terminated with TWO
         *                zeros at the end of the set. The caller remains owner
         *                of the memory.
         *
         * @return The number of strings in the set.
         */
        static size_type count(const char_type *multiSz);

        /**
         * Answer the length of the set of zero-terminated strings 'multi_sz' 
         * including all terminating zeros. Note that is is indispensable that 
         * 'multi_sz' is terminated using TWO zeros. It is safe to pass nullptr.
         *
         * @param multiSz A set of zero-terminated strings, terminated with TWO
         *                zeros at the end of the set. The caller remains owner
         *                of the memory.
         * 
         * @return The number of characters that are required to store the
         *         'multi_sz'.
         */
        inline static size_type length(const char_type *multiSz) {
            THE_STACK_TRACE;
            return multi_sz::size(multiSz);
        }

        /**
         * Answer the 'idx' string in the set of zero-terminated string
         * 'multi_sz' or nullptr if 'idx' is out of range. Note that is is
         * indispensable that 'multi_sz' is terminated using TWO zeros. It
         * is safe to pass nullptr.
         *
         * @param idx     The index of the string to retrieve.
         * @param multiSz A set of zero-terminated strings to get the 'idx'th
         *                element from. The caller remains owner of the memory.
         *
         * @return A pointer to the begin of the 'idx'th string or nullptr, if
         *         'idx' is out of range.
         */
        static const char_type *peek_at(const size_type idx,
            const char_type *multiSz);

        /**
         * Answer the length of the set of zero-terminated strings 'multi_sz' 
         * including all terminating zeros. Note that is is indispensable that 
         * 'multi_sz' is terminated using TWO zeros. It is safe to pass nullptr.
         *
         * @param multiSz A set of zero-terminated strings, terminated with TWO
         *                zeros at the end of the set. The caller remains owner
         *                of the memory.
         * 
         * @return The number of characters that are required to store the
         *         'multi_sz'.
         */
        static size_type size(const char_type *multiSz);

        /**
         * Create a new object using the the set of zero-terminated strings
         * 'multi_sz' for initialisation. Note that is is indispensable that
         * 'multi_sz' is terminated using TWO zeros. It is safe to pass nullptr.
         *
         * @param multiSz The set of zero-terminated strings used for 
         *                initialisation. If this is nullptr, an empty
         *                multi_sz is created. The caller remains owner of the
         *                memory.
         *
         * @throws std::bad_alloc If the memory required for the multi_sz could
         *                        not be allocated.
         */
        inline explicit multi_sz(const char_type *multiSz = nullptr)
                : multiSz(nullptr) {
            THE_STACK_TRACE;
            *this = multiSz;
        }

        /**
         * Create a new object from an array of strings.
         *
         * @param strings    An array of strings. It is safe to pass nullptr.
         *                   The caller remains owner of the memory.
         * @param cntStrings The number of elements in 'strings'.
         *
         * @throws std::bad_alloc If the memory required for the multi_sz could
         *                        not be allocated.
         */
        multi_sz(const char_type **strings, const size_type cntStrings);

        /**
         * Clone 'rhs'.
         *
         * @param rhs The object to be cloned.
         *
         * @throws std::bad_alloc If the memory required for the multi_sz could
         *                        not be allocated.
         */
        inline multi_sz(const multi_sz& rhs) : multiSz(nullptr) {
            THE_STACK_TRACE;
            *this = rhs;
        }

        /**
         * Move 'rhs'.
         *
         * @param rhs The object to be moved into the new one. This will be an
         *            empty multi_sz afterwards.
         */
        inline multi_sz(multi_sz&& rhs) {
            THE_STACK_TRACE;
            this->multiSz = rhs.multiSz;
            rhs.multiSz = nullptr;
        }

        /** Dtor. */
        ~multi_sz(void);

        /**
         * Append 'str' at the end of the multi_sz.
         *
         * @param str The string to be appended. It is safe to pass nullptr
         *            (nothing will happen in this case). Nothing will happen if
         *            'str' is an empty string. The caller remains owner of the 
         *            memory.
         *
         * @throws std::bad_alloc If the memory required for the multi_sz could
         *                        not be allocated.
         */
        void add(const char_type *str);

        /**
         * Append 'str' at the end of the multi_sz.
         *
         * @param str The string to be appended. If the string is empty, nothing
         *            will happen.
         *
         * @throws std::bad_alloc If the memory required for the multi_sz could
         *                        not be allocated.
         */
        inline void add(const string_type& str) {
            THE_STACK_TRACE;
            this->add(str.c_str());
        }

        /**
         * Deletes the current content of the multi_sz, allocates memory for
         * 'cnt' characters, and returns a pointer to this buffer. The buffer
         * will not be initialised.
         *
         * Note: You must specify the memory for the terminating zeros yourself!
         *
         * @param cnt The new size of the buffer. This will be the exact new
         *            size, no space for zeros will be added!
         *
         * @return The pointer to the new internal buffer.
         *
         * @throws std::bad_alloc If the memory requested amount of memory could
         *                        not be allocated.
         */
        char_type *allocate(const size_type cnt);

        /**
         * Gets an iterator for the multi_sz.
         *
         * @return An iterator pointing at the first element.
         */
        iterator begin(void) const {
            THE_STACK_TRACE;
            return iterator(this->multiSz);
        }

        /**
         * Clear all elements in the multi_sz.
         */
        inline void clear(void) {
            THE_STACK_TRACE;
            the::safe_array_delete(this->multiSz);
            THE_ASSERT(this->multiSz == nullptr);
        }

        /**
         * Answer the number of strings in the multi_sz.
         *
         * @return The number of strings in the multi_sz.
         */
        inline size_type count(void) const {
            THE_STACK_TRACE;
            return multi_sz::count(this->multiSz);
        }

        /**
         * Answer the raw data pointer. Note, that is unsafe to operate with
         * this pointer and that the pointer might become invalid as the
         * multi_sz is manipulated using other methods.
         *
         * @return The raw data pointer.
         */
        inline const char_type *data(void) const {
            THE_STACK_TRACE;
            return this->multiSz;
        }

        /**
         * Answer whether there is no string at all in the multi_sz.
         *
         * @return true if the set is empty, false otherwise.
         */
        inline bool empty(void) const {
            THE_STACK_TRACE;
            return (this->multiSz == nullptr);
        }

        /**
         * Gets an iterator pointing after the last element.
         *
         * @return An iterator pointing to the end.
         */
        inline iterator end(void) const {
            THE_STACK_TRACE;
            return iterator(nullptr);
        }

        /**
         * Answer the string at the 'idx'th position in the multi_sz.
         *
         * @param idx The index of the string to retrieve.
         *
         * @return The string at the requested position or an empty string if
         *         the index is out of range.
         */
        string_type get_at(const size_type idx) const;

        /**
         * Insert 'str' at the 'idx'th position. All elements behind
         * 'idx' are shifted one element right. 'idx' must be a valid index in
         * the multi_sz or the index directly following the end, i.e. count().
         * In the latter case, the method behaves like add().
         *
         * @param idx The position to insert the string at.
         * @param str The string to add. The caller remains owner of the memory.
         *            It is safe to pass nullptr. Nothing will happen in the 
         *            latter case. Nothing will happen, too, if the string is 
         *            empty.
         *
         * @throws the::index_out_of_range_exception If 'idx' is not within
         *                                           [0, this->count()].
         * @throws std::bad_alloc If the memory required for the multi_sz could
         *                        not be allocated.
         */
        void insert(const size_type idx, const char_type *str);

        /**
         * Insert 'str' at the 'idx'th position. All elements behind
         * 'idx' are shifted one element right. 'idx' must be a valid index in
         * the multi_sz or the index directly following the end, i.e. count().
         * In the latter case, the method behaves like add().
         *
         * @param idx The position to insert the string at.
         * @param str The string to add. The caller remains owner of the memory.
         *            It is safe to pass nullptr. Nothing will happen in the 
         *            latter case. Nothing will happen, too, if the string is 
         *            empty.
         *
         * @throws the::index_out_of_range_exception If 'idx' is not within
         *                                           [0, this->count()].
         * @throws std::bad_alloc If the memory required for the multi_sz could
         *                        not be allocated.
         */
        inline void insert(const size_type idx, const string_type& str) {
            THE_STACK_TRACE;
            this->insert(idx, str.c_str());
        }

        /**
         * Answer the length of the multi_sz including all terminating zeros.
         * 
         * @return The number of characters that are required to store the
         *         multi_sz.
         */
        inline size_type length(void) const {
            THE_STACK_TRACE;
            return multi_sz::size(this->multiSz);
        }

        /**
         * Answer the raw data of the 'idx'th string in the multi_sz or nullptr
         * if no such element exists.
         *
         * @param idx The index of the string to get.
         *
         * @return A pointer to the begin of the 'idx'th string or nullptr, if 
         *         the index is out of range.
         */
        inline const char_type *peek_at(const size_type idx) const {
            THE_STACK_TRACE;
            return multi_sz::peek_at(idx, this->multiSz);
        }

        /**
         * Remove all occurrences of 'str' in the set.
         *
         * @param str The string to be removed. It is safe to pass nullptr
         *            pointer or an empty string. The caller remains owner
         *            of the memory.
         */
        void remove(const char_type *str);

        /**
         * Remove all occurrences of 'str' in the set.
         *
         * @param str The string to be removed. It is safe to pass an empty
         *            string. 
         */
        inline void remove(const string_type& str) {
            THE_STACK_TRACE;
            this->remove(str.c_str());
        }

        /**
         * Answer the length of the multi_sz including all terminating zeros.
         * 
         * @return The number of characters that are required to store the
         *         multi_sz.
         */
        inline size_type size(void) const {
            THE_STACK_TRACE;
            return multi_sz::size(this->multiSz);
        }

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        multi_sz& operator =(const multi_sz& rhs);

        /**
         * Assignment from a raw set of zero-terminated strings 'rhs'. Note
         * that is is indispensable that 'rhs' is terminated using TWO zeros. 
         * It is safe to pass nullptr.
         *
         * @param rhs The right hand side operand, which must be a set of 
         *            zero-terminated strings, terminated with TWO zeros at 
         *            the end of the set. The caller remains owner of the 
         *            memory.
         *
         * @return *this.
         */
        multi_sz& operator =(const char_type *rhs);

        /**
         * Test for equality.
         *
         * @param rhs The right hand side operand.
         *
         * @return true if 'rhs' and this object are equal, false otherwise.
         */
        bool operator ==(const multi_sz& rhs) const;

        /**
         * Test for inequality.
         *
         * @param rhs The right hand side operand.
         *
         * @return true if 'rhs' and this object are  not equal,
         *         false otherwise.
         */
        inline bool operator !=(const multi_sz& rhs) const {
            THE_STACK_TRACE;
            return !(*this == rhs);
        }

        /**
         * Appends 'rhs' to the multi_sz.
         *
         * @param rhs The right hand side operand. It is safe to pass nullptr
         *            (nothing will happen in this case). Nothing will happen if
         *            'rhs' is an empty string. The caller remains owner of the 
         *            memory.
         *
         * @return *this.
         *
         * @throws std::bad_alloc If the memory required for the multi_sz could
         *                        not be allocated.
         */
        inline multi_sz& operator +=(const char_type *rhs) {
            THE_STACK_TRACE;
            this->add(rhs);
            return *this;
        }

        /**
         * Appends 'rhs' to the multi_sz.
         *
         * @param rhs The right hand side operand. Nothing will happen if
         *            'rhs' is an empty string. The caller remains owner of the 
         *            memory.
         *
         * @return *this.
         *
         * @throws std::bad_alloc If the memory required for the multi_sz could
         *                        not be allocated.
         */
        inline multi_sz& operator +=(const string_type& rhs) {
            THE_STACK_TRACE;
            this->add(rhs);
            return *this;
        }

        /**
         * Answer the string at the 'idx'th position in the multi_sz.
         *
         * @param idx The index of the string to retrieve.
         *
         * @return The string at the requested position.
         *
         * @throws the::index_out_of_range_exception If 'idx' is not within 
         *                                           [0, count()[.
         */
        string_type operator [](const size_type idx) const;

    private:

        /** The raw data. */
        char_type *multiSz;

    };

    /** Template instantiation for ANSI strings. */
    typedef multi_sz<char> multi_sza;

    /** Template instantiation for wide strings. */
    typedef multi_sz<wchar_t> multi_szw;

    /** Template instantiation for TCHARs. */
    typedef multi_sz<TCHAR> tmulti_sz;

} /* end namespace the */


/*
 * the::multi_sz<T>::iterator::~iterator
 */
template<class T> the::multi_sz<T>::iterator::~iterator(void) {
    THE_STACK_TRACE;
}


/*
 * the::multi_sz<T>::iterator::equals
 */
template<class T>
bool the::multi_sz<T>::iterator::equals(const iterator& rhs) const {
    THE_STACK_TRACE;
    return (this->current == rhs.current);
}


/*
 * the::multi_sz<T>::iterator::operator *
 */
template<class T> typename the::multi_sz<T>::iterator::reference
the::multi_sz<T>::iterator::operator *(void) const {
    THE_STACK_TRACE;
    return this->current;
}


/*
 * the::multi_sz<T>::iterator::operator ->
 */
template<class T> typename the::multi_sz<T>::iterator::pointer
the::multi_sz<T>::iterator::operator ->(void) const {
    THE_STACK_TRACE;
    return this->current;
}


/*
 * the::multi_sz<T>::iterator::operator ++
 */
template<class T> typename the::multi_sz<T>::iterator&
the::multi_sz<T>::iterator::operator ++(void) {
    THE_STACK_TRACE;
    if (this->current != nullptr) {
        THE_ASSERT(*this->current != 0);
        while (*this->current++ != 0);

        // Note: The loop above always increments the pointer, i.e. if we
        // detect a zero here, it is the second in a row. Hence, we set
        // 'current' to nullptr to signal that the end was reached.
        if (*this->current == 0) {
            this->current = nullptr;
        }
    }
    return *this;
}


/*
 * the::multi_sz<T>::iterator::operator ++
 */
template<class T> typename the::multi_sz<T>::iterator
the::multi_sz<T>::iterator::operator ++(int) {
    THE_STACK_TRACE;
    iterator retval(*this);
    ++(*this);
    return retval;
}


/*
 * the::multi_sz<T>::count
 */
template<class T> typename the::multi_sz<T>::size_type the::multi_sz<T>::count(
        const char_type *multiSz) {
    THE_STACK_TRACE;
    size_type retval = 0;
    const char_type *cursor = multiSz;

    if (cursor != nullptr) {
        while (*cursor != 0) {
            while (*cursor++ != 0);
            retval++;
        }
    }

    return retval;
}


/* 
 * the::multi_sz<T>::peek_at
 */
template<class T> 
const typename the::multi_sz<T>::char_type *the::multi_sz<T>::peek_at(
        const size_type idx, const char_type *multiSz) {
    THE_STACK_TRACE;
    const char_type *cursor = multiSz;

    if (cursor != nullptr) {
        for (SIZE_T i = 0; (i < idx) && (*cursor != 0); i++) {
            while (*cursor++ != 0);
        }

        if (*cursor == 0) {
            cursor = nullptr;
        }
    }

    return cursor;
}


/*
 * the::multi_sz<T>::size
 */
template<class T> typename the::multi_sz<T>::size_type the::multi_sz<T>::size(
        const char_type *multiSz) {
    THE_STACK_TRACE;
    const char_type *cursor = multiSz;

    if (cursor != nullptr) {
        while ((*(++cursor - 1) != 0) || (*cursor != 0));
        return (cursor - multiSz + 1);
    } else {
        return 0;
    }
}


/*
 * the::multi_sz<T>::multi_sz
 */
template<class T> 
the::multi_sz<T>::multi_sz(const char_type **strings,
        const size_type cntStrings) : multiSz(nullptr) {
    THE_STACK_TRACE;
    if ((strings != nullptr) && (cntStrings > 0)) {
        size_type cnt = 0;
        char_type *dstPos = nullptr;
        const char_type *srcPos = nullptr;

        for (size_type i = 0; i < cntStrings; i++) {
            cnt += traits_type::length(strings[i]) + 1;
        }

        this->multiSz = dstPos = new char_type[cnt + 1];

        for (size_type i = 0; i < cntStrings; i++) {
            if ((strings[i] != nullptr) && (*(strings[i]) != 0)) {
                srcPos = strings[i];
                while ((*dstPos++ = *srcPos++) != 0);
            }
        }

        /* Add double-zero at end. */
        *dstPos = 0;
    } /* end if ((strings != nullptr) && (cntStrings > 0)) */
}


/*
 * the::multi_sz<T>::~multi_sz
 */
template<class T> the::multi_sz<T>::~multi_sz(void) {
    THE_STACK_TRACE;
    this->clear();
}


/*
 * the::multi_sz<T>::add
 */
template<class T> void the::multi_sz<T>::add(const char_type *str) {
    THE_STACK_TRACE;
    if ((str != nullptr) && (*str != 0)) {
        size_type oldLen = multi_sz::size(this->multiSz);
        size_type strLen = traits_type::length(str) + 1;
        char_type *newData = nullptr;
        THE_ASSERT(strLen > 0);

        if (oldLen > 0) {
            newData = new char_type[oldLen + strLen];
            ::memcpy(newData, this->multiSz, oldLen * sizeof(char_type));
            ::memcpy(newData + oldLen - 1, str, strLen * sizeof(char_type));
            newData[oldLen + strLen - 1] = 0;
        } else {
            newData = new char_type[strLen + 1];
            ::memcpy(newData, str, strLen * sizeof(char_type));
            newData[strLen] = 0;
        }

        this->clear();
        this->multiSz = newData;
    }
}


/*
 * the::multi_sz<T>::allocate
 */
template<class T>
typename the::multi_sz<T>::char_type *the::multi_sz<T>::allocate(
        const size_type cnt) {
    THE_STACK_TRACE;
    this->clear();
    if (cnt > 0) {
        this->multiSz = new char_type[cnt];
    }
    return this->multiSz;
}


/*
 * the::multi_sz<T>::get_at
 */
template<class T>
typename the::multi_sz<T>::string_type the::multi_sz<T>::get_at(
        const size_type idx) const {
    THE_STACK_TRACE;
    const char_type *str = multi_sz::peek_at(idx, this->multiSz);
    return (str != nullptr) ? string_type(str) : string_type();
}


/*
 * the::multi_sz<T>::insert
 */
template<class T> 
void the::multi_sz<T>::insert(const size_type idx, const char_type *str) {
    THE_STACK_TRACE;
    size_type oldCnt = multi_sz::count(this->multiSz);

    if (idx == oldCnt) {
        this->add(str);

    } else if (idx < oldCnt) {
        if ((str != nullptr) && (*str != 0)) {
            size_type oldLen = multi_sz::size(this->multiSz);
            size_type strLen = traits_type::length(str) + 1;
            size_type offset = this->peek_at(idx) - this->multiSz;
            char_type *newData = nullptr;
            THE_ASSERT(oldLen > 0);
            THE_ASSERT(strLen > 0);

            newData = new char_type[oldLen + strLen];
            ::memcpy(newData, this->multiSz, offset * sizeof(char_type));
            ::memcpy(newData + offset, str, strLen * sizeof(char_type));
            ::memcpy(newData + offset + strLen, this->multiSz + offset,
                    (oldLen - offset) * sizeof(char_type));
            THE_ASSERT(newData[oldLen + strLen - 1] == 0);
            THE_ASSERT(newData[oldLen + strLen - 2] == 0);

            this->clear();
            this->multiSz = newData;
        }

    } else {
        throw the::index_out_of_range_exception(static_cast<int>(idx), 0,
            static_cast<int>(oldCnt), __FILE__, __LINE__);
    }
}


/*
 * the::multi_sz<T>::remove
 */
template<class T> void the::multi_sz<T>::remove(const char_type *str) {
    THE_STACK_TRACE;
    if ((str != nullptr) && (*str != 0) && (this->multiSz != nullptr)) {
        size_type cnt = 0;
        size_type newLen = 0;
        size_type oldLen = multi_sz::size(this->multiSz);
        size_type strLen = traits_type::length(str) + 1;
        const char_type *cursor = this->multiSz;
        const char_type *s = nullptr;
        const char_type *strStart = nullptr;
        char_type *newData = nullptr;
        char_type *insPos = nullptr;

        /* Count occurrences of string to be removed. */
        while (*cursor != 0) {
            s = str;
            while ((*s == *cursor) && (*cursor != 0)) {
                s++;
                cursor++;
            }
            if (*s == *cursor) {
                cursor++;
                cnt++;
            } else {
                /* Consume the rest of the string. */
                while (*cursor++ != 0);
            }
        }

        /* Reallocate and copy non-removed elements. */
        if ((newLen = oldLen - cnt * strLen) > 2) {
            newData = insPos = new char_type[newLen];
            cursor = this->multiSz;

            while (*cursor != 0) {
                // TODO: Das könnte man mit einer one-pass-Lösung noch
                // chefmäßiger machen.
                s = str;
                strStart = cursor;
                while ((*s == *cursor) && (*cursor != 0)) {
                    s++;
                    cursor++;
                }
                if (*s != *cursor) {
                    /* No match, copy to new list. */
                    cursor = strStart;
                    while ((*insPos++ = *cursor++) != 0);
                } else {
                    /* Skip trailing zero of active string. */
                    cursor++;
                }
            }

            newData[newLen - 1] = 0;    // Enfore double-zero.
            THE_ASSERT(newData[newLen - 1] == 0);
            THE_ASSERT(newData[newLen - 2] == 0);

            this->clear();
            this->multiSz = newData;

        } else {
            /* Remove everything. */
            this->clear();
        } /* end if ((newLen = oldLen - cnt * strLen) > 2) */
    } /* end if ((str != nullptr) && (*str != 0) && ... */
}


/*
 * the::multi_sz<T>::operator =
 */
template<class T>
the::multi_sz<T>& the::multi_sz<T>::operator =(const multi_sz& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        this->clear();
        size_type cnt = rhs.size();
        if (cnt > 0) {
            THE_ASSERT(cnt > 2);
            this->multiSz = new char_type[cnt];
            ::memcpy(this->multiSz, rhs.multiSz, cnt * sizeof(char_type));
        }
    }

    return *this;
}


/*
 * the::multi_sz<T>::operator =
 */
template<class T>
the::multi_sz<T>& the::multi_sz<T>::operator =(const char_type *rhs) {
    THE_STACK_TRACE;
    this->clear();

    if ((rhs != nullptr) && (*rhs != 0)) {
        size_type cnt = multi_sz::size(rhs);
        THE_ASSERT(cnt > 2);
        this->multiSz = new char_type[cnt];
        ::memcpy(this->multiSz, rhs, cnt * sizeof(char_type));
    }

    return *this;
}


/*
 * the::multi_sz<T>::operator ==
 */
template<class T>
bool the::multi_sz<T>::operator ==(const multi_sz& rhs) const {
    THE_STACK_TRACE;
    const char_type *l = this->multiSz;
    const char_type *r = rhs.multiSz;

    if ((l != nullptr) && (r != nullptr)) {
        while ((*l != 0) && (*r != 0)) {
            while (*l++ == *r++) {
                if (*l == 0) {
                    ++l; ++r;
                    break;
                }
            }
        }

        return (*l == *r);

    } else {
        /* 'l' and 'r' must both be nullptr for being equal. */
        return (l == r);
    }
}


/*
 * the::multi_sz<T>::operator []
 */
template<class T> 
typename the::multi_sz<T>::string_type the::multi_sz<T>::operator [](
        const size_type idx) const {
    const char_type *tmp = multi_sz::peek_at(idx, this->multiSz);
    if (tmp != nullptr) {
        return string_type(tmp);
    } else {
        throw the::index_out_of_range_exception(static_cast<int>(idx), 0,
            static_cast<int>(this->count()) - 1, __FILE__, __LINE__);
    }
}


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MULTI_SZ_H_INCLUDED */
