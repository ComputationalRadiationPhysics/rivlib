/*
 * the/collections/array.h
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
/*
 * Array.h
 *
 * Copyright (C) 2006 - 2007 by Universitaet Stuttgart (VIS).
 * Alle Rechte vorbehalten.
 * Copyright (C) 2006 - 2008 by Christoph Mueller. Alle Rechte vorbehalten.
 */

#ifndef THE_COLLECTIONS_ARRAY_H_INCLUDED
#define THE_COLLECTIONS_ARRAY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#ifdef THE_WINDOWS
//#include <cstdlib>
#else /* THE_WINDOWS */
#include <stack>
#endif /* THE_WINDOWS */

//#if !defined(WIN32) || !defined(INCLUDED_FROM_ARRAY_CPP) /* avoid warning LNK4221 */
//#include <stdexcept>
//#endif /* (!defined(WIN32)) || !defined(INCLUDED_FROM_ARRAY_CPP) */
//
#include "the/assert.h"
#include "the/deprecated.h"
#include "the/comparable.h"
#include "the/equatable.h"
#include "the/index_out_of_range_exception.h"
#include "the/invalid_operation_exception.h"
#include "the/memory.h"
#include "the/types.h"

#include "the/collections/array_element_dft_ctor.h"
#include "the/collections/const_enumerator.h"
#include "the/collections/enumerator_base.h"

#include "the/system/threading/auto_lock.h"
#include "the/system/threading/no_lock.h"


namespace the {
namespace collections {


    /**
     * A dynamically growing array of type T.
     *
     * The array grows dynamically by the defined increment, if elements are
     * appended that do not fit into the current capacity. It is never
     * shrinked except on user request.
     *
     * The array used typeless memory that can be reallocated.
     *
     * Note that the array will call the dtor of its elements before the memory
     * of a elements is released, just like the array delete operator. The
     * construction/destruction function C is used for this operation.
     *
     * Class L is a synchronisation primitive that can be used to construct a
     * thread-safe array. By default, the::system::threading::no_lock is used, 
     * i.e. the array is not thread-safe.
     *
     * Note, that thread-safety is not guaranteed for any external iterator 
     * object that might be retrieved from the array. Note, that also 
     * references elements retrieved by accessors are not protected when being
     * used after the accessor returns. To ensure thread-safety over a longer
     * period of time, use the lock() and unlock() methods to synchronise the
     * whole array explicitly.
     *
     * No memory is allocated, if the capacity is forced to zero.
     *
     *
     * TODO: I BELIEVE THIS DOCUMENTATION IS NOT TRUE:
     * Implementation note: The construction/destruction policy of the array is 
     * that all elements within the current capacity must have been constructed.
     * In order to prevent memory leaks in the derived ptr_array class, elements
     * that are erased are immediately dtored and the free element(s) is/are
     * reconstructed using the default ctor after that.
     *
     * @tparam T The type of the elements being stored in the array.
     * @tparam L The type of the synchronisation primitive used to protect the
     *           array against concurrent access.
     * @tparam C The construction/destruction functor for elements.
     */
    template <class T, class L = system::threading::no_lock,
            class C = array_element_dft_ctor<T> >
        class array : public equatable<array<T, L, C> > {

    public:

        /** A constant pointer to the type in the collection. */
        typedef const T *const_pointer_type;

        /** A constant reference to the type in the collection. */
        typedef const T& const_reference_type;

        /** The type used for size differences. */
        typedef ptrdiff_t difference_type;

        /** The type of lock used in the collection. */
        typedef L lock_type;

        /** A pointer to the type in the collection. */
        typedef T *pointer_type;

        /** A reference to the type in the collection. */
        typedef T& reference_type;

        /** The type used for measuring the collection size. */
        typedef size_t size_type;

        /** The type in the collection. */
        typedef T value_type;

        /**
         * An enumerator through the array.
         */
        class enumerator : public enumerator_base<array> {

        protected:

            /** Super class typedef. */
            typedef enumerator_base<array> base;

        public:

            /** The type of collection being enumerated. */
            typedef typename base::collection_type collection_type;

            /** A constant pointer to the type in the collection. */
            typedef typename base::const_pointer_type *const_pointer_type;

            /** A constant reference to the type in the collection. */
            typedef typename base::const_reference_type& const_reference_type;

            /** The type of lock used in the collection. */
            typedef typename base::lock_type lock_type;

            /** A pointer to the type in the collection. */
            typedef typename base::pointer_type *pointer_type;

            /** A reference to the type in the collection. */
            typedef typename base::reference_type& reference_type;

            /** The type in the collection. */
            typedef typename base::value_type value_type;

            /**
             * Initialise a new instance.
             */
            inline enumerator(void) : base(nullptr), pos(0) {
                THE_STACK_TRACE;
            }

            /**
             * Create a clone of 'rhs'.
             *
             * @param rhs The object to be cloned.
             */
            inline enumerator(const enumerator& rhs) : base(nullptr), pos(0) {
                THE_STACK_TRACE;
                *this = rhs;
            }

            /**
             * Initialise a new instance.
             *
             * @param collection The collection being enumerated.
             */
            inline enumerator(collection_type& collection)
                    : base(&collection), pos(0) {
                THE_STACK_TRACE;
            }

            /**
             * Dtor.
             */
            virtual ~enumerator(void);

