/*
 * src\graphics\win32_window_behaviour.cpp
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

#include "the/graphics/win32_window_behaviour.h"

#include <windowsx.h>

#include "the/assert.h"
#include "the/invalid_operation_exception.h"
#include "the/stack_trace.h"


/*
 * the::graphics::win32_window_behaviour::one_time_finalise
 */
void the::graphics::win32_window_behaviour::one_time_finalise(void) {
    THE_STACK_TRACE;
    THE_ASSERT(win32_window_behaviour::hInstance != NULL);

    if (!::UnregisterClassW(win32_window_behaviour::window_class_name,
            win32_window_behaviour::hInstance)) {
        // TODO: Check how to avoid this...
        //THROW_THE_SYSTEM_EXCEPTION;
    }

    win32_window_behaviour::hInstance = NULL;
}


/*
 * the::graphics::win32_window_behaviour::one_time_initialise
 */
void the::graphics::win32_window_behaviour::one_time_initialise(void) {
    THE_STACK_TRACE;
    THE_ASSERT(win32_window_behaviour::hInstance == NULL);

    WNDCLASSEXW wndClass;   // The window class we want to use/register.

    /* Get the instance handle. */
    win32_window_behaviour::hInstance = ::GetModuleHandle(NULL);
    if (win32_window_behaviour::hInstance == NULL) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

    /*
     * Test whether another application instance using THElib has already 
     * registered the class, and if not, register it.
     */
    if (!::GetClassInfoExW(win32_window_behaviour::hInstance, 
            win32_window_behaviour::window_class_name, &wndClass)) {
        ::ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
        wndClass.cbSize = sizeof(WNDCLASSEX);
        wndClass.style = CS_CLASSDC;
        wndClass.lpfnWndProc = win32_window_behaviour::wndProc;
        wndClass.hInstance = win32_window_behaviour::hInstance;
        wndClass.lpszClassName = win32_window_behaviour::window_class_name;

        if (!::RegisterClassExW(&wndClass)) {
            THROW_THE_SYSTEM_EXCEPTION;
        }
    }
}


/*
 * the::graphics::win32_window_behaviour::process_message
 */
bool the::graphics::win32_window_behaviour::process_message(void) {
    THE_STACK_TRACE;
    MSG msg;

    if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);

        //that = abstract_win32_window::get_object(msg.hwnd);
        //if ((that != NULL) && !that->on_message(msg)) {
        //    return false;
        //} else {
        //    return (msg.message != WM_QUIT);
        //}
        return (msg.message != WM_QUIT);
    } else {
        return true;
    }
}


/*
 * the::graphics::win32_window_behaviour::~win32_window_behaviour
 */
the::graphics::win32_window_behaviour::~win32_window_behaviour(void) {
    THE_STACK_TRACE;
    if (this->hWnd != NULL) {
        this->destroy();
    }
}


/*
 * the::graphics::win32_window_behaviour::destroy
 */
void the::graphics::win32_window_behaviour::destroy(void) {
    THE_STACK_TRACE;
    if (::DestroyWindow(this->hWnd)) {
        this->hWnd = NULL;
    } else {
        THROW_THE_SYSTEM_EXCEPTION;
    }
}


/*
 * the::graphics::win32_window_behaviour::hide
 */
void the::graphics::win32_window_behaviour::hide(void) {
    THE_STACK_TRACE;
    ::ShowWindow(this->hWnd, SW_HIDE);
}


/*
 * the::graphics::win32_window_behaviour::invalidate
 */
void the::graphics::win32_window_behaviour::invalidate(void) {
    THE_STACK_TRACE;
    if (!::InvalidateRect(this->hWnd, NULL, FALSE)) {
        THROW_THE_SYSTEM_EXCEPTION;
    }
}


/*
 * the::graphics::win32_window_behaviour::move
 */
void the::graphics::win32_window_behaviour::move(const position_type x,
        const position_type y) {
    THE_STACK_TRACE;
    if (!::SetWindowPos(this->hWnd, NULL, x, y, 0, 0,
            SWP_NOSIZE | SWP_NOZORDER)) {
        THROW_THE_SYSTEM_EXCEPTION;
    }
}


