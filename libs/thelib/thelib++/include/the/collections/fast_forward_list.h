/*
 * the/collections/fast_forward_list.h
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
 * SingleLinkedList.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_COLLECTIONS_FAST_FORWARD_LIST_H_INCLUDED
#define THE_COLLECTIONS_FAST_FORWARD_LIST_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/collections/const_enumerator.h"
#include "the/system/threading/no_lock.h"
#include "the/argument_exception.h"
#include "the/assert.h"
#include "the/invalid_operation_exception.h"
#include "the/memory.h"
#include "the/no_such_element_exception.h"


namespace the {
namespace collections {


    /**
     * class of a single linked list of object from type T. Class L is a
     * 'Lockable' for synchronisation of all commands.
     */
    template <class T, class L = system::threading::no_lock>
    class fast_forward_list {

    private:

        /** Type for storing items */
        typedef struct _sll_item {
            T item;
            struct _sll_item *next;
        } item;

    public:

        /** A constant pointer to the type in the collection. */
        typedef const T *const_pointer_type;

        /** A constant reference to the type in the collection. */
        typedef const T& const_reference_type;

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
         * An enumerator through the list.
         */
        class enumerator : public enumerator_base<fast_forward_list> {

        protected:

            /** Super class typedef. */
            typedef enumerator_base<fast_forward_list> base;

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
            inline enumerator(void) : base(nullptr), pos(nullptr) {
                THE_STACK_TRACE;
            }

            /**
             * Create a clone of 'rhs'.
             *
             * @param rhs The object to be cloned.
             */
            inline enumerator(const enumerator& rhs)
                    : base(nullptr), pos(nullptr) {
                THE_STACK_TRACE;
                *this = rhs;
            }

            /**
             * Initialise a new instance.
             *
             * @param collection The collection being enumerated.
             */
            inline enumerator(collection_type& collection)
                    : base(&collection), pos(collection.firstItem) {
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
            item *pos;
        }; /* end class enumerator */

        /** Enumerator for constant lists. */
        typedef the::collections::const_enumerator<enumerator> const_enumerator;

        /** ctor */
        fast_forward_list(void);

        /**
         * copy ctor 
         * the created list creates items identical to the items of rhs.
         *
         * @param rhs The linked list to copy from.
         */
        fast_forward_list(const fast_forward_list<T, L>& rhs);

        /**
         * copy ctor 
         * the created list creates items identical to the items of rhs.
         *
         * @param rhs The linked list to copy from.
         */
        template<class Lp>
        fast_forward_list(const fast_forward_list<T, Lp>& rhs);

        /** Dtor. */
        virtual ~fast_forward_list(void);

        /** 
         * Appends an item to the end of the list. Runtime complexity: O(1)
         *
         * @param item The item to be added.
         */
        virtual inline void add(const T& element) {
            this->append(element);
        }

        /** 
         * Appends an item to the end of the list. Runtime complexity: O(1)
         *
         * @param item The item to be added.
         */
        virtual void append(const T& item);

        /**
         * Clears the whole list.
         */
        virtual void clear(void);

        /**
         * Checks whether an item is contained in the list.
         *
         * @param item The item.
         *
         * @return true if the item is contained in the list, false otherwise.
         */
        virtual bool contains(const T& item) const;

        /**
         * Answer the number of items in the collection.
         *
         * @return Number of items in the collection.
         */
        virtual size_type count(void) const;

        /**
         * Answer a pointer to the first copy of 'element' in the collection. 
         * If no element equal to 'element' is found, a NULL pointer is 
         * returned.
         *
         * @param element The element to be tested.
         *
         * @return A pointer to the local copy of 'element' or NULL, if no such
         *         element is found.
         */
        virtual const T *find(const T& element) const;

        /**
         * Answer a pointer to the first copy of 'element' in the collection. 
         * If no element equal to 'element' is found, a NULL pointer is 
         * returned.
         *
         * @param element The element to be tested.
         *
         * @return A pointer to the local copy of 'element' or NULL, if no such
         *         element is found.
         */
        virtual T *find(const T& element);

        /**
         * Answer the first element in the collection. Runtime complexity: O(1)
         *
         * @return A reference to the first element.
         *
         * @throws no_such_element_exception, if the collection is empty.
         */
        virtual const T& first(void) const;

        /**
         * Answer the first element in the collection. Runtime complexity: O(1)
         *
         * @return A reference to the first element.
         *
         * @throws no_such_element_exception, if the collection is empty.
         */
        virtual T& first(void);

        /**
         * Answer an enumerator over the list.
         *
         * If the collection is protected by an actual lock, the enumerator
         * will acquire the lock and hold it until (i) it reaches the end of
         * the enumeration, or (ii) the enumerator instance is destroyed,
         * whatever happens first.
         *
         * Please be aware that enumerators of collections protected by
         * actual locks cannot be transported from one thread to another!
         *
         * @return An enumerator starting at the begin of the list.
         */
        inline enumerator get_enumerator(void) {
            THE_STACK_TRACE;
            return enumerator(*this);
        }

        /**
         * Answer an enumerator over the list.
         *
         * If the collection is protected by an actual lock, the enumerator
         * will acquire the lock and hold it until (i) it reaches the end of
         * the enumeration, or (ii) the enumerator instance is destroyed,
         * whatever happens first.
         *
         * Please be aware that enumerators of collections protected by
         * actual locks cannot be transported from one thread to another!
         *
         * @return An enumerator starting at the begin of the list.
         */
        inline const_enumerator get_enumerator(void) const {
            THE_STACK_TRACE;
            return const_enumerator(enumerator(
                *const_cast<fast_forward_list *>(this)));
        }

        /**
         * Answer whether there is no element in the collection. Runtime 
         * complexity: O(1)
         *
         * @return true, if the collection is empty, false otherwise.
         */
        virtual inline bool is_empty(void) const {
            // no need to sync here (this race condition is acceptable)
            return this->firstItem == NULL;
        }

        /**
         * Answer the last element in the collection. Runtime complexity: O(1)
         *
         * @return A reference to the last element.
         *
         * @throws no_such_element_exception, if the collection is empty.
         */
        virtual const T& last(void) const;

        /**
         * Answer the last element in the collection. Runtime complexity: O(1)
         *
         * @return A reference to the last element.
         *
         * @throws no_such_element_exception, if the collection is empty.
         */
        virtual T& last(void);

        /**
         * Lock the list for the calling thread.
         */
        inline void lock(void) const {
            THE_STACK_TRACE;
            this->syncRoot.lock();
        }

        /**
         * Merges a second single linked list 'from' into 'this' list. All 
         * items will be moved from 'from' to 'this' list.
         *
         * @param from The list to be merged from
         */
        template<class Lp>
        void merge(fast_forward_list<T, Lp> &from);

        /**
         * Adds an item to the beginning of the list. Runtime complexity: O(1)
         *
         * @param item The item to be added.
         */
        virtual void prepend(const T& item);

        /**
         * Remove the first occurrence of an element that is equal to 'element' 
         * from the collection.
         *
         * @param item The element to be removed.
         */
        virtual void remove(const T& item);
        /**
         * Removes an item from the list.
         * This method removes all items from the list that are equal to the
         * provided item.
         *
         * @param item The item to be removed.
         */
        virtual void remove_all(const T& item);

        ///**
        // * Removes an item from the list.
        // * This method removes the item the given iterator has returned the 
        // * last time 'Next' was called from the list. Items considered equal 
        // * to that item are not removed. 
        // *
        // * @param iter The iterator of the item to be removed.
        // *
        // * @throw IllegalParamException if the iterator has not returned any
        // *        item of this list at the last 'Next' call.
        // */
        //virtual void remove(iterator& iter);

        /**
         * Remove the first element from the collection. If the collection
         * is empty, this method has no effect. Runtime complexity: O(1)
         */
        virtual void remove_first(void);

        /**
         * Remove the last element from the collection. If the collection is
         * empty, this method has no effect. Runtime complexity: O(n)
         */
        virtual void remove_last(void);

        /**
         * Answer the size of the list.
         *
         * This operation has linear runtime complexity.
         *
         * @return The number of elements in the list.
         */
        inline size_type size(void) const {
            THE_STACK_TRACE;
            return this->count();
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
        virtual void sort(int (*comparator)(const T& lhs, const T& rhs));

        /**
         * Release the lock that the calling thread has on the list.
         */
        inline void unlock(void) const {
            THE_STACK_TRACE;
            this->syncRoot.unlock();
        }

        /**
         * Assignment operator. This list removes all items and then creates 
         * new items identical to the items of rhs.
         *
         * THIS OPERATOR IS NEEDED because otherwise a default assignment
         * operator is created.
         *
         * @param rhs The linked list to copy from.
         *
         * @return Reference to this list.
         */
        fast_forward_list<T, L>& operator=(
            const fast_forward_list<T, L>& rhs);

        /**
         * Assignment operator. This list removes all items and then creates 
         * new items identical to the items of rhs.
         *
         * @param rhs The linked list to copy from.
         *
         * @return Reference to this list.
         */
        template<class Lp>
        fast_forward_list<T, L>& operator=(
            const fast_forward_list<T, Lp>& rhs);

        /**
         * Compare operator. Two single linked lists are equal if the elements
         * in both lists are equal and in same order. Runtime complexity: O(n)
         *
         * @param rhs The right hand side operand
         *
         * @return if the lists are considered equal
         */
        bool operator==(const fast_forward_list<T, L>& rhs) const;

        /**
         * Compare operator. Two single linked lists are equal if the elements
         * in both lists are equal and in same order. Runtime complexity: O(n)
         *
         * @param rhs The right hand side operand
         *
         * @return if the lists are considered equal
         */
        template<class Lp>
        bool operator==(const fast_forward_list<T, Lp>& rhs) const;

    private:

        /**
         * Performs a merge sort.
         *
         * WARNING: Needs (Count / 2) * sizeof(void*) Bytes temporary memory
         *          in one single block!
         *
         * @param comparator The compare function defining the sort order.
         * 
         * @return The sorted list.
         */
        item *merge_sort(int (*comparator)(const T& lhs, const T& rhs));

        /**
         * Performs a merge sort merge operation
         *
         * @param left The left sorted list.
         * @param right The right sorted list.
         * @param comparator The compare function defining the sort order.
         *
         * @return The sorted list.
         */
        item *merge_sort_merge(item *left, item *right, int (*comparator)(
            const T& lhs, const T& rhs));


        /** anchor of the single linked list */
        item *firstItem;

        /** last element of the single linked list */
        item *lastItem;

        /** The lock protecting access to the array. */
        mutable lock_type syncRoot;
    };


    /*
     * fast_forward_list<T, L>::fast_forward_list
     */
    template<class T, class L>
    fast_forward_list<T, L>::fast_forward_list(void)
            : firstItem(NULL), lastItem(NULL) {
        // intentionally empty
    }


    /*
     * fast_forward_list<T, L>::fast_forward_list
     */
    template<class T, class L>
    fast_forward_list<T, L>::fast_forward_list(
            const fast_forward_list<T, L>& rhs)
            : firstItem(NULL), lastItem(NULL) {
        *this = rhs;
    }


    /*
     * fast_forward_list<T, L>::fast_forward_list
     */
    template<class T, class L> template<class Lp>
    fast_forward_list<T, L>::fast_forward_list(
            const fast_forward_list<T, Lp>& rhs)
            : firstItem(NULL), lastItem(NULL) {
        *this = rhs;
    }


    /*
     * fast_forward_list<T, L>::~fast_forward_list
     */
    template<class T, class L>
    fast_forward_list<T, L>::~fast_forward_list(void) {
        this->clear();
    }


    /*
     * fast_forward_list<T, L>::append
     */
    template<class T, class L>
    void fast_forward_list<T, L>::append(const T& item) {
        this->lock();
        if (this->lastItem) {
            this->lastItem->next = new fast_forward_list<T, L>::item;
            this->lastItem = this->lastItem->next;
        } else {
            this->firstItem = this->lastItem
                = new fast_forward_list<T, L>::item;
        }
        this->lastItem->next = NULL;
        this->lastItem->item = item;
        this->unlock();
    }


    /*
     * fast_forward_list<T, L>::clear
     */
    template<class T, class L>
    void fast_forward_list<T, L>::clear(void) {
        this->lock();
        while (firstItem) {
            lastItem = firstItem->next;
            delete firstItem;
            firstItem = lastItem;
        }
        this->unlock();
    }


    /*
     * fast_forward_list<T, L>::contains
     */
    template<class T, class L>
    bool fast_forward_list<T, L>::contains(const T& item) const {
        this->lock();
        const fast_forward_list<T, L>::item *i = this->firstItem;
        while(i) {
            if (i->item == item) {
                this->unlock();
                return true;
            }
            i = i->next;
        }
        this->unlock();
        return false;
    }


    /*
     * fast_forward_list<T, L>::count 
     */
    template<class T, class L>
    size_t fast_forward_list<T, L>::count(void) const {
        unsigned int c = 0;
        this->lock();
        const fast_forward_list<T, L>::item *i = this->firstItem;
        while (i) {
            c++;
            i = i->next;
        }
        this->unlock();
        return c;
    }


    /*
     * fast_forward_list<T, L>::find
     */
    template<class T, class L>
    const T *fast_forward_list<T, L>::find(const T& element) const {
        this->lock();
        const fast_forward_list<T, L>::item *i = this->firstItem;
        while(i) {
            if (i->item == element) {
                this->unlock();
                return &i->item;
            }
            i = i->next;
        }
        this->unlock();
        return NULL;
    }


    /*
     * fast_forward_list<T, L>::find
     */
    template<class T, class L>
    T *fast_forward_list<T, L>::find(const T& element) {
        this->lock();
        fast_forward_list<T, L>::item *i = this->firstItem;
        while(i) {
            if (i->item == element) {
                this->unlock();
                return &i->item;
            }
            i = i->next;
        }
        this->unlock();
        return NULL;
    }


    /*
     * fast_forward_list<T, L>::first
     */
    template<class T, class L>
    const T& fast_forward_list<T, L>::first(void) const {
        this->lock();
        if (this->firstItem == NULL) {
            this->unlock();
            throw the::no_such_element_exception("List is empty",
                __FILE__, __LINE__);
        }
        const T& retval = this->firstItem->item;
        this->unlock();
        return retval;
    }


    /*
     * fast_forward_list<T, L>::first
     */
    template<class T, class L> T& fast_forward_list<T, L>::first(void) {
        this->lock();
        if (this->firstItem == NULL) {
            this->unlock();
            throw the::no_such_element_exception("List is empty",
                __FILE__, __LINE__);
        }
        T& retval = this->firstItem->item;
        this->unlock();
        return retval;
    }


    /*
     * fast_forward_list<T, L>::last
     */
    template<class T, class L>
    const T& fast_forward_list<T, L>::last(void) const {
        this->lock();
        if (this->lastItem == NULL) {
            this->unlock();
            throw the::no_such_element_exception("List is empty",
                __FILE__, __LINE__);
        }
        const T& retval = this->lastItem->item;
        this->unlock();
        return retval;
    }


    /*
     * fast_forward_list<T, L>::last
     */
    template<class T, class L> T& fast_forward_list<T, L>::last(void) {
        this->lock();
        if (this->lastItem == NULL) {
            this->unlock();
            throw the::no_such_element_exception("List is empty",
                __FILE__, __LINE__);
        }
        T& retval = this->lastItem->item;
        this->unlock();
        return retval;
    }


    /*
     * fast_forward_list<T, L>::merge
     */
    template<class T, class L> template<class Lp>
    void fast_forward_list<T, L>::merge(fast_forward_list<T, Lp> &from) {
        this->lock();
        from.lock();
        if ((this == &from) || (from.firstItem == NULL)) {
            this->unlock();
            from.unlock();
            return; // invalid pair of operands or from is empty
        }
        if (this->firstItem == NULL) {
            this->firstItem = from.firstItem;
        } else {
            this->lastItem->next = from.firstItem;
        }
        this->lastItem = from.lastItem;
        from.firstItem = from.lastItem = NULL;
        this->unlock();
        from.unlock();
    }


    /*
     * fast_forward_list<T, L>::prepend
     */
    template<class T, class L>
    void fast_forward_list<T, L>::prepend(const T& item) {
        fast_forward_list<T, L>::item *i
            = new fast_forward_list<T, L>::item;
        this->lock();
        i->next = this->firstItem;
        this->firstItem = i;
        if (!this->lastItem) {
            this->lastItem = this->firstItem;
        }
        i->item = item;
        this->unlock();
    }


    /*
     * fast_forward_list<T, L>::remove
     */
    template<class T, class L>
    void fast_forward_list<T, L>::remove(const T& item) {
        this->lock();
        fast_forward_list<T, L>::item *i = this->firstItem, *j = NULL;
        while(i) {
            if (i->item == item) {
                if (j) {
                    j->next = i->next;
                    if (this->lastItem == i) {
                        this->lastItem = j;
                    }
                    delete i;
                    i = j->next;
                } else {
                    ASSERT(this->firstItem == i);
                    this->firstItem = i->next;
                    if (this->lastItem == i) {
                        this->lastItem = i->next;
                    }
                    delete i;
                    i = this->firstItem;
                }
                break;  // Only remove first.
            } else {
                j = i;
                i = i->next;
            }
        }
        this->unlock();
    }


    /*
     * fast_forward_list<T, L>::remove_all
     */
    template<class T, class L>
    void fast_forward_list<T, L>::remove_all(const T& item) {
        this->lock();
        fast_forward_list<T, L>::item *i = this->firstItem, *j = NULL;
        while(i) {
            if (i->item == item) {
                if (j) {
                    j->next = i->next;
                    if (this->lastItem == i) {
                        this->lastItem = j;
                    }
                    delete i;
                    i = j->next;
                } else {
                    ASSERT(this->firstItem == i);
                    this->firstItem = i->next;
                    if (this->lastItem == i) {
                        this->lastItem = i->next;
                    }
                    delete i;
                    i = this->firstItem;
                }
            } else {
                j = i;
                i = i->next;
            }
        }
        this->unlock();
    }


    ///*
    // * fast_forward_list<T, L>::remove
    // */
    //template<class T, class L>
    //void fast_forward_list<T, L>::remove(
    //        typename fast_forward_list<T, L>::iterator& iter) {
    //    if (iter.prevItem == NULL) {
    //        throw argument_exception("Invalid iterator state", 
    //            __FILE__, __LINE__);
    //    }

    //    this->lock();
    //    fast_forward_list<T, L>::item *i = this->firstItem;

    //    if (this->firstItem == iter.prevItem) {
    //        this->firstItem = this->firstItem->next;
    //        if (!this->firstItem) this->lastItem = NULL;
    //        delete i;
    //    } else {
    //        while ((i->next) && (i->next != iter.prevItem)) {
    //            i = i->next;
    //        }
    //        if (i->next == iter.prevItem) {
    //            i->next = iter.nextItem;
    //            if (i->next == NULL) this->lastItem = i;
    //            delete iter.prevItem;
    //        } else {
    //            this->unlock();
    //            throw argument_exception("Invalid iterator", 
    //                __FILE__, __LINE__);
    //        }
    //    }
    //    iter.prevItem = NULL;
    //    this->unlock();
    //}


    /*
     * fast_forward_list<T, L>::remove_first
     */
    template<class T, class L>
    void fast_forward_list<T, L>::remove_first(void) {
        this->lock();
        if (this->firstItem) {
            fast_forward_list<T, L>::item *i = this->firstItem;
            this->firstItem = this->firstItem->next;
            if (!this->firstItem) this->lastItem = NULL;
            delete i;
        }
        this->unlock();
    }


    /*
     * fast_forward_list<T, L>::remove_last
     */
    template<class T, class L>
    void fast_forward_list<T, L>::remove_last(void) {
        this->lock();
        if (this->lastItem) {
            fast_forward_list<T, L>::item *newlast = NULL;
            if (this->firstItem != this->lastItem) {
                newlast = this->firstItem;
                while (newlast->next != this->lastItem) {
                    newlast = newlast->next;
                    ASSERT(newlast);
                }
                newlast->next = NULL;
            } else {
                this->firstItem = NULL;
            }
            delete this->lastItem;
            this->lastItem = newlast;
        }
        this->unlock();
    }


    /*
     * fast_forward_list<T, L>::sort
     */
    template<class T, class L>
    void fast_forward_list<T, L>::sort(int (*comparator)(const T& lhs, const T& rhs)) {
        this->lock();
        if ((this->firstItem == this->lastItem) || (this->firstItem == NULL)) {
            return;
        }

        this->firstItem = this->merge_sort(comparator);

        this->lastItem = this->firstItem;
        while (this->lastItem->next != NULL) {
            this->lastItem = this->lastItem->next;
        }
        this->unlock();
    }


    /*
     * fast_forward_list<T, L>::operator=
     */
    template<class T, class L>
    fast_forward_list<T, L>& fast_forward_list<T, L>::operator=(
            const fast_forward_list<T, L>& rhs) {
        if (this == &rhs) {
            return *this;
        }

        this->lock();

        // might be implemented more intelligent reusing the item object 
        //  already present in this
        this->clear();
        typename fast_forward_list<T, L>::iterator it
            = const_cast<fast_forward_list<T, L>&>(rhs).get_iterator();
        while(it.has_next()) {
            this->append(it.Next());
        }

        this->unlock();

        return *this;
    }


    /*
     * fast_forward_list<T, L>::operator=
     */
    template<class T, class L> template<class Lp>
    fast_forward_list<T, L>& fast_forward_list<T, L>::operator=(
            const fast_forward_list<T, Lp>& rhs) {
        if (this == &rhs) {
            return *this;
        }

        this->lock();

        // might be implemented more intelligent reusing the item object 
        //  already present in this
        this->clear();
        typename fast_forward_list<T, Lp>::iterator it
            = const_cast<fast_forward_list<T, Lp>&>(rhs).get_iterator();
        while(it.has_next()) {
            this->append(it.next());
        }

        this->unlock();

        return *this;
    }


    /*
     * fast_forward_list<T, L>::operator==
     */
    template<class T, class L>
    bool fast_forward_list<T, L>::operator==(
            const fast_forward_list<T, L>& rhs) const {
        this->lock();
        const fast_forward_list<T, L>::item *i = this->firstItem;
        const fast_forward_list<T, L>::item *j = rhs.firstItem;

        while (i) {
            if ((!j) || (!(i->item == j->item))) {
                this->unlock();
                return false;
            }
            i = i->next;
            j = j->next;
        }
        this->unlock();

        return (j == NULL);
    }


    /*
     * fast_forward_list<T, L>::operator==
     */
    template<class T, class L> template<class Lp>
    bool fast_forward_list<T, L>::operator==(
            const fast_forward_list<T, Lp>& rhs) const {
        this->lock();
        const fast_forward_list<T, L>::item *i = this->firstItem;
        const fast_forward_list<T, L>::item *j = rhs.firstItem;

        while (i) {
            if ((!j) || (!(i->item == j->item))) {
                this->unlock();
                return false;
            }
            i = i->next;
            j = j->next;
        }
        this->unlock();

        return (j == NULL);
    }


    /*
     * fast_forward_list<T, L>::merge_sort
     */
    template<class T, class L>
    typename fast_forward_list<T, L>::item * fast_forward_list<T, L>::merge_sort(
            int (*comparator)(const T& lhs, const T& rhs)) {
        //*

        // sequential merge sort
        unsigned int subListSize = 1;
        unsigned int subListCount = 2; // to fool the main while loop at the first time.
        fast_forward_list<T, L>::item *left, *lastLeft, *right,
            *lastRight, *remaining, *lastMerged, helphead;
        unsigned int i;

        helphead.next = this->firstItem;

        while (subListCount > 1) {
            
            // initialise iteration
            subListCount = 0;
            remaining = helphead.next;
            lastMerged = &helphead;

            // Iteration on sublists
            while (remaining != NULL) {

                subListCount++;

                // extract left sublist of 'subListSize' elements
                lastLeft = left = remaining;
                for (i = 1; (i < subListSize) && (lastLeft->next != NULL); 
                        i++) {
                    lastLeft = lastLeft->next;
                }
                if (lastLeft->next == NULL) {
                    lastMerged->next = remaining;
                    break;
                }

                // extract right sublist of 'subListSize' elements
                lastRight = right = lastLeft->next;
                lastLeft->next = NULL;
                for (i = 1; (i < subListSize) && (lastRight->next != NULL); 
                        i++) {
                    lastRight = lastRight->next;
                }
                remaining = lastRight->next;
                lastRight->next = NULL;

                // merge sublists
                lastMerged->next = merge_sort_merge(left, right, comparator);
                lastMerged = (lastLeft->next == NULL) ? lastLeft : lastRight;
                
            }

            // all sublists of current size are now sorted!
            subListSize *= 2;
        }

        return helphead.next;

        /*/

        // sequential implementation with A LOT of temporary memory
        size_t cnt = this->Count();
        ASSERT(cnt > 1);
        if (cnt % 2) cnt++;
        cnt /= 2;
        item **Array = new item*[cnt];
        item *i1, *i2, *i3;

        cnt = 0;
        i1 = this->first;
        while (i1 != NULL) {
            i2 = i1->next;
            if (i2 == NULL) {
                Array[cnt] = i1;
                cnt++;
                break;
            }
            i3 = i2->next;

            i1->next = NULL;
            i2->next = NULL;
            Array[cnt] = mergeSortMerge(i1, i2, comparator);
            cnt++;

            i1 = i3;
        }

        int pos;
        do {
            pos = 0;
            for (int i = 0; i < cnt; i += 2) {
                if (i + 1 < cnt) {
                    Array[pos] = mergeSortMerge(Array[i], Array[i + 1], 
                        comparator);
                } else {
                    Array[pos] = Array[i];
                }
                pos++;
            }
            cnt = pos;
        } while (cnt > 1);

        i1 = Array[0];

        delete[] Array;

        return i1;
        //*/
    }


    /*
     * fast_forward_list<T, L>::merge_sort_merge
     */
    template<class T, class L>
    typename fast_forward_list<T, L>::item *
        fast_forward_list<T, L>::merge_sort_merge(
            typename fast_forward_list<T, L>::item *left, 
            typename fast_forward_list<T, L>::item *right, 
            int (*comparator)(const T& lhs, const T& rhs)) {
        fast_forward_list<T, L>::item *retval, *cur;

        if (left != NULL) {
            if (right != NULL) {
                if (comparator(left->item, right->item) <= 0) {
                    retval = left;
                    left = left->next;
                } else {
                    retval = right;
                    right = right->next;
                }
            } else {
                return left;
            }
        } else {
            if (right != NULL) {
                return right;
            } else {
                return NULL;
            }
        }
        cur = retval;

        while ((left != NULL) && (right != NULL)) {
            if (comparator(left->item, right->item) <= 0) {
                cur->next = left;
                left = left->next;
            } else {
                cur->next = right;
                right = right->next;
            }
            cur = cur->next;
        }

        cur->next = (left != NULL) ? left : right;

        return retval;
    }


} /* end namespace collections */
} /* end namespace the */