            /**
             * Answer whether the enumerated collection contains at least one
             * more element.
             *
             * @return true if the collection contains more elements and next()
             *         can be safely called, false otherwise.
             */
            virtual bool has_next(void);

            /**
             * Return the next element in the enumeration.
             *
             * @return The next element.
             */
            virtual reference_type next(void);

            /**
             * Assign values of 'rhs' to this object.
             *
             * @param rhs The right hand side operand.
             *
             * @return *this.
             */
            enumerator& operator =(const enumerator& rhs);

        private:

            /** The current iterator position. */
            size_type pos;
        }; /* end class enumerator */

        /** Enumerator for constant arrays. */
        typedef the::collections::const_enumerator<enumerator> const_enumerator;

#if 0
        /**
         * A random access iterator for arrays.
         */
        class iterator : public std::iterator<std::random_access_iterator_tag,
                value_type, difference_type>, public comparable<iterator> {

        protected:

            /** Super class typedef. */
            typedef std::iterator<std::random_access_iterator_tag,
                value_type, difference_type> base;

        public:

            typedef typename base::iterator_category iterator_category;

            typedef typename base::value_type value_type;

            typedef typename base::difference_type difference_type;

            typedef typename base::distance_type distance_type;

            typedef typename base::pointer pointer;

            typedef typename base::reference reference;

            /**
             * Initialise a new instance.
             */
            inline iterator(void) {
                THE_STACK_TRACE;
            }

            /**
             * Create a clone of 'rhs'.
             *
             * @param rhs The object to be cloned.
             */
            inline iterator(const iterator& rhs) {
                THE_STACK_TRACE;
                *this = rhs;
            }

            /**
             * Dtor.
             */
            virtual ~iterator(void);

            /**
             * Compare two iterators.
             *
             * @param rhs The object to be compared.
             *
             * @return A negative number if this object is less than 'rhs',
             *         zero if this object is equal to 'rhs',
             *         a positive number if this object is larger thatn 'rhs'.
             */
            virtual int compare(const iterator& rhs) const;

            /**
             * Assign values of 'rhs' to this object.
             *
             * @param rhs The right hand side operand.
             *
             * @return *this.
             */
            iterator& operator =(const iterator& rhs);

            reference operator *(void) const;

            pointer operator ->(void) const;

            /**
             * Prefix increment operator.
             *
             * @return The new value of the iterator.
             */
            iterator& operator ++(void);

            /**
             * Postfix increment operator.
             *
             * @return The previous value of the iterator.
             */
            iterator operator ++(int);

            /**
             * Prefix decrement operator.
             *
             * @return The new value of the iterator.
             */
            iterator& operator --(void);

            /**
             * Postfix decrement operator.
             *
             * @return The previous value of the iterator.
             */
            iterator operator --(int);

            iterator operator +(const difference_type rhs) const;

            iterator& operator +=(const difference_type rhs);

            iterator operator -(const difference_type rhs) const;

            iterator& operator -=(const difference_type rhs);

            iterator& operator +(const iterator rhs) const;

            iterator& operator -(const iterator rhs) const;

            reference operator [](const difference_type& n) const;

            difference_type operator +(const iterator& rhs) const;

            difference_type operator -(const iterator& rhs) const;

        private:

        }; /* end class iterator */
#endif

        /** The default initial capacity */
        static const size_type default_capacity;

        /** The default initial capacity increment */
        static const size_type default_capacity_increment;

        /** This constant signals an invalid index position. */
        static const difference_type invalid_index;

        /** 
         * Create an array with the specified initial capacity.
         *
         * @param capacity  The initial capacity of the array.
         * @param increment The initial capacity increment of the array.
         */
        array(const size_t capacity = default_capacity, const size_t increment
            = default_capacity_increment);

        /**
         * Create a new array with the specified initial count and capacity
         * and use 'element' as default value for all these elements.
         *
         * @param count     The initial capacity of the array and number of
         *                  elements.
         * @param element   The default value to set.
         * @param increment The initial capacity increment of the array.
         */
        array(const size_t count, const T& element, const size_t increment
            = default_capacity_increment);

        /**
         * Clone 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        array(const array& rhs);

        /** Dtor. */
        virtual ~array(void);

        /** 
         * Appends an element to the end of the array. If necessary, the 
         * capacity is increased by the capacity increment.
         *
         * @param element The item to be appended.
         */
        inline void add(const_reference_type element) {
            THE_STACK_TRACE;
            this->append(element);
        }

        /** 
         * Appends an element to the end of the array. If necessary, the 
         * capacity is increased by the capacity increment.
         *
         * @param element The item to be appended.
         */
        void append(const_reference_type element);

        /**
         * Reserves memory for at least 'capacity' elements in the array. If
         * 'capacity' is less than or equal to the current capacity of the 
         * array, this method has no effect.
         *
         * @param capacity The minimum number of elements that should be 
         *                 allocated.
         *
         * @return The actual capacity of the array.
         *
         * @throws std::bad_alloc If there was insufficient memory for 
         *                        allocating the array.
         */
        size_type assert_capacity(const size_type capacity);

        /**
         * Erase all elements from the array.
         *
         * @param doTrim If true, the array is trimmed to have zero size,
         *               otherwise the memory remains allocated.
         *               This parameter defaults to false.
         */
        void clear(const bool doTrim = false);

