/*
 * testcollection.cpp
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
 * testcollection.cpp
 *
 * Copyright (C) 2006 - 2007 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */
#include "stdafx.h"
#include "testcollection.h"
#include "testhelper.h"

#include "the/collections/array.h"
#include "the/collections/heap.h"
//#include "vislib/Map.h"
#include "the/collections/ptr_array.h"
#include "the/collections/fast_forward_list.h"
#include "the/config.h"
#include "the/string.h"
//#include "vislib/SynchronisedArray.h"
#include <utility>
#include <forward_list>


void TestFFListSort(void);
void TestArraySort(void);
void TestFFListPerformance(void);


/*
 * TestArray
 */
void TestArray(void) {
    using the::collections::array;
    using the::collections::ptr_array;

    array<int> intAry;

    ::AssertEqual("array default capacity", intAry.capacity(), array<int>::default_capacity);
    ::AssertEqual("array initially empty", intAry.size(), size_t(0));
    ::AssertTrue("is_empty method", intAry.empty());
    
    intAry.append(4);
    ::AssertEqual("Appended one element", intAry.size(), size_t(1));
    ::AssertTrue("New element in array", intAry.contains(4));
    ::AssertEqual("New element is at expected position", intAry[0], 4);

    intAry.append(6);
    intAry.append(43);
    ::AssertEqual("Appended two elements", intAry.size(), size_t(3));

    intAry.assert_capacity(array<int>::default_capacity + 5);
    ::AssertEqual("Increased capacity", intAry.capacity(), array<int>::default_capacity + 5);

    intAry.trim();
    ::AssertEqual("capacity set to current count", intAry.size(), intAry.capacity());

    intAry.append(99);
    ::AssertEqual("New element appended to full array", intAry[int(intAry.size() - 1)], 99);

    intAry.erase(1, 2);
    ::AssertEqual("Two elements erased", intAry.size(), size_t(2));
    ::AssertEqual("capacity unchanged", intAry.capacity(), size_t(4));
    ::AssertEqual("First element unchanged", intAry[0], 4);
    ::AssertEqual("Last element unchanged", intAry[1], 99);

    intAry.insert(1, 5);
    ::AssertEqual("First element unchanged", intAry[0], 4);
    ::AssertEqual("Element inserted", intAry[1], 5);
    ::AssertEqual("Last element moved", intAry[2], 99);

    intAry.insert(0, 1);
    ::AssertEqual("Insert at begin", intAry[0], 1);

    intAry.insert(intAry.size() - 1, 1981);
    ::AssertEqual("Insert at end", intAry[intAry.size() - 2], 1981);
    ::AssertEqual("End moved", intAry.last(), 99);

    intAry.insert(intAry.size(), 2007);
    ::AssertEqual("Append using Insert", intAry.last(), 2007);
    

    array<int> intAry2(10, 42);
    
    ::AssertEqual("Initially filled array", intAry2.size(), size_t(10));

    intAry = intAry2;
    ::AssertEqual("Assignment copies all elements", intAry.size(), size_t(10));
    ::AssertTrue("Assignment copies correct elements", intAry.contains(42));
    
    intAry.remove(43);
    ::AssertEqual("Removing non-exisiting element has no effect", intAry.size(), size_t(10));
    
    intAry.remove(42);
    ::AssertFalse("Remove element", intAry.contains(42));
    ::AssertTrue("Remove affects all matching elements", intAry.empty());

    intAry2.erase(intAry2.size());
    ::AssertEqual("Erase on non-exisiting index has no effect", intAry2.size(), size_t(10));

    intAry2.erase(3);
    ::AssertEqual("Erase one element", intAry2.size(), size_t(9));

    intAry2.erase(2, 5);
    ::AssertEqual("Erase five elements", intAry2.size(), size_t(4));

    intAry2.erase(2, 5);
    ::AssertEqual("Erase begining at 2", intAry2.size(), size_t(2));

    intAry2.clear();
    ::AssertTrue("Clear array", intAry2.empty());

    intAry2.trim();
    ::AssertEqual("Trim empty array", intAry2.capacity(), size_t(0));

    array<the::astring> strAry;
    strAry.append("Horst");
    AssertTrue("Contains \"Horst\"", strAry.contains("Horst"));


    ptr_array<int> intPtrAry;
    intPtrAry.add(new int(5));
    AssertEqual("Added pointer to 5", *(intPtrAry[0]), 5);
    
    intPtrAry.add(new int(4));
    AssertEqual("Added pointer to 4", *(intPtrAry[1]), 4);
    
    intPtrAry.resize(1);
    AssertEqual("Removed all but first element", intPtrAry.size(), size_t(1));
    AssertEqual("First element was not changed", *(intPtrAry[0]), 5);

    intPtrAry.clear(true);
    AssertEqual("Removed all elements", intPtrAry.size(), size_t(0));
    AssertEqual("Deallocated whole array", intPtrAry.capacity(), size_t(0));

    // TODO: Do we still need this?
    //the::sys::SynchronisedArray<int> syncedIntArray;

    intAry.clear();
    intAry.add(1);
    intAry.add(2);
    intAry.add(3);
    intAry.add(4);
    intAry.erase(0, 1);
    AssertEqual("array has 3 elements", intAry.size(), size_t(3));
    AssertEqual("array[0] == 2", intAry[0], 2);
    AssertEqual("array[1] == 3", intAry[1], 3);
    AssertEqual("array[2] == 4", intAry[2], 4);

    intAry.clear();
    intAry.add(1);
    intAry.add(2);
    intAry.add(3);
    intAry.add(4);
    intAry.erase(1, 1);
    AssertEqual("array has 3 elements", intAry.size(), size_t(3));
    AssertEqual("array[0] == 1", intAry[0], 1);
    AssertEqual("array[1] == 3", intAry[1], 3);
    AssertEqual("array[2] == 4", intAry[2], 4);

    int enumReference = 0;
    intAry.clear();
    intAry.add(++enumReference);
    intAry.add(++enumReference);
    intAry.add(++enumReference);
    array<int>::enumerator e = intAry.get_enumerator();
    enumReference = 0;
    while (e.has_next()) {
        AssertEqual("array enumeration", e.next(), ++enumReference);
        AssertTrue("end of enumeration", e.has_next() || enumReference == intAry.size());
    }

    const array<int> constIntAry(5, 42);
    array<int>::const_enumerator ce = constIntAry.get_enumerator();
    enumReference = 0;
    while (e.has_next()) {
        AssertEqual("constant array enumeration", e.next(), 42);
        AssertTrue("end of constant enumeration", e.has_next() || enumReference == intAry.size());
    }

    TestArraySort();
}


