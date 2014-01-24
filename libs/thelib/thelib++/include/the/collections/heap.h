/*
 * the/collections/heap.h
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
 * Heap.h
 *
 * Copyright (C) 2006 - 2007 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_COLLECTIONS_HEAP_H_INCLUDED
#define THE_COLLECTIONS_HEAP_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/collections/array.h"

#include "the/assert.h"
#include "the/deprecated.h"
#include "the/utils.h"



namespace the {
namespace collections {


    /**
     * This class implements a binary heap, which can be used as a priority
     * queue.
     *
     * The heap accepts multiple entries with the same key, i. e. the same
     * priporty.
     *
     * The return order of elements having the same key is undefined as
     * heaps are not stable.
     *
     * The template class T must expose a field
     *
     *   key_type first
     *
     * with key_type having an operator <
     *
     * You can use std::pair for instantiating a heap using the first element
     * as key.
     *
     * Rationale: We decided to implement a binary heap because it has no memory
     * overhead (if implemented using an array). Binomial heaps and Fibonacci 
     * heaps are faster, but must be implemented using double-linked lists.
     */
    template<class T> class heap {

    public:

        /** 
         * Create a new heap with the specified initial capacity allocated for
         * elements.
         * 
         * It is important to specify a meaningful capacity, e. g. the maximum
         * number of elements expected to be queued at one time in a priority
         * queue.
         *
         * @param capacity The initial capacity of the underlying array. This
         *                 amount of memory is directly allocated. If the heap,
         *                 however, needs more memory, it will dynamically 
         *                 reallocate the array.
         */
        heap(const size_t capacity = array<T>::default_capacity);

        /** Dtor. */
        ~heap(void);

        /**
         * Add a new element to the heap.
         *
         * This method has logarithmic runtime complexity.
         */
        inline void add(const T& element) {
            this->elements.append(element);
            THE_ASSERT(!this->elements.empty());
            this->sift_up(this->elements.size() - 1);
        }

        /**
         * Reserves memory for at least 'capacity' elements in the heap. If
         * 'capacity' is less than or equal to the current capacity of the 
         * array, this method has no effect.
         *
         * @param capacity The minimum number of elements that should be 
         *                 allocated.
         *
         * @throws std::bad_alloc If there was insufficient memory for 
         *                        allocating the array.
         */
        inline void assert_capacity(const size_t capacity) {
            this->elements.assert_capacity(capacity);
        }

        /**
         * Answer the number of entries allocated for the heap.
         *
         * @return The current capacity of the heap.
         */
        inline size_t capacity(void) const {
            return this->elements.capacity();
        }

        /**
         * Remove all elements from the heap.
         */
        inline void clear(void) {
            this->elements.clear();
        }

        // TODO: Contains using template for key?

        /**
         * Answer the number of elements in the heap.
         *
         * @return The number of elements in the heap.
         */
        inline size_t count(void) const {
            return this->elements.size();
        }

        // TODO: Find using template for key?

        /**
         * Answer the first element in the heap, i. e. its root.
         *
         * This method has constant runtime complexity.
         *
         * @return The root element of the heap.
         *
         * @throws OutOfRangeException, if the heap is empty.
         */
        inline const T& first(void) const {
            return this->elements[0];
        }

        /**
         * Answer whether the heap is empty. 
         * 
         * Note, that even if the heap is empty, memory might be allocated. Use
         * Capacity to determine the current capacity of the heap.
         *
         * @return true, if no element is in the heap, false otherwise.
         */
        inline bool is_empty(void) const {
            return this->elements.empty();
        }

        /**
         * Remove the root of the heap.
         *
         * This method has logarithmic runtime complexity.
         */
        void remove_first(void);

        /**
         * Trim the capacity of the heap to match the current number of 
         * elements.
         */
        inline void trim(void) {
            this->elements.trim();
        }

    private:

        /** 
         * Enforces the heap property after removing an element at 'idx'.
         *
         * @param idx The index of the element removed. This should be the first
         *            element.
         */
        void sift_down(size_t idx);

        /**
         * Enforces the heap property after inserting an element at 'idx'.
         *
         * @param idx The index of the inserted element. This should be the last
         *            element.
         */
        void sift_up(size_t idx);

        /** The array of heap elements. */
        array<T> elements;

    };


    /*
     * the::heap<T>::heap
     */
    template<class T> 
    heap<T>::heap(const size_t capacity) : elements(capacity) {
    }


    /*
     * the::heap<T>::~heap
     */
    template<class T> heap<T>::~heap(void) {
    }


    /*
     * the::heap<T>::remove_first
     */
    template<class T> void heap<T>::remove_first(void) {
        if (this->elements.size() > 1) {
            this->elements[0] = this->elements.last();
            this->elements.remove_last();
            this->sift_down(0);
        } else {
            this->elements.remove_first();
        }
    }


    /*
     * the::heap<T>::sift_down
     */
    template<class T> void heap<T>::sift_down(size_t idx) {
        size_t nextIdx = 0;

        while (idx < this->elements.size() / 2) {
            nextIdx = 2 * idx + 1;

            if (nextIdx + 1 < this->elements.size()) {
                /* Node at 'idx' has two children, check for smaller one. */
                
                if (this->elements[nextIdx + 1].first 
                        < this->elements[nextIdx].first) {
                    nextIdx++;
                }
            }
            ASSERT(idx >= 0);
            ASSERT(idx < this->elements.size());
            ASSERT(nextIdx >= 0);
            ASSERT(nextIdx < this->elements.size());

            if (this->elements[idx].first < this->elements[nextIdx].first) {
                /* heap property fulfilled. */
                break;

            } else {
                /* Bubble down. */
                swap(this->elements[idx], this->elements[nextIdx]);
                idx = nextIdx;
            }
        }
    }


    /*
     * the::heap<T>::sift_up
     */
    template<class T> void heap<T>::sift_up(size_t idx) {
        size_t nextIdx = 0;

        while (idx > 0) {
            nextIdx = (idx - 1) / 2;
            ASSERT(idx >= 0);
            ASSERT(idx < this->elements.size());
            ASSERT(nextIdx >= 0);
            ASSERT(nextIdx < this->elements.size());

            if (this->elements[nextIdx].first < this->elements[idx].first) {
                /* heap property fulfilled. */
                break;

            } else {
                /* Bubble up. */
                swap(this->elements[idx], this->elements[nextIdx]);
                idx = nextIdx;
            } 
        }
    }
    
} /* end namespace collections */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_COLLECTIONS_HEAP_H_INCLUDED */

