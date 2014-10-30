/*
 * test_size_and_rect.h
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
 * dimandrect.cpp
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */
#include "stdafx.h"
#include "test_size_and_rect.h"
#include "testhelper.h"
//
#include "the/math/size.h"
#include "the/math/rectangle.h"
#include "the/math/shallow_rectangle.h"


void test_size2D(void) {
    typedef the::math::size<float, 2> extent2DF;
    typedef the::math::size<int, 2> extent2DI;

    std::cout << std::endl << "extent2D ..." << std::endl;

    extent2DF fdim1;
    extent2DF fdim2(1.0f, 2.0f);
    extent2DF fdim3(fdim2);
    extent2DI idim1(fdim2);

    ::AssertEqual("Zero extent ctor.", fdim1[0], 0.0f);
    ::AssertEqual("Zero extent ctor.", fdim1[1], 0.0f);
    ::AssertEqual("GetWidth().", fdim1.get_width(), 0.0f);
    ::AssertEqual("GetHeight().", fdim1.get_height(), 0.0f);
    ::AssertEqual("Width/height ctor.", fdim2[0], 1.0f);
    ::AssertEqual("Width/height ctor.", fdim2[1], 2.0f);
    ::AssertEqual("GetWidth().", fdim2.get_width(), 1.0f);
    ::AssertEqual("GetHeight().", fdim2.get_height(), 2.0f);
    ::AssertEqual("Width().", fdim2.width(), 1.0f);
    ::AssertEqual("Height().", fdim2.height(), 2.0f);
    ::AssertEqual("Copy ctor.", fdim3[0], fdim2[0]);
    ::AssertEqual("Copy ctor.", fdim3[1], fdim2[1]);
    ::AssertEqual("Conversion copy ctor.", idim1[0], int(fdim2[0]));
    ::AssertEqual("Conversion copy ctor.", idim1[1], int(fdim2[1]));

    fdim1 = fdim2;
    ::AssertEqual("Assignment.", fdim1[0], fdim2[0]);
    ::AssertEqual("Assignment.", fdim1[1], fdim2[1]);

    fdim1.set_width(10.0);
    ::AssertEqual("SetWidth", fdim1[0], 10.0f);
    fdim1.set_height(11.0);
    ::AssertEqual("SetHeight", fdim1[1], 11.0f);

    idim1 = fdim1;
    ::AssertEqual("Conversion assignment.", idim1[0], int(fdim1[0]));
    ::AssertEqual("Conversion assignment.", idim1[1], int(fdim1[1]));

    ::AssertTrue("Equality", fdim2 == fdim3);
    ::AssertTrue("Conversion equality", idim1 == fdim1);

    ::AssertFalse("Inequality", fdim2 != fdim3);
    ::AssertFalse("Conversion inequality", idim1 != fdim1);

}