        /**
         * Answer the capacity of the array.
         *
         * @return The number of elements that are currently allocated.
         */
        inline size_type capacity(void) const {
            THE_STACK_TRACE;
            auto_lock_type l(this->syncRoot);
            return this->maxSize;
        }

        /**
         * Answer the capacity increment of the array.
         *
         * @return The capacity increment
         */
        inline size_type capacity_increment(void) const {
            THE_STACK_TRACE;
            auto_lock_type l(this->syncRoot);
            return this->capacityIncrement;
        }

        /**
         * Answer whether 'element' is in the array.
         *
         * @param element The element to be tested.
         *
         * @return true, if 'element' is at least once in the array, false 
         *         otherwise.
         */
        inline bool contains(const_reference_type element) const {
            THE_STACK_TRACE;
            return (this->index_of(element) >= 0);
        }

        ///**
        // * Answer the number of items in the array. Note that the result is not
        // * the capacity of the array which is currently allocated.
        // *
        // * @return Number of items in the array.
        // */
        //virtual size_t count(void) const;

        /**
         * Exposes the internal data pointer.
         *
         * Using this method is inherently unsafe as the pointer is only 
         * guaranteed to be valid until an arbitrary method is called on the
         * array.
         *
         * YOU SHOULD NEVER MODIFY THIS POINTER, ESPECIALLY NEVER RELEASE THE
         * MEMORY!
         *
         * The data pointer might be NULL.
         *
         * @return The internal data pointer.
         */
        inline pointer_type data(void) {
            THE_STACK_TRACE;
            return this->elements;
        }

        /**
         * Exposes the internal data pointer.
         *
         * Using this method is inherently unsafe as the pointer is only 
         * guaranteed to be valid until an arbitrary method is called on the
         * array.
         *
         * YOU SHOULD NEVER MODIFY THIS POINTER, ESPECIALLY NEVER RELEASE THE
         * MEMORY!
         *
         * The data pointer might be NULL.
         *
         * @return The internal data pointer.
         */
        inline const_pointer_type data(void) const {
            THE_STACK_TRACE;
            return this->elements;
        }

        /**
         * Answer whether there is no element in the array. Note that a return
         * value of true does not mean that no memory is allocated.
         *
         * @return true, if there is no element in the array, false otherwise.
         */
        inline bool empty(void) const {
            THE_STACK_TRACE;
            auto_lock_type l(this->syncRoot);
            return (this->curSize == 0);
        }

        /**
         * Determines whether this object and 'rhs' are equal.
         *
         * @param rhs The object to be compared.
         *
         * @return true if this object and 'rhs' are equal, false otherwise.
         */
        virtual bool equals(const array& rhs) const;

        /**
         * Erase the element at position 'idx' from the array. If 'idx' is out
         * of range, this method has no effect.
         *
         * @param idx The index of the element to be removed.
         */
        void erase(const size_type idx);

        /**
         * Erase all 'cnt' elements beginning at 'beginIdx'. If elements out of
         * range would be affected, these are simply ignored.
         *
         * @param beginIdx The index of the first element to be removed.
         * @param cnt      The number of elements to be removed.
         */
        void erase(const size_type beginIdx, const size_t cnt);

        /**
         * Answer an enumerator over the array.
         *
         * If the collection is protected by an actual lock, the enumerator
         * will acquire the lock and hold it until (i) it reaches the end of
         * the enumeration, or (ii) the enumerator instance is destroyed,
         * whatever happens first.
         *
         * Please be aware that enumerators of collections protected by
         * actual locks cannot be transported from one thread to another!
         *
         * @return An enumerator starting at the begin of the array.
         */
        inline enumerator get_enumerator(void) {
            THE_STACK_TRACE;
            return enumerator(*this);
        }

        /**
         * Answer an enumerator over the array.
         *
         * If the collection is protected by an actual lock, the enumerator
         * will acquire the lock and hold it until (i) it reaches the end of
         * the enumeration, or (ii) the enumerator instance is destroyed,
         * whatever happens first.
         *
         * Please be aware that enumerators of collections protected by
         * actual locks cannot be transported from one thread to another!
         *
         * @return An enumerator starting at the begin of the array.
         */
        inline const_enumerator get_enumerator(void) const {
            THE_STACK_TRACE;
            return const_enumerator(enumerator(*const_cast<array *>(this)));
        }

        /**
         * Answer a pointer to the first copy of 'element' in the array. If no
         * element equal to 'element' is found, a NULL pointer is returned.
         *
         * @param element The element to be tested.
         *
         * @return A pointer to the local copy of 'element' or NULL, if no such
         *         element is found.
         */
        inline pointer_type find(const_reference_type element) {
            THE_STACK_TRACE;
            difference_type idx = this->index_of(element);
            return (idx >= 0) ? (this->elements + idx) : NULL;
        }

        /**
         * Answer a pointer to the first copy of 'element' in the array. If no
         * element equal to 'element' is found, a NULL pointer is returned.
         *
         * @param element The element to be tested.
         *
         * @return A pointer to the local copy of 'element' or NULL, if no such
         *         element is found.
         */
        inline const_pointer_type find(const_reference_type element) const {
            THE_STACK_TRACE;
            difference_type idx = this->index_of(element);
            return (idx >= 0) ? (this->elements + idx) : NULL;
        }