/*
 * the::collections::fast_forward_list<T, L>::~enumerator::enumerator
 */
template<class T, class L>
the::collections::fast_forward_list<T, L>::enumerator::~enumerator(void) {
    THE_STACK_TRACE;
}


/*
 * the::collections::fast_forward_list<T, L>::enumerator::has_next
 */
template<class T, class L>
bool the::collections::fast_forward_list<T, L>::enumerator::has_next(void) {
    THE_STACK_TRACE;
    fast_forward_list *c = this->get_collection();
    bool retval = ((c != nullptr) && (this->pos != nullptr));
    if (!retval) {
        this->unlock_collection();
        THE_ASSERT(this->get_collection() == nullptr);
    }
    return retval;
}


/*
 * the::collections::fast_forward_list<T, L>::enumerator::next
 */
template<class T, class L>
typename the::collections::fast_forward_list<T, L>::enumerator::reference_type
the::collections::fast_forward_list<T, L>::enumerator::next(void) {
    THE_STACK_TRACE;
    fast_forward_list *c = this->get_collection();
    fast_forward_list::item *i = this->pos;
    if (i == nullptr) {
        THROW_THE_EXCEPTION(invalid_operation_exception, "The list "
            "does not contain more elements.");
    }
    this->pos = i->next;
    return i->item;
}


/*
 * the::collections::fast_forward_list<T, L>::enumerator::operator =
 */
template<class T, class L>
typename the::collections::fast_forward_list<T, L>::enumerator&
the::collections::fast_forward_list<T, L>::enumerator::operator =(
        const enumerator& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        base::operator =(rhs);
        this->pos = rhs.pos;
    }
    return *this;
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_COLLECTIONS_FAST_FORWARD_LIST_H_INCLUDED */
