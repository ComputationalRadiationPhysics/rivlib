/*
 * test_render_window.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVPROVTEST_TEST_RENDER_WINDOW_H_INCLUDED
#define VICCI_RIVPROVTEST_TEST_RENDER_WINDOW_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "glut_window.h"
#include "the/blob.h"
#include "the/system/performance_counter.h"
#include "rivlib/provider.h"
#include "rivlib/raw_image_data_binding.h"


namespace eu_vicci {
namespace riv_prov_test {

    /**
     * Window implementing the rendering test provider
     */
    class test_render_window : public glut_window {
    public:

        /** Ctor */
        test_render_window(void);

        /** Dtor */
        virtual ~test_render_window(void);

        /**
         * Sets the rivlib provider object
         *
         * @param p The rivlib provider object
         */
        void set_rivlib_provider(rivlib::provider::ptr p);

    protected:

        /** The display callback to render the window content */
        virtual void on_display(void);

        /**
         * The reshape callback to handle window resize events
         *
         * @param width The new window width
         * @param height The new window height
         */
        virtual void on_reshape(int width, int height);

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

    private:

        /** The number of flares */
        static const unsigned int FLARE_COUNT = 32;

        /** Nested flare type */
        class flare {
        public:

            /** ctor */
            flare(void);

            /** dtor */
            ~flare(void);

            /**
             * Answer the left-most coordinate of the flare
             *
             * @return The left-most coordinate of the flare
             */
            inline float get_left(void) const {
                return this->x - this->w - this->h;
            }

            /** draws the flare */
            void draw(void);

            /**
             * Updates the flare
             *
             * @param time The secondes passed since the last call of 'update'
             */
            void update(float time);

            /**
             * Resets the flare to random starting conditions
             */
            void reset_to_random(void);

        private:

            /**
             * Answer a random number [0..1]
             *
             * @return A random number
             */
            inline float frand(void);

            /** The x coordinate */
            float x;

            /** The y coordinate */
            float y;

            /** The height */
            float h;

            /** The width */
            float w;

            /** The speed in x direction */
            float sx;

            /** The rgb colour */
            unsigned char r, g, b;

        };

        /**
         * A user message has been received
         *
         * @param id The id of the message (should normally be a user message using RIVLIB_USERMSG)
         * @param size The size of the message data in byte
         * @param data The message data
         * @param ctxt The callback function context
         */
        static void on_user_message_proxy(unsigned int id, unsigned int size, const char* data, void *ctxt);

        /**
         * A user message has been received
         *
         * @param id The id of the message (should normally be a user message using RIVLIB_USERMSG)
         * @param size The size of the message data in byte
         * @param data The message data
         */
        void on_user_message(unsigned int id, unsigned int size, const char* data);

        /** A performance counter to measure the frame rate */
        the::system::performance_counter counter;

        /** The flares */
        flare flares[FLARE_COUNT];

        /** the pixel data */
        the::blob pixdata;

        /** The rivlib provider object */
        rivlib::provider::ptr provider;

        /** The rivlib image binding */
        rivlib::raw_image_data_binding::ptr img_binding;

        /** The view rotation */
        float view_rot;

    };


} /* end namespace riv_prov_test */
} /* end namespace eu_vicci */

#endif /* VICCI_RIVPROVTEST_GLUT_WINDOW_H_INCLUDED */
