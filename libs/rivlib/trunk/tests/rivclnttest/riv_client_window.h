/*
 * riv_client_window.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVCLNTTEST_RIV_CLIENT_WINDOW_H_INCLUDED
#define VICCI_RIVCLNTTEST_RIV_CLIENT_WINDOW_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "glut_window.h"
#include "AntTweakBar.h"
#include "the/delegate.h"
#include "the/system/threading/critical_section.h"
#include "rivlib/control_connection.h"
#include "rivlib/image_stream_connection.h"
#include "the/blob.h"
#include "vislib/OpenGLTexture2D.h"


namespace eu_vicci {
namespace riv_clnt_test {

    /**
     * Reference implementation of a RIV client window
     */
    class riv_client_window : public glut_window,
        public rivlib::control_connection::listener,
        public rivlib::image_stream_connection::listener {
    public:

        /** Ctor */
        riv_client_window(void);

        /** Dtor */
        virtual ~riv_client_window(void);

        /**
         * Sets the status message
         *
         * @param msg The new status message
         */
        void set_status(const char *msg);

        /**
         * Sets the uri
         *
         * @param uri The uri
         */
        void set_uri(const char *uri);

        /**
         * Enables the auto_connect flag
         */
        inline void enable_auto_connect(void) {
            this->auto_connect = true;
        }

        /**
         * Called when the control_connection has been connected to a
         * provider
         *
         * @param comm The calling control_connection
         */
        virtual void on_connected(rivlib::control_connection::ptr comm) throw();

        /**
         * Called when the control_connection has been disconnected from a
         * provider
         *
         * @param comm The calling control_connection
         */
        virtual void on_disconnected(rivlib::control_connection::ptr comm) throw();

        /**
         * Called when an error occured. If the error occured while
         * establishing a connection, the connection attempts are
         * canceled.
         *
         * @param comm The calling control_connection
         * @param msg The error message
         */
        virtual void on_error(rivlib::control_connection::ptr comm, const char *msg) throw();

        /**
         * Called when a (non-internal) message has been received from the
         * provider
         *
         * @param comm The calling control_connection
         * @param id The message id
         * @param size The message data size (in bytes)
         * @param data The message data
         */
        virtual void on_msg(rivlib::control_connection::ptr comm, 
            unsigned int id, unsigned int size, const void *data) throw();

        /**
         * Called when the control_connection has been connected to a
         * provider
         *
         * @param comm The calling control_connection
         */
        virtual void on_connected(rivlib::image_stream_connection::ptr comm) throw();

        /**
         * Called when the control_connection has been disconnected from a
         * provider
         *
         * @param comm The calling control_connection
         */
        virtual void on_disconnected(rivlib::image_stream_connection::ptr comm) throw();

        /**
         * Called when an error occured. If the error occured while
         * establishing a connection, the connection attempts are
         * canceled.
         *
         * @param comm The calling control_connection
         * @param msg The error message
         */
        virtual void on_error(rivlib::image_stream_connection::ptr comm, const char *msg) throw();

        /**
         * Called upon new incoming image data
         *
         * @param comm The calling object
         * @param width The width of the image in pixel
         * @param height The height of the image in pixel
         * @param rgbpix The uncompressed rgb pixel data
         */
        virtual void on_image_data(rivlib::image_stream_connection::ptr comm, uint32_t width, uint32_t height, const void* rgbpix) throw();

    protected:

        /** The display callback to render the window content */
        virtual void on_display(void);

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

        /** The glut close callback whenever the window is closed */
        virtual void on_close(void);

        /**
         * The reshape callback to handle window resize events
         *
         * @param width The new window width
         * @param height The new window height
         */
        virtual void on_reshape(int width, int height);

        /** Glut idle callback called whenever all messages from the message queue are completed */
        virtual void on_idle(void);

    private:

        /** Called when the 'connect' button is clicked */
        void on_connect_clicked(void);

        /** Called when the 'disconnect' button is clicked */
        void on_disconnect_clicked(void);

        /** Updates the status label text based on the connection status of the rivlib::control_connection */
        void on_ctrl_connection_status_changed(void);

        /** Called when the 'rotate' button is clicked */
        void on_rotate_clicked(void);

        /** Called when the 'get info' button is clicked */
        void on_get_info_clicked(void);

        /** The tweak bar */
        TwBar* bar;

        /** The connection uri */
        std::string uri;

        /** The auto_connect flag */
        bool auto_connect;

        /** The on connect clicked delegate */
        the::delegate<void> on_connect_clicked_delegate;

        /** The on disconnect clicked delegate */
        the::delegate<void> on_disconnect_clicked_delegate;

        /** The control connection */
        rivlib::control_connection::ptr ctrl;

        /** The image stream connection */
        rivlib::image_stream_connection::ptr img_strm;

        /** The input image data thread-lock */
        the::system::threading::critical_section in_img_buf_lock;

        /** The input image data */
        the::blob in_img_dat;

        /** The input image width */
        uint32_t in_img_width;

        /** The input image height */
        uint32_t in_img_height;

        /** The input image texture */
        vislib::graphics::gl::OpenGLTexture2D tex;

        /** The width of the texture image in pixel */
        uint32_t tex_width;

        /** The height of the texture image in pixel */
        uint32_t tex_height;

        /** The on disconnect clicked delegate */
        the::delegate<void> on_rotate_clicked_delegate;

        /** The on disconnect clicked delegate */
        the::delegate<void> on_get_info_clicked_delegate;

    };

} /* end namespace riv_clnt_test */
} /* end namespace eu_vicci */

#endif /* VICCI_RIVPROVTEST_GLUT_WINDOW_H_INCLUDED */