/*
 * the::graphics::win32_window_behaviour::resize
 */
void the::graphics::win32_window_behaviour::resize(const size_type width,
        const size_type height) {
    THE_STACK_TRACE;
    if (!::SetWindowPos(this->hWnd, NULL, 0, 0, width, height,
            SWP_NOMOVE | SWP_NOZORDER)) {
        THROW_THE_SYSTEM_EXCEPTION;
    }
}


/*
 * the::graphics::win32_window_behaviour::show
 */
void the::graphics::win32_window_behaviour::show(void) {
    THE_STACK_TRACE;
    if (this->hWnd != NULL) {
        ::ShowWindow(this->hWnd, SW_SHOW);
    } else {
        throw invalid_operation_exception(THE_SYSTEM_STRING_CONSTANT("The ")
            THE_SYSTEM_STRING_CONSTANT("window handle is invalid"), __FILE__,
            __LINE__);
    }
}


/*
 * the::graphics::win32_window_behaviour::hInstance
 */
HINSTANCE the::graphics::win32_window_behaviour::hInstance = NULL;


/*
 * the::graphics::win32_window_behaviour::win32_window_behaviour
 */
the::graphics::win32_window_behaviour::win32_window_behaviour(void)
        : hWnd(NULL) {
    THE_STACK_TRACE;
}


/*
 * the::graphics::win32_window_behaviour::initialise
 */
void the::graphics::win32_window_behaviour::initialise(
        const system::system_char *title,
        const position_type x, const position_type y,
        const size_type width, const size_type height) {
    THE_STACK_TRACE;
    DWORD style = WS_OVERLAPPEDWINDOW;
    DWORD styleEx = 0;
    RECT wndRect;

    /* Sanity checks. */
    if (this->hWnd != NULL) {
        throw invalid_operation_exception(THE_SYSTEM_STRING_CONSTANT("The ")
            THE_SYSTEM_STRING_CONSTANT("window has already been initialised."),
            __FILE__, __LINE__);
    }
    if (win32_window_behaviour::hInstance == NULL) {
        throw invalid_operation_exception(THE_SYSTEM_STRING_CONSTANT("The ")
            THE_SYSTEM_STRING_CONSTANT("instance handle has not been set. Did ")
            THE_SYSTEM_STRING_CONSTANT("miss calling one_time_initialise()?"),
            __FILE__, __LINE__);
    }

    /* Grant sub-classes the option to modify the window style. */
    this->on_initialising(style, styleEx);

    /* Adjust window size to match the requested client size. */
    wndRect.left = x;
    wndRect.top = y;
    wndRect.right = wndRect.left + width;
    wndRect.bottom = wndRect.top + height;
    if (::AdjustWindowRectEx(&wndRect, style, FALSE, styleEx) == FALSE) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

    /* Create the window. */
    this->hWnd = ::CreateWindowExW(styleEx,
        win32_window_behaviour::window_class_name,
        title,
        style,
        wndRect.left, wndRect.top,
        wndRect.right - wndRect.left, wndRect.bottom - wndRect.top,
        NULL, NULL,
        win32_window_behaviour::hInstance,
        this);
    if (this->hWnd == NULL) {
        THROW_THE_SYSTEM_EXCEPTION;
    }
}


/*
 * the::graphics::win32_window_behaviour::on_close
 */
void the::graphics::win32_window_behaviour::on_close(void) throw() {
    THE_STACK_TRACE;
}


/*
 * the::graphics::win32_window_behaviour::on_char
 */
void the::graphics::win32_window_behaviour::on_char(
        const native_char_type c) throw() {
    THE_STACK_TRACE;
}


/*
 * the::graphics::win32_window_behaviour::on_initialising
 */
void the::graphics::win32_window_behaviour::on_initialising(DWORD& inOutStyle,
        DWORD& inOutStyleEx) throw() {
    THE_STACK_TRACE;
}


/*
 * the::graphics::win32_window_behaviour::on_key_down
 */
void the::graphics::win32_window_behaviour::on_key_down(
        const native_virtual_key_type& key) throw() {
    THE_STACK_TRACE;
}


/*
 * the::graphics::win32_window_behaviour::on_key_up
 */
