/*
 * src\graphics\x11_window_behaviour.cpp
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

#include "the/graphics/x11_window_behaviour.h"

#include "the/assert.h"
#include "the/stack_trace.h"


#include "the/not_implemented_exception.h"


/*
 * the::graphics::x11_window_behaviour::one_time_finalise
 */
void the::graphics::x11_window_behaviour::one_time_finalise(void) {
    THE_STACK_TRACE;
    THE_ASSERT(x11_window_behaviour::display != nullptr);

    x11_window_behaviour::dispatch_map.clear();

    ::XCloseDisplay(x11_window_behaviour::display);// TODO: meaning of return value unkown.
    x11_window_behaviour::display = nullptr;

    x11_window_behaviour::screen = 0;
}


/*
 * the::graphics::x11_window_behaviour::one_time_initialise
 */
void the::graphics::x11_window_behaviour::one_time_initialise(void) {
    THE_STACK_TRACE;
    THE_ASSERT(x11_window_behaviour::display == nullptr);
    THE_ASSERT(x11_window_behaviour::dispatch_map.empty());

    x11_window_behaviour::display = ::XOpenDisplay(NULL);
    if (display == nullptr) {
        throw exception(THE_SYSTEM_STRING_CONSTANT("XOpenDisplay failed and ")
            THE_SYSTEM_STRING_CONSTANT("Linux (like always) cannot tell why."),
            __FILE__, __LINE__);
    }

    x11_window_behaviour::screen = DefaultScreen(
        x11_window_behaviour::display);
}


/*
 * the::graphics::x11_window_behaviour::process_message
 */
bool the::graphics::x11_window_behaviour::process_message(void) {
    THE_STACK_TRACE;
    XEvent event;
    bool isHandled = false;
    x11_window_behaviour *wnd = NULL;


    if (::XPending(x11_window_behaviour::display)) {
        ::XNextEvent(x11_window_behaviour::display, &event);

        // TODO on_native_message
        //wnd = x11_window_behaviour::find_window(event);

        switch (event.type) {
            case KeyPress:
                if ((wnd = find_window(event.xkey.window)) != nullptr) {
                    // TODO
                }
                break;

            case KeyRelease:
                if ((wnd = find_window(event.xkey.window)) != nullptr) {
                    // TODO
                }
                break;

            case ButtonPress:
                if ((wnd = find_window(event.xbutton.window)) != nullptr) {
                    wnd->on_mouse_down(event.xbutton.button);
                }
                break;

            case ButtonRelease:
                if ((wnd = find_window(event.xbutton.window)) != nullptr) {
                    wnd->on_mouse_up(event.xbutton.button);
                }
                break;

            case MotionNotify:
                if ((wnd = find_window(event.xmotion.window)) != nullptr) {
                    wnd->on_mouse_move(event.xmotion.x, event.xmotion.y);
                }
                break;

            case DestroyNotify:
                if ((wnd = find_window(event.xdestroywindow.window)) 
                        != nullptr) {
                    wnd->on_close();
                    x11_window_behaviour::dispatch_map.erase(
                        event.xdestroywindow.window);
                }
                break;
        }
    }

    return true;
}


/*
 * the::graphics::x11_window_behaviour::~x11_window_behaviour
 */
the::graphics::x11_window_behaviour::~x11_window_behaviour(void) {
    THE_STACK_TRACE;
    this->destroy();
}

/*
 * the::graphics::x11_window_behaviour::destroy
 */
void the::graphics::x11_window_behaviour::destroy(void) {
    THE_STACK_TRACE;
    ::XDestroyWindow(x11_window_behaviour::display, this->hWnd);
}


/*
 * the::graphics::x11_window_behaviour::hide
 */
void the::graphics::x11_window_behaviour::hide(void) {
    THE_STACK_TRACE;
    ::XUnmapWindow(x11_window_behaviour::display, this->hWnd);
}


/*
 * the::graphics::x11_window_behaviour::invalidate
 */
void the::graphics::x11_window_behaviour::invalidate(void) {
    THE_STACK_TRACE;
    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
}


/*
 * the::graphics::x11_window_behaviour::move
 */
void the::graphics::x11_window_behaviour::move(const position_type x,
        const position_type y) {
    THE_STACK_TRACE;
    ::XMoveWindow(x11_window_behaviour::display, this->hWnd, x, y);
}


/*
 * the::graphics::x11_window_behaviour::resize
 */
void the::graphics::x11_window_behaviour::resize(const size_type width,
        const size_type height) {
    THE_STACK_TRACE;
    ::XResizeWindow(x11_window_behaviour::display, this->hWnd, width, height);
}


/*
 * the::graphics::x11_window_behaviour::show
 */
