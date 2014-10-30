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
 * testvector.cpp  27.11.2008
 *
 * Copyright (C) 2008 by Universitaet Stuttgart (VISUS). 
 * Alle Rechte vorbehalten.
 * Copyright (C) 2008 by Christoph Müller. Alle Rechte vorbehalten.
 */
#include "stdafx.h"
#include "testpoint.h"

#include "the/math/point.h"
#include "the/math/shallow_point.h"
#include "the/string.h"
#include "the/text/string_builder.h"
#include "testhelper.h"

typedef the::math::point<float, 2> pt2f;
typedef the::math::point<float, 3> pt3f;
typedef the::math::vector<float, 2> v2f;
typedef the::math::vector<float, 3> v3f;


void TestPoint2D(void) {
    pt2f p1;
    pt2f p2;
    pt2f p3;

    ::AssertEqual("Default ctor x", p1.x(), 0.0f);
    ::AssertEqual("Default ctor x (get)", p1.get_x(), 0.0f);
    ::AssertEqual("Default ctor y", p1.y(), 0.0f);
    ::AssertEqual("Default ctor y (get)", p1.get_y(), 0.0f);
    ::AssertTrue("is_origin", p1.is_origin());
    ::AssertTrue("Equality", p1 == p2);
    ::AssertFalse("Inequality", p1 != p2);

    p1.set_x(1.0f);
    ::AssertEqual("set_x", p1.x(), 1.0f);
    ::AssertFalse("!is_origin", p1.is_origin());
    ::AssertFalse("Equality", p1 == p2);
    ::AssertTrue("Inequality", p1 != p2);

    p1.set_y(2.0f);
    ::AssertEqual("set_y", p1.y(), 2.0f);
    ::AssertFalse("!is_origin", p1.is_origin());
    ::AssertFalse("Equality", p1 == p2);
    ::AssertTrue("Inequality", p1 != p2);

    p1[0] = 2.0f;
    ::AssertEqual("Array write x", p1.x(), 2.0f);

    p1[1] = 4.0f;
    ::AssertEqual("Array write y", p1.y(), 4.0f);

    AssertException("Illegal array access", p1[2], the::index_out_of_range_exception);

    p2 = p1;
    ::AssertTrue("Assignment", p1 == p2);

    p1.set(0.0f, 0.0f);
    p2.set(1.0f, 0.0f);
    p3 = p1.interpolate(p2, 0.5f);
    ::AssertEqual("Interpolate x", p3.x(), 0.5f);
    ::AssertEqual("Interpolate y", p3.y(), 0.0f);
}


void TestPoint3D(void) {
    pt3f p1;
    pt3f p2;
    pt3f p3;

    ::AssertEqual("Default ctor x", p1.x(), 0.0f);
    ::AssertEqual("Default ctor x (get)", p1.get_x(), 0.0f);
    ::AssertEqual("Default ctor y", p1.y(), 0.0f);
    ::AssertEqual("Default ctor y (get)", p1.get_y(), 0.0f);
    ::AssertEqual("Default ctor z", p1.z(), 0.0f);
    ::AssertEqual("Default ctor z (get)", p1.get_z(), 0.0f);
    ::AssertTrue("is_origin", p1.is_origin());
    ::AssertTrue("Equality", p1 == p2);
    ::AssertFalse("Inequality", p1 != p2);

    p1.set_x(1.0f);
    ::AssertEqual("set_x", p1.x(), 1.0f);
    ::AssertFalse("!is_origin", p1.is_origin());
    ::AssertFalse("Equality", p1 == p2);
    ::AssertTrue("Inequality", p1 != p2);

    p1.set_y(2.0f);
    ::AssertEqual("set_y", p1.y(), 2.0f);
    ::AssertFalse("!is_origin", p1.is_origin());
    ::AssertFalse("Equality", p1 == p2);
    ::AssertTrue("Inequality", p1 != p2);

    p1.set_z(5.0f);
    ::AssertEqual("set_z", p1.z(), 5.0f);
    ::AssertFalse("!is_origin", p1.is_origin());
    ::AssertFalse("Equality", p1 == p2);
    ::AssertTrue("Inequality", p1 != p2);

    p1[0] = 2.0f;
    ::AssertEqual("Array write x", p1.x(), 2.0f);

    p1[1] = 4.0f;
    ::AssertEqual("Array write y", p1.y(), 4.0f);

    p1[2] = 7.0f;
    ::AssertEqual("Array write z", p1.z(), 7.0f);

    AssertException("Illegal array access", p1[3], the::index_out_of_range_exception);

    p2 = p1;
    ::AssertTrue("Assignment", p1 == p2);

    p1.set(0.0f, 0.0f, 0.0f);
    p2.set(1.0f, 0.0f, 0.0f);
    p3 = p1.interpolate(p2, 0.5f);
    ::AssertEqual("Interpolate x", p3.x(), 0.5f);
    ::AssertEqual("Interpolate y", p3.y(), 0.0f);
    ::AssertEqual("Interpolate z", p3.y(), 0.0f);
}


