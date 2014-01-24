/*
 * include\the\graphics\window.h
 *
 * Copyright (C) 2013 TheLib Team (http://www.thelib.org/license)
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

#ifndef THE_GRAPHICS_WINDOW_H_INCLUDED
#define THE_GRAPHICS_WINDOW_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include <memory>

#include "the/assert.h"
#include "the/stack_trace.h"

#include "the/system/system_string.h"


namespace the {
namespace graphics {

    /**
     * Provides a basic window implementation based on a template that provides
     * the system-dependent parts of it.
     *
     * @tparam T The native window behaviour that implements the basic
     *           system-dependent functionality of the window. This class must
     *           expose the following public members:
     *
     *           typedef ... native_char_type;
     *           typedef ... native_handle_type;
     *           typedef ... native_virtual_key_type;
     *           typedef ... position_type;
     *           typedef ... size_type;
     *
     *           static void one_time_finalise(void);
     *           static void one_time_initialise(void);
     *           static bool process_message(void);
     *
     *           virtual void destroy(void);
     *           virtual void hide(void);
     *           virtual void invalidate(void);
     *           virtual void move(const position_type x,
     *               const position_type y);
     *           virtual void resize(const size_type width,
     *               const size_type height);
     *           virtual void show(void);
     *
     *           Additionally, the class must expose the following
     *           protected members (the methods must be invoked when a
     *           respective event occurred):
     *
     *           virtual void initialise(const system::system_char *title,
     *               const position_type x, const position_type y,
     *               const size_type width, const size_type height);
     *           virtual void on_close(void) throw();
     *           virtual void on_char(const native_char_type c) throw();
     *           virtual void on_key_down(
     *               const native_virtual_key_type& key) throw();
     *           virtual void on_key_up(
     *               const native_virtual_key_type& key) throw();
     *           virtual void on_mouse_down(const int button) throw();
     *           virtual void on_mouse_move(const position_type x,
     *              const position_type y) throw();
     *           virtual void on_mouse_up(const int button) throw();
     *           virtual void on_resized(const size_type width,
     *               const size_type height) throw();
     *
     *           native_handle_type hWnd;
     */
    template<class T> class window : public T {

    public:

        /** The native character type. */
        typedef typename T::native_char_type native_char_type;

        /** The type of the native window handle. */
        typedef typename T::native_handle_type native_handle_type;

        /** The type for specifying positions of the window. */
        typedef typename T::position_type position_type;

        /** The type for specifying the dimensions of a window. */
        typedef typename T::size_type size_type;

        /** Shared pointer to a window. */
        typedef std::shared_ptr<window> shared_pointer_type;

        /**
         * Creates and initialises a new window.
         *
         * @param title  The title of the window.
         * @param x      The abscissa of the initial position of the window.
         * @param y      The ordinate of the initial position of the window.
         * @param width  The initial width of the window.
         * @param height The initial height of the window.
         *
         * @return A pointer to the new window.
         *
         * @throws the::invalid_operation_exception In case the one-time
         *                                          initialisation of the window
         *                                          class has not been properly
         *                                          performed.
         * @throws the::system::system_exception In case the window could not be
         *                                       created.
         */
        static shared_pointer_type create(const system::system_char *title,
            const position_type x, const position_type y,
            const size_type width, const size_type height);

        /** Dtor. */
        virtual ~window(void);

        /**
         * Answer the native window handle of this window.
         *
         * @return The native window handle.
         */
        inline const native_handle_type get_handle(void) const {
            THE_STACK_TRACE;
            return this->hWnd;
        }

        /**
         * Answer the native window handle of this window.
         *
         * @return The native window handle.
         */
        inline native_handle_type get_handle(void) {
            THE_STACK_TRACE;
            return this->hWnd;
        }

    protected:

        /** Super class typedef. */
        typedef T base;

        /**
         * Initialises a new instance.
         */
        window(void);

    };

} /* end namespace graphics */
} /* end namespace the */


/*
 * the::graphics::window<T>::create
 */
template<class T> typename the::graphics::window<T>::shared_pointer_type
the::graphics::window<T>::create(const system::system_char *title,
        const position_type x, const position_type y,
        const size_type width, const size_type height) {
    THE_STACK_TRACE;
    shared_pointer_type retval = new window();
    retval->initialise(title, x, y, width, height);
    return retval;
}


/*
 * the::graphics::window<T>::window
 */
template<class T> the::graphics::window<T>::window(void) : base() {
    THE_STACK_TRACE;
}


/*
 * the::graphics::window<T>::~window
 */
template<class T> the::graphics::window<T>::~window(void) {
    THE_STACK_TRACE;
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_GRAPHICS_WINDOW_H_INCLUDED */