void the::graphics::x11_window_behaviour::show(void) {
    THE_STACK_TRACE;
    ::XMapWindow(x11_window_behaviour::display, this->hWnd);
    ::XFlush(x11_window_behaviour::display);
}


/*
 * the::graphics::x11_window_behaviour::display
 */
Display *the::graphics::x11_window_behaviour::display = nullptr;


/*
 * the::graphics::x11_window_behaviour::screen
 */
int the::graphics::x11_window_behaviour::screen = 0;


/*
 * the::graphics::x11_window_behaviour::x11_window_behaviour
 */
the::graphics::x11_window_behaviour::x11_window_behaviour(void) throw() 
        : base()  {
    THE_STACK_TRACE;
}


/*
 * the::graphics::x11_window_behaviour::initialise
 */
void the::graphics::x11_window_behaviour::initialise(
        const system::system_char *title,
        const position_type x, const position_type y,
        const size_type width, const size_type height) {
    THE_STACK_TRACE;

    /* Sanity checks. */
    //if (this->hWnd != NULL) {
    //    throw invalid_operation_exception(THE_SYSTEM_STRING_CONSTANT("The ")
    //        THE_SYSTEM_STRING_CONSTANT("window has already been initialised."),
    //        __FILE__, __LINE__);
    //}
    if (x11_window_behaviour::display == nullptr) {
        throw invalid_operation_exception(THE_SYSTEM_STRING_CONSTANT("The ")
            THE_SYSTEM_STRING_CONSTANT("display has not been opened. Did ")
            THE_SYSTEM_STRING_CONSTANT("miss calling one_time_initialise()?"),
            __FILE__, __LINE__);
    }

    this->hWnd = ::XCreateSimpleWindow(x11_window_behaviour::display,
        RootWindow(x11_window_behaviour::display, x11_window_behaviour::screen),
        x, y, width, height,
        1,
        BlackPixel(x11_window_behaviour::display, x11_window_behaviour::screen),
        WhitePixel(x11_window_behaviour::display, x11_window_behaviour::screen));
    // TODO: error handling?!

    x11_window_behaviour::dispatch_map.insert(dispatch_map_type::value_type(
        this->hWnd, this));
}


/*
 * the::graphics::x11_window_behaviour::on_char
 */
void the::graphics::x11_window_behaviour::on_char(
        const native_char_type c) throw() {
    THE_STACK_TRACE;
}


///*
// * the::graphics::x11_window_behaviour::on_initialising
// */
//void the::graphics::x11_window_behaviour::on_initialising(DWORD& inOutStyle,
//        DWORD& inOutStyleEx) throw() {
//    THE_STACK_TRACE;
//}


/*
 * the::graphics::x11_window_behaviour::on_key_down
 */
void the::graphics::x11_window_behaviour::on_key_down(
        const native_virtual_key_type& key) throw() {
    THE_STACK_TRACE;
}


/*
 * the::graphics::x11_window_behaviour::on_key_up
 */
void the::graphics::x11_window_behaviour::on_key_up(
        const native_virtual_key_type& key) throw() {
    THE_STACK_TRACE;
}


/*
 * the::graphics::x11_window_behaviour::on_mouse_down
 */
void the::graphics::x11_window_behaviour::on_mouse_down(
        const int button) throw() {
    THE_STACK_TRACE;
}


/*
 * the::graphics::x11_window_behaviour::on_mouse_move
 */
void the::graphics::x11_window_behaviour::on_mouse_move(
        const position_type x, const position_type y) throw() {
    THE_STACK_TRACE;
}


/*
 * the::graphics::x11_window_behaviour::on_mouse_up
 */
void the::graphics::x11_window_behaviour::on_mouse_up(
        const int button) throw() {
    THE_STACK_TRACE;
}


/*
 * the::graphics::x11_window_behaviour::on_native_message
 */
void the::graphics::x11_window_behaviour::on_native_message(
        bool& outHandled, const XEvent& event) throw() {
    THE_STACK_TRACE;
    outHandled = false;
}


/*
 * the::graphics::x11_window_behaviour::on_resized
 */
void the::graphics::x11_window_behaviour::on_resized(const size_type width,
        const size_type height) throw() {
    THE_STACK_TRACE;
}


/*
 * the::graphics::x11_window_behaviour::find_window
 */
the::graphics::x11_window_behaviour *
the::graphics::x11_window_behaviour::find_window(const Window& window) {
    THE_STACK_TRACE;
    dispatch_map_type::iterator it = x11_window_behaviour::dispatch_map.find(
        window);
    return (it != x11_window_behaviour::dispatch_map.end()) 
        ? it->second : nullptr;
}


/*
 * the::graphics::x11_window_behaviour::dispatch_map
 */
the::graphics::x11_window_behaviour::dispatch_map_type
the::graphics::x11_window_behaviour::dispatch_map;