        /**
         * Answer the first element in the array.
         *
         * @return A reference to the first element in the array.
         *
         * @throws OutOfRangeException, if the array is empty.
         */
        inline reference_type first(void) {
            THE_STACK_TRACE;
            return (*this)[0];
        }

        /**
         * Answer the first element in the array.
         *
         * @return A reference to the first element in the array.
         *
         * @throws OutOfRangeException, if the array is empty.
         */
        inline const_reference_type first(void) const {
            THE_STACK_TRACE;
            return (*this)[0];
        }

        /**
         * Answer the index of the first occurrence of 'element' in the array
         * after 'beginAt'.
         *
         * @param element The element to be searched.
         * @param beginAt The first index to be checked. Defaults to 0.
         *
         * @return The index of the first occurrence of 'element' in the array,
         *         or invalid_index if the element is not in the array.
         */
        difference_type index_of(const_reference_type element,
            const size_t beginAt = 0) const;

        /**
         * Insert 'element' at position 'idx' in the array. All elements behind
         * 'idx' are shifted one element right. 'idx' must be a valid index in 
         * the array or the index directly following the end, i. e. Count(). In
         * the latter case, the method behaves like Append().
         *
         * @param idx     The position to insert the element at.
         * @param element The element to add.
         *
         * @throws OutOfRangeException If 'idx' is not within 
         *                             [0, this->Count()].
         */
        void insert(const size_t idx, const_reference_type element);

        /**
         * Answer the last element in the array.
         *
         * @return A reference to the last element in the array.
         *
         * @throws OutOfRangeException, if the array is empty.
         */
        inline reference_type& last(void) {
            // TODO: Implementation depends of numeric overflow! This is hugly.
            THE_STACK_TRACE;
            return (*this)[this->curSize - 1];
        }

        /**
         * Answer the last element in the array.
         *
         * @return A reference to the last element in the array.
         *
         * @throws OutOfRangeException, if the array is empty.
         */
        inline const_reference_type last(void) const {
            // TODO: Implementation depends of numeric overflow! This is hugly.
            THE_STACK_TRACE;
            return (*this)[this->curSize - 1];
        }

        /**
         * Lock the array for the calling thread.
         */
        inline void lock(void) const {
            THE_STACK_TRACE;
            this->syncRoot.lock();
        }

        /**
         * Add 'element' as first element of the array. If necessary, the 
         * capacity is increased by the capacity increment.
         *
         * @param element The element to be added.
         */
        void prepend(const_reference_type element);

        /** 
         * Appends an element to the end of the array. If necessary, the 
         * capacity is increased by the capacity increment.
         *
         * @param element The item to be appended.
         */
        inline void push_back(const_reference_type element) {
            THE_STACK_TRACE;
            this->append(element);
        }

        /**
         * Add 'element' as first element of the array. If necessary, the 
         * capacity is increased by the capacity increment.
         *
         * @param element The element to be added.
         */
        inline void push_front(const_reference_type element) {
            THE_STACK_TRACE;
            this->prepend(element);
        }

        /**
         * Remove an element that is equal to 'element' from the collection.
         *
         * @param element   The element to be removed.
         * @param firstOnly If true, remove only the first occurrence of 
         *                  'element', otherwise, remove all. This parameter
         *                  defaults to false.
         */
        void remove(const_reference_type element, const bool firstOnly = false);

        /**
         * Erase the element at position 'idx' from the array. If 'idx' is out
         * of range, this method has no effect.
         *
         * @param idx The index of the element to be removed.
         */
        inline void remove_at(const size_type idx) {
            THE_STACK_TRACE;
            this->erase(idx);
        }

        /**
         * Remove the first element from the collection.
         */
        inline void remove_first(void) {
            THE_STACK_TRACE;
            this->erase(0);
        }

        /**
         * Remove the last element from the collection.
         */
        void remove_last(void) {
            THE_STACK_TRACE;
            this->erase(this->curSize - 1);
        }

        /**
         * Resize the array to have exactly 'capacity' elements. If 'capacity'
         * is less than the current number of elements in the array, all 
         * elements at and behind the index 'capacity' are erased.
         *
         * @param capacity The new size of the array.
         */
        void resize(const size_t capacity);

        /**
         * Set the capacity increment for the array.
         *
         * @param capacityIncrement the new increment
         */
        void set_capacity_increment(const size_t capacityIncrement);

        /**
         * Make the array assume that it has 'count' valid elements. If 'count'
         * is less than the current number of elements, this has the same effect
         * as erasing all elements beginning at index 'count' until the end. If
         * 'count' is larger than the current element count, the requested 
         * number of elements is allocated. Note that the new elements might 
         * have undefined content. The method only guarantees the default ctor
         * to be called.
         *
         * @param count The new number of elements in the array.
         */
        void set_size(const size_t count);

        /**
         * Answer the current size (number of valid elements) of the array.
         *
         * @return The size of the array.
         */
        inline size_type size(void) const {
            THE_STACK_TRACE;
            auto_lock_type l(this->syncRoot);
            return this->curSize;
        }

        /**
         * Sorts the elements in the collection based on the results of the 
         * 'comparator' function:
         *   = 0 if lhs == rhs
         *   < 0 if lhs < rhs
         *   > 0 if lhs > rhs
         *
         * @param comparator The compare function defining the sort order.
         */
        THE_DEPRECATED void sort(int (*comparator)(const T& lhs, const T& rhs));

