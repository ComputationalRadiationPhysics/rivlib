/*
 * testvector.h  14.09.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */
#include "stdafx.h"
#include "testhelper.h"

#include "the/math/shallow_vector.h"
#include "the/math/vector.h"


void Testvector(void) {
    typedef the::math::vector<float, 2> vector2D;
    typedef the::math::shallow_vector<float, 2> shallow_vector2D;

    float sv1data[2];
    float sv2data[2];

    vector2D v1;
    shallow_vector2D sv1(sv1data);
    shallow_vector2D sv2(sv2data);
    
    ::AssertEqual("v1[0] == 0", v1[0], 0.0f);
    ::AssertEqual("v1[1] == 0", v1[1], 0.0f);
    ::AssertEqual("v1.get_x() == 0", v1.get_x(), 0.0f);
    ::AssertEqual("v1.get_y() == 0", v1.get_y(), 0.0f);
    ::AssertEqual("v1.X() == 0", v1.x(), 0.0f);
    ::AssertEqual("v1.Y() == 0", v1.y(), 0.0f);

    ::AssertEqual("v1.length() == 0", v1.length(), 0.0f);
    ::AssertEqual("v1.max_norm(() == 0", v1.max_norm(), 0.0f);
    ::AssertTrue("v1.is_null()", v1.is_null());

    sv1 = v1;
    ::AssertTrue("Deep to shallow assignment", (v1 == sv1));
    ::AssertEqual("sv1[0] == 0", sv1[0], 0.0f);
    ::AssertEqual("sv1[1] == 0", sv1[1], 0.0f);
    ::AssertEqual("sv1.get_x() == 0", sv1.get_x(), 0.0f);
    ::AssertEqual("sv1.get_y() == 0", sv1.get_y(), 0.0f);
    ::AssertEqual("sv1.X() == 0", sv1.x(), 0.0f);
    ::AssertEqual("sv1.Y() == 0", sv1.y(), 0.0f);

    sv2 = sv1;
    ::AssertEqual("Shallow to shallow assignment", sv1, sv2);
    ::AssertEqual("sv2[0] == 0", sv2[0], 0.0f);
    ::AssertEqual("sv2[1] == 0", sv2[1], 0.0f);
    ::AssertEqual("sv2.get_x() == 0", sv2.get_x(), 0.0f);
    ::AssertEqual("sv2.get_y() == 0", sv2.get_y(), 0.0f);
    ::AssertEqual("sv2.X() == 0", sv2.x(), 0.0f);
    ::AssertEqual("sv1.Y() == 0", sv2.y(), 0.0f);

    v1.set_x(2.0f);
    ::AssertEqual("v1[0] == 2", v1[0], 2.0f);
    ::AssertEqual("v1[1] == 0", v1[1], 0.0f);
    ::AssertEqual("v1.get_x() == 2", v1.get_x(), 2.0f);
    ::AssertEqual("v1.get_y() == 0", v1.get_y(), 0.0f);
    ::AssertEqual("v1.X() == 2", v1.x(), 2.0f);
    ::AssertEqual("v1.Y() == 0", v1.y(), 0.0f);

    ::AssertTrue("v1.length() == 2", the::math::is_equal(v1.length(), 2.0f));
    ::AssertEqual("v1.max_norm(() == 3", v1.max_norm(), 2.0f);
    ::AssertFalse("!v1.is_null()", v1.is_null());

    v1.set_y(3.0f);
    ::AssertEqual("v1[0] == 2", v1[0], 2.0f);
    ::AssertEqual("v1[1] == 3", v1[1], 3.0f);
    ::AssertEqual("v1.get_x() == 2", v1.get_x(), 2.0f);
    ::AssertEqual("v1.get_y() == 3", v1.get_y(), 3.0f);
    ::AssertEqual("v1.X() == 2", v1.x(), 2.0f);
    ::AssertEqual("v1.Y() == 3", v1.y(), 3.0f);
    
    ::AssertTrue("v1.length()", the::math::is_equal(v1.length(), ::sqrtf(2.0f * 2.0f + 3.0f * 3.0f)));
    ::AssertEqual("v1.max_norm(() == 3", v1.max_norm(), 3.0f);
    ::AssertFalse("!v1.is_null()", v1.is_null());
}
