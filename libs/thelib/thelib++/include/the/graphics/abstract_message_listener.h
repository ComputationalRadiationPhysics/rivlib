/*
 * include\the\graphics\abstract_message_listener.h
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

#ifndef THE_GRAPHICS_ABSTRACT_MESSAGE_LISTENER_H_INCLUDED
#define THE_GRAPHICS_ABSTRACT_MESSAGE_LISTENER_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/stack_trace.h"


namespace the {
namespace graphics {

    /**
     * This class (i) defines the message listener policy of THElib and (ii)
     * provides a basic implementation for it.
     *
     * All public methods that are provided in the class form the minimal 
     * interface of the message listener policy. The listener must include
     * all methods starting on_... The typedefs are not part of the policy.
     *
     * The intention of this class is to provide a basic abstraction like GLUT,
     * but in an object-oriented manner and with the possibility to get access
     * to the native events themselves.
     *
     * @tparam S The type of the source that reports the events. This type must
     *           expose the window policy of THElib and is therefore usually a
     *           THElib window class.
     */
    template<class S>
    class abstract_message_listener {

    public:

        /** The native character type. */
        typedef typename S::native_char_type native_char_type;

        /** The type of native key codes. */
        typedef typename S::native_virtual_key_type native_virtual_key_type;

        ///** The type of native window messages. */
        //typedef typename S::native_message_type native_message_type;

        /** The type for position in the interface. */
        typedef typename S::position_type position_type;

        /** The type of the message originator. */
        typedef S source_type;

        /** The type for size in the interface. */
        typedef typename S::position_type size_type;

        /** Dtor. */
        virtual ~abstract_message_listener(void);

        /**
         * This method is invoked if the source received a character input.
         *
         * This method is part of the message listener policy.
         *
         * The default implementation does nothing.
         *
         * @param src The source that invoked the method.
         * @param chr The character that was received.
         */
        virtual void on_char(const source_type& src,
            const native_char_type chr);

        /**
         * This method is invoked if the source received a window message.
         *
         * This method is part of the message listener policy.
         *
         * The default implementation returns true.
         *
         * @param src The source that invoked the method.
         * @param msg The native window message that was received.
         *
         * @return true in order to indicate to the source that it should
         *         continue receiving messages, false to indicate that the
         *         source should stop delivering messages.
         */
        //virtual bool on_message(const source_type& src,
        //    const native_message_type& msg);

        /**
         * This method is invoked if the source received a key down message.
         *
         * This method is part of the message listener policy.
         *
         * The default implementation does nothing.
         *
         * @param src The source that invoked the method.
         * @param key The virtual key code of the key that was pressed. The
         *            semantics of this code is implementation-dependent.
         */
        virtual void on_key_down(const source_type& src,
            const native_virtual_key_type& key);

        /**
         * This method is invoked if the source received a key up message.
         *
         * This method is part of the message listener policy.
         *
         * The default implementation does nothing.
         *
         * @param src The source that invoked the method.
         * @param key The virtual key code of the key that was released. The
         *            semantics of this code is implementation-dependent.
         */
        virtual void on_key_up(const source_type& src,
            const native_virtual_key_type& key);

        /**
         * This method is invoked if the source received a mouse down message.
         *
         * This method is part of the message listener policy.
         *
         * The default implementation does nothing.
         *
         * @param src    The source that invoked the method.
         * @param button The zero-based index of the mouse button that was
         *               pressed with zero being the left button.
         */
        virtual void on_mouse_down(const source_type& src,
            const int button);

        /**
         * This method is invoked if the source detected a mouse motion.
         *
         * This method is part of the message listener policy.
         *
         * The default implementation does nothing.
         *
         * @param src The source that invoked the method.
         * @param x   The abscissa of the mouse position.
         * @param y   The ordinate of the mouse position.
         */
        virtual void on_mouse_move(const source_type& src,
            const position_type x, const position_type y);

        /**
         * This method is invoked if the source received a mouse down message.
         *
         * This method is part of the message listener policy.
         *
         * The default implementation does nothing.
         *
         * @param src    The source that invoked the method.
         * @param button The zero-based index of the mouse button that was
         *               released with zero being the left button.
         */
        virtual void on_mouse_up(const source_type& src,
            const int button);

        /**
         * This method is invoked if the source was resized.
         *
         * This method is part of the message listener policy.
         *
         * The default implementation does nothing.
         *
         * @param src    The source that invoked the method.
         * @param width  The new width of the source.
         * @param height The new height of the source.
         */
        virtual void on_resize(const source_type& src, const size_type width,
            const size_type height);

    protected:

        /**
         * Initialises a new instance.
         */
        abstract_message_listener(void);
    };

} /* end namespace graphics */
} /* end namespace the */


/*
 * the::graphics::abstract_message_listener<S>::~abstract_message_listener
 */
template<class S>
the::graphics::abstract_message_listener<S>::~abstract_message_listener(void) {
    THE_STACK_TRACE;
}


/*
 * the::graphics::abstract_message_listener<S>::on_char
 */
template<class S>
void the::graphics::abstract_message_listener<S>::on_char(
        const source_type& src, const native_char_type chr) {
    THE_STACK_TRACE;
}


/*
 * the::graphics::abstract_message_listener<M, K, S>::on_message
 */
//template<class S>
//bool the::graphics::abstract_message_listener<S>::on_message(
//        const source_type& src, const native_message_type& msg) {
//    THE_STACK_TRACE;
//    return true;
//}


/*
 * the::graphics::abstract_message_listener<S>::on_key_down
 */
template<class S>
void the::graphics::abstract_message_listener<S>::on_key_down(
        const source_type& src, const native_virtual_key_type& key) {
    THE_STACK_TRACE;
}


/*
 * the::graphics::abstract_message_listener<S>::on_key_up
 */
template<class S>
void the::graphics::abstract_message_listener<S>::on_key_up(
        const source_type& src, const native_virtual_key_type& key) {
    THE_STACK_TRACE;
}


/*
 * the::graphics::abstract_message_listener<S>::on_mouse_down
 */
template<class S>
void the::graphics::abstract_message_listener<S>::on_mouse_down(
        const source_type& src, const int button) {
    THE_STACK_TRACE;
}


/*
 * the::graphics::abstract_message_listener<S>::on_mouse_move
 */
template<class S>
void the::graphics::abstract_message_listener<S>::on_mouse_move(
        const source_type& src, const position_type x, const position_type y) {
    THE_STACK_TRACE;
}


/*
 * the::graphics::abstract_message_listener<S>::on_mouse_up
 */
template<class S>
void the::graphics::abstract_message_listener<S>::on_mouse_up(
        const source_type& src, const int button) {
    THE_STACK_TRACE;
}


/*
 * the::graphics::abstract_message_listener<S>::on_resize
 */
template<class S>
void the::graphics::abstract_message_listener<S>::on_resize(
        const source_type& src, const size_type width, const size_type height) {
    THE_STACK_TRACE;
}


/*
 * the::graphics::abstract_message_listener<S>::abstract_message_listener
 */
template<class S>
the::graphics::abstract_message_listener<S>::abstract_message_listener(void) {
    THE_STACK_TRACE;
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_GRAPHICS_ABSTRACT_MESSAGE_LISTENER_H_INCLUDED */
