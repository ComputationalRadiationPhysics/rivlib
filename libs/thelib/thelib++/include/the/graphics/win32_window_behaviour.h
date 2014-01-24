/*
 * include\the\graphics\win32_window_behaviour.h
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

#ifndef THE_GRAPHICS_WIN32_WINDOW_BEHAVIOUR_H_INCLUDED
#define THE_GRAPHICS_WIN32_WINDOW_BEHAVIOUR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include <Windows.h>

#include "the/delegate.h"
#include "the/not_copyable.h"

#include "the/system/system_exception.h"
#include "the/system/system_string.h"


namespace the {
namespace graphics {

    /**
     * Implements the platform-dependent behaviour of a window on Windows
     * platforms.
     *
     * This class is intended to be used as template parameter of
     * the::graphics::window.
     */
    class win32_window_behaviour : public the::not_copyable {

    public:

        /** The native character type. */
        typedef wchar_t native_char_type;

        /** The type of the native window handle. */
        typedef HWND native_handle_type;

        /** The type of native key codes. */
        typedef int native_virtual_key_type;

        //typedef delegate<LRESULT, bool&, UINT, WPARAM, LPARAM>
        //    native_message_handler;

        /** The type of native window messages. */
        //typedef MSG native_message_type;

        /** The type for specifying positions of the window. */
        typedef int position_type;

        /** The type for specifying the dimensions of a window. */
        typedef int size_type;

        //typedef delegate<void, native_char_type> char_handler_type;

        //typedef delegate<void, native_virtual_key_type> key_handler_type;

        //typedef delegate<void, position_type, position_type> moved_handler_type;

        //typedef delegate<void, size_type, size_type> resized_handler_type;

//        /** Virtual key code names used within THElib. */
//        typedef enum virtual_key_t {
//            none = 0,
//            lbutton = VK_LBUTTON,
//            rbutton = VK_RBUTTON,
//            cancel = VK_CANCEL,
//            mbutton = VK_MBUTTON,
//#if (_WIN32_WINNT >= 0x0500)
//            xbutton1 = VK_XBUTTON1,
//            xbutton2 = VK_XBUTTON2,
//#else (_WIN32_WINNT >= 0x0500)
//            xbutton1 = 0,
//            xbutton2 = 0,
//#endif /* _WIN32_WINNT >= 0x0500 */
//            back = VK_BACK,
//            tab = VK_TAB,
//            clear = VK_CLEAR,
//            enter = VK_RETURN,
//            shift = VK_SHIFT,
//            control = VK_CONTROL,
//            menu = VK_MENU,
//            pause = VK_PAUSE,
//            capital = VK_CAPITAL,
//            kana = VK_KANA,
//            hangul = VK_HANGUL,
//            junja = VK_JUNJA,
//            final = VK_FINAL,
//            hanja = VK_HANJA,
//            kanji = VK_KANJI,
//            escape = VK_ESCAPE,
//            convert = VK_CONVERT,
//            nonconvert = VK_NONCONVERT,
//            accept = VK_ACCEPT,
//            modechange = VK_MODECHANGE,
//            space = VK_SPACE,
//            prior = VK_PRIOR,
//            next = VK_NEXT,
//            end = VK_END,
//            home = VK_HOME,
//            left = VK_LEFT,
//            up = VK_UP,
//            right = VK_RIGHT,
//            down = VK_DOWN,
//            select = VK_SELECT,
//            print = VK_PRINT,
//            execute = VK_EXECUTE,
//            snapshot = VK_SNAPSHOT,
//            insert = VK_INSERT,
//            del = VK_DELETE,
//            help = VK_HELP,
//            num0 = 0x30,
//            num1,
//            num2,
//            num3,
//            num4,
//            num5,
//            num6,
//            num7,
//            num8,
//            num9,
//            a = 0x41,
//            b,
//            c,
//            d,
//            e,
//            f,
//            g,
//            h,
//            i,
//            j,
//            k,
//            l,
//            m,
//            n,
//            o,
//            p,
//            q,
//            r,
//            s,
//            t,
//            u,
//            v,
//            w,
//            x,
//            y,
//            z,
//            lwin = VK_LWIN,
//            rwin = VK_RWIN,
//            apps = VK_APPS,
//            sleep = VK_SLEEP,
//            numpad0 = VK_NUMPAD0,
//            numpad1 = VK_NUMPAD1,
//            numpad2 = VK_NUMPAD2,
//            numpad3 = VK_NUMPAD3,
//            numpad4 = VK_NUMPAD4,
//            numpad5 = VK_NUMPAD5,
//            numpad6 = VK_NUMPAD6,
//            numpad7 = VK_NUMPAD7,
//            numpad8 = VK_NUMPAD8,
//            numpad9 = VK_NUMPAD9,
//            multiply = VK_MULTIPLY,
//            add = VK_ADD,
//            separator = VK_SEPARATOR,
//            subtract = VK_SUBTRACT,
//            decimal = VK_DECIMAL,
//            divide = VK_DIVIDE,
//            f1 = VK_F1,
//            f2 = VK_F2,
//            f3 = VK_F3,
//            f4 = VK_F4,
//            f5 = VK_F5,
//            f6 = VK_F6,
//            f7 = VK_F7,
//            f8 = VK_F8,
//            f9 = VK_F9,
//            f10 = VK_F10,
//            f11 = VK_F11,
//            f12 = VK_F12,
//            f13 = VK_F13,
//            f14 = VK_F14,
//            f15 = VK_F15,
//            f16 = VK_F16,
//            f17 = VK_F17,
//            f18 = VK_F18,
//            f19 = VK_F19,
//            f20 = VK_F20,
//            f21 = VK_F21,
//            f22 = VK_F22,
//            f23 = VK_F23,
//            f24 = VK_F24,
//            numlock = VK_NUMLOCK,
//            scroll = VK_SCROLL,
//
/////*
//// * NEC PC-9800 kbd definitions
//// */
////#define VK_OEM_NEC_EQUAL  0x92   // '=' key on numpad
////
/////*
//// * Fujitsu/OASYS kbd definitions
//// */
////#define VK_OEM_FJ_JISHO   0x92   // 'Dictionary' key
////#define VK_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key
////#define VK_OEM_FJ_TOUROKU 0x94   // 'Register word' key
////#define VK_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key
////#define VK_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key
////
/////*
//// * 0x97 - 0x9F : unassigned
//// */
////
/////*
//// * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
//// * Used only as parameters to GetAsyncKeyState() and GetKeyState().
//// * No other API or message will distinguish left and right keys in this way.
//// */
////#define VK_LSHIFT         0xA0
////#define VK_RSHIFT         0xA1
////#define VK_LCONTROL       0xA2
////#define VK_RCONTROL       0xA3
////#define VK_LMENU          0xA4
////#define VK_RMENU          0xA5
////
////#if(_WIN32_WINNT >= 0x0500)
////#define VK_BROWSER_BACK        0xA6
////#define VK_BROWSER_FORWARD     0xA7
////#define VK_BROWSER_REFRESH     0xA8
////#define VK_BROWSER_STOP        0xA9
////#define VK_BROWSER_SEARCH      0xAA
////#define VK_BROWSER_FAVORITES   0xAB
////#define VK_BROWSER_HOME        0xAC
////
////#define VK_VOLUME_MUTE         0xAD
////#define VK_VOLUME_DOWN         0xAE
////#define VK_VOLUME_UP           0xAF
////#define VK_MEDIA_NEXT_TRACK    0xB0
////#define VK_MEDIA_PREV_TRACK    0xB1
////#define VK_MEDIA_STOP          0xB2
////#define VK_MEDIA_PLAY_PAUSE    0xB3
////#define VK_LAUNCH_MAIL         0xB4
////#define VK_LAUNCH_MEDIA_SELECT 0xB5
////#define VK_LAUNCH_APP1         0xB6
////#define VK_LAUNCH_APP2         0xB7
////
////#endif /* _WIN32_WINNT >= 0x0500 */
////
/////*
//// * 0xB8 - 0xB9 : reserved
//// */
////
////#define VK_OEM_1          0xBA   // ';:' for US
////#define VK_OEM_PLUS       0xBB   // '+' any country
////#define VK_OEM_COMMA      0xBC   // ',' any country
////#define VK_OEM_MINUS      0xBD   // '-' any country
////#define VK_OEM_PERIOD     0xBE   // '.' any country
////#define VK_OEM_2          0xBF   // '/?' for US
////#define VK_OEM_3          0xC0   // '`~' for US
////
/////*
//// * 0xC1 - 0xD7 : reserved
//// */
////
/////*
//// * 0xD8 - 0xDA : unassigned
//// */
////
////#define VK_OEM_4          0xDB  //  '[{' for US
////#define VK_OEM_5          0xDC  //  '\|' for US
////#define VK_OEM_6          0xDD  //  ']}' for US
////#define VK_OEM_7          0xDE  //  ''"' for US
////#define VK_OEM_8          0xDF
////
/////*
//// * 0xE0 : reserved
//// */
////
/////*
//// * Various extended or enhanced keyboards
//// */
////#define VK_OEM_AX         0xE1  //  'AX' key on Japanese AX kbd
////#define VK_OEM_102        0xE2  //  "<>" or "\|" on RT 102-key kbd.
////#define VK_ICO_HELP       0xE3  //  Help key on ICO
////#define VK_ICO_00         0xE4  //  00 key on ICO
////
////#if(WINVER >= 0x0400)
////#define VK_PROCESSKEY     0xE5
////#endif /* WINVER >= 0x0400 */
////
////#define VK_ICO_CLEAR      0xE6
////
////
////#if(_WIN32_WINNT >= 0x0500)
////#define VK_PACKET         0xE7
////#endif /* _WIN32_WINNT >= 0x0500 */
////
/////*
//// * 0xE8 : unassigned
//// */
////
/////*
//// * Nokia/Ericsson definitions
//// */
////#define VK_OEM_RESET      0xE9
////#define VK_OEM_JUMP       0xEA
////#define VK_OEM_PA1        0xEB
////#define VK_OEM_PA2        0xEC
////#define VK_OEM_PA3        0xED
////#define VK_OEM_WSCTRL     0xEE
////#define VK_OEM_CUSEL      0xEF
////#define VK_OEM_ATTN       0xF0
////#define VK_OEM_FINISH     0xF1
////#define VK_OEM_COPY       0xF2
////#define VK_OEM_AUTO       0xF3
////#define VK_OEM_ENLW       0xF4
////#define VK_OEM_BACKTAB    0xF5
////
////#define VK_ATTN           0xF6
////#define VK_CRSEL          0xF7
////#define VK_EXSEL          0xF8
////#define VK_EREOF          0xF9
////#define VK_PLAY           0xFA
////#define VK_ZOOM           0xFB
////#define VK_NONAME         0xFC
////#define VK_PA1            0xFD
////#define VK_OEM_CLEAR      0xFE
//        } virtual_key;

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
        virtual ~win32_window_behaviour(void);

        /**
         * Destroys the window.
         *
         * @throws the::system:::system_exception In case of an error.
         */
        virtual void destroy(void);

        /**
         * Hides the window.
         */
        virtual void hide(void);

        virtual void invalidate(void);

        /**
         * Moves the window to the designated coordinates.
         *
         * @param x The new left border of the window.
         * @param y The new top border of the window.
         */
        virtual void move(const position_type x, const position_type y);
        
        /**
         * Resizes the window.
         *
         * @param width The new width of the window.
         * @param height The new height of the window.
         */
        virtual void resize(const size_type width, const size_type height);

        /**
         * Shows the window.
         */
        virtual void show(void);

    protected:

        /** Super class typedef. */
        typedef the::not_copyable base;

        /** The instance handle, which is acquired in one_time_initialise(). */
        static HINSTANCE hInstance;

        /**
         * Initialises a new instance, but does not yet allocate the system
         * resources for the window.
         */
        win32_window_behaviour(void) throw();

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
         * This method is invoked if the window receives a character event.
         *
         * @param c The character that has been typed.
         */
        virtual void on_char(const native_char_type c) throw();

        /**
         * This method is invoked if the window is closed.
         */
        virtual void on_close(void) throw();

        /**
         * The class will call this method immediately before allocating the
         * system resources for the window. Subclasses can use this method to
         * influence the style of the window that is to be created.
         *
         * @param inOutStyle   The window style that will be applied.
         * @param inOutStyleEx The extended window style that will be applied.
         */
        virtual void on_initialising(DWORD& inOutStyle,
            DWORD& inOutStyleEx) throw();

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
         * @param msg
         * @param wParam
         * @param lParam
         *
         * @return
         */
        virtual LRESULT on_native_message(bool& outHandled, UINT msg,
            WPARAM wParam, LPARAM lParam) throw();

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
         * Callback function that processes events of a window.
         *
         * @param hWnd
         * @param msg
         * @param wParam
         * @param lParam
         *
         * @return
         */
        static LRESULT WINAPI wndProc(HWND hWnd, UINT msg, WPARAM wParam,
            LPARAM lParam);

        /** 
         * The name of the window class we register in the 
         * one_time_initialise() method.
         */
        static const system::system_char *window_class_name;
    };

} /* end namespace graphics */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_GRAPHICS_WIN32_WINDOW_BEHAVIOUR_H_INCLUDED */