void TestHalfspace2D(void) {
#define DO_TEST(res) \
    the::text::astring_builder::format_to(desc, \
        "p = (%3.1f, %3.1f), n = (%3.1f, %3.1f), t = (%3.1f, %3.1f)", \
        planePt.x(), planePt.y(), \
        normal.x(), normal.y(), \
        testPt.x(), testPt.y()); \
    AssertEqual(desc.c_str(), planePt.half_space(normal, testPt), the::math::half_space::res)
    
    //using the::math::half_space;
    the::astring desc;
    pt2f planePt;
    v2f normal;
    pt2f testPt;
    
    planePt.set(0.0f, 0.0f);
    normal.set(1.0f, 0.0f);

    testPt.set(0.0f, 0.0f);
    DO_TEST(in_plane);

    testPt.set_y(1.0f);
    DO_TEST(in_plane);

    testPt.set_y(-1.0f);
    DO_TEST(in_plane);

    testPt.set_x(1.0f);
    DO_TEST(positive);

    testPt.set_x(-1.0f);
    DO_TEST(negative);

    testPt.set(0.5f, 0.5f);
    DO_TEST(positive);

    testPt.set_x(-1.0f);
    DO_TEST(negative);
    

    normal.set(0.0f, 1.0f);

    testPt.set(0.0f, 0.0f);
    DO_TEST(in_plane);

    testPt.set_x(1.0f);
    DO_TEST(in_plane);

    testPt.set_x(-1.0f);
    DO_TEST(in_plane);

    testPt.set_y(1.0f);
    DO_TEST(positive);

    testPt.set_y(-1.0f);
    DO_TEST(negative);


    normal.set(0.5f, 0.5f);

    testPt.set(0.0f, 0.0f);
    DO_TEST(in_plane);

    testPt.set(0.5f, -0.5f);
    DO_TEST(in_plane);

    testPt.set(-0.5f, 0.5f);
    DO_TEST(in_plane);

    testPt.set(1.0f, 1.0f);
    DO_TEST(positive);

    testPt.set(-1.0f, -1.0f);
    DO_TEST(negative);

    testPt.set(1.0f, 0.0f);
    DO_TEST(positive);

    testPt.set(-1.0f, 0.0f);
    DO_TEST(negative);


    planePt.set(1.0f, 0.0f);
    normal.set(1.0f, 0.0f);

    testPt.set(0.0f, 0.0f);
    DO_TEST(negative);

    testPt.set_y(1.0f);
    DO_TEST(negative);

    testPt.set_y(-1.0f);
    DO_TEST(negative);

    testPt.set_x(1.0f);
    DO_TEST(in_plane);

    testPt.set_x(-1.0f);
    DO_TEST(negative);

    testPt.set_x(2.0f);
    DO_TEST(positive);

#undef DO_TEST
}


