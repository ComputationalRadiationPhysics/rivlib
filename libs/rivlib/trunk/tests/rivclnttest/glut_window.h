/*
 * glut_window.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVCLNTTEST_GLUT_WINDOW_H_INCLUDED
#define VICCI_RIVCLNTTEST_GLUT_WINDOW_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */


namespace eu_vicci {
namespace riv_clnt_test {

    /**
     * Class managing a glut window
     */
    class glut_window {
    public:

        /**
         * Ctor
         *
         * @param caption The window caption
         */
        glut_window(const char* caption);

        /** Dtor */
        virtual ~glut_window(void);

        /** Closes the window */
        void close(void);

        /**
         * Answers the width of the window
         *
         * @return the width of the window
         */
        inline const int& get_width(void) const {
            return this->width;
        }

        /**
         * Answers the height of the window
         *
         * @return the height of the window
         */
        inline const int& get_height(void) const {
            return this->height;
        }

    protected:

        /** The display callback to render the window content */
        virtual void on_display(void);

        /** call when drawing of the scene starts */
        virtual void on_display_begin(void);

        /** call when drawing of the scene is completed */
        virtual void on_display_end(void);

        /**
         * The reshape callback to handle window resize events
         *
         * @param width The new window width
         * @param height The new window height
         */
        virtual void on_reshape(int width, int height);

        /**
         * The mouse button callback to handle pressing and releasing mouse buttons
         *
         * @param button The mouse button
         * @param state The state of the mouse buttons
         * @param x The x coordinate of the mouse cursor
         * @param y The y coordinate of the mouse cursor
         *
         * @return True if the event has been consumed, False otherwise
         */
        virtual bool on_mouse_button(int button, int state, int x, int y);

        /**
         * The glut motion callback to handle mouse motions
         *
         * @param x The x coordinate of the mouse cursor
         * @param y The y coordinate of the mouse cursor
         *
         * @return True if the event has been consumed, False otherwise
         */
        virtual bool on_mouse_motion(int x, int y);

        /**
         * The glut keyboard callback to handle key presses
         *
         * @param key The keyboard key pressed
         * @param x The x coordinate of the mouse cursor
         * @param y The y coordinate of the mouse cursor
         *
         * @return True if the event has been consumed, False otherwise
         */
        virtual bool on_keyboard(unsigned char key, int x, int y);

        /**
         * The glut keyboard callback to handle key presses
         *
         * @param key The keyboard key pressed
         * @param x The x coordinate of the mouse cursor
         * @param y The y coordinate of the mouse cursor
         *
         * @return True if the event has been consumed, False otherwise
         */
        virtual bool on_special_keyboard(int key, int x, int y);

        /** Glut idle callback called whenever all messages from the message queue are completed */
        virtual void on_idle(void);

        /** The glut close callback whenever the window is closed */
        virtual void on_close(void);

        /**
         * Gets the glut window id 
         *
         * @return The glut window id
         */
        inline int get_wnd_id(void) const {
            return this->wnd_id;
        }

    private:

        /** The glut display callback to render the window content */
        static void glut_display(void);

        /**
         * The glut reshape callback to handle window resize events
         *
         * @param width The new window width
         * @param height The new window height
         */
        static void glut_reshape(int width, int height);

        /**
         * The glut mouse button callback to handle pressing and releasing mouse buttons
         *
         * @param button The mouse button
         * @param state The state of the mouse buttons
         * @param x The x coordinate of the mouse cursor
         * @param y The y coordinate of the mouse cursor
         */
        static void glut_mouse_button(int button, int state, int x, int y);

        /**
         * The glut motion callback to handle mouse motions
         *
         * @param x The x coordinate of the mouse cursor
         * @param y The y coordinate of the mouse cursor
         */
        static void glut_mouse_motion(int x, int y);

        /**
         * The glut keyboard callback to handle key presses
         *
         * @param key The keyboard key pressed
         * @param x The x coordinate of the mouse cursor
         * @param y The y coordinate of the mouse cursor
         */
        static void glut_keyboard(unsigned char key, int x, int y);

        /**
         * The glut keyboard callback to handle key presses
         *
         * @param key The keyboard key pressed
         * @param x The x coordinate of the mouse cursor
         * @param y The y coordinate of the mouse cursor
         */
        static void glut_special_keyboard(int key, int x, int y);

        /** Glut idle callback called whenever all messages from the message queue are completed */
        static void glut_idle(void);

        /** The glut close callback whenever the window is closed */
        static void glut_close(void);

        /** The number of windows */
        static unsigned int wnd_cnt;

        /** The glut window id */
        int wnd_id;

        /** Flag whether or not this window is open */
        bool wnd_opened;

        /** The width of the window */
        int width;

        /** The height of the window */
        int height;

    };

} /* end namespace riv_clnt_test */
} /* end namespace eu_vicci */

#endif /* VICCI_RIVPROVTEST_GLUT_WINDOW_H_INCLUDED */
