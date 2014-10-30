/*
 * testmultisz.cpp
 *
 * Copyright (c) 2013, TheLib Team (http://www.thelib.org/license)
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
 * testmultisz.cpp
 *
 * Copyright (C) 2006 - 2007 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#include "stdafx.h"
#include "testmultisz.h"
#include "testhelper.h"

#include <iostream>

#include "the/multi_sz.h"


void TestMultiSz(void) {
    using namespace std;
    using namespace the;

    multi_sza multiSz;

    AssertTrue("MultiSz is intially empty.", multiSz.empty());
    AssertEqual("Empty MultiSz has zero entries.", multiSz.count(), size_t(0));
    AssertEqual("Empty MultiSz has length zero.", multiSz.length(), size_t(0));
    multiSz.clear();
    AssertEqual("Clearing empty MultiSz has no effect.", multiSz.count(), size_t(0));

    const char *strings[] = { "Horst", "Hugo", "Heinz", "Hans" };
    multi_sza multiSz2(strings, 4);
    AssertFalse("MultiSz is not empty.", multiSz2.empty());
    AssertEqual("MultiSz has 4 entries.", multiSz2.count(), size_t(4));
    AssertEqual("MultiSz has length 23.", multiSz2.length(), size_t(23));
    AssertEqual("First element (peek) is \"Horst\".", ::strcmp(multiSz2.peek_at(0), "Horst"), 0);
    AssertEqual("First element (get) is \"Horst\".", ::strcmp(multiSz2.get_at(0).c_str(), "Horst"), 0);
    AssertEqual("First element (array) is \"Horst\".", ::strcmp(multiSz2[0].c_str(), "Horst"), 0);
    AssertEqual("Second element (peek) is \"Hugo\".", ::strcmp(multiSz2.peek_at(1), "Hugo"), 0);
    AssertEqual("Second element (get) is \"Hugo\".", ::strcmp(multiSz2.get_at(1).c_str(), "Hugo"), 0);
    AssertEqual("Second element (array) is \"Hugo\".", ::strcmp(multiSz2[1].c_str(), "Hugo"), 0);
    AssertEqual("Third element (peek) is \"Heinz\".", ::strcmp(multiSz2.peek_at(2), "Heinz"), 0);
    AssertEqual("Third element (get) is \"Heinz\".", ::strcmp(multiSz2.get_at(2).c_str(), "Heinz"), 0);
    AssertEqual("Third element (array) is \"Heinz\".", ::strcmp(multiSz2[2].c_str(), "Heinz"), 0);
    AssertEqual("Fourth element (peek) is \"Hans\".", ::strcmp(multiSz2.peek_at(3), "Hans"), 0);
    AssertEqual("Fourth element (get) is \"Hans\".", ::strcmp(multiSz2.get_at(3).c_str(), "Hans"), 0);
    AssertEqual("Fourth element (array) is \"Hans\".", ::strcmp(multiSz2[3].c_str(), "Hans"), 0);

    multiSz = multiSz2;
    AssertEqual("Assignment of MultiSz", multiSz, multiSz2);
    AssertTrue("Assignment of MultiSz (equality)", multiSz == multiSz2);
    AssertFalse("Assignment of MultiSz (not inequal)", multiSz != multiSz2);
    AssertEqual("First element now is \"Horst\".", ::strcmp(multiSz[0].c_str(), "Horst"), 0);
    AssertEqual("Second element now is \"Hugo\".", ::strcmp(multiSz[1].c_str(), "Hugo"), 0);
    AssertEqual("Third element now is \"Heinz\".", ::strcmp(multiSz[2].c_str(), "Heinz"), 0);
    AssertEqual("Fourth element now is \"Hans\".", ::strcmp(multiSz[3].c_str(), "Hans"), 0);

    multiSz2.clear();
    AssertTrue("Clear leaves empty MultiSz.", multiSz2.empty());
    AssertEqual("Clear leaves MultiSz with 0 entries.", multiSz2.count(), size_t(0));

    AssertFalse("Test fore inequality of MultiSz (equality)", multiSz == multiSz2);
    AssertTrue("Test fore inequality of MultiSz (not inequal)", multiSz != multiSz2);

    multiSz2 = multiSz.data();
    AssertEqual("Assignment of buffer", multiSz, multiSz2);

    multi_sza multiSz3(multiSz);
    AssertEqual("Copy ctor", multiSz, multiSz3);

    multiSz3.clear();
    AssertTrue("Empty before append.", multiSz3.empty());

    multiSz3.add("Helga");
    AssertFalse("Not empty after append.", multiSz3.empty());
    AssertEqual("One element appended.", multiSz3.count(), size_t(1));
    AssertEqual("Element appended is \"Helga\".", ::strcmp(multiSz3[0].c_str(), "Helga"), 0);

    multiSz3.add("Horst");
    AssertEqual("One element appended.", multiSz3.count(), size_t(2));
    AssertEqual("First element untouched.", ::strcmp(multiSz3[0].c_str(), "Helga"), 0);
    AssertEqual("Element appended is \"Horst\".", ::strcmp(multiSz3[1].c_str(), "Horst"), 0);

    multiSz3.insert(2, "Heinz");
    AssertEqual("One element inserted.", multiSz3.count(), size_t(3));
    AssertEqual("Appended using insert.", ::strcmp(multiSz3[2].c_str(), "Heinz"), 0);
    AssertEqual("Element before untouched.", ::strcmp(multiSz3[1].c_str(), "Horst"), 0);

    multiSz3.insert(0, "Hans");
    AssertEqual("One element inserted.", multiSz3.count(), size_t(4));
    AssertEqual("Correct element inserted.", ::strcmp(multiSz3[0].c_str(), "Hans"), 0);

    multiSz3.insert(3, "Holger");
    AssertEqual("One element inserted.", multiSz3.count(), size_t(5));
    AssertEqual("Correct element inserted before last.", ::strcmp(multiSz3[3].c_str(), "Holger"), 0);

    AssertEqual("multiSz3[0] == \"Hans\".", ::strcmp(multiSz3[0].c_str(), "Hans"), 0);
    AssertEqual("multiSz3[1] == \"Helga\".", ::strcmp(multiSz3[1].c_str(), "Helga"), 0);
    AssertEqual("multiSz3[2] == \"Horst\".", ::strcmp(multiSz3[2].c_str(), "Horst"), 0);
    AssertEqual("multiSz3[3] == \"Holger\".", ::strcmp(multiSz3[3].c_str(), "Holger"), 0);
    AssertEqual("multiSz3[4] == \"Heinz\".", ::strcmp(multiSz3[4].c_str(), "Heinz"), 0);

    multiSz3.clear();
    AssertTrue("Empty before insert.", multiSz3.empty());
    multiSz3.insert(0, "Hans");
    AssertEqual("Insert into empty.", ::strcmp(multiSz3[0].c_str(), "Hans"), 0);
    
    multiSz3.clear();
    AssertException("Insert at invalid index", multiSz3.insert(1, "Hugo"), the::index_out_of_range_exception);
    AssertException("Array operator at invalid index", multiSz3[0], the::index_out_of_range_exception);
    AssertEqual("get_at at invalid index", multiSz3.get_at(0), std::string());
    AssertEqual("peek_at invalid index", (void *) multiSz3.peek_at(0), (void *) NULL);

    multiSz.clear();
    AssertTrue("Empty before append.", multiSz.empty());
    multiSz.add("Hugo");
    multiSz.add("Hugo");
    multiSz.add("Hugo");
    AssertEqual("Three elements appended.", multiSz.count(), size_t(3));
    multiSz.remove("Hugo");
    AssertEqual("All elements removed.", multiSz.count(), size_t(0));
    AssertTrue("Empty after remove.", multiSz.empty());

    multiSz.clear();
    AssertTrue("Empty before append.", multiSz.empty());
    multiSz.add("Hugo");
    multiSz.add("Horst");
    multiSz.add("Heinz");
    multiSz.add("Helga");
    multiSz.add("Heinz");
    multiSz.add("Hans");
    AssertEqual("Three elements appended.", multiSz.count(), size_t(6));
    
    multiSz.remove("Heinz");
    AssertEqual("Two elements removed.", multiSz.count(), size_t(4));
    AssertEqual("multiSz[0] == \"Hugo\".", ::strcmp(multiSz[0].c_str(), "Hugo"), 0);
    AssertEqual("multiSz[1] == \"Horst\".", ::strcmp(multiSz[1].c_str(), "Horst"), 0);
    AssertEqual("multiSz[2] == \"Helga\".", ::strcmp(multiSz[2].c_str(), "Helga"), 0);
    AssertEqual("multiSz[3] == \"Hans\".", ::strcmp(multiSz[3].c_str(), "Hans"), 0);
    
    multiSz.remove("Hans");
    AssertEqual("One element removed.", multiSz.count(), size_t(3));
    AssertEqual("multiSz[0] == \"Hugo\".", ::strcmp(multiSz[0].c_str(), "Hugo"), 0);
    AssertEqual("multiSz[1] == \"Horst\".", ::strcmp(multiSz[1].c_str(), "Horst"), 0);
    AssertEqual("multiSz[2] == \"Helga\".", ::strcmp(multiSz[2].c_str(), "Helga"), 0);

    multiSz.remove("Horst");
    AssertEqual("One element removed.", multiSz.count(), size_t(2));
    AssertEqual("multiSz[0] == \"Hugo\".", ::strcmp(multiSz[0].c_str(), "Hugo"), 0);
    AssertEqual("multiSz[1] == \"Helga\".", ::strcmp(multiSz[1].c_str(), "Helga"), 0);

    multiSz.remove("Hugo");
    AssertEqual("One element removed.", multiSz.count(), size_t(1));
    AssertEqual("multiSz[0] == \"Helga\".", ::strcmp(multiSz[0].c_str(), "Helga"), 0);

    multiSz.remove("Hugo");
    AssertEqual("No element removed.", multiSz.count(), size_t(1));
    AssertEqual("multiSz[0] == \"Helga\".", ::strcmp(multiSz[0].c_str(), "Helga"), 0);

    multiSz.remove("");
    AssertEqual("No element removed.", multiSz.count(), size_t(1));
    AssertEqual("multiSz[0] == \"Helga\".", ::strcmp(multiSz[0].c_str(), "Helga"), 0);

    multiSz.remove(NULL);
    AssertEqual("No element removed.", multiSz.count(), size_t(1));
    AssertEqual("multiSz[0] == \"Helga\".", ::strcmp(multiSz[0].c_str(), "Helga"), 0);

    multiSz.remove(std::string());
    AssertEqual("No element removed.", multiSz.count(), size_t(1));
    AssertEqual("multiSz[0] == \"Helga\".", ::strcmp(multiSz[0].c_str(), "Helga"), 0);

    multiSz.remove("Helga");
    AssertEqual("One element removed.", multiSz.count(), size_t(0));
    AssertTrue("Is empty now.", multiSz.empty());

    multiSz = multi_sza(strings, 4);
    multiSz2.clear();
    AssertTrue("Test for inequality yields true", multiSz != multiSz2);
    AssertFalse("Test for equality yields false", multiSz == multiSz2);
    multiSz2 = multiSz;
    AssertFalse("Test for inequality yields false", multiSz != multiSz2);
    AssertTrue("Test for equality yields true", multiSz == multiSz2);
    multiSz2.remove("Hans");
    AssertTrue("Test for inequality yields true", multiSz != multiSz2);
    AssertFalse("Test for equality yields false", multiSz == multiSz2);

    for (multi_sza::iterator it = multiSz.begin(); it != multiSz.end(); ++it) {
        std::cout << *it << std::endl;
    }
}