void the::graphics::win32_window_behaviour::on_key_up(
        const native_virtual_key_type& key) throw() {
    THE_STACK_TRACE;
}


/*
 * the::graphics::win32_window_behaviour::on_mouse_down
 */
void the::graphics::win32_window_behaviour::on_mouse_down(
        const int button) throw() {
    THE_STACK_TRACE;
}


/*
 * the::graphics::win32_window_behaviour::on_mouse_move
 */
void the::graphics::win32_window_behaviour::on_mouse_move(
        const position_type x, const position_type y) throw() {
    THE_STACK_TRACE;
}


/*
 * the::graphics::win32_window_behaviour::on_mouse_up
 */
void the::graphics::win32_window_behaviour::on_mouse_up(const int button) {
    THE_STACK_TRACE;
}


/*
 * the::graphics::win32_window_behaviour::on_native_message
 */
LRESULT the::graphics::win32_window_behaviour::on_native_message(
        bool& outHandled, UINT msg, WPARAM wParam, LPARAM lParam) throw() {
    THE_STACK_TRACE;
    outHandled = false;
    return 0;
}


/*
 * the::graphics::win32_window_behaviour::on_resized
 */
void the::graphics::win32_window_behaviour::on_resized(const size_type width,
        const size_type height) throw() {
    THE_STACK_TRACE;
}


/*
 *  the::graphics::win32_window_behaviour::wndProc
 */
LRESULT WINAPI the::graphics::win32_window_behaviour::wndProc(HWND hWnd,
        UINT msg, WPARAM wParam, LPARAM lParam) {
    win32_window_behaviour *that = reinterpret_cast<win32_window_behaviour *>(
        ::GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    CREATESTRUCTW *cs = NULL;
    bool isHandled = false;
    LRESULT retval = 0;

    if (that != NULL) {
        retval = that->on_native_message(isHandled, msg, wParam, lParam);
    }

    if (!isHandled) {
        switch (msg) {
            case WM_CHAR:
                THE_ASSERT(that != NULL);
                that->on_char(static_cast<native_char_type>(wParam));
                break;

            case WM_CLOSE:
                THE_ASSERT(that != NULL);
                that->on_close();
                break;

            case WM_CREATE:
                cs = reinterpret_cast<CREATESTRUCTW *>(lParam);
                if ((::SetWindowLongPtrW(hWnd, GWLP_USERDATA,
                        reinterpret_cast<LONG_PTR>(cs->lpCreateParams)) == 0)) {
                    retval = ::GetLastError();
                } else {
                    retval = 0;
                }
                break;

            case WM_KEYDOWN:
                THE_ASSERT(that != NULL);
                that->on_key_down(static_cast<native_virtual_key_type>(wParam));
                retval = 0;
                break;

            case WM_KEYUP:
                THE_ASSERT(that != NULL);
                that->on_key_up(static_cast<native_virtual_key_type>(wParam));
                retval = 0;
                break;

            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN: {
                THE_ASSERT(that != NULL);
                int button = 0;
                if (msg == WM_MBUTTONDOWN) {
                    button = 1;
                } else if (msg == WM_RBUTTONDOWN) {
                    button = 2;
                }
                that->on_mouse_down(button);
                retval = 0;
                } break;

            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP: {
                THE_ASSERT(that != NULL);
                int button = 0;
                if (msg == WM_MBUTTONUP) {
                    button = 1;
                } else if (msg == WM_RBUTTONUP) {
                    button = 2;
                }
                that->on_mouse_up(button);
                retval = 0;
                } break;

            case WM_MOUSEMOVE:
                THE_ASSERT(that != NULL);
                that->on_mouse_move(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                retval = 0;
                break;

            case WM_SIZE:
                THE_ASSERT(that != NULL);
                that->on_resized(LOWORD(lParam), HIWORD(lParam));
                retval = 0;
                break;


            default:
                retval = ::DefWindowProc(hWnd, msg, wParam, lParam);
                break;
        }
    }

    return retval;
}


/*
 * the::graphics::win32_window_behaviour::window_class_name
 */
const the::system::system_char *the::graphics::win32_window_behaviour
    ::window_class_name = THE_SYSTEM_STRING_CONSTANT("THELIB_WINDOW");
