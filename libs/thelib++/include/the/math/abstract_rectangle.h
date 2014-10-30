/*
 * the/math/abstract_rectangle.h
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
 * AbstractRectangle.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Mueller. Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_ABSTRACT_RECTANGLE_H_INCLUDED
#define THE_MATH_ABSTRACT_RECTANGLE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"
#include "the/assert.h"
#include "the/math/size.h"
#include "the/math/functions.h"
#include "the/memory.h"
#include "the/math/point.h"
#include "the/stack_trace.h"
#include "the/types.h"
#include "the/utils.h"


namespace the {
namespace math {

    /**
     * This class represents a rectangle. The origin is located in the 
     * left/bottom corner of the rectangle.
     *
     * The rectangle can be instantiated for different types builtin number
     * types T and storage classes S. For a rectangle that holds its own 
     * corner points, use T[4] as storage class (this is the default). For a
     * rectangle that does not own the storage (we call this "shallow 
     * rectangle"), use T * for S.
     */
    template<class T, class S = T[4]> class abstract_rectangle {

    public:

        /** A bitmask representing all rectangle borders. */
        static const uint32_t border_all;

        /** A bitmask representing the bottom border of the rectangle. */
        static const uint32_t border_bottom;

        /** A bitmask representing the left border of the rectangle. */
        static const uint32_t border_left;

        /** A bitmask representing the right border of the rectangle. */
        static const uint32_t border_right;

        /** A bitmask representing the top border of the rectangle. */
        static const uint32_t border_top;

        /** Dtor. */
        ~abstract_rectangle(void);

        /**
         * Answer the area covered by the rectangle.
         *
         * @return The area covered by the rectangle.
         */
        inline T area(void) const {
            THE_STACK_TRACE;
            return (this->width() * this->height());
        }

        /**
         * Answer the aspect ratio of the rectangle.
         *
         * @return The aspect ratio of the rectangle.
         */
        inline double aspect_ratio(void) const {
            THE_STACK_TRACE;
            if (is_equal<double>(this->height(), 0.0)) {
                return 0.0;
            }
            return static_cast<double>(this->width()) 
                / static_cast<double>(this->height());
        }

        /**
         * Provide direct access to the y-coordinate of the left/bottom point.
         *
         * @return A reference to the y-coordinate of the left/bottom point.
         */
        inline const T& bottom(void) const {
            THE_STACK_TRACE;
            return this->bounds[IDx_BOTTOM];
        }

        /** 
         * Answer the center point of the rectangle.
         *
         * @return The center point of the rectangle.
         */
        point<T, 2> calc_center(void) const;

        /**
         * Answer whether the point 'point' lies within the rectangle.
         *
         * @param point         The point to be tested.
         * @param includeBorder An arbitrary combination of the BORDER_LEFT, 
         *                      BORDER_BOTTOM, BORDER_RIGHT and BORDER_TOP 
         *                      bitmasks. If the border bit is set, points lying 
         *                      on the respective border are regarded as in the
         *                      rectangle, otherwise they are out. Defaults to
         *                      zero, i. e. no border is included.
         *
         * @return True if the point lies within the rectangle, false otherwise.
         */
        template<class Sp>
        bool contains(const abstract_point<T, 2, Sp>& point, 
            const uint32_t includeBorder = 0) const;

        /**
         * Answer the height of the rectangle.
         *
         * @return The height of the rectangle.
         */
        inline T height(void) const {
            THE_STACK_TRACE;
            return (this->bounds[IDx_TOP] > this->bounds[IDx_BOTTOM])
                ? (this->bounds[IDx_TOP] - this->bounds[IDx_BOTTOM])
                : (this->bounds[IDx_BOTTOM] - this->bounds[IDx_TOP]);
        }

        /**
         * Answer the width of the rectangle.
         *
         * @return The width of the rectangle.
         */
        inline T width(void) const {
            THE_STACK_TRACE;
            return (this->bounds[IDx_RIGHT] > this->bounds[IDx_LEFT])
                ? (this->bounds[IDx_RIGHT] - this->bounds[IDx_LEFT])
                : (this->bounds[IDx_LEFT] - this->bounds[IDx_RIGHT]);
        }

        /**
         * Ensures that the size of the rectangle is positive by
         * swapping the left/right and/or top/bottom sides, if they
         * are in the wrong order.
         */
        void enforce_positive_size(void);

        /**
         * Answer the y-coordinate of the left/bottom point.
         *
         * @return The y-coordinate of the left/bottom point.
         */
        inline const T& get_bottom(void) const {
            THE_STACK_TRACE;
            return this->bounds[IDx_BOTTOM];
        }

        /**
         * Answer the x-coordinate of the left/bottom point.
         *
         * @return The x-coordinate of the left/bottom point.
         */
        inline const T& get_left(void) const {
            THE_STACK_TRACE;
            return this->bounds[IDx_LEFT];
        }

        /**
         * Answer the left/bottom point of the rectangle.
         *
         * @return The left/bottom point.
         */
        inline point<T, 2> get_left_bottom(void) const {
            THE_STACK_TRACE;
            return point<T, 2>(this->bounds[IDx_LEFT], 
                this->bounds[IDx_BOTTOM]);
        }

        /**
         * Answer the left/top point of the rectangle.
         *
         * @return The left/top point.
         */
        inline point<T, 2> get_left_top(void) const {
            THE_STACK_TRACE;
            return point<T, 2>(this->bounds[IDx_LEFT], 
                this->bounds[IDx_BOTTOM]);
        }

        /** 
         * Answer the rectangle origin (left/bottom).
         *
         * @return The origin point.
         */
        inline point<T, 2> get_origin(void) const {
            THE_STACK_TRACE;
            return point<T, 2>(this->bounds[IDx_LEFT], 
                this->bounds[IDx_BOTTOM]);
        }

        /**
         * Answer the y-coordinate of the right/top point.
         *
         * @return The y-coordinate of the right/top point.
         */
        inline const T& get_right(void) const {
            THE_STACK_TRACE;
            return this->bounds[IDx_RIGHT];
        }

        /**
         * Answer the right/bottom point of the rectangle.
         *
         * @return The right/bottom point of the rectangle.
         */
        inline point<T, 2> get_right_bottom(void) const {
            THE_STACK_TRACE;
            return point<T, 2>(this->bounds[IDx_RIGHT], 
                this->bounds[IDx_BOTTOM]);
        }

        /**
         * Answer the right/top point of the rectangle.
         *
         * @return The right/top point.
         */
        inline point<T, 2> get_right_top(void) const {
            THE_STACK_TRACE;
            return point<T, 2>(this->bounds[IDx_RIGHT], 
                this->bounds[IDx_TOP]);
        }

        /**
         * Answer the dimensions of the rectangle.
         *
         * @return The dimensions of the rectangle.
         */
        inline size<T, 2> get_size(void) const {
            THE_STACK_TRACE;
            return size<T, 2>(this->width(), this->height());
        }

        /**
         * Answer the y-coordinate of the right/top point.
         *
         * @return The y-coordinate of the right/top point.
         */
        inline const T& get_top(void) const {
            THE_STACK_TRACE;
            return this->bounds[IDx_TOP];
        }

        /**
         * Increases the size of the rectangle to include the given point. 
         * Implicitly calls 'enforce_positive_size'.
         *
         * @param p The point to be included.
         */
        template<class Tp, class Sp>
        inline void grow_to_point(const abstract_point<Tp, 2, Sp>& p) {
            THE_STACK_TRACE;
            this->grow_to_point(p.x(), p.y());
        }

        /**
         * Increases the size of the rectangle to include the given point.
         * Implicitly calls 'enforce_positive_size'.
         *
         * @param x The x coordinate of the point to be included.
         * @param y The y coordinate of the point to be included.
         */
        template<class Tp>
        void grow_to_point(const Tp& x, const Tp& y);

        /**
         * set this rectangle to the intersection of itself and 'rect'.
         *
         * @param rect The rectangle to build the intersection with.
         *
         * @return true, if there is an intersection, false otherwise, i. e.
         *         if the rectangle is an empty one after that.
         */
        template<class Sp> bool intersect(const abstract_rectangle<T, Sp>& rect);

        ///**
        // * Answer the intersection of this rectangle and 'rect'.
        // *
        // * @param rect The rectangle to build the intersection with.
        // *
        // * @return The intersection rectangle.
        // */
        //template<class Sp> inline abstract_rectangle<T, T[4]> intersection(
        //        const abstract_rectangle<T, Sp>& rect) const {
        //    abstract_rectangle<T, T[4]> retval = *this;
        //    retval.intersect(rect);
        //    return retval;
        //}

        /**
         * Answer whether the rectangle has no area.
         *
         * @return true, if the rectangle has no area, false otherwise.
         */
        inline bool is_empty(void) const {
            THE_STACK_TRACE;
            return (is_equal<T>(this->bounds[IDx_LEFT], this->bounds[IDx_RIGHT])
                && is_equal<T>(this->bounds[IDx_BOTTOM], this->bounds[IDx_TOP]));
        }

        /**
         * Provide direct access to the x-coordinate of the left/bottom point.
         *
         * @return A reference to the x-coordinate of the left/bottom point.
         */
        inline const T& left(void) const {
            THE_STACK_TRACE;
            return this->bounds[IDx_LEFT];
        }

        /**
         * move the rectangle.
         *
         * @param dx The offset in x-direction.
         * @param dy The offset in y-direction.
         */
        inline void move(const T& dx, const T& dy) {
            THE_STACK_TRACE;
            this->bounds[IDx_BOTTOM] += dy;
            this->bounds[IDx_LEFT] += dx;
            this->bounds[IDx_RIGHT] += dx;
            this->bounds[IDx_TOP] += dy;
        }

        /**
         * move the bottom side of the rectangle (keeping its size) to the
         * specified location.
         *
         * @param bottom The new bottom border.
         */
        inline void move_bottom_to(const T& bottom) {
            THE_STACK_TRACE;
            this->move(static_cast<T>(0), bottom - this->bottom());
        }

        /**
         * move the left side of the rectangle (keeping its size) to the
         * specified location.
         *
         * @param left The new left border.
         */
        inline void move_left_to(const T& left) {
            THE_STACK_TRACE;
            this->move(left - this->left(), static_cast<T>(0));
        }

        /**
         * move the right side of the rectangle (keeping its size) to the
         * specified location.
         *
         * @param right The new right border.
         */
        inline void move_right_to(const T& right) {
            THE_STACK_TRACE;
            this->move(right - this->right(), static_cast<T>(0));
        }

        /**
         * move the left/bottom side of the rectangle (keeping its size) to the 
         * specified location.
         *
         * Implicitly calls 'enforce_positive_size'.
         *
         * @param left   The new left border.
         * @param bottom The new bottom border.
         */
        inline void move_to(const T& left, const T& bottom) {
            THE_STACK_TRACE;
            this->enforce_positive_size();
            this->move(left - this->left(), bottom - this->bottom());
        }

        /**
         * move the top side of the rectangle (keeping its size) to the
         * specified location.
         *
         * @param top The new top border.
         */
        inline void move_top_to(const T& top) {
            THE_STACK_TRACE;
            this->move(static_cast<T>(0), top - this->top());
        }

        /**
         * Directly access the internal bounds of the rectangle.
         *
         * @return A pointer to the rectangle bounds.
         */
        inline const T *peek_bounds(void) const {
            THE_STACK_TRACE;
            return this->bounds;
        }

        /**
         * Directly access the internal bounds of the rectangle.
         *
         * @return A pointer to the rectangle bounds.
         */
        inline T *peek_bounds(void) {
            THE_STACK_TRACE;
            return this->bounds;
        }

        /**
         * Provide direct access to the x-coordinate of the right/top point.
         *
         * @return A reference to the x-coordinate of the right/top point.
         */
        inline const T& right(void) const {
            THE_STACK_TRACE;
            return this->bounds[IDx_RIGHT];
        }

        /**
         * set new rectangle bounds.
         *
         * @param left   The x-coordinate of the left/bottom point.
         * @param bottom The y-coordinate of the left/bottom point.
         * @param right  The x-coordinate of the right/top point.
         * @param top    The y-coordinate of the right/top point.
         */
        inline void set(const T& left, const T& bottom, const T& right, 
                const T& top) {
            THE_STACK_TRACE;
            this->bounds[IDx_BOTTOM] = bottom;
            this->bounds[IDx_LEFT] = left;
            this->bounds[IDx_RIGHT] = right;
            this->bounds[IDx_TOP] = top;
        }

        /**
         * Change the y-coordinate of the left/bottom point.
         *
         * @param bottom The new y-coordinate of the left/bottom point.
         */
        inline void set_bottom(const T& bottom) {
            THE_STACK_TRACE;
            this->bounds[IDx_BOTTOM] = bottom;
        }

        /**
         * set new rectangle bounds.
         *
         * @param left   The x-coordinate of the left/bottom point.
         * @param bottom The y-coordinate of the left/bottom point.
         * @param right  The x-coordinate of the right/top point.
         * @param top    The y-coordinate of the right/top point.
         */
        inline void set_from_bounds(const T& left, const T& bottom, 
                const T& right, const T& top) {
            THE_STACK_TRACE;
            this->set(left, bottom, right, top);
        }

        /**
         * set new rectangle bounds.
         *
         * @param left   The x-coordinate of the left/bottom point.
         * @param bottom The y-coordinate of the left/bottom point.
         * @param width  The width of the rectangle.
         * @param height The height of the rectangle.
         */
        inline void set_from_size(const T& left, const T& bottom, 
                const T& width, const T& height) {
            THE_STACK_TRACE;
            this->bounds[IDx_BOTTOM] = bottom;
            this->bounds[IDx_LEFT] = left;
            this->bounds[IDx_RIGHT] = left + width;
            this->bounds[IDx_TOP] = bottom +height;
        }

        /**
         * set a new height.
         *
         * @param height The new height of the rectangle.
         */
        inline void set_height(const T& height) {
            THE_STACK_TRACE;
            this->bounds[IDx_TOP] = this->bounds[IDx_BOTTOM] + height;
        }

        /**
         * Change the x-coordinate of the left/bottom point.
         *
         * @param left The new x-coordinate of the left/bottom point.
         */
        inline void set_left(const T& left) {
            THE_STACK_TRACE;
            this->bounds[IDx_LEFT] = left;
        }

        /**
         * Make the rectangle an empty rectangle a (0, 0).
         */
        inline void set_null(void) {
            THE_STACK_TRACE;
            this->set(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
                static_cast<T>(0));
        }

        /**
         * Change the y-coordinate of the right/top point.
         *
         * @param right The new y-coordinate of the right/top point.
         */
        inline void set_right(const T& right) {
            THE_STACK_TRACE;
            this->bounds[IDx_RIGHT] = right;
        }

        /**
         * set a new size of the rectangle.
         *
         * @param size The new rectangle dimensions.
         */
        template<class Tp, class Sp>
        inline void set_size(const abstract_size<Tp, 2, Sp>& size) {
            THE_STACK_TRACE;
            this->set_width(static_cast<T>(size.get_width()));
            this->set_height(static_cast<T>(size.get_height()));
        }

        /**
         * Change the y-coordinate of the right/top point.
         *
         * @param top The new y-coordinate of the right/top point.
         */
        inline void set_top(const T& top) {
            THE_STACK_TRACE;
            this->bounds[IDx_TOP] = top;
        }

        /**
         * set a new width.
         *
         * @param width The new width of the rectangle.
         */
        inline void set_width(const T& width) {
            THE_STACK_TRACE;
            this->bounds[IDx_RIGHT] = this->bounds[IDx_LEFT] + width;
        }

        /**
         * swap the left and the right x-coordinate.
         */
        inline void swap_left_right(void) {
            THE_STACK_TRACE;
            swap(this->bounds[IDx_LEFT], this->bounds[IDx_RIGHT]);
        }

        /**
         * swap the top and the bottom y-coordinate.
         */
        inline void swap_top_bottom(void) {
            THE_STACK_TRACE;
            swap(this->bounds[IDx_TOP], this->bounds[IDx_BOTTOM]);
        }

        /**
         * Provide direct access to the y-coordinate of the right/top point.
         *
         * @return A reference to the y-coordinate of the right/top point.
         */
        inline const T& top(void) const {
            THE_STACK_TRACE;
            return this->bounds[IDx_TOP];
        }

        /**
         * set this rectangle to the bounding rectangle of itself and 'rect'.
         *
         * @param rect The rectangle to compute the union with.
         */
        template<class Sp> void unite(const abstract_rectangle<T, Sp>& rect);

        /**
         * Assigment operator. This operator never creates an alias, even for
         * shallow rectangles!
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        abstract_rectangle& operator =(const abstract_rectangle& rhs);

        /**
         * Assigment operator. This operator never creates an alias, even for
         * shallow rectangles!
         *
         * This assignment allows for arbitrary rectangle to rectangle
         * conversions.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        template<class Tp, class Sp>
        abstract_rectangle& operator =(const abstract_rectangle<Tp, Sp>& rhs);

        /**
         * Test for equality. The operator uses the IsEqual<T, S> function for each
         * member.
         *
         * @param rhs The right hand side operand.
         *
         * @param true, if 'rhs' and this vector are equal, false otherwise.
         */
        bool operator ==(const abstract_rectangle& rhs) const;

        /**
         * Test for inequality. The operator uses the IsEqual<T, S> function for each
         * member.
         *
         * @param rhs The right hand side operand.
         *
         * @param true, if 'rhs' and this vector are not equal, false otherwise.
         */
        inline bool operator !=(const abstract_rectangle& rhs) const {
            THE_STACK_TRACE;
            return !(*this == rhs);
        }

    protected:

        /** The index of the bottom coordinate in 'bounds'. */
        static const uintptr_t IDx_BOTTOM;

        /** The index of the right coordinate in 'bounds'. */
        static const uintptr_t IDx_RIGHT;

        /** The index of the left coordinate in 'bounds'. */
        static const uintptr_t IDx_LEFT;

        /** The index of the top coordinate in 'bounds'. */
        static const uintptr_t IDx_TOP;

        /**
         * Forbidden default ctor. This does nothing.
         */
        inline abstract_rectangle(void) {
            THE_STACK_TRACE;
        }

        /** 
         * The bounds of the rectangle in following order: left, bottom, 
         * right, top. 
         */
        S bounds;
    };


    /*
     * the::math::abstract_rectangle<T, S>::border_all
     */
    template<class T, class S>
    const uint32_t abstract_rectangle<T, S>::border_all
        = abstract_rectangle<T, S>::border_left
        | abstract_rectangle<T, S>::border_bottom
        | abstract_rectangle<T, S>::border_right
        | abstract_rectangle<T, S>::border_top;


    /*
     * the::math::abstract_rectangle<T, S>::border_bottom
     */
    template<class T, class S>
    const uint32_t abstract_rectangle<T, S>::border_bottom 
        = 1 << abstract_rectangle<T, S>::IDx_BOTTOM;

    /*
     * the::math::abstract_rectangle<T, S>::border_left
     */
    template<class T, class S>
    const uint32_t abstract_rectangle<T, S>::border_left 
        = 1 << abstract_rectangle<T, S>::IDx_LEFT;


    /*
     * the::math::abstract_rectangle<T, S>::border_right
     */
    template<class T, class S>
    const uint32_t abstract_rectangle<T, S>::border_right 
        = 1 << abstract_rectangle<T, S>::IDx_RIGHT;


    /*
     * the::math::abstract_rectangle<T, S>::border_top
     */
    template<class T, class S>
    const uint32_t abstract_rectangle<T, S>::border_top
        = 1 << abstract_rectangle<T, S>::IDx_TOP;


    /*
     * the::math::abstract_rectangle<T, S>::~abstract_rectangle
     */
    template<class T, class S>
    abstract_rectangle<T, S>::~abstract_rectangle(void) {
        THE_STACK_TRACE;
    }


    /*
     * the::math::abstract_rectangle<T, S>::calc_center
     */
    template<class T, class S> 
    point<T, 2> abstract_rectangle<T, S>::calc_center(void) const {
        THE_STACK_TRACE;
        return point<T, 2>(
            this->bounds[IDx_LEFT] + this->width() / static_cast<T>(2),
            this->bounds[IDx_BOTTOM] + this->height() / static_cast<T>(2));
    }


    /*
     * abstract_rectangle<T, S>::contains
     */
    template<class T, class S> 
    template<class Sp>
    bool abstract_rectangle<T, S>::contains(const abstract_point<T, 2, Sp>& point,
            const uint32_t includeBorder) const {
        THE_STACK_TRACE;

        if ((point.x() < this->bounds[IDx_LEFT])
                || ((((includeBorder & border_left) == 0))
                && (point.x() == this->bounds[IDx_LEFT]))) {
            /* Point is left of rectangle. */
            return false;
        }

        if ((point.y() < this->bounds[IDx_BOTTOM])
                || ((((includeBorder & border_bottom) == 0))
                && (point.y() == this->bounds[IDx_BOTTOM]))) {
            /* Point is below rectangle. */
            return false;
        }

        if ((point.x() > this->bounds[IDx_RIGHT])
                || ((((includeBorder & border_right) == 0))
                && (point.x() == this->bounds[IDx_RIGHT]))) {
            /* Point is right of rectangle. */
            return false;
        }

        if ((point.y() > this->bounds[IDx_TOP])
                || ((((includeBorder & border_top) == 0))
                && (point.y() == this->bounds[IDx_TOP]))) {
            /* Point is above rectangle. */
            return false;
        }

        return true;
    }


    /*
     * the::math::abstract_rectangle<T, S>::enforce_positive_size
     */
    template<class T, class S> 
    void abstract_rectangle<T, S>::enforce_positive_size(void) {
        THE_STACK_TRACE;
        if (this->bounds[IDx_BOTTOM] > this->bounds[IDx_TOP]) {
            swap(this->bounds[IDx_BOTTOM], this->bounds[IDx_TOP]);
        }

        if (this->bounds[IDx_LEFT] > this->bounds[IDx_RIGHT]) {
            swap(this->bounds[IDx_LEFT], this->bounds[IDx_RIGHT]);
        }
    }


    /*
     * the::math::abstract_rectangle<T>::grow_to_point
     */
    template<class T, class S> template<class Tp>
    void abstract_rectangle<T, S>::grow_to_point(const Tp& x, const Tp& y) {
        THE_STACK_TRACE;
        this->enforce_positive_size();
        if (this->bounds[IDx_LEFT] > x) {
            this->bounds[IDx_LEFT] = x;
        }
        if (this->bounds[IDx_BOTTOM] > y) {
            this->bounds[IDx_BOTTOM] = y;
        }
        if (this->bounds[IDx_RIGHT] < x) {
            this->bounds[IDx_RIGHT] = x;
        }
        if (this->bounds[IDx_TOP] < y) {
            this->bounds[IDx_TOP] = y;
        }
    }


    /*
     * the::math::abstract_rectangle<T, S>::intersect
     */
    template<class T, class S>
    template<class Sp>
    bool abstract_rectangle<T, S>::intersect(
            const abstract_rectangle<T, Sp>& rect) {
        THE_STACK_TRACE;
        T bottom = maximum(this->bounds[IDx_BOTTOM], rect.bottom());
        T left = maximum(this->bounds[IDx_LEFT], rect.left());
        T right = minimum(this->bounds[IDx_RIGHT], rect.right());
        T top = minimum(this->bounds[IDx_TOP], rect.top());
        
        if ((top < bottom) || (right < left)) {
            this->set_null();
            return false;

        } else {
            this->set(left, bottom, right, top);
            return true;
        }
    }


    /*
     * the::math::abstract_rectangle<T, S>::union
     */
    template<class T, class S>
    template<class Sp>
    void abstract_rectangle<T, S>::unite(const abstract_rectangle<T, Sp>& rect) {
        THE_STACK_TRACE;
        T rectbottom, rectleft, rectright, recttop;
        
        if (rect.bottom() < rect.top()) {
            rectbottom = rect.bottom();
            recttop = rect.top();
        } else {
            rectbottom = rect.top();
            recttop = rect.bottom();
        }

        if (rect.left() < rect.right()) {
            rectleft = rect.left();
            rectright = rect.right();
        } else {
            rectleft = rect.right();
            rectright = rect.left();
        }

        this->enforce_positive_size();

        THE_ASSERT(this->bounds[IDx_LEFT] <= this->bounds[IDx_RIGHT]);
        THE_ASSERT(this->bounds[IDx_BOTTOM] <= this->bounds[IDx_TOP]);
        THE_ASSERT(rectleft <= rectright);
        THE_ASSERT(rectbottom <= recttop);

        if (rectleft < this->bounds[IDx_LEFT]) {
            this->bounds[IDx_LEFT] = rectleft;
        }

        if (rectright > this->bounds[IDx_RIGHT]) {
            this->bounds[IDx_RIGHT] = rectright;
        }

        if (recttop > this->bounds[IDx_TOP]) {
            this->bounds[IDx_TOP] = recttop;
        }

        if (rectbottom < this->bounds[IDx_BOTTOM]) {
            this->bounds[IDx_BOTTOM] = rectbottom;
        }
    }


    /*
     * the::math::abstract_rectangle<T, S>::operator =
     */
    template<class T, class S>
    abstract_rectangle<T, S>& abstract_rectangle<T, S>::operator =(
            const abstract_rectangle& rhs) {
        THE_STACK_TRACE;

        if (this != &rhs) {
            ::memcpy(this->bounds, rhs.bounds, 4 * sizeof(T));
        }

        return *this;
    }


    /*
     * the::math::abstract_rectangle<T, S>::operator =
     */
    template<class T, class S>
    template<class Tp, class Sp>
    abstract_rectangle<T, S>& abstract_rectangle<T, S>::operator =(
            const abstract_rectangle<Tp, Sp>& rhs) {
        THE_STACK_TRACE;

        if (static_cast<void *>(this) != static_cast<const void *>(&rhs)) {
            this->bounds[IDx_BOTTOM] = static_cast<T>(rhs.bottom());
            this->bounds[IDx_LEFT] = static_cast<T>(rhs.left());
            this->bounds[IDx_RIGHT] = static_cast<T>(rhs.right());
            this->bounds[IDx_TOP] = static_cast<T>(rhs.top());
        }

        return *this;
    }


    /*
     * the::math::abstract_rectangle<T, S>::operator ==
     */
    template<class T, class S> 
    bool abstract_rectangle<T, S>::operator ==(
            const abstract_rectangle& rhs) const {
        THE_STACK_TRACE;
        return (is_equal<T>(this->bounds[IDx_BOTTOM], rhs.bounds[IDx_BOTTOM])
            && is_equal<T>(this->bounds[IDx_LEFT], rhs.bounds[IDx_LEFT]) 
            && is_equal<T>(this->bounds[IDx_RIGHT], rhs.bounds[IDx_RIGHT]) 
            && is_equal<T>(this->bounds[IDx_TOP], rhs.bounds[IDx_TOP]));
    }


    /*
     * the::math::abstract_rectangle<T, S>::IDx_BOTTOM
     */
    template<class T, class S> 
    const uintptr_t abstract_rectangle<T, S>::IDx_BOTTOM = 1;


    /*
     * the::math::abstract_rectangle<T, S>::IDx_RIGHT
     */
    template<class T, class S> 
    const uintptr_t abstract_rectangle<T, S>::IDx_RIGHT = 2;


    /*
     * the::math::abstract_rectangle<T, S>::IDx_LEFT
     */
    template<class T, class S> 
    const uintptr_t abstract_rectangle<T, S>::IDx_LEFT = 0;


    /*
     * the::math::abstract_rectangle<T, S>::IDx_TOP
     */
    template<class T, class S> 
    const uintptr_t abstract_rectangle<T, S>::IDx_TOP = 3;

} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_ABSTRACT_RECTANGLE_H_INCLUDED */
