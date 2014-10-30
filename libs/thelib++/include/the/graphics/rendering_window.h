/*
 * include\the\graphics\rendering_window.h
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

#ifndef THE_GRAPHICS_RENDERING_WINDOW_H_INCLUDED
#define THE_GRAPHICS_RENDERING_WINDOW_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/assert.h"
#include "the/stack_trace.h"
#include "the/trace.h"

#include "the/system/system_string.h"


namespace the {
namespace graphics {

    /**
     * This class provides a simple object-oriented implementation of a
     * platform-independent GPU rendering window.
     *
     * The platform-dependent behaviour as well as the rendering API-dependent
     * behaviour is factored out into template parameters.
     *
     * @tparam W The window behaviour, e.g. an instantiation of 
     *           the::graphics::window.
     * @tparam R The rendering behaviour, e.g.
     *           the::graphics::directx::d3d11_rendering_behaviour.
     */
    template<class W, class R> class rendering_window : public W, public R {

    public:

        /** The native character type. */
        typedef typename W::native_char_type native_char_type;

        /** The type of the native window handle. */
        typedef typename W::native_handle_type native_handle_type;

        /** The type for specifying positions of the window. */
        typedef typename W::position_type position_type;

        /** The type for specifying the dimensions of a window. */
        typedef typename W::size_type size_type;

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
         *                                          and/or the renderer class
         *                                          has not been properly
         *                                          performed.
         * @throws the::system::system_exception In case the window could not be
         *                                       created.
         *
         * @tparam T The type of a derived class that is being instantiated 
         *           instead of rendering window.
         */
        template<class T>
        static std::shared_ptr<T> create(const system::system_char *title,
            const position_type x, const position_type y,
            const size_type width, const size_type height);

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
         *                                          and/or the renderer class
         *                                          has not been properly
         *                                          performed.
         * @throws the::system::system_exception In case the window could not be
         *                                       created.
         */
        inline std::shared_ptr<rendering_window> create(
                const system::system_char *title,
                const position_type x, const position_type y,
                const size_type width, const size_type height) {
            THE_STACK_TRACE;
            return rendering_window::create<rendering_window>(title, x, y,
                width, height);
        }

        /**
         * Performs one-time cleanup of the renderer and the window.
         */
        static inline void one_time_finalise(void) {
            THE_STACK_TRACE;
            rendering_behaviour::one_time_finalise();
            window_behaviour::one_time_finalise();
        }

        /**
         * Performs one-time initialisation of the renderer and the window.
         */
        static inline void one_time_initialise(void) {
            THE_STACK_TRACE;
            window_behaviour::one_time_initialise();
            rendering_behaviour::one_time_initialise();
        }

        /** Dtor. */
        virtual ~rendering_window(void);

    protected:

        /** The rendering behaviour that is implemented by the window. */
        typedef R rendering_behaviour;

        /** The window behaviour itself. */
        typedef W window_behaviour;

        /**
         * Initialises a new instance.
         */
        rendering_window(void);

        /**
         * Allocates the system resources for the window and the renderer 
         * resources as well.
         *
         * @param title  The title of the window.
         * @param x      The abscissa of the initial position of the window.
         * @param y      The ordinate of the initial position of the window.
         * @param width  The initial width of the window.
         * @param height The initial height of the window.
         *
         * @throws the::invalid_operation_exception
         * @throws the::system::system_exception
         */
        virtual void initialise(const system::system_char *title,
            const position_type x, const position_type y,
            const size_type width, const size_type height);

        /**
         * Resizes the renderer if the window was resized.
         *
         * @param width  The new width of the window.
         * @param height The new height of the window.
         */
        virtual void on_resized(const size_type width,
            const size_type height) throw();

    };

} /* end namespace graphics */
} /* end namespace the */


/*
 * the::graphics::rendering_window<T>::create
 */
template<class W, class R>
template<class T>
std::shared_ptr<T> the::graphics::rendering_window<W, R>::create(
        const system::system_char *title,
        const position_type x, const position_type y,
        const size_type width, const size_type height) {
    THE_STACK_TRACE;
    std::shared_ptr<T> retval(new T());
    retval->initialise(title, x, y, width, height);
    return retval;
}


/*
 * the::graphics::rendering_window<W, R>::~rendering_window
 */
template<class W, class R>
the::graphics::rendering_window<W, R>::~rendering_window(void) {
    THE_STACK_TRACE;
}


/*
 * the::graphics::rendering_window<W, R>::rendering_window
 */
template<class W, class R>
the::graphics::rendering_window<W, R>::rendering_window(void) 
        : window_behaviour(), rendering_behaviour() {
    THE_STACK_TRACE;
}


/*
 * the::graphics::rendering_window<W, R>::initialise
 */
template<class W, class R>
void the::graphics::rendering_window<W, R>::initialise(
        const system::system_char *title,
        const position_type x, const position_type y,
        const size_type width, const size_type height) {
    THE_STACK_TRACE;
    window_behaviour::initialise(title, x, y, width, height);
    rendering_behaviour::initialise(this->get_handle());
}


/*
 * the::graphics::rendering_window<W, R>::on_resized
 */
template<class W, class R>
void the::graphics::rendering_window<W, R>::on_resized(const size_type width,
        const size_type height) throw() {
    THE_STACK_TRACE;
    THE_ASSERT(width >= 0);
    THE_ASSERT(height >= 0);
    window_behaviour::on_resized(width, height);
    try {
        rendering_behaviour::resize(width, height);
    } catch (...) {
        THE_TRACE(THE_TRCCHL_THELIB_COMMON + 15, THE_TRCLVL_ERROR, "The "
            "renderer threw an exception after resizing the window.");
    }
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_GRAPHICS_RENDERING_WINDOW_H_INCLUDED */
