/*
 * testdelegate.h
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
 * testdelegate.cpp
 *
 * Copyright (C) 2011 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */
#include "stdafx.h"
#include "testdelegate.h"
#include "testhelper.h"
#include "the/delegate.h"
#include "the/multicast_delegate.h"
#include <cstdio>

//#define _DO_NOT_COMPILE 1

static int funcNum = 0;

static void func1(bool& b, int i) {
    printf("func1 called\n");
    funcNum = 1;
    b = (i != 0);
}

static void func1Ctxt(bool& b, int i, const int* j) {
    printf("func1Ctxt called\n");
    funcNum = 5;
    b = (i != *j);
}

#ifdef _DO_NOT_COMPILE
static void func2(int i1, int i2) {
    printf("func2 called\n");
    funcNum = 2;
}
#endif /* _DO_NOT_COMPILE */

class TestClass {
public:
    void Meth1(bool& b, int i) {
        printf("meth1 called\n");
        funcNum = -1;
        b = (i != 0);
    }
    void Meth1Ctxt(bool& b, int i, const int* j) {
        printf("meth1Ctxt called\n");
        funcNum = -3;
        b = (i != *j);
    }
#ifdef _DO_NOT_COMPILE
    void Meth2(int i1, int i2) {
        printf("meth2 called\n");
        funcNum = -2;
    }
#endif /* _DO_NOT_COMPILE */
};

static double funct3(bool b, int i, float f) {
    printf("funct3 called\n");
    funcNum = 3;
    return static_cast<double>(static_cast<float>(b ? i : 0) + f);
}

static void funct4(void) {
    printf("funct4 called\n");
    funcNum = 4;
}

static void func5(int &to, int from) {
    to += from;
}


static the::delegate<void, int&, int> getFunc5(void) {
    return &func5;
}


/*
 * TestDelegate
 */
void TestDelegate(void) {
    TestClass testObj;
    the::delegate<void, bool&, int> testNullDelegate;
    the::delegate<void, bool&, int> testFuncDelegate(&func1);
    the::delegate<void, bool&, int> testFuncDelegate2s(&func1);
#ifdef _DO_NOT_COMPILE
    the::Delegate<void, bool, int> testBadFuncDelegate(&func2);
#endif /* _DO_NOT_COMPILE */
    the::delegate<void, bool&, int> testClassDelegate(testObj, &TestClass::Meth1);
#ifdef _DO_NOT_COMPILE
    the::Delegate<void, bool, int> testBadClassDelegate(testObj, &TestClass::Meth2);
#endif /* _DO_NOT_COMPILE */
    the::delegate<double, bool, int, float> testFuncDelegate3(&funct3);
    the::delegate<> testFuncDelegate0(&funct4);
    int j = 0;
    the::delegate<void, bool&, int> testFuncCtxtDelegate(&func1Ctxt, &j);
    testFuncCtxtDelegate.set(the::delegate<void, bool&, int>(&func1Ctxt, &j));
    the::delegate<void, bool&, int> testClassCtxtDelegate(testObj, &TestClass::Meth1Ctxt, &j);
    testClassCtxtDelegate = the::delegate<void, bool&, int>(testObj, &TestClass::Meth1Ctxt, &j);

    bool b = false;

    ::AssertEqual("b == false", b, false);
    ::AssertTrue("testFuncDelegate == testFuncDelegate2s", testFuncDelegate == testFuncDelegate2s);
    testFuncDelegate2s.unset();
    ::AssertFalse("testFuncDelegate != testFuncDelegate2s", testFuncDelegate == testFuncDelegate2s);
    testFuncDelegate2s = testFuncDelegate;
    ::AssertTrue("testFuncDelegate == testFuncDelegate2s", testFuncDelegate == testFuncDelegate2s);
    ::AssertEqual("funcNum = 0", funcNum, 0);
    testFuncDelegate(b, 1);
    ::AssertEqual("b == true", b, true);
    ::AssertEqual("funcNum = 1", funcNum, 1);
    testClassDelegate(b, 0);
    ::AssertEqual("b == false", b, false);
    ::AssertEqual("funcNum = -1", funcNum, -1);
    testFuncCtxtDelegate(b, 1);
    ::AssertEqual("b == true", b, true);
    ::AssertEqual("funcNum = 5", funcNum, 5);
    testClassCtxtDelegate(b, 0);
    ::AssertEqual("b == false", b, false);
    ::AssertEqual("funcNum = -3", funcNum, -3);
    double d = testFuncDelegate3(true, 1, 1.0f);
    ::AssertEqual("funcNum = 3", funcNum, 3);
    ::AssertTrue("d = 2.0", abs(d - 2.0) < 0.0001);
    testFuncDelegate0();
    ::AssertEqual("funcNum = 4", funcNum, 4);

}