/*
 * TestFastForwardList
 */
void TestFastForwardList(void) {
    typedef the::collections::fast_forward_list<int> IntList;

    IntList list;

    list.append(12);
    list.add(7);
    list.prepend(6);

    AssertEqual("List contains 3 elements", list.count(), size_t(3));

    IntList::enumerator i = list.get_enumerator();
    AssertTrue("iterator valid", i.has_next());
    AssertEqual("Element[1] = 6", i.next(), 6);
    AssertTrue("iterator valid", i.has_next());
    AssertEqual("Element[2] = 12", i.next(), 12);
    AssertTrue("iterator valid", i.has_next());
    AssertEqual("Element[3] = 7", i.next(), 7);
    AssertFalse("iterator at end", i.has_next());

    IntList::const_enumerator ci(list.get_enumerator());
    AssertTrue("iterator valid", ci.has_next());
    AssertEqual("Element[1] = 6", ci.next(), 6);
    AssertTrue("iterator valid", ci.has_next());
    AssertEqual("Element[2] = 12", ci.next(), 12);
    AssertTrue("iterator valid", ci.has_next());
    AssertEqual("Element[3] = 7", ci.next(), 7);
    AssertFalse("iterator at end", ci.has_next());

    TestFFListSort();
    //TestFFListPerformance(); // tooo slow! so don't test this by default
}


/*
 * TestHeap
 */