void test_size3D(void) {
    typedef the::math::size<float, 3> extent3DF;
    typedef the::math::size<int, 3> extent3DI;

    std::cout << std::endl << "extent3D ..." << std::endl;

    extent3DF fdim1;
    extent3DF fdim2(1.0f, 2.0f, 3.0f);
    extent3DF fdim3(fdim2);
    extent3DI idim1(fdim2);

    ::AssertEqual("Zero extent ctor.", fdim1[0], 0.0f);
    ::AssertEqual("Zero extent ctor.", fdim1[1], 0.0f);
    ::AssertEqual("Zero extent ctor.", fdim1[2], 0.0f);
    ::AssertEqual("GetWidth().", fdim1.get_width(), 0.0f);
    ::AssertEqual("GetHeight().", fdim1.get_height(), 0.0f);
    ::AssertEqual("GetDepth().", fdim1.get_depth(), 0.0f);
    ::AssertEqual("Width/height/depth ctor.", fdim2[0], 1.0f);
    ::AssertEqual("Width/height/depth ctor.", fdim2[1], 2.0f);
    ::AssertEqual("Width/height/depth ctor.", fdim2[2], 3.0f);
    ::AssertEqual("GetWidth().", fdim2.get_width(), 1.0f);
    ::AssertEqual("GetHeight().", fdim2.get_height(), 2.0f);
    ::AssertEqual("GetDepth().", fdim2.get_depth(), 3.0f);
    ::AssertEqual("Width().", fdim2.width(), 1.0f);
    ::AssertEqual("Height().", fdim2.height(), 2.0f);
    ::AssertEqual("Depth().", fdim2.depth(), 3.0f);
    ::AssertEqual("Copy ctor.", fdim3[0], fdim2[0]);
    ::AssertEqual("Copy ctor.", fdim3[1], fdim2[1]);
    ::AssertEqual("Copy ctor.", fdim3[2], fdim2[2]);
    ::AssertEqual("Conversion copy ctor.", idim1[0], int(fdim2[0]));
    ::AssertEqual("Conversion copy ctor.", idim1[1], int(fdim2[1]));
    ::AssertEqual("Conversion copy ctor.", idim1[2], int(fdim2[2]));

    fdim1 = fdim2;
    ::AssertEqual("Assignment.", fdim1[0], fdim2[0]);
    ::AssertEqual("Assignment.", fdim1[1], fdim2[1]);
    ::AssertEqual("Assignment.", fdim1[2], fdim2[2]);

    fdim1.set_width(10.0);
    ::AssertEqual("SetWidth", fdim1[0], 10.0f);
    fdim1.set_height(11.0);
    ::AssertEqual("SetHeight", fdim1[1], 11.0f);
    fdim1.set_depth(12.0);
    ::AssertEqual("SetDepth", fdim1[2], 12.0f);

    idim1 = fdim1;
    ::AssertEqual("Conversion assignment.", idim1[0], int(fdim1[0]));
    ::AssertEqual("Conversion assignment.", idim1[1], int(fdim1[1]));
    ::AssertEqual("Conversion assignment.", idim1[2], int(fdim1[2]));

    ::AssertTrue("Equality", fdim2 == fdim3);
    ::AssertTrue("Conversion equality", idim1 == fdim1);

    ::AssertFalse("Inequality", fdim2 != fdim3);
    ::AssertFalse("Conversion inequality", idim1 != fdim1);
}


void test_size(void) {
    ::test_size2D();
    ::test_size3D();
}


