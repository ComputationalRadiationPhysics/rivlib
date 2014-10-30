/*
 * testpoint.cpp
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
 * testpolynom.cpp
 *
 * Copyright (C) 2010 by VISUS (Universitaet Stuttgart)
 * Alle Rechte vorbehalten.
 */
#include "stdafx.h"
#include "testpolynom.h"
#include "the/math/polynom.h"
#include "the/math/functions.h"
#include "testhelper.h"


/*
 * Testpolynom
 */
void Testpolynom(void) {
    the::math::polynom<float, 3> poly3;

    AssertNearlyEqual("Coefficient a0 = 0", poly3[0], 0.0f);
    AssertNearlyEqual("Coefficient a1 = 0", poly3[1], 0.0f);
    AssertNearlyEqual("Coefficient a2 = 0", poly3[2], 0.0f);
    AssertTrue("polynom is zero", poly3.is_zero());
    AssertEqual("Effective degree is 0", poly3.effective_degree(), 0U);
    poly3[0] = 2.0f;
    AssertNearlyEqual("Coefficient a0 = 2", poly3[0], 2.0f);
    AssertEqual("Effective degree is 0", poly3.effective_degree(), 0U);
    AssertFalse("polynom is not zero", poly3.is_zero());
    poly3[1] = -4.0f;
    AssertNearlyEqual("Coefficient a1 = -4", poly3[1], -4.0f);
    AssertEqual("Effective degree is 1", poly3.effective_degree(), 1U);
    AssertFalse("polynom is not zero", poly3.is_zero());
    poly3[2] = 0.5f;
    AssertNearlyEqual("Coefficient a2 = 0.5", poly3[2], 0.5f);
    AssertEqual("Effective degree is 2", poly3.effective_degree(), 2U);
    AssertFalse("polynom is not zero", poly3.is_zero());
    poly3[3] = -1.0f;
    AssertNearlyEqual("Coefficient a3 = -1", poly3[3], -1.0f);
    AssertEqual("Effective degree is 3", poly3.effective_degree(), 3U);
    AssertFalse("polynom is not zero", poly3.is_zero());

    // - x^3 + 0.5 * x^2 - 4 * x + 2
    AssertNearlyEqual("p(0) = 2", poly3(0.0f), 2.0f);
    // 1 + 0.5 + 4 + 2
    AssertNearlyEqual("p(-1) = 7.5", poly3(-1.0f), 7.5f);
    // -8 + 2 - 8 + 2
    AssertNearlyEqual("p(2) = -12", poly3(2.0f), -12.0f);

    the::math::polynom<float, 2> poly2 = poly3.derivative();
    AssertEqual("Effective degree of derivative is 2", poly2.effective_degree(), 2U);
    AssertNearlyEqual("Coefficient a'0 = -4", poly2[0], -4.0f);
    AssertNearlyEqual("Coefficient a'1 = 1", poly2[1], 1.0f);
    AssertNearlyEqual("Coefficient a'2 = -3", poly2[2], -3.0f);

    the::math::polynom<float, 1> poly1 = poly2.derivative();
    AssertEqual("Effective degree of second derivative is 1", poly1.effective_degree(), 1U);
    AssertNearlyEqual("Coefficient a''0 = 1", poly1[0], 1.0f);
    AssertNearlyEqual("Coefficient a''1 = -6", poly1[1], -6.0f);

    //the::math::polynom<float, 0> poly0 = poly1.Derivative(); // possible problem under linux
    //AssertEqual("Effective degree of third derivative is 0", poly0.effective_degree(), 0U);
    //AssertEqual("Coefficient a'''0 = -6", poly0[0], -6.0f);
    //poly0.Derivative(); // problem under windows

    the::math::polynom<double, 3> poly3d(poly3);
    the::math::polynom<float, 3> poly3s(poly3);

    AssertNearlyEqual("Coefficient a0 = 2", poly3s[0], 2.0f);
    AssertNearlyEqual("Coefficient a1 = -4", poly3s[1], -4.0f);
    AssertNearlyEqual("Coefficient a2 = 0.5", poly3s[2], 0.5f);
    AssertNearlyEqual("Coefficient a3 = -1", poly3s[3], -1.0f);

    AssertNearlyEqual("Coefficient a0 = 2", poly3d[0], 2.0);
    AssertNearlyEqual("Coefficient a1 = -4", poly3d[1], -4.0);
    AssertNearlyEqual("Coefficient a2 = 0.5", poly3d[2], 0.5);
    AssertNearlyEqual("Coefficient a3 = -1", poly3d[3], -1.0);

    poly3s = poly3;
    poly3d = poly3s;

    AssertNearlyEqual("Coefficient a0 = 2", poly3s[0], 2.0f);
    AssertNearlyEqual("Coefficient a1 = -4", poly3s[1], -4.0f);
    AssertNearlyEqual("Coefficient a2 = 0.5", poly3s[2], 0.5f);
    AssertNearlyEqual("Coefficient a3 = -1", poly3s[3], -1.0f);

    AssertNearlyEqual("Coefficient a0 = 2", poly3d[0], 2.0);
    AssertNearlyEqual("Coefficient a1 = -4", poly3d[1], -4.0);
    AssertNearlyEqual("Coefficient a2 = 0.5", poly3d[2], 0.5);
    AssertNearlyEqual("Coefficient a3 = -1", poly3d[3], -1.0);

    AssertEqual("Assigment and comparison works", poly3, poly3s);
    AssertTrue("Assigment and comparison works", poly3 == poly3d);

    poly3d = poly1;
    AssertEqual("Effective degree of second derivative is 1", poly3d.effective_degree(), 1U);
    AssertTrue("Assigment and comparison works", poly1 == poly3d);

    // polynom with three real roots
    // f(x) = (x + 2)(x - 4)(x - 5)
    //      = x((x-4)(x-5)) + 2((x-4)(x-5))
    //      = x(x(x-5)-4(x-5)) + 2(x(x-5)-4(x-5))
    //      = x(x^2-9x+20) + 2(x^2-9x+20)
    //      = x^3 - 9x^2 + 20x + 2x^2 - 18x + 40
    //      = x^3 - 7x^2 + 2x + 40
    poly3[0] = 40.0f;
    poly3[1] = 2.0f;
    poly3[2] = -7.0f;
    poly3[3] = 1.0f;

    AssertNearlyEqual("Root at -2", poly3(-2.0f), 0.0f);
    AssertNearlyEqual("Root at 4", poly3(4.0f), 0.0f);
    AssertNearlyEqual("Root at 5", poly3(5.0f), 0.0f);

    float roots[5];
    unsigned int rootsCnt = poly3.find_roots(roots, 5);
    AssertEqual("Three roots found", rootsCnt, 3U);

    float testroot = -2.0f;
    AssertTrue("Root -2 found", (the::math::is_equal(roots[0], testroot)
        || the::math::is_equal(roots[1], testroot)
        || the::math::is_equal(roots[2], testroot)));
    testroot = 4.0f;
    AssertTrue("Root 4 found", (the::math::is_equal(roots[0], testroot)
        || the::math::is_equal(roots[1], testroot)
        || the::math::is_equal(roots[2], testroot)));
    testroot = 5.0f;
    AssertTrue("Root 5 found", (the::math::is_equal(roots[0], testroot)
        || the::math::is_equal(roots[1], testroot)
        || the::math::is_equal(roots[2], testroot)));

    poly3[0] = 0.0f;
    poly3[1] = -1.0f;
    poly3[2] = 0.0f;
    poly3[3] = 1.0f;

    AssertNearlyEqual("Root at -1", poly3(-1.0f), 0.0f);
    AssertNearlyEqual("Root at 0", poly3(0.0f), 0.0f);
    AssertNearlyEqual("Root at 1", poly3(1.0f), 0.0f);

    rootsCnt = poly3.find_roots(roots, 5);
    AssertEqual("Three roots found", rootsCnt, 3U);

    testroot = -1.0f;
    AssertTrue("Root -1 found", (the::math::is_equal(roots[0], testroot)
        || the::math::is_equal(roots[1], testroot)
        || the::math::is_equal(roots[2], testroot)));
    testroot = 0.0f;
    AssertTrue("Root 0 found", (the::math::is_equal(roots[0], testroot)
        || the::math::is_equal(roots[1], testroot)
        || the::math::is_equal(roots[2], testroot)));
    testroot = 1.0f;
    AssertTrue("Root 1 found", (the::math::is_equal(roots[0], testroot)
        || the::math::is_equal(roots[1], testroot)
        || the::math::is_equal(roots[2], testroot)));

    poly3[1] = 0.0f;

    AssertNearlyEqual("Root at 0", poly3(0.0f), 0.0f);

    rootsCnt = poly3.find_roots(roots, 5);
    AssertEqual("One roots found", rootsCnt, 1U);

    testroot = 0.0f;
    AssertTrue("Root 0 found", (the::math::is_equal(roots[0], testroot)
        || the::math::is_equal(roots[1], testroot)
        || the::math::is_equal(roots[2], testroot)));

    the::math::polynom<float, 4> poly4;
    // f(x) = (x - 4)(x + 3)(x - 2)(x + 1)
    //      = (x^2 - 6x + 8)(x^2 + 4x + 3)
    //      = x^4 + 4x^3 + 3x^2 - 6x^3 - 24x^2 - 18x + 8x^2 + 32x + 24
    //      = x^4 - 2x^3 - 13x^2 + 14x + 24
    poly4[0] = 24.0f;
    poly4[1] = 14.0f;
    poly4[2] = -13.0f;
    poly4[3] = -2.0f;
    poly4[4] = 1.0f;

    AssertNearlyEqual("Root at -3", poly4(-3.0f), 0.0f);
    AssertNearlyEqual("Root at -1", poly4(-1.0f), 0.0f);
    AssertNearlyEqual("Root at 2", poly4(2.0f), 0.0f);
    AssertNearlyEqual("Root at 4", poly4(4.0f), 0.0f);

    try {
        rootsCnt = poly4.find_roots(roots, 5);
        AssertEqual("Four roots found", rootsCnt, 4U);

        testroot = -3.0f;
        AssertTrue("Root -3 found", (the::math::is_equal(roots[0], testroot)
            || the::math::is_equal(roots[1], testroot)
            || the::math::is_equal(roots[2], testroot)
            || the::math::is_equal(roots[3], testroot)));
        testroot = -1.0f;
        AssertTrue("Root -1 found", (the::math::is_equal(roots[0], testroot)
            || the::math::is_equal(roots[1], testroot)
            || the::math::is_equal(roots[2], testroot)
            || the::math::is_equal(roots[3], testroot)));
        testroot = 2.0f;
        AssertTrue("Root 2 found", (the::math::is_equal(roots[0], testroot)
            || the::math::is_equal(roots[1], testroot)
            || the::math::is_equal(roots[2], testroot)
            || the::math::is_equal(roots[3], testroot)));
        testroot = 4.0f;
        AssertTrue("Root 4 found", (the::math::is_equal(roots[0], testroot)
            || the::math::is_equal(roots[1], testroot)
            || the::math::is_equal(roots[2], testroot)
            || the::math::is_equal(roots[3], testroot)));
    } catch (...) {
        AssertTrue("Root finding for polynom of degree 4 failed", false);
    }

    /*

    This feature is currently not implemented

    the::math::polynom<float, 5> poly5;
    // f(x) = (x + 5)(x - 4)(x + 3)(x - 2)(x + 1)
    //      = (x + 5)(x^2 - 6x + 8)(x^2 + 4x + 3)
    //      = (x + 5)(x^4 + 4x^3 + 3x^2 - 6x^3 - 24x^2 - 18x + 8x^2 + 32x + 24)
    //      = (x + 5)(x^4 - 2x^3 - 13x^2 + 14x + 24)
    //      = x^5 - 2x^4 - 13x^3 + 14x^2 + 24x + 5x^4 - 10x^3 - 65x^2 + 70x + 120
    //      = x^5 + 3x^4 - 23x^3 - 51x^2 + 94x + 120
    poly5[0] = 120.0f;
    poly5[1] = 94.0f;
    poly5[2] = -51.0f;
    poly5[3] = -23.0f;
    poly5[4] = 3.0f;
    poly5[5] = 1.0f;

    AssertNearlyEqual("Root at -5", poly5(-5.0f), 0.0f);
    AssertNearlyEqual("Root at -3", poly5(-3.0f), 0.0f);
    AssertNearlyEqual("Root at -1", poly5(-1.0f), 0.0f);
    AssertNearlyEqual("Root at 2", poly5(2.0f), 0.0f);
    AssertNearlyEqual("Root at 4", poly5(4.0f), 0.0f);

    try {
        rootsCnt = poly5.find_roots(roots, 5);
        AssertEqual("five roots found", rootsCnt, 5U);

        testroot = -5.0f;
        AssertTrue("Root -5 found", (the::math::is_equal(roots[0], testroot)
            || the::math::is_equal(roots[1], testroot)
            || the::math::is_equal(roots[2], testroot)
            || the::math::is_equal(roots[3], testroot)
            || the::math::is_equal(roots[4], testroot)));
        testroot = -3.0f;
        AssertTrue("Root -3 found", (the::math::is_equal(roots[0], testroot)
            || the::math::is_equal(roots[1], testroot)
            || the::math::is_equal(roots[2], testroot)
            || the::math::is_equal(roots[3], testroot)
            || the::math::is_equal(roots[4], testroot)));
        testroot = -1.0f;
        AssertTrue("Root -1 found", (the::math::is_equal(roots[0], testroot)
            || the::math::is_equal(roots[1], testroot)
            || the::math::is_equal(roots[2], testroot)
            || the::math::is_equal(roots[3], testroot)
            || the::math::is_equal(roots[4], testroot)));
        testroot = 2.0f;
        AssertTrue("Root 2 found", (the::math::is_equal(roots[0], testroot)
            || the::math::is_equal(roots[1], testroot)
            || the::math::is_equal(roots[2], testroot)
            || the::math::is_equal(roots[3], testroot)
            || the::math::is_equal(roots[4], testroot)));
        testroot = 4.0f;
        AssertTrue("Root 4 found", (the::math::is_equal(roots[0], testroot)
            || the::math::is_equal(roots[1], testroot)
            || the::math::is_equal(roots[2], testroot)
            || the::math::is_equal(roots[3], testroot)
            || the::math::is_equal(roots[4], testroot)));
    } catch (...) {
        AssertTrue("Root finding for polynom of degree 5 failed", false);
    }

    */
}