void TestHeap(void) {
    using the::collections::array;
    using the::collections::heap;

    typedef std::pair<int, char> MyPair;
    
    heap<MyPair> testheap;

    ::AssertEqual("heap default capacity", testheap.capacity(), array<MyPair>::default_capacity);
    ::AssertEqual("heap initially empty", testheap.count(), size_t(0));
    ::AssertTrue("is_empty method", testheap.is_empty());

    testheap.add(MyPair(3, 'H'));
    ::AssertEqual("One element added", testheap.count(), size_t(1));
    ::AssertFalse("is_empty method", testheap.is_empty());

    testheap.add(MyPair(7, 't'));
    testheap.add(MyPair(5, 'r'));
    testheap.add(MyPair(6, 's'));
    testheap.add(MyPair(4, 'o'));
    ::AssertEqual("Four additional elements added", testheap.count(), size_t(5));

    ::AssertEqual("Get element 'H'", testheap.first().second, 'H');
    testheap.remove_first();
    ::AssertEqual("Get element 'o'", testheap.first().second, 'o');
    testheap.remove_first();
    ::AssertEqual("Get element 'r'", testheap.first().second, 'r');
    testheap.remove_first();
    ::AssertEqual("Get element 's'", testheap.first().second, 's');
    testheap.remove_first();
    ::AssertEqual("Get element 't'", testheap.first().second, 't');
    testheap.remove_first();
    ::AssertTrue("heap is empty now", testheap.is_empty());

    testheap.add(MyPair(7, 't'));
    testheap.add(MyPair(5, 'r'));
    testheap.add(MyPair(4, 'o'));
    testheap.add(MyPair(0, 'H'));
    testheap.add(MyPair(6, 's'));
    testheap.add(MyPair(4, 'o'));
    ::AssertEqual("heap filled with duplicate key", testheap.count(), size_t(6));

    ::AssertEqual("Get element 'H'", testheap.first().second, 'H');
    testheap.remove_first();
    ::AssertEqual("Get element 'o'", testheap.first().second, 'o');
    testheap.remove_first();
    ::AssertEqual("Get element 'o'", testheap.first().second, 'o');
    testheap.remove_first();
    ::AssertEqual("Get element 'r'", testheap.first().second, 'r');
    testheap.remove_first();
    ::AssertEqual("Get element 's'", testheap.first().second, 's');
    testheap.remove_first();
    ::AssertEqual("Get element 't'", testheap.first().second, 't');
    testheap.remove_first();
    ::AssertTrue("heap is empty now", testheap.is_empty());

    testheap.add(MyPair(4, 'o'));
    testheap.clear();
    ::AssertTrue("heap is empty after Clear", testheap.is_empty());
}


/*
 * TestMap
 */
void TestMap(void) {
    //using the::Map;
    //using the::fast_forward_list;

    //Map<int, float> map;

    //map.Set(12, 13.5f);
    //::AssertEqual("Map contains one element", map.size(), static_cast<size_t>(1));
    //::AssertEqual("Map element correct", map[12], 13.5f);
    //::AssertEqual("New Map element correct", map[11], 0.0f);
    //::AssertEqual("Map contains two elements", map.size(), static_cast<size_t>(2));
    //map[11] = 1.01f;
    //::AssertEqual("New Map element correct", map[11], 1.01f);
    //map.Remove(11);
    //::AssertEqual("Map contains one element", map.size(), static_cast<size_t>(1));
    //map[10] = 2.0f;
    //map[13] = 2.0f;
    //map[0] = 2.0f;
    //::AssertEqual("Map contains four elements", map.size(), static_cast<size_t>(4));
    //fast_forward_list<int> keys = map.FindKeys(2.0f);
    //::AssertEqual("Three keys associate '2.0f'", keys.size(), static_cast<size_t>(3));
    //::AssertTrue("Key 0 present", keys.Contains(0));
    //::AssertTrue("Key 10 present", keys.Contains(10));
    //::AssertTrue("Key 13 present", keys.Contains(13));
    //::AssertTrue("Key 13 present", map.Contains(13));
    //::AssertFalse("Key 14 not present", map.Contains(14));
    //::AssertEqual<float*>("Key 14 not present", map.FindValue(14), NULL);
    //::AssertEqual<float*>("Key 14 not present", map.FindValue(14), NULL);
    //::AssertEqual("Map[10] correct", map[10], 2.0f);
    //::AssertEqual("Map[13] correct", map[13], 2.0f);
    //map[10] = 4.0f;
    //::AssertEqual("Map[10] correct", map[10], 4.0f);
    //::AssertEqual("Map[13] correct", map[13], 2.0f);
    //map.Clear();
    //::AssertEqual("Map is empty", map.size(), static_cast<size_t>(0));
    //::AssertTrue("Map is empty", map.is_empty());

}


/*
 * intSortCompare
 */
int intSortCompare(const int& lhs, const int& rhs) {
    return lhs - rhs;
}


/*
 * TestFFListSort
 */