void test_rectangle(void) {
    typedef the::math::rectangle<float> FloatRectangle;
    typedef the::math::rectangle<int> IntRectangle;
    typedef the::math::shallow_rectangle<int> ShallowIntRectangle;
    typedef the::math::point<int, 2> IntPoint;

    FloatRectangle frect1;
    FloatRectangle frect2(1.0, 1.0, 2.0, 2.0);
    FloatRectangle frect3(frect2);
    IntRectangle irect1(frect2);
    int irect2data[4];
    ::memset(irect2data, 0, sizeof(irect2data));
    ShallowIntRectangle irect2(irect2data);
    ShallowIntRectangle irect3(irect2);

    ::AssertEqual("Default ctor get_left().", frect1.get_left(), 0.0f);
    ::AssertEqual("Default ctor get_bottom().", frect1.get_bottom(), 0.0f);
    ::AssertEqual("Default ctor get_right().", frect1.get_right(), 0.0f);
    ::AssertEqual("Default ctor get_top().", frect1.get_top(), 0.0f);
    ::AssertEqual("Default ctor left().", frect1.left(), 0.0f);
    ::AssertEqual("Default ctor bottom().", frect1.bottom(), 0.0f);
    ::AssertEqual("Default ctor right().", frect1.right(), 0.0f);
    ::AssertEqual("Default ctor top().", frect1.top(), 0.0f);

    ::AssertTrue("IsEmpty", frect1.is_empty());

    ::AssertEqual("Init ctor get_left().", frect2.get_left(), 1.0f);
    ::AssertEqual("Init ctor get_bottom().", frect2.get_bottom(), 1.0f);
    ::AssertEqual("Init ctor get_right().", frect2.get_right(), 2.0f);
    ::AssertEqual("Init ctor get_top().", frect2.get_top(), 2.0f);
    ::AssertEqual("Init ctor left().", frect2.left(), 1.0f);
    ::AssertEqual("Init ctor bottom().", frect2.bottom(), 1.0f);
    ::AssertEqual("Init ctor right().", frect2.right(), 2.0f);
    ::AssertEqual("Init ctor top().", frect2.top(), 2.0f);

    ::AssertEqual("Copy ctor left().", frect3.left(), 1.0f);
    ::AssertEqual("Copy ctor bottom().", frect3.bottom(), 1.0f);
    ::AssertEqual("Copy ctor right().", frect3.right(), 2.0f);
    ::AssertEqual("Copy ctor top().", frect3.top(), 2.0f);

    ::AssertTrue("Equality", frect2 == frect3);
    ::AssertFalse("Inequality", frect2 != frect3);

    ::AssertEqual("Conversion ctor left().", irect1.left(), int(frect2.left()));
    ::AssertEqual("Conversion ctor bottom().", irect1.bottom(), int(frect2.bottom()));
    ::AssertEqual("Conversion ctor right().", irect1.right(), int(frect2.right()));
    ::AssertEqual("Conversion ctor top().", irect1.top(), int(frect2.top()));

    //::AssertTrue("Conversion equality", frect2 == irect1);
    //::AssertFalse("Conversion inequality", frect2 != irect1);

    ::AssertEqual("Shallow default ctor get_left().", irect2.get_left(), 0);
    ::AssertEqual("Shallow default ctor get_bottom().", irect2.get_bottom(), 0);
    ::AssertEqual("Shallow default ctor get_right().", irect2.get_right(), 0);
    ::AssertEqual("Shallow default ctor get_top().", irect2.get_top(), 0);
    ::AssertEqual("Shallow default ctor left().", irect2.left(), 0);
    ::AssertEqual("Shallow default ctor bottom().", irect2.bottom(), 0);
    ::AssertEqual("Shallow default ctor right().", irect2.right(), 0);
    ::AssertEqual("Shallow default ctor top().", irect2.top(), 0);
    
    ::AssertEqual<const int *>("Shallow data pointer.", irect2.peek_bounds(), irect2data);
    ::AssertEqual<const int *>("Shallow aliasing.", irect2.peek_bounds(), irect3.peek_bounds());

    ::AssertTrue("Equality", irect2 == irect3);
    ::AssertFalse("Inequality", irect2 != irect3);

    irect3 = frect3;
    ::AssertEqual("Shallow conversion assignment left().", irect3.left(), int(frect3.left()));
    ::AssertEqual("Shallow conversion assignment bottom().", irect3.bottom(), int(frect3.bottom()));
    ::AssertEqual("Shallow conversion assignment right().", irect3.right(), int(frect3.right()));
    ::AssertEqual("Shallow conversion assignment top().", irect3.top(), int(frect3.top()));

    ::AssertTrue("Shallow assignment aliasing", irect3 == irect2);


    IntRectangle irect4(1, 1, 10, 10);
    ::AssertTrue("Inside on left border.", irect4.contains(IntPoint(1, 1), IntRectangle::border_all));
    ::AssertTrue("Inside on bottom border.", irect4.contains(IntPoint(1, 1), IntRectangle::border_all));
    ::AssertTrue("Inside on right border.", irect4.contains(IntPoint(10, 10), IntRectangle::border_all));
    ::AssertTrue("Inside on top border.", irect4.contains(IntPoint(10, 10), IntRectangle::border_all));

    ::AssertFalse("Outside on left border.", irect4.contains(IntPoint(1, 1)));
    ::AssertFalse("Outside on bottom border.", irect4.contains(IntPoint(1, 1)));
    ::AssertFalse("Outside on right border.", irect4.contains(IntPoint(10, 10)));
    ::AssertFalse("Outside on top border.", irect4.contains(IntPoint(10, 10)));

    ::AssertFalse("Outside left.", irect4.contains(IntPoint(0, 1)));
    ::AssertFalse("Outside bottom.", irect4.contains(IntPoint(1, 0)));
    ::AssertFalse("Outside right.", irect4.contains(IntPoint(11, 10)));
    ::AssertFalse("Outside top.", irect4.contains(IntPoint(10, 11)));

    ::AssertFalse("Outside left including border.", irect4.contains(IntPoint(0, 1), IntRectangle::border_all));
    ::AssertFalse("Outside bottom including border.", irect4.contains(IntPoint(1, 0), IntRectangle::border_all));
    ::AssertFalse("Outside right including border.", irect4.contains(IntPoint(11, 10), IntRectangle::border_all));
    ::AssertFalse("Outside top including border.", irect4.contains(IntPoint(10, 11), IntRectangle::border_all));
}