        /**
         * Trim the capacity of the array to match the current number of 
         * elements. This has the same effect as calling Resize(Count()).
         */
        inline void trim(void) {
            this->resize(this->curSize);
        }

        /**
         * Release the lock that the calling thread has on the array.
         */
        inline void unlock(void) const {
            THE_STACK_TRACE;
            this->syncRoot.unlock();
        }

        /**
         * Access the 'idx'th element in the array.
         *
         * @param idx The index of the element to access. This must be a value
         *            within [0, this->Count()[.
         *

         * @return A reference to the 'idx'th element.
         *
         * @throws OutOfRangeException If 'idx' is not within 
         *                             [0, this->Count()[.
         */
        reference_type operator [](const size_type idx);

        /**
         * Access the 'idx'th element in the array.
         *
         * @param idx The index of the element to access. This must be a value
         *            within [0, this->Count()[.
         *
         * @return A reference to the 'idx'th element.
         *
         * @throws OutOfRangeException If 'idx' is not within 
         *                             [0, this->Count()[.
         */
        const_reference_type operator [](const size_type idx) const;

//        /**
//         * Access the 'idx'th element in the array.
//         *
//         * @param idx The index of the element to access. This must be a value
//         *            within [0, this->Count()[.
//         *
//         * @return A reference to the 'idx'th element.
//         *
//         * @throws OutOfRangeException If 'idx' is not within 
//         *                             [0, this->Count()[.
//         */
//        inline T& operator [](const int idx) {
//            return (*this)[static_cast<size_t>(idx)];
//        }
//
//        /**
//         * Access the 'idx'th element in the array.
//         *
//         * @param idx The index of the element to access. This must be a value
//         *            within [0, this->Count()[.
//         *
//         * @return A reference to the 'idx'th element.
//         *
//         * @throws OutOfRangeException If 'idx' is not within 
//         *                             [0, this->Count()[.
//         */
//        const T& operator [](const int idx) const {
//            return (*this)[static_cast<size_t>(idx)];
//        }
//
//#if (defined THE_WINDOWS64) || (defined THE_LINUX64)
//        // this define is correct!
//        //  used on all windows platforms 
//        //  and on 64 bit linux!
//
//        /**
//         * Access the 'idx'th element in the array.
//         *
//         * @param idx The index of the element to access. This must be a value
//         *            within [0, this->Count()[.
//         *
//         * @return A reference to the 'idx'th element.
//         *
//         * @throws OutOfRangeException If 'idx' is not within 
//         *                             [0, this->Count()[.
//         */
//        inline T& operator [](const unsigned int idx) {
//            return (*this)[static_cast<size_t>(idx)];
//        }
//
//        /**
//         * Access the 'idx'th element in the array.
//         *
//         * @param idx The index of the element to access. This must be a value
//         *            within [0, this->Count()[.
//         *
//         * @return A reference to the 'idx'th element.
//         *
//         * @throws OutOfRangeException If 'idx' is not within 
//         *                             [0, this->Count()[.
//         */
//        const T& operator [](const unsigned int idx) const {
//            return (*this)[static_cast<size_t>(idx)];
//        }
//#endif /* (defined THE_WINDOWS) || (defined THE_LINUX64) */

        /**
         * Assignment.
         *
         * @param rhs The right hand operand.
         *
         * @return *this.
         */
        array& operator =(const array& rhs);

    private:

        /** The type of the scoped lock to acquire 'syncRoot'. */
        typedef the::system::threading::auto_lock<L> auto_lock_type;

        /** Super class typedef. */
        typedef equatable<array> base;

#ifdef THE_WINDOWS
        /**
         * Helper function used by the quicksort algorithm.
         *
         * @param context The context of the comparison
         * @param lhs The left hand side operand
         * @param rhs The right hand side operand
         *
         * @return The compare value used by 'qsort_s'
         */
        static int qsort_helper(void * context, const void * lhs, 
            const void * rhs);
#endif /* THE_WINDOWS */

        /** The capacity increment */
        size_type capacityIncrement;

        /** The number of used elements in 'elements' */
        size_type curSize;

        /** The actual array (PtrArray must have access) */
        pointer_type elements;

        /** The number of actually allocated elements in 'elements' */
        size_type maxSize;

        /** The lock protecting access to the array. */
        mutable lock_type syncRoot;
    };


