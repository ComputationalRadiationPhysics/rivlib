/*
 * testmatrix.cpp
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
 * testmatrix.cpp
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */
#include "stdafx.h"
#include "testmatrix.h"

#include "testhelper.h"

#define USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <climits>
#include <time.h>

#include "the/math/functions.h"
#include "the/math/matrix.h"
#include "the/math/polynom.h"
#include "the/math/quaternion.h"
#include "the/math/pcautils.h"
#include "the/text/string_builder.h"


void TestMatrix(void) {
    using namespace std;
    using namespace the;
    using namespace the::math;

    matrix<double, 4, matrix_layout::column_major> m1;
    matrix<double, 4, matrix_layout::row_major> m2;
    matrix<double, 4, matrix_layout::column_major> m3;
    matrix<double, 4, matrix_layout::row_major> m4;
    matrix<double, 4, matrix_layout::column_major> m5;
    matrix<double, 3, matrix_layout::column_major> m6;

    ::AssertTrue("Default ctor creates id matrix.", m1.is_identity());
    ::AssertTrue("Default ctor creates id matrix.", m2.is_identity());
    ::AssertFalse("Default ctor creates no null matrix.", m1.is_null());

    ::AssertTrue("Compare differently layouted matrices.", m1 == m2);

    ::AssertTrue("Compare Matrix<4> and Matrix4.", m1 == m5);

    ::AssertEqual("get_at 0, 0", m1.get_at(0, 0), 1.0);
    ::AssertEqual("Function call get at 0, 0", m1(0, 0), 1.0);
    ::AssertEqual("get_at 1, 0", m1.get_at(1, 0), 0.0);
    ::AssertEqual("Function call get at 1, 0", m1(1, 0), 0.0);

    m1.set_at(0, 0, 0);
    ::AssertFalse("Id matrix destroyed.", m1.is_identity());

    ::AssertFalse("Compare differently layouted matrices.", m1 == m2);
    ::AssertTrue("Compare differently layouted matrices.", m1 != m2);

    m1.set_at(0, 0, 1);
    ::AssertTrue("Id matrix restored.", m1.is_identity());

    m1.set_at(0, 0, 0);
    ::AssertEqual("set_at 0, 0", m1.get_at(0, 0), 0.0);
    m1.set_at(0, 1, 2.0);
    ::AssertEqual("set_at 0, 1", m1.get_at(0, 1), 2.0);
    m1.transpose();
    ::AssertEqual("Transposed 0, 0", m1.get_at(0, 0), 0.0);
    ::AssertNotEqual("Transposed 0, 1", m1.get_at(0, 1), 2.0);
    ::AssertEqual("Transposed 1, 0", m1.get_at(1, 0), 2.0);

    m1.set_identity();
    ::AssertTrue("Id matrix restored.", m1.is_identity());
    m1.invert();
    ::AssertTrue("inverting identity.", m1.is_identity());

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            m1.set_at(r, c, static_cast<float>(c));
        }
    }
    m3 = m1;
    ::AssertFalse("inverting uninvertable matrix.", m1.invert());
    ::AssertEqual("Univertable matrix has determinant 0.", m1.determinant(), 0.0);
    ::AssertEqual("Matrix unchanged after invert failed.", m1, m3);

    m1.set_at(0, 0, 1.0);
    m1.set_at(1, 0, 2.0);
    m1.set_at(2, 0, 3.0);
    m1.set_at(3, 0, 4.0);

    m1.set_at(0, 1, 4.0);
    m1.set_at(1, 1, 1.0);
    m1.set_at(2, 1, 2.0);
    m1.set_at(3, 1, 3.0);

    m1.set_at(0, 2, 3.0);
    m1.set_at(1, 2, 4.0);
    m1.set_at(2, 2, 1.0);
    m1.set_at(3, 2, 2.0);

    m1.set_at(0, 3, 2.0);
    m1.set_at(1, 3, 3.0);
    m1.set_at(2, 3, 4.0);
    m1.set_at(3, 3, 1.0);

    m1.dump(cout);

    m3 = m1;
    ::AssertEqual("Assignment.", m1, m3);

    ::AssertTrue("Determinant", is_equal(m3.determinant(), -160.0));

    m2 = m1;
    ::AssertTrue("Converstion assignment.", m1 == m2);

    m4 = m2;
    ::AssertEqual("Assignment.", m2, m4);

    ::AssertEqual("m1 @ 0, 0", m1.get_at(0, 0), 1.0);
    ::AssertEqual("m1 @ 1, 0", m1.get_at(1, 0), 2.0);
    ::AssertEqual("m1 @ 2, 0", m1.get_at(2, 0), 3.0);
    ::AssertEqual("m1 @ 3, 0", m1.get_at(3, 0), 4.0);

    ::AssertEqual("m1 @ 0, 1", m1.get_at(0, 1), 4.0);
    ::AssertEqual("m1 @ 1, 1", m1.get_at(1, 1), 1.0);
    ::AssertEqual("m1 @ 2, 1", m1.get_at(2, 1), 2.0);
    ::AssertEqual("m1 @ 3, 1", m1.get_at(3, 1), 3.0);

    ::AssertEqual("m1 @ 0, 2", m1.get_at(0, 2), 3.0);
    ::AssertEqual("m1 @ 1, 2", m1.get_at(1, 2), 4.0);
    ::AssertEqual("m1 @ 2, 2", m1.get_at(2, 2), 1.0);
    ::AssertEqual("m1 @ 3, 2", m1.get_at(3, 2), 2.0);

    ::AssertEqual("m1 @ 0, 3", m1.get_at(0, 3), 2.0);
    ::AssertEqual("m1 @ 1, 3", m1.get_at(1, 3), 3.0);
    ::AssertEqual("m1 @ 2, 3", m1.get_at(2, 3), 4.0);
    ::AssertEqual("m1 @ 3, 3", m1.get_at(3, 3), 1.0);

    ::AssertEqual("m1 @ 0, 0 == @ 0", m1.get_at(0, 0), m1.peek_components()[0 + 0 * 4]);
    ::AssertEqual("m1 @ 1, 0 == @ 1", m1.get_at(1, 0), m1.peek_components()[1 + 0 * 4]);
    ::AssertEqual("m1 @ 2, 0 == @ 2", m1.get_at(2, 0), m1.peek_components()[2 + 0 * 4]);
    ::AssertEqual("m1 @ 3, 0 == @ 3", m1.get_at(3, 0), m1.peek_components()[3 + 0 * 4]);

    ::AssertEqual("m1 @ 0, 1 == @ 4", m1.get_at(0, 1), m1.peek_components()[0 + 1 * 4]);
    ::AssertEqual("m1 @ 1, 1 == @ 5", m1.get_at(1, 1), m1.peek_components()[1 + 1 * 4]);
    ::AssertEqual("m1 @ 2, 1 == @ 6", m1.get_at(2, 1), m1.peek_components()[2 + 1 * 4]);
    ::AssertEqual("m1 @ 3, 1 == @ 7", m1.get_at(3, 1), m1.peek_components()[3 + 1 * 4]);

    ::AssertEqual("m1 @ 0, 2 == @ 8", m1.get_at(0, 2), m1.peek_components()[0 + 2 * 4]);
    ::AssertEqual("m1 @ 1, 2 == @ 9", m1.get_at(1, 2), m1.peek_components()[1 + 2 * 4]);
    ::AssertEqual("m1 @ 2, 2 == @ 10", m1.get_at(2, 2), m1.peek_components()[2 + 2 * 4]);
    ::AssertEqual("m1 @ 3, 2 == @ 11", m1.get_at(3, 2), m1.peek_components()[3 + 2 * 4]);

    ::AssertEqual("m1 @ 0, 3 == @ 12", m1.get_at(0, 3), m1.peek_components()[0 + 3 * 4]);
    ::AssertEqual("m1 @ 1, 3 == @ 13", m1.get_at(1, 3), m1.peek_components()[1 + 3 * 4]);
    ::AssertEqual("m1 @ 2, 3 == @ 14", m1.get_at(2, 3), m1.peek_components()[2 + 3 * 4]);
    ::AssertEqual("m1 @ 3, 3 == @ 15", m1.get_at(3, 3), m1.peek_components()[3 + 3 * 4]);


    ::AssertEqual("m1 @ 0, 0 == @ 0", m2.get_at(0, 0), m2.peek_components()[0 * 4 + 0]);
    ::AssertEqual("m1 @ 1, 0 == @ 4", m2.get_at(1, 0), m2.peek_components()[1 * 4 + 0]);
    ::AssertEqual("m1 @ 2, 0 == @ 8", m2.get_at(2, 0), m2.peek_components()[2 * 4 + 0]);
    ::AssertEqual("m1 @ 3, 0 == @ 12", m2.get_at(3, 0), m2.peek_components()[3 * 4 + 0]);

    ::AssertEqual("m1 @ 0, 1 == @ 1", m2.get_at(0, 1), m2.peek_components()[0 * 4 + 1]);
    ::AssertEqual("m1 @ 1, 1 == @ 5", m2.get_at(1, 1), m2.peek_components()[1 * 4 + 1]);
    ::AssertEqual("m1 @ 2, 1 == @ 9", m2.get_at(2, 1), m2.peek_components()[2 * 4 + 1]);
    ::AssertEqual("m1 @ 3, 1 == @ 13", m2.get_at(3, 1), m2.peek_components()[3 * 4 + 1]);

    ::AssertEqual("m1 @ 0, 2 == @ 2", m2.get_at(0, 2), m2.peek_components()[0 * 4 + 2]);
    ::AssertEqual("m1 @ 1, 2 == @ 6", m2.get_at(1, 2), m2.peek_components()[1 * 4 + 2]);
    ::AssertEqual("m1 @ 2, 2 == @ 10", m2.get_at(2, 2), m2.peek_components()[2 * 4 + 2]);
    ::AssertEqual("m1 @ 3, 2 == @ 14", m2.get_at(3, 2), m2.peek_components()[3 * 4 + 2]);

    ::AssertEqual("m1 @ 0, 3 == @ 3", m2.get_at(0, 3), m2.peek_components()[0 * 4 + 3]);
    ::AssertEqual("m1 @ 1, 3 == @ 7", m2.get_at(1, 3), m2.peek_components()[1 * 4 + 3]);
    ::AssertEqual("m1 @ 2, 3 == @ 11", m2.get_at(2, 3), m2.peek_components()[2 * 4 + 3]);
    ::AssertEqual("m1 @ 3, 3 == @ 15", m2.get_at(3, 3), m2.peek_components()[3 * 4 + 3]);


    ::AssertTrue("invert matrix.", m1.invert());
    ::AssertNearlyEqual("inverted @ 0, 0.", m1.get_at(0, 0), -9.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 1, 0.", m1.get_at(1, 0), 11.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 2, 0.", m1.get_at(2, 0), 1.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 3, 0.", m1.get_at(3, 0), 1.0 / 40.0);

    ::AssertNearlyEqual("inverted @ 0, 1.", m1.get_at(0, 1), 1.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 1, 1.", m1.get_at(1, 1), -9.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 2, 1.", m1.get_at(2, 1), 11.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 3, 1.", m1.get_at(3, 1), 1.0 / 40.0);

    ::AssertNearlyEqual("inverted @ 0, 2.", m1.get_at(0, 2), 1.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 1, 2.", m1.get_at(1, 2), 1.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 2, 2.", m1.get_at(2, 2), -9.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 3, 2.", m1.get_at(3, 2), 11.0 / 40.0);

    ::AssertNearlyEqual("inverted @ 0, 3.", m1.get_at(0, 3), 11.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 1, 3.", m1.get_at(1, 3), 1.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 2, 3.", m1.get_at(2, 3), 1.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 3, 3.", m1.get_at(3, 3), -9.0 / 40.0);

    ::AssertTrue("invert matrix.", m2.invert());
    ::AssertNearlyEqual("inverted @ 0, 0.", m2.get_at(0, 0), -9.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 1, 0.", m2.get_at(1, 0), 11.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 2, 0.", m2.get_at(2, 0), 1.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 3, 0.", m2.get_at(3, 0), 1.0 / 40.0);

    ::AssertNearlyEqual("inverted @ 0, 1.", m2.get_at(0, 1), 1.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 1, 1.", m2.get_at(1, 1), -9.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 2, 1.", m2.get_at(2, 1), 11.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 3, 1.", m2.get_at(3, 1), 1.0 / 40.0);

    ::AssertNearlyEqual("inverted @ 0, 2.", m2.get_at(0, 2), 1.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 1, 2.", m2.get_at(1, 2), 1.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 2, 2.", m2.get_at(2, 2), -9.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 3, 2.", m2.get_at(3, 2), 11.0 / 40.0);

    ::AssertNearlyEqual("inverted @ 0, 3.", m2.get_at(0, 3), 11.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 1, 3.", m2.get_at(1, 3), 1.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 2, 3.", m2.get_at(2, 3), 1.0 / 40.0);
    ::AssertNearlyEqual("inverted @ 3, 3.", m2.get_at(3, 3), -9.0 / 40.0);

    m3 *= m1;
    ::AssertTrue("m * m^-1 = id", m3.is_identity());
    
    m4 = m4 * m2;
    ::AssertTrue("m * m^-1 = id", m4.is_identity());


    vector<double, 3> axis(1.0, 0.0, 0.0);
    quaternion<double> q1(1.0, axis);
    matrix<double, 4, matrix_layout::column_major> rm1(q1);

    ::AssertNearlyEqual("Rotation from quaterion @ 0, 0.", rm1.get_at(0, 0), 1.0);
    ::AssertNearlyEqual("Rotation from quaterion @ 1, 0.", rm1.get_at(1, 0), 0.0);
    ::AssertNearlyEqual("Rotation from quaterion @ 2, 0.", rm1.get_at(2, 0), 0.0);
    ::AssertNearlyEqual("Rotation from quaterion @ 3, 0.", rm1.get_at(3, 0), 0.0);

    ::AssertNearlyEqual("Rotation from quaterion @ 0, 1.", rm1.get_at(0, 1), 0.0);
    ::AssertNearlyEqual<double>("Rotation from quaterion @ 1, 1.", rm1.get_at(1, 1), cos(1.0));
    ::AssertNearlyEqual<double>("Rotation from quaterion @ 2, 1.", rm1.get_at(2, 1), sin(1.0));
    ::AssertNearlyEqual("Rotation from quaterion @ 3, 1.", rm1.get_at(3, 1), 0.0);

    ::AssertNearlyEqual("Rotation from quaterion @ 0, 2.", rm1.get_at(0, 2), 0.0);
    ::AssertNearlyEqual<double>("Rotation from quaterion @ 1, 2.", rm1.get_at(1, 2), -sin(1.0));
    ::AssertNearlyEqual<double>("Rotation from quaterion @ 2, 2.", rm1.get_at(2, 2), cos(1.0));
    ::AssertNearlyEqual("Rotation from quaterion @ 3, 2.", rm1.get_at(3, 2), 0.0);

    ::AssertNearlyEqual("Rotation from quaterion @ 0, 3.", rm1.get_at(0, 3), 0.0);
    ::AssertNearlyEqual("Rotation from quaterion @ 1, 3.", rm1.get_at(1, 3), 0.0);
    ::AssertNearlyEqual("Rotation from quaterion @ 2, 3.", rm1.get_at(2, 3), 0.0);
    ::AssertNearlyEqual("Rotation from quaterion @ 3, 3.", rm1.get_at(3, 3), 1.0);

    ::AssertTrue("Rotation Matrix", rm1.is_rotation());

/*
http://de.wikipedia.org/wiki/Charakteristisches_polynom
0 2 -1
2 -1 1
2 -1 3
charakteristisches polynom:
    -x^3 + 2x^2 + 4x - 8
reelle Eigenwerte:
    {-2, 2, (2)}
Eigenvektor zu Eigenwert -2:
    {-3, 4, 2}
Eigenvektor zu Eigenwert 2:
    {1, 0, -2}
*/
    m6.set_at(0, 0, 0.0);
    m6.set_at(1, 0, 2.0);
    m6.set_at(2, 0, -1.0);

    m6.set_at(0, 1, 2.0);
    m6.set_at(1, 1, -1.0);
    m6.set_at(2, 1, 1.0);

    m6.set_at(0, 2, 2.0);
    m6.set_at(1, 2, -1.0);
    m6.set_at(2, 2, 3.0);

    ::AssertFalse("Not a rotation matrix", m6.is_rotation());

    polynom<double, 3> cp6(m6.characteristic_polynom());
    if (is_equal(cp6[3], 1.0)) cp6 *= -1.0;
    AssertNearlyEqual("Coefficient a0 = -8", cp6[0], -8.0);
    AssertNearlyEqual("Coefficient a1 = 4", cp6[1], 4.0);
    AssertNearlyEqual("Coefficient a2 = 2", cp6[2], 2.0);
    AssertNearlyEqual("Coefficient a3 = -1", cp6[3], -1.0);

    double ev[4];
    unsigned int evc;
    double evt;

    evc = cp6.find_roots(ev, 4);
    AssertEqual("Found two eigenvalues", evc, 2U);

    evt = 2.0;
    AssertTrue("Eigenvalue 2 found", is_equal(ev[0], evt) || is_equal(ev[1], evt));
    evt = -2.0;
    AssertTrue("Eigenvalue -2 found", is_equal(ev[0], evt) || is_equal(ev[1], evt));

    vector<double, 3> eigenvectors6[3];

    //Finding Eigenvectors currently only works for symmetric matrices
    //try {
    //    evc = m6.find_eigenvalues(ev, eigenvectors6, 3);
    //    AssertEqual("Found two eigenvalues", evc, 2U);

    //    double te6[2];
    //    vector<double, 3> tv6[2];
    //    te6[0] = -2.0;
    //    tv6[0].set(-3.0, 4.0, 2.0);
    //    te6[1] = 2.0;
    //    tv6[1].set(1.0, 0.0, -2.0);

    //    for (int i = 0; i < 2; i++) {
    //        int p = -1;
    //        for (unsigned int j = 0; j < evc; j++) {
    //            if (is_equal(ev[j], te6[i])) {
    //                p = j;
    //                break;
    //            }
    //        }
    //        the::astring msg;
    //        the::text::astring_builder::format_to(msg, "Eigenvalue %f found", te6[i]);
    //        if (p < 0) {
    //            AssertTrue(msg.c_str(), false);
    //            continue;
    //        }
    //        AssertTrue(msg.c_str(), true);

    //        the::text::astring_builder::format_to(msg, "Eigenvector %d correct", i);
    //        AssertTrue(msg.c_str(), tv6[i].is_parallel(eigenvectors6[p]));
    //    }
    //} catch(the::exception ex) {
    //    printf("Exception: %s\n", ex.get_msg_astr());
    //    AssertTrue("Find eigenvalues", false);
    //} catch(...) {
    //    AssertTrue("Find eigenvalues", false);
    //}

/*
http://www.arndt-bruenner.de/mathe/scripts/eigenwert2.htm
Symetrische Matrix
1 2 3
2 4 5
3 5 6
charakteristisches polynom:
    -x^3 + 11x^2 + 4x - 1
reelle Eigenwerte:
    {-0,5157294715892572 ;  0,1709151888271795 ;  11,344814282762078}
Eigenvektoren:
 zum Eigenwert -0,5157294715892572:
   [ -1,2469796037174667 ; -0,5549581320873715 ; 1 ]
 zum Eigenwert 0,1709151888271795:
   [ 1,8019377358048385 ; -2,246979603717467 ; 1 ]
 zum Eigenwert 11,344814282762078:
   [ 0,4450418679126288 ; 0,8019377358048381 ; 1 ]
*/
    m6.set_at(0, 0, 1.0);
    m6.set_at(1, 0, 2.0);
    m6.set_at(2, 0, 3.0);

    m6.set_at(0, 1, 2.0);
    m6.set_at(1, 1, 4.0);
    m6.set_at(2, 1, 5.0);

    m6.set_at(0, 2, 3.0);
    m6.set_at(1, 2, 5.0);
    m6.set_at(2, 2, 6.0);

    cp6 = m6.characteristic_polynom();
    if (is_equal(cp6[3], 1.0)) cp6 *= -1.0;
    AssertNearlyEqual("Coefficient a0 = -1", cp6[0], -1.0);
    AssertNearlyEqual("Coefficient a1 = 4", cp6[1], 4.0);
    AssertNearlyEqual("Coefficient a2 = 11", cp6[2], 11.0);
    AssertNearlyEqual("Coefficient a3 = -1", cp6[3], -1.0);

    evc = cp6.find_roots(ev, 4);
    AssertEqual("Found three eigenvalues", evc, 3U);

    evt = -0.5157294715892572;
    AssertTrue("Eigenvalue -0.5157294715892572 found", is_equal(ev[0], evt) || is_equal(ev[1], evt) || is_equal(ev[2], evt));
    evt = 0.1709151888271795;
    AssertTrue("Eigenvalue 0.1709151888271795 found", is_equal(ev[0], evt) || is_equal(ev[1], evt) || is_equal(ev[2], evt));
    evt = 11.344814282762078;
    AssertTrue("Eigenvalue 11.344814282762078 found", is_equal(ev[0], evt) || is_equal(ev[1], evt) || is_equal(ev[2], evt));

    try {
        evc = m6.find_eigenvalues(ev, eigenvectors6, 3);
        AssertEqual("Found three eigenvalues", evc, 3U);

        double te6[3];
        vector<double, 3> tv6[3];
        te6[0] = -0.5157294715892572;
        tv6[0].set(-1.2469796037174667, -0.5549581320873715, 1.0);
        te6[1] = 0.1709151888271795;
        tv6[1].set(1.8019377358048385, -2.246979603717467, 1.0);
        te6[2] = 11.344814282762078;
        tv6[2].set(0.4450418679126288, 0.8019377358048381, 1.0);

        for (int i = 0; i < 3; i++) {
            int p = -1;
            for (unsigned int j = 0; j < evc; j++) {
                if (is_equal(ev[j], te6[i])) {
                    p = j;
                    break;
                }
            }
            the::astring msg;
            the::text::astring_builder::format_to(msg, "Eigenvalue %f found", te6[i]);
            if (p < 0) {
                AssertTrue(msg.c_str(), false);
                continue;
            }
            AssertTrue(msg.c_str(), true);

            the::text::astring_builder::format_to(msg, "Eigenvector %d correct", i);
            AssertTrue(msg.c_str(), tv6[i].is_parallel(eigenvectors6[p]));
        }
    } catch(the::exception ex) {
        printf("Exception: %s\n", ex.get_msg_astr());
        AssertTrue("Find eigenvalues", false);
    } catch(...) {
        AssertTrue("Find eigenvalues", false);
    }

/*
http://www.arndt-bruenner.de/mathe/scripts/eigenwert.htm
  -3  -4  -8   4
   5  -7   8   1
  -5   4  -7   7
  -3   5   7  -6
charakteristisches polynom:
    x^4 + 23x^3 + 99x^2 - 675x - 3416
reelle Eigenwerte:
    {-12,589159228961312; -11,096724174256692; -4,613900261118899; 5,299783664336905}
Eigenvektor zu Eigenwert -12,589159228961312: 
    (0,05939964936610216; -0,8369818269339811; 0,5360406456563438; 0,0927012903997002)
Eigenvektor zu Eigenwert -11,096724174256692: 
    (-0,06271378020655696; -0,8667725283798831; 0,4604325354943767; 0,18103658767323305)
Eigenvektor zu Eigenwert -4,613900261118899: 
    (0,6759032660943609; 0,7055384845142796; -0,21288595069057908; 0,007056468722919981)
Eigenvektor zu Eigenwert 5,299783664336905: 
    (-0,4245882668759824; 0,2734502816046086; 0,6093868049835593; 0,611226201200128)
*/
    m1.set_at(0, 0, -3.0);
    m1.set_at(1, 0, -4.0);
    m1.set_at(2, 0, -8.0);
    m1.set_at(3, 0, 4.0);

    m1.set_at(0, 1, 5.0);
    m1.set_at(1, 1, -7.0);
    m1.set_at(2, 1, 8.0);
    m1.set_at(3, 1, 1.0);

    m1.set_at(0, 2, -5.0);
    m1.set_at(1, 2, 4.0);
    m1.set_at(2, 2, -7.0);
    m1.set_at(3, 2, 7.0);

    m1.set_at(0, 3, -3.0);
    m1.set_at(1, 3, 5.0);
    m1.set_at(2, 3, 7.0);
    m1.set_at(3, 3, -6.0);

    ::AssertFalse("Not a rotation matrix", m1.is_rotation());

    polynom<double, 4> cp1(m1.characteristic_polynom());
    if (is_equal(cp1[4], -1.0)) cp1 *= -1.0;
    AssertNearlyEqual("Coefficient a0 = -3416", cp1[0], -3416.0);
    AssertNearlyEqual("Coefficient a1 = -675", cp1[1], -675.0);
    AssertNearlyEqual("Coefficient a2 = 99", cp1[2], 99.0);
    AssertNearlyEqual("Coefficient a3 = 23", cp1[3], 23.0);
    AssertNearlyEqual("Coefficient a4 = 1", cp1[4], 1.0);

    evc = cp1.find_roots(ev, 4);
    AssertEqual("Found four eigenvalues", evc, 4U);

    evt = -12.589159228961312;
    AssertTrue("Eigenvalue -12.589159228961312 found", is_equal(ev[0], evt)
        || is_equal(ev[1], evt) || is_equal(ev[2], evt)
        || is_equal(ev[3], evt));
    evt = -11.096724174256692;
    AssertTrue("Eigenvalue -11.096724174256692 found", is_equal(ev[0], evt)
        || is_equal(ev[1], evt) || is_equal(ev[2], evt)
        || is_equal(ev[3], evt));
    evt = -4.613900261118899;
    AssertTrue("Eigenvalue -4.613900261118899 found", is_equal(ev[0], evt)
        || is_equal(ev[1], evt) || is_equal(ev[2], evt)
        || is_equal(ev[3], evt));
    evt = 5.299783664336905;
    AssertTrue("Eigenvalue 5.299783664336905 found", is_equal(ev[0], evt)
        || is_equal(ev[1], evt) || is_equal(ev[2], evt)
        || is_equal(ev[3], evt));


    vector<double, 4> eigenvectors1[4];

    ////Finding Eigenvectors currently only works for symmetric matrices
    //try {
    //    evc = m1.find_eigenvalues(ev, eigenvectors1, 4);
    //    AssertEqual("Found four eigenvalues", evc, 4U);

    //    double te1[4];
    //    vector<double, 4> tv1[4];
    //    te1[0] = -12.589159228961312;
    //    tv1[0][0] = 0.05939964936610216;
    //    tv1[0][1] = -0.8369818269339811;
    //    tv1[0][2] = 0.5360406456563438;
    //    tv1[0][3] = 0.0927012903997002;
    //    te1[1] = -11.096724174256692;
    //    tv1[1][0] = -0.06271378020655696;
    //    tv1[1][1] = -0.8667725283798831;
    //    tv1[1][2] = 0.4604325354943767;
    //    tv1[1][3] = 0.18103658767323305;
    //    te1[2] = -4.613900261118899;
    //    tv1[2][0] = 0.6759032660943609;
    //    tv1[2][1] = 0.7055384845142796;
    //    tv1[2][2] = -0.21288595069057908;
    //    tv1[2][3] = 0.007056468722919981;
    //    te1[3] = 5.299783664336905;
    //    tv1[3][0] = -0.4245882668759824;
    //    tv1[3][1] = 0.2734502816046086;
    //    tv1[3][2] = 0.6093868049835593;
    //    tv1[3][3] = 0.611226201200128;

    //    for (int i = 0; i < 4; i++) {
    //        int p = -1;
    //        for (unsigned int j = 0; j < evc; j++) {
    //            if (is_equal(ev[j], te1[i])) {
    //                p = j;
    //                break;
    //            }
    //        }
    //        the::astring msg;
    //        the::text::astring_builder::format_to(msg, "Eigenvalue %f found", te1[i]);
    //        if (p < 0) {
    //            AssertTrue(msg.c_str(), false);
    //            continue;
    //        }
    //        AssertTrue(msg.c_str(), true);

    //        the::text::astring_builder::format_to(msg, "Eigenvector %d correct", i);
    //        AssertTrue(msg.c_str(), tv1[i].is_parallel(eigenvectors1[p]));
    //    }
    //} catch(the::exception ex) {
    //    printf("Exception: %s\n", ex.get_msg_astr());
    //    AssertTrue("Find eigenvalues", false);
    //} catch(...) {
    //    AssertTrue("Find eigenvalues", false);
    //}

    ::srand(static_cast<unsigned int>(::time(NULL)));
    for (unsigned int tc = 0; tc < 10; tc++) {
        double a1 = static_cast<double>(::rand() % SHRT_MAX) / static_cast<double>(SHRT_MAX) * 2.0 * pi_double;
        double a2 = (static_cast<double>(::rand() % SHRT_MAX) / static_cast<double>(SHRT_MAX - 1) - 0.5) * pi_double;
        double a3 = (static_cast<double>(::rand() % SHRT_MAX) / static_cast<double>(SHRT_MAX) - 0.5) * 2.0 * pi_double;

        vector<double, 3> v(cos(a2) * cos(a1), cos(a2) * sin(a1), sin(a2));
        quaternion<double> qi(a3, v);
        quaternion<double> qo;
        matrix<double, 3, matrix_layout::column_major> m3d;
        matrix<double, 4, matrix_layout::column_major> m4d(qi);
        m3d = qi;

        v.set(1.0, 0.0, 0.0);
        vector<double, 3> v2 = qi * v;
        vector<double, 3> v3 = m3d * v;

        AssertTrue("Quaternion convertes to rotation matrix", m3d.is_rotation());
        AssertTrue("Quaternion convertes to rotation matrix", m4d.is_rotation());
        AssertEqual("Matrix and quaternion describe the same rotation", v2, v3);

        qo = m3d;
        AssertEqual("Quaternion reconstructed from matrix", qo, qi);
        qo = m4d;
        AssertEqual("Quaternion reconstructed from matrix", qo, qi);

    }

    {
        quaternion<double> qi(pi_double, vector<double, 3>(1.0, 0.0, 0.0));
        matrix<double, 3, matrix_layout::column_major> m3d(qi);
        quaternion<double> qo = m3d;
        AssertEqual("Quaternion reconstructed from matrix", qo, qi);
        qi.set(pi_double, vector<double, 3>(0.0, 1.0, 0.0));
        m3d = qi;
        qo = m3d;
        AssertEqual("Quaternion reconstructed from matrix", qo, qi);
    }

}


/*
 * TestCovarianceMatrix
 */
void TestCovarianceMatrix(void) {

    the::collections::array<the::math::vector<float, 3> > relCoords;
    relCoords.add(the::math::vector<float, 3>(3.0f, 0.0f, 0.0f));
    relCoords.add(the::math::vector<float, 3>(-3.0f, 0.0f, 0.0f));
    relCoords.add(the::math::vector<float, 3>(0.0f, 2.0f, 0.0f));
    relCoords.add(the::math::vector<float, 3>(0.0f, -2.0f, 0.0f));
    relCoords.add(the::math::vector<float, 3>(0.0f, 0.0f, 1.0f));
    relCoords.add(the::math::vector<float, 3>(0.0f, 0.0f, -1.0f));

    the::math::matrix<float, 3, the::math::matrix_layout::column_major> covMat;

    AssertNoException("Calculate Covariance Matrix", the::math::calc_covariance_matrix(covMat, relCoords));

    float eva[3];
    the::math::vector<float, 3> eve[3];
    unsigned int evc = covMat.find_eigenvalues(eva, eve, 3);

    AssertEqual("Covariance Matrix has three real eigenvalues", evc, 3u);
    AssertNoException("Sorting Eigenvectors", the::math::sort_eigenvectors(&*eve, eva, evc));

    AssertEqual("Smallest Eigenvalue correct", eva[0], 0.33333334f); // sqrt(1) / 3 <= variance / dimensionality
    AssertEqual("Second Eigenvalue correct", eva[1], 1.3333334f); // sqrt(2) / 3
    AssertEqual("Largest Eigenvalue correct", eva[2], 3.0f); // sqrt(3) / 3

    AssertTrue("Smallest Eigenvector correct", eve[0].is_parallel(the::math::vector<float, 3>(0.0f, 0.0f, 1.0f)));
    AssertTrue("Second Eigenvector correct", eve[1].is_parallel(the::math::vector<float, 3>(0.0f, 1.0f, 0.0f)));
    AssertTrue("Largest Eigenvector correct", eve[2].is_parallel(the::math::vector<float, 3>(1.0f, 0.0f, 0.0f)));

}