void TestHalfspace3D(void) {
#define DO_TEST(res) \
    the::text::astring_builder::format_to(desc, \
        "p = (%3.1f, %3.1f, %3.1f), n = (%3.1f, %3.1f, %3.1f), t = (%3.1f, %3.1f, %3.1f)", \
        planePt.x(), planePt.y(), planePt.z(), \
        normal.x(), normal.y(), planePt.z(), \
        testPt.x(), testPt.y(), planePt.z()); \
    AssertEqual(desc.c_str(), planePt.half_space(normal, testPt), the::math::half_space::res)

    //using the::math::HalfSpace;
    the::astring desc;
    pt3f planePt;
    v3f normal;
    pt3f testPt;

    // Repeat 2D tests - these must succeed, too.
    planePt.set(0.0f, 0.0f, 0.0f);
    normal.set(1.0f, 0.0f, 0.0f);

    testPt.set(0.0f, 0.0f, 0.0f);
    DO_TEST(in_plane);

    testPt.set_y(1.0f);
    DO_TEST(in_plane);

    testPt.set_y(-1.0f);
    DO_TEST(in_plane);

    testPt.set_x(1.0f);
    DO_TEST(positive);

    testPt.set_x(-1.0f);
    DO_TEST(negative);

    testPt.set(0.5f, 0.5f, 0.0f);
    DO_TEST(positive);

    testPt.set_x(-1.0f);
    DO_TEST(negative);
    

    normal.set(0.0f, 1.0f, 0.0f);

    testPt.set(0.0f, 0.0f, 0.0f);
    DO_TEST(in_plane);

    testPt.set_x(1.0f);
    DO_TEST(in_plane);

    testPt.set_x(-1.0f);
    DO_TEST(in_plane);

    testPt.set_y(1.0f);
    DO_TEST(positive);

    testPt.set_y(-1.0f);
    DO_TEST(negative);


    normal.set(0.5f, 0.5f, 0.0f);

    testPt.set(0.0f, 0.0f, 0.0f);
    DO_TEST(in_plane);

    testPt.set(0.5f, -0.5f, 0.0f);
    DO_TEST(in_plane);

    testPt.set(-0.5f, 0.5f, 0.0f);
    DO_TEST(in_plane);

    testPt.set(1.0f, 1.0f, 0.0f);
    DO_TEST(positive);

    testPt.set(-1.0f, -1.0f, 0.0f);
    DO_TEST(negative);

    testPt.set(1.0f, 0.0f, 0.0f);
    DO_TEST(positive);

    testPt.set(-1.0f, 0.0f, 0.0f);
    DO_TEST(negative);


    planePt.set(1.0f, 0.0f, 0.0f);
    normal.set(1.0f, 0.0f, 0.0f);

    testPt.set(0.0f, 0.0f, 0.0f);
    DO_TEST(negative);

    testPt.set_y(1.0f);
    DO_TEST(negative);

    testPt.set_y(-1.0f);
    DO_TEST(negative);

    testPt.set_x(1.0f);
    DO_TEST(in_plane);

    testPt.set_x(-1.0f);
    DO_TEST(negative);

    testPt.set_x(2.0f);
    DO_TEST(positive);


    // True 3D tests
    planePt.set(0.0f, 0.0f, 0.0f);
    normal.set(1.0f, 1.0f, 1.0f);

    testPt.set(0.0f, 0.0f, 0.0f);
    DO_TEST(in_plane);

    testPt.set(1.0f, 1.0f, 1.0f);
    DO_TEST(positive);

    testPt.set(1.0f, 0.0f, 0.0f);
    DO_TEST(positive);

    testPt.set(0.0f, 1.0f, 0.0f);
    DO_TEST(positive);

    testPt.set(0.0f, 0.0f, 1.0f);
    DO_TEST(positive);

    testPt.set(-1.0f, 0.0f, 0.0f);
    DO_TEST(negative);

    testPt.set(0.0f, -1.0f, 0.0f);
    DO_TEST(negative);

    testPt.set(0.0f, 0.0f, -1.0f);
    DO_TEST(negative);

    testPt.set(1.0f, 0.0f, -1.0f);
    DO_TEST(in_plane);

    testPt.set(1.0f, -1.0f, 0.0f);
    DO_TEST(in_plane);

    testPt.set(0.0f, -10.0f, 10.0f);
    DO_TEST(in_plane);

    testPt.set(10.0f, -10.0f, 0.0f);
    DO_TEST(in_plane);
}


void TestPoint(void) {
    ::TestPoint2D();
    ::TestHalfspace2D();
    ::TestPoint3D();
    ::TestHalfspace3D();
}
