/*
 * testquaternion.cpp
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
 * testquaternion.h
 *
 * Copyright (C) 2006 - 2007 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */
#include "stdafx.h"
#include "testquaternion.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include "testhelper.h"
#include "the/math/quaternion.h"
#include "the/math/vector.h"


void TestQuaternion(void) {
    using namespace the::math;
    using the::math::sqrt;
    quaternion<double> q1;
    quaternion<double> q2(1.0, 0.0, 0.0, 1.0);
    quaternion<double> q3;

    AssertEqual("Quaternion dft ctor x", q1.x(), 0.0);
    AssertEqual("Quaternion dft ctor y", q1.y(), 0.0);
    AssertEqual("Quaternion dft ctor z", q1.z(), 0.0);
    AssertEqual("Quaternion dft ctor w", q1.w(), 1.0);

    AssertEqual("Quaternion 4 component ctor x", q2.x(), 1.0);
    AssertEqual("Quaternion 4 component ctor y", q2.y(), 0.0);
    AssertEqual("Quaternion 4 component ctor z", q2.z(), 0.0);
    AssertEqual("Quaternion 4 component ctor w", q2.w(), 1.0);

    q3 = q1 * q2;

    q1.set(M_PI * 0.5, vector<double, 3>(0.0, 1.0, 0.0)); // rotates 90° around the y-axis
    vector<double, 3> v1 = q1 * vector<double, 3>(1.0, 0.0f, 0.0);
    AssertEqual("Rotation 1 works", v1, vector<double, 3>(0.0, 0.0, -1.0));

    q2.set(M_PI * 0.25, vector<double, 3>(1.0, 0.0, 0.0)); // rotates 45° around the x-axis
    vector<double, 3> v2 = q2 * v1;
    AssertEqual("Rotation 2 works", v2, vector<double, 3>(0.0, 0.5 * sqrt(2.0), -0.5 * sqrt(2.0)));

    q3 = q2 * q1;
    vector<double, 3> v3 = q3 * vector<double, 3>(1.0, 0.0f, 0.0);
    AssertEqual("Combined rotation works", v2, v3);

    v1.set(1.0, -2.0, 3.0);
    v1.normalise();
    double a = pi_double * 0.987654321;
    q1.set(a, v1);
    double b;
    q1.angle_and_axis(b, v2);

    AssertTrue("Angle reconstructed", is_equal(a, b));
    AssertEqual("Axis reconstructed", v1, v2);

}