    // TODO: Remove this after adding STL iterators!
    /*
     * array<T, L, C>::sort
     */
    template<class T, class L, class C>
    void array<T, L, C>::sort(int (*comparator)(const T& lhs, const T& rhs)) {
        this->lock();

#ifdef THE_WINDOWS
        ::qsort_s(this->elements, this->curSize, sizeof(T), qsort_helper, 
            reinterpret_cast<void *>(comparator));

#else /* THE_WINDOWS */
        // qsort by hand, because of frowsty ANSI api (based on Sedgewick)
        uint64_t l, r, i, j;
        T tmp;
        std::stack<uint64_t> stack;

        l = 0;
        r = this->curSize - 1;

        stack.push(l); 
        stack.push(r);

        do {
            if (r > l) {
                // i = partition(l, r);
                T value = this->elements[r];
                i = l - 1;
                j = r;
                do {
                    do { 
                        i = i + 1;
                    } while (comparator(this->elements[i], value) < 0);
                    do { 
                        j = j - 1;
                    } while (comparator(this->elements[j], value) > 0);
                    tmp = this->elements[i];
                    this->elements[i] = this->elements[j];
                    this->elements[j] = tmp;
                } while (j > i);
                this->elements[j] = this->elements[i];
                this->elements[i] = this->elements[r];
                this->elements[r] = tmp;

                // recursion (unfold using the stack)
                if ((i - l) > (r - i)) {
                    stack.push(l);
                    stack.push(i - 1);
                    l = i + 1;
                } else {
                    stack.push(i + 1);
                    stack.push(r);
                    r = i - 1;
                }
            } else {
                r = stack.top();
                stack.pop();
                l = stack.top();
                stack.pop();
            }
        } while (!stack.empty());

#endif /* THE_WINDOWS */

        this->unlock();
    }

#ifdef THE_WINDOWS
    /* 
     * array<T, L, C>::qsort_helper
     */
    template<class T, class L, class C> 
    int array<T, L, C>::qsort_helper(void * context, const void * lhs, 
            const void * rhs) {

        int (*comparator)(const T& lhs, const T& rhs) 
            = reinterpret_cast<int (*)(const T& lhs, const T& rhs)>(context);

        return comparator(*static_cast<const T*>(lhs), 
            *static_cast<const T*>(rhs));
    }
#endif /* THE_WINDOWS */

} /* end namespace collections */
} /* end namespace the */


/*
 * the::collections::array<T, L, C>::enumerator::~enumerator
 */
template<class T, class L, class C>
the::collections::array<T, L, C>::enumerator::~enumerator(void) {
    THE_STACK_TRACE;
}


/*
 * the::collections::array<T, L, C>::enumerator::has_next
 */
template<class T, class L, class C>
bool the::collections::array<T, L, C>::enumerator::has_next(void) {
    THE_STACK_TRACE;
    array *c = this->get_collection();
    bool retval = ((c != nullptr) && (this->pos < c->size()));
    if (!retval) {
        this->unlock_collection();
        THE_ASSERT(this->get_collection() == nullptr);
    }
    return retval;
}


/*
 * the::collections::array<T, L, C>::enumerator::next
 */
template<class T, class L, class C>
typename the::collections::array<T, L, C>::enumerator::reference_type
the::collections::array<T, L, C>::enumerator::next(void) {
    THE_STACK_TRACE;
    if (!this->has_next()) {
        THROW_THE_EXCEPTION(invalid_operation_exception, "The array "
            "does not contain more elements.");
    }
    return (*this->get_collection())[this->pos++];
}


/*
 * the::collections::array<T, L, C>::enumerator::operator =
 */
template<class T, class L, class C>
typename the::collections::array<T, L, C>::enumerator&
the::collections::array<T, L, C>::enumerator::operator =(
        const enumerator& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        base::operator =(rhs);
        this->pos = rhs.pos;
    }
    return *this;
}


/*
 * the::collections::array<T, L, C>::default_capacity
 */
template<class T, class L, class C>
const typename the::collections::array<T, L, C>::size_type
the::collections::array<T, L, C>::default_capacity = 8;


/*
 * the::collections::array<T, L, C>::default_capacity_increment
 */
template<class T, class L, class C>
const typename the::collections::array<T, L, C>::size_type
the::collections::array<T, L, C>::default_capacity_increment = 1;


/*
 * the::collections::array<T, L, C>::invalid_index
 */
template<class T, class L, class C>
const typename the::collections::array<T, L, C>::difference_type
the::collections::array<T, L, C>::invalid_index = -1;


/*
 * the::collections::array<T, L, C>::array
 */
template<class T, class L, class C>
the::collections::array<T, L, C>::array(const size_t capacity,
        const size_t capacityIncrement) 
    : base(), capacityIncrement(capacityIncrement), curSize(0),
        elements(NULL), maxSize(0) {
    THE_STACK_TRACE;
    THE_ASSERT(capacityIncrement > 0);
    this->assert_capacity(capacity);
}


/*
 * the::collections::array<T, L, C>::array
 */
template<class T, class L, class C>
the::collections::array<T, L, C>::array(const size_t count, const T& element,
        const size_t capacityIncrement) 
    : base(), capacityIncrement(capacityIncrement), curSize(count), 
        elements(NULL), maxSize(0) {
    THE_STACK_TRACE;
    THE_ASSERT(capacityIncrement > 0);

    auto_lock_type l(this->syncRoot);
    this->assert_capacity(count);
    for (size_type i = 0; i < this->curSize; i++) {
        this->elements[i] = element;
    }
}


/*
 * the::collections::array<T, L, C>::array
 */
template<class T, class L, class C>
the::collections::array<T, L, C>::array(const array& rhs)
        : base(), capacityIncrement(1), curSize(0),
        elements(NULL), maxSize(0) {
    THE_STACK_TRACE;
    *this = rhs;
}


/*
 * the::collections::array<T, L, C>::~array
 */
template<class T, class L, class C>
the::collections::array<T, L, C>::~array(void) {
    THE_STACK_TRACE;
    this->resize(0);
    THE_ASSERT(this->elements == NULL);
}


/*
 * the::collections::array<T, L, C>::append
 */