class TheHugo {
public:
    int other;
    void power(int& x, int y);
};
void TheHugo::power(int& x, int y) {
    x += (this->other + y);
}


static bool succ;


class TheHorst {
public:
    static void removeAll(void) {
        e();
    }
    static inline int count(void) {
        return cnt;
    }
    TheHorst(int i) : i(i) {
        cnt++;
        e += the::delegate<>(*this, &TheHorst::removeMe);
    }
private:
    void removeMe(void) {
        cnt--;
        e -= the::delegate<>(*this, &TheHorst::removeMe);
    }
    static int cnt;
    static the::multicast_delegate<the::delegate<> > e;
    int i;
};

int TheHorst::cnt = 0;
the::multicast_delegate<the::delegate<> > TheHorst::e;


/*
 * TestMulticastDelegate
 */
void TestMulticastDelegate(void) {
    int i = 0;
    the::multicast_delegate<the::delegate<void, int&, int> > mcd;

    mcd += &func5;
    mcd.add(getFunc5());

    mcd(i, 4);
    ::AssertEqual("i = 8", i, 8);

    mcd -= &func5;

    mcd(i, 2);
    ::AssertEqual("i = 10", i, 10);

    mcd.remove(&func5);

    mcd(i, 55);
    ::AssertEqual("i = 10", i, 10);

    TheHugo h;
    h.other = 3;
    mcd.add(&func5);
    mcd += the::delegate<void, int&, int>(h, &TheHugo::power);

    mcd(i, 2);
    ::AssertEqual("i = 17", i, 17);

    mcd -= the::multicast_delegate<the::delegate<void, int&, int> >::single_delegate_type(h, &TheHugo::power);

    mcd(i, 1);
    ::AssertEqual("i = 18", i, 18);

    mcd -= getFunc5();

    mcd(i, 55);
    ::AssertEqual("i = 18", i, 18);

    // test instantiation of templates
    the::multicast_delegate<the::delegate<> > m0;
    the::multicast_delegate<the::delegate<void, int> > m1;
    the::multicast_delegate<the::delegate<void, int, int> > m2;
    the::multicast_delegate<the::delegate<void, int, int, int> > m3;
    the::multicast_delegate<the::delegate<void, int, int, int, int> > m4;
    the::multicast_delegate<the::delegate<void, int, int, int, int, int> > m5;
    the::multicast_delegate<the::delegate<void, int, int, int, int, int, int> > m6;
    the::multicast_delegate<the::delegate<void, int, int, int, int, int, int, int> > m7;
    the::multicast_delegate<the::delegate<void, int, int, int, int, int, int, int, int> > m8;
    the::multicast_delegate<the::delegate<void, int, int, int, int, int, int, int, int, int> > m9;
    the::multicast_delegate<the::delegate<void, int, int, int, int, int, int, int, int, int, int> > m10;

    succ = false;
    // ok. problem here: the lambda expression can never be matched again, thus this delegate cannot be removed from the multicast_delegate if it is not stored as an object.
    the::delegate<void> d([](){ printf("I am lambda\n"); succ = true; });
    m0 += d;
    ::AssertFalse("pre lambda", succ);
    m0();
    ::AssertTrue("post lambda", succ);

    succ = false;
    m0 -= d;
    ::AssertFalse("pre non-lambda", succ);
    m0();
    ::AssertFalse("post non-lambda", succ);

    // These do not work as the lambda can not be implicitly cased to a delegate
    // m0 += [](){ printf("I am lambda\n"); succ = true; };
    // m0.add([](){ succ = true; });

    // Test for changing the multicast target list while iterating!
    ::AssertEqual("Remove-Count == 0", TheHorst::count(), 0);
    TheHorst h1(1);
    TheHorst h2(2);
    TheHorst h3(3);
    TheHorst h4(4);
    TheHorst h5(5);
    ::AssertEqual("Remove-Count after adding == 5", TheHorst::count(), 5);
    TheHorst::removeAll();
    ::AssertEqual("Remove-Count after remove == 0", TheHorst::count(), 0);
    TheHorst::removeAll();
    ::AssertEqual("Remove-Count after second remove == 0", TheHorst::count(), 0);

}