void TestFFListSort(void) {
    the::collections::fast_forward_list<int> list;

    list.add(22);
    list.add(8);
    list.add(21);
    list.add(22);
    list.add(50);
    list.add(2);
    list.add(1);
    list.add(10);

    AssertEqual<int>("List filled with 8 Elements", int(list.count()), 8);

    list.sort(intSortCompare);

    AssertEqual<int>("List still contains 8 Elements", int(list.count()), 8);

    the::collections::fast_forward_list<int>::enumerator iter = list.get_enumerator();
    AssertTrue("iterator before Element 1", iter.has_next());
    AssertEqual("Element 1 = 1", iter.next(), 1);
    AssertTrue("iterator before Element 2", iter.has_next());
    AssertEqual("Element 2 = 2", iter.next(), 2);
    AssertTrue("iterator before Element 3", iter.has_next());
    AssertEqual("Element 3 = 8", iter.next(), 8);
    AssertTrue("iterator before Element 4", iter.has_next());
    AssertEqual("Element 4 = 10", iter.next(), 10);
    AssertTrue("iterator before Element 5", iter.has_next());
    AssertEqual("Element 5 = 21", iter.next(), 21);
    AssertTrue("iterator before Element 6", iter.has_next());
    AssertEqual("Element 6 = 22", iter.next(), 22);
    AssertTrue("iterator before Element 7", iter.has_next());
    AssertEqual("Element 7 = 22", iter.next(), 22);
    AssertTrue("iterator before Element 8", iter.has_next());
    AssertEqual("Element 8 = 50", iter.next(), 50);
    AssertFalse("iterator at end of list", iter.has_next());

    list.clear();

    const size_t cnt = 100000;
    for (size_t i = 0; i < cnt; i++) {
        list.add(rand());
    }

#ifdef THE_WINDOWS
    DWORD startTick = GetTickCount();
#endif /* THE_WINDOWS */
    AssertEqual("List filled with random Elements", list.count(), cnt);
    list.sort(intSortCompare);
    AssertEqual("List still contains random Elements", list.count(), cnt);
#ifdef THE_WINDOWS
    DWORD duration = GetTickCount() - startTick;
    printf("Sorted in %u milliseconds\n", duration);
#endif /* THE_WINDOWS */

    bool growing = true;
    int ov = -1;
    iter = list.get_enumerator();
    while (iter.has_next()) {
        int v = iter.next();
        if (v < ov) {
            growing = false;
        }
        ov = v;
    }
    AssertTrue("List sorted Accending", growing);

}


/*
 * TestArraySort
 */
void TestArraySort(void) {
    the::collections::array<int> arr;
    const unsigned int size = 1000000;
    unsigned int i;

    printf("Filling array for sorting test\n");
    arr.set_size(size);
    for (i = 0; i < size; i++) {
        arr[i] = rand();
    }

    arr.sort(intSortCompare);

    printf("Testing Sort output\n");
    bool growing = true;
    for (i = 1; i < size; i++) {
        if (arr[i - 1] > arr[i]) {
            growing = false;
        }
    }

    AssertTrue("array sorted Accending", growing);

}


/*
 * TestFFListPerformance
 */
void TestFFListPerformance(void) {
#ifdef THE_WINDOWS
    using the::collections::fast_forward_list;
    using std::forward_list;

    // now I would like to have a performance counter ...
    ULONGLONG start, end;

    fast_forward_list<int> ourIntList;
    fast_forward_list<int>::enumerator ourIntIter;
    forward_list<int> stdIntList;
    forward_list<int>::iterator stdIntIter, stdIntEnd;

    const int cnt = 10000000;

    // test 1: add ints
    start = ::GetTickCount64();
    for (int i = 0; i < cnt; i++) {
        ourIntList.prepend(i);
    }
    end = ::GetTickCount64();
    printf("Our: prepended %u ints in %u ticks\n", cnt, (end - start));

    ourIntIter = ourIntList.get_enumerator();
    start = ::GetTickCount64();
    while (ourIntIter.has_next()) {
        ourIntIter.next();
    }
    end = ::GetTickCount64();
    printf("Our: iterated through list in %u ticks\n", (end - start));

    ourIntList.clear();

    start = ::GetTickCount64();
    for (int i = 0; i < cnt; i++) {
        stdIntList.push_front(i);
    }
    end = ::GetTickCount64();
    printf("STL: prepended %u ints in %u ticks\n", cnt, (end - start));

    stdIntEnd = stdIntList.end();
    for (stdIntIter = stdIntList.begin(); stdIntIter != stdIntEnd; stdIntIter++) {
    }
    end = ::GetTickCount64();
    printf("STL: iterated through list in %u ticks\n", (end - start));

    stdIntList.clear();

#endif /* THE_WINDOWS */
}