template<class T, class L, class C>
void the::collections::array<T, L, C>::append(const T& element) {
    THE_STACK_TRACE;
    auto_lock_type l(this->syncRoot);
    this->assert_capacity(this->curSize + 1);
    this->elements[this->curSize] = element;
    this->curSize++;
}


/*
 * the::collections::array<T, L, C>::assert_capacity
 */
template<class T, class L, class C>
typename the::collections::array<T, L, C>::size_type
the::collections::array<T, L, C>::assert_capacity(const size_t capacity) {
    THE_STACK_TRACE;
    auto_lock_type l(this->syncRoot);
    if (this->maxSize < capacity) {
        if (capacity - this->maxSize < this->capacityIncrement) {
            this->resize(this->maxSize + this->capacityIncrement);
        } else {
            this->resize(capacity);
        }
    }
    return this->maxSize;
}


/*
 * the::collections:array<T, L, C>::clear
 */
template<class T, class L, class C>
void the::collections::array<T, L, C>::clear(const bool doTrim) {
    THE_STACK_TRACE;
    auto_lock_type l(this->syncRoot);
    this->curSize = 0;
    if (doTrim) {
        this->trim();
    }
}


/*
 * the::collections::equals
 */
template<class T, class L, class C>
bool the::collections::array<T, L, C>::equals(const array& rhs) const {
    THE_STACK_TRACE;

    if (this == &rhs) {
        return true;
    }

    auto_lock_type l(this->syncRoot);
    if (this->curSize != rhs.curSize) {
        return false;
    }

    //for (size_t i = 0; i < this->curSize; i++) {
    //    if (!(this->elements[i] == rhs.elements[i])) {
    //        return false;
    //    }
    //}
    ///* No difference found. */
    //return true;
    return (::memcmp(this->elements, rhs.elements,
        this->curSize * sizeof(value_type)) == 0);
}


/*
 * the::collections::array<T, L, C>::erase
 */
template<class T, class L, class C>
void the::collections::array<T, L, C>::erase(const size_type idx) {
    THE_STACK_TRACE;
    auto_lock_type l(this->syncRoot);

    if (idx < this->curSize) {
        /* Destruct element to erase. */
        C::dtor(this->elements + idx);

        /* Move elements forward. */
        for (size_type i = idx + 1; i < this->curSize; i++) {
            this->elements[i - 1] = this->elements[i];
        }
        this->curSize--;

        /* Element after valid range must now be reconstructed. */
        C::ctor(this->elements + this->curSize);
    }
}


/*
 * the::collections::array<T, L, C>::erase
 */
template<class T, class L, class C>
void the::collections::array<T, L, C>::erase(const size_type beginIdx,
        const size_type cnt) {
    THE_STACK_TRACE;
    size_type cntRemoved = cnt;
    size_type range = cnt;
    size_type cntMove = range;

    auto_lock_type l(this->syncRoot);

    if (beginIdx < this->curSize) {

        /* Sanity check. */
        if (beginIdx + range >= this->curSize) {
            cntRemoved = range = cntMove = this->curSize - beginIdx;
        }
        THE_ASSERT(beginIdx + range <= this->curSize);

        /* Dtor element range to erase. */
        for (size_type i = beginIdx; i < beginIdx + range; i++) {
            C::dtor(this->elements + i);
        }

        /* Fill empty range. */
        for (size_type i = beginIdx + range; i < this->curSize; i += range) {
            if (i + range >= this->curSize) {
                cntMove = this->curSize - i;
            }
            ::memcpy(this->elements + (i - range), this->elements + i, 
                cntMove * sizeof(T));
        }
        this->curSize -= cntRemoved;

        /* 'cntRemoved' elements after valid range must be reconstructed. */
        for (size_type i = this->curSize; i < this->curSize + cntRemoved; i++) {
            C::ctor(this->elements + i);
        }
    }
}


/*
 * the::collections::array<T, L, C>::index_of
 */
template<class T, class L, class C>
typename the::collections::array<T, L, C>::difference_type
the::collections::array<T, L, C>::index_of(const_reference_type element,
        const size_type beginAt) const {
    THE_STACK_TRACE;
    auto_lock_type l(this->syncRoot);

    for (size_type i = 0; i < this->curSize; i++) {
        if (this->elements[i] == element) {
            return i;
        }
    }
    /* Nothing found. */

    return invalid_index;
}


/*
 * the::collections::array<T, L, C>::insert
 */
template<class T, class L, class C>
void the::collections::array<T, L, C>::insert(const size_type idx,
        const_reference_type element) {
    THE_STACK_TRACE;
    auto_lock_type l(this->syncRoot);

    if (idx <= this->curSize) {
        this->assert_capacity(this->curSize + 1);

        for (size_type i = this->curSize; i > idx; i--) {
            this->elements[i] = this->elements[i - 1];
        }

        this->elements[idx] = element;
        this->curSize++;

    } else {
        throw index_out_of_range_exception(static_cast<int>(idx), 0,
            static_cast<int>(this->curSize), __FILE__, __LINE__);
    }
}


/*
 * the::collections::array<T, L, C>::prepend
 */
