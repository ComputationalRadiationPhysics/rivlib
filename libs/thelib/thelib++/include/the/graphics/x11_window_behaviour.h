/*
 * include\the\graphics\x11_window_behaviour.h
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

#ifndef THE_GRAPHICS_X11_WINDOW_BEHAVIOUR_H_INCLUDED
#define THE_GRAPHICS_X11_WINDOW_BEHAVIOUR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include <unordered_map>

#include <X11/Xlib.h>

#include "the/not_copyable.h"

#include "the/system/system_exception.h"
#include "the/system/system_string.h"


namespace the {
namespace graphics {

    /**
     * Implements the platform-dependent behaviour of a window on X11 platforms.
     *
     * This class is intended to be used as template parameter of
     * the::graphics::window.
     */
    class x11_window_behaviour : public the::not_copyable {

    public:

        /** The native character type. */
        typedef char native_char_type;

        /** The type of the native window handle. */
        typedef Window native_handle_type;

        /** The type of native key codes. */
        typedef int native_virtual_key_type;

        /** The type for specifying positions of the window. */
        typedef int position_type;

        /** The type for specifying the dimensions of a window. */
        typedef int size_type;

        /**
         * Performs one-time cleanup after the last window was destroyed undoing
         * all initialisation performed in one_time_initialise().
         *
         * @throws the::system::system_exception If the operation failed.
         */
        static void one_time_finalise(void);

        /**
         * Performs one-time initialisation that must be done before the first
         * window is created.
         *
         * The implementation here stores the current application instance.
         *
         * @throws the::system::system_exception If the initialisation failed.
         */
        static void one_time_initialise(void);

        /**
         * Retrieves the next messge from the message queue and processes it.
         *
         * @return true to indicate that the method should be called again,
         *         false otherwise.
         */
        static bool process_message(void);

        /** Dtor. */
        virtual ~x11_window_behaviour(void);

        /**
         * Destroys the window.
         */
        virtual void destroy(void);

        virtual void hide(void);

        virtual void invalidate(void);

        virtual void move(const position_type x, const position_type y);
        
        virtual void resize(const size_type width, const size_type height);

        virtual void show(void);

    protected:

        /** Super class typedef. */
        typedef the::not_copyable base;

        /** The display used for displaying windows. */
        static Display *display;

        /** The screen used for displaying windows. */
        static int screen;

        /**
         * Initialises a new instance, but does not yet allocate the system
         * resources for the window.
         */
        x11_window_behaviour(void) throw();

        /**
         * Allocates the system resources for the window.
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
         * This method is invoked if the window is closed.
         */
        virtual void on_close(void) throw();

        /**
         * This method is invoked if the window receives a character event.
         *
         * @param c The character that has been typed.
         */
        virtual void on_char(const native_char_type c) throw();

        ///**
        // * The class will call this method immediately before allocating the
        // * system resources for the window. Subclasses can use this method to
        // * influence the style of the window that is to be created.
        // *
        // * @param inOutStyle
        // * @param inOutStyleEx
        // */
        //virtual void on_initialising(DWORD& inOutStyle,
        //    DWORD& inOutStyleEx) throw();

        /**
         * This method is invoked if the window receives a key-down event.
         *
         * @param key The key that has been pressed.
         */
        virtual void on_key_down(const native_virtual_key_type& key) throw();

        /**
         * This method is invoked if the window receives a key-up event.
         *
         * @param key The key that has been released.
         */
        virtual void on_key_up(const native_virtual_key_type& key) throw();

        /**
         * This method is invoked if the window receives a mouse-down event.
         *
         * @param button The mouse button that has been pressed.
         *               The left button is 0, the right button is 1, the middle
         *               button is 2, special button have higher numbers.
         */
        virtual void on_mouse_down(const int button) throw();

        /**
         * This method is invoked if the window receives a mouse motion event.
         *
         * @param x The new x-coordinate of the mouse cursor.
         * @param y The new y-coordinate of the mouse cursor.
         */
        virtual void on_mouse_move(const position_type x,
            const position_type y) throw();

        /**
         * This method is invoked if the window receives a mouse-up event.
         *
         * @param button The mouse button that has been released.
         *               The left button is 0, the right button is 1, the middle
         *               button is 2, special button have higher numbers.
         */
        virtual void on_mouse_up(const int button) throw();

        /**
         * This method is invoked if the window receives a method. It allows
         * sub-classes to intercept messages before the standard processing
         * is done.
         *
         * @param outHandled
         * @param event
         *
         * @return
         */
        virtual void on_native_message(bool& outHandled,
            const XEvent& event) throw();

        /**
         * This method is invoked if the window has been resized.
         *
         * @param width  The new width of the window.
         * @param height The new height of the window.
         */
        virtual void on_resized(const size_type width,
            const size_type height) throw();

        /** The native window handle. */
        native_handle_type hWnd;

    private:

        /**
         * A dictionary which maps the X11 window to the window class that has
         * been associated with it. There seems no other way for doing this,
         * because we cannot attach custom data to the window handle like it is
         * possible on Windows.
         */
        typedef std::unordered_map<native_handle_type,
            x11_window_behaviour *> dispatch_map_type;

        /**
         * Tries to get the x11_window_behaviour for a given X11 window.
         *
         * @param window The window to get the behaviour for.
         *
         * @return The x11_window_behaviour or nullptr if no associated
         *         behaviour was found.
         */
        static x11_window_behaviour *find_window(const Window& window);

        // TODO: Need a nasty hack that performs something like that:
        //template<class E> 
        //static inline x11_window_behaviour *find_window(const E& event) {
        //    THE_STACK_TRACE;
        //    return x11_window_behaviour::find_window(event.window);
        //}

        /**
         * The dictionary that we need to dispatch events to the correct
         * object.
         */
        static dispatch_map_type dispatch_map;
    };

} /* end namespace graphics */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_GRAPHICS_X11_WINDOW_BEHAVIOUR_H_INCLUDED */