template<class T, class L, class C>
void the::collections::array<T, L, C>::prepend(const_reference_type element) {
    THE_STACK_TRACE;
    auto_lock_type l(this->syncRoot);

    // TODO: This implementation is extremely inefficient. Could use single
    // memcpy when reallocating.

    this->assert_capacity(this->curSize + 1);

    for (size_type i = this->curSize; i > 0; i--) {
        this->elements[i] = this->elements[i - 1];
    }

    this->elements[0] = element;
    this->curSize++;
}


/*
 * the::collections::array<T, L, C>::remove
 */
template<class T, class L, class C>
void the::collections::array<T, L, C>::remove(const_reference_type element,
        const bool firstOnly) {
    THE_STACK_TRACE;
    auto_lock_type l(this->syncRoot);

    for (size_type i = 0; i < this->curSize; i++) {
        if (this->elements[i] == element) {
            /* Dtor element to remove. */
            C::dtor(this->elements + i);

            /* Move elements forward. */
            for (size_type j = i + 1; j < this->curSize; j++) {
                this->elements[j - 1] = this->elements[j];
            }
            this->curSize--;

            /* Reconstruct invalid element at end. */
            C::ctor(this->elements + this->curSize);

            if (firstOnly) {
                break;          // Remove first element only.
            } else {
                --i;            // One index was lost, so next moved back.
            }
        }
    } /* end for (size_type i = 0; i < this->curSize; i++) */
}


/*
 * the::collections::array<T, L, C>::resize
 */
template<class T, class L, class C>
void the::collections::array<T, L, C>::resize(const size_type capacity) {
    THE_STACK_TRACE;
    auto_lock_type l(this->syncRoot);
    void *newPtr = NULL;
    size_t oldCapacity = this->maxSize;

    /*
     * Erase elements, if new capacity is smaller than old one. Ensure that 
     * the dtor of the elements is called, as we use typeless memory for the
     * array.
     */
    if (capacity < this->maxSize) {
        for (size_t i = capacity; i < this->maxSize; i++) {
            C::dtor(this->elements + i);
        }

        if (capacity < this->curSize) {
            /* Count cannot exceed capacity. */
            this->curSize = capacity;
        }
    }

    /* Allocate the new capacity. */
    this->maxSize = capacity;
    if (this->maxSize == 0) {
        /* Array is empty now, make 'this->elements' a NULL pointer. */
        safe_free(this->elements);

    } else if (oldCapacity != this->maxSize) {
        /* Reallocate elements. */
        if ((newPtr = ::realloc(this->elements, this->maxSize
                * sizeof(value_type))) != NULL) {
            this->elements = static_cast<T *>(newPtr);

            for (size_type i = oldCapacity; i < this->maxSize; i++) {
                C::ctor(this->elements + i);
            }

        } else {
            /* Allocation failed. */
            for (size_type i = 0; i < oldCapacity; i++) {
                C::dtor(this->elements + i);
            }
            safe_free(this->elements);
            throw std::bad_alloc();
        }
    }
}


/*
 * the::collections::array<T, L, C>::set_size
 */
template<class T, class L, class C>
void the::collections::array<T, L, C>::set_size(const size_type count) {
    THE_STACK_TRACE;
    auto_lock_type l(this->syncRoot);

    if (count < this->curSize) {
        this->erase(count, this->curSize - count);
    } else {
        this->assert_capacity(count);
        this->curSize = count;
    }
}


/*
 * the::collections::array<T, L, C>::set_capacity_increment
 */
template<class T, class L, class C>
void the::collections::array<T, L, C>::set_capacity_increment(
        const size_type capacityIncrement) {
    THE_STACK_TRACE;
    auto_lock_type l(this->syncRoot);
    this->capacityIncrement = capacityIncrement;
}


/*
 * the::collections::array<T, L, C>::operator []
 */
template<class T, class L, class C>
typename the::collections::array<T, L, C>::reference_type
the::collections::array<T, L, C>::operator [](const size_type idx) {
    THE_STACK_TRACE;
    auto_lock_type l(this->syncRoot);

    if (idx < this->curSize) {
        return this->elements[idx];
    } else {
        throw index_out_of_range_exception(static_cast<int>(idx), 0,
            static_cast<int>(this->curSize - 1), __FILE__, __LINE__);
    }
}


/*
 * the::collections::array<T, L, C>::operator []
 */
template<class T, class L, class C>
typename the::collections::array<T, L, C>::const_reference_type
the::collections::array<T, L, C>::operator [](const size_type idx) const {
    THE_STACK_TRACE;
    auto_lock_type l(this->syncRoot);

    if (idx < this->curSize) {
        return this->elements[idx];
    } else {
        throw index_out_of_range_exception(static_cast<int>(idx), 0,
            static_cast<int>(this->curSize - 1), __FILE__, __LINE__);
    }
}


/*
 * the::collections::array<T, L, C>::operator =
 */
template<class T, class L, class C>
the::collections::array<T, L, C>& the::collections::array<T, L, C>::operator =(
        const array& rhs) {
    THE_STACK_TRACE;

    if (this != &rhs) {
        auto_lock_type l(this->syncRoot);

        this->resize(rhs.maxSize);
        this->curSize = rhs.curSize;
        this->capacityIncrement = rhs.capacityIncrement;

        for (size_type i = 0; i < rhs.curSize; i++) {
            this->elements[i] = rhs.elements[i];
        }
    }

    return *this;
}


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_COLLECTIONS_ARRAY_H_INCLUDED */
