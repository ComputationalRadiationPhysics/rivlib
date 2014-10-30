/*
 * test_render_window.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#include "stdafx.h"
//#ifdef THE_WINDOWS
#define GLH_EXT_SINGLE_FILE 1
//#endif
#include "glh/glh_extensions.h"
#include "test_render_window.h"
//#include <GL/gl.h>
#include "AntTweakBar.h"
#include "GL/freeglut.h"
#include "rivlib/raw_image_data_binding.h"
#include "the/text/string_builder.h"

using namespace eu_vicci;
using namespace eu_vicci::riv_prov_test;


/*
 * test_render_window::flare::flare
 */
test_render_window::flare::flare(void)
        : x(0.5f), y(0.0f), h(0.1f), w(1.0f), sx(1.0f), r(255), g(255), b(255) {
    this->reset_to_random();
    this->x -= this->sx * this->frand();
}


/*
 * test_render_window::flare::~flare
 */
test_render_window::flare::~flare(void) {
    // intentionally empty
}


/*
 * test_render_window::flare::draw
 */
void test_render_window::flare::draw(void) {

    ::glBegin(GL_QUAD_STRIP);

    ::glColor4ub(this->r, this->g, this->b, 255u);
    ::glVertex2f(this->x - this->h, this->y - this->h);

    ::glColor4ub(this->r, this->g, this->b, 0u);
    ::glVertex2f(this->x - this->h - this->w, this->y - this->h);

    ::glColor4ub(this->r, this->g, this->b, 255u);
    ::glVertex2f(this->x, this->y);

    ::glColor4ub(this->r, this->g, this->b, 0u);
    ::glVertex2f(this->x - this->w, this->y);

    ::glColor4ub(this->r, this->g, this->b, 255u);
    ::glVertex2f(this->x - this->h, this->y + this->h);

    ::glColor4ub(this->r, this->g, this->b, 0u);
    ::glVertex2f(this->x - this->h - this->w, this->y + this->h);

    ::glEnd();
}


/*
 * test_render_window::flare::update
 */
void test_render_window::flare::update(float time) {
    this->x += this->sx * time;
}


/*
 * test_render_window::flare::reset_to_random
 */
void test_render_window::flare::reset_to_random(void) {
    this->x = -1.0f - /*2.0f * */this->frand();
    this->y = -1.0f + 2.0f * this->frand();
    this->h = 0.01f + 0.09f * this->frand();
    this->sx = 2.5f + 2.5f * this->frand();
    this->w = 0.25f * this->sx;
    this->r = static_cast<unsigned char>(255.0f * this->frand());
    this->g = static_cast<unsigned char>(255.0f * this->frand());
    this->b = static_cast<unsigned char>(255.0f * this->frand());
}


/*
 * test_render_window::flare::frand
 */
float test_render_window::flare::frand(void) {
    return static_cast<float>(::rand()) / static_cast<float>(RAND_MAX);
}

/****************************************************************************/


/*
 * test_render_window::test_render_window
 */
test_render_window::test_render_window(void) : glut_window("RIV Provider Test"),
        counter(), provider(), img_binding(), view_rot(0.0f) {
    this->counter.start();
}


/*
 * test_render_window::~test_render_window
 */
test_render_window::~test_render_window(void) {
    this->img_binding.reset();
    if (this->provider) {
        this->provider->remove_user_message_callback(&test_render_window::on_user_message_proxy, this);
        this->provider->shutdown();
        this->provider.reset();
    }
}


/*
 * test_render_window::set_rivlib_provider
 */
void test_render_window::set_rivlib_provider(rivlib::provider::ptr p) {
    this->img_binding.reset();
    if (this->provider) {
        this->provider->remove_user_message_callback(&test_render_window::on_user_message_proxy, this);
        this->provider->shutdown();
        this->provider.reset();
    }
    this->provider = p;

    if (this->provider) {
        this->provider->add_user_message_callback(&test_render_window::on_user_message_proxy, this);
        if (!this->pixdata.is_empty()) {
            this->img_binding = rivlib::raw_image_data_binding::create(
                this->pixdata, this->get_width(), this->get_height(),
                rivlib::image_colour_type::rgb, rivlib::image_data_type::byte,
                rivlib::image_orientation::bottom_up, this->get_width() * 3);
            THE_ASSERT(this->img_binding);
            this->provider->add_data_binding(this->img_binding);
        }
    }

}


/*
 * test_render_window::on_display
 */
void test_render_window::on_display(void) {

    this->on_display_begin();

    ::glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ::glEnable(GL_BLEND);
    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ::glMatrixMode(GL_MODELVIEW);
    ::glLoadIdentity();
    ::glRotatef(this->view_rot, 0.0f, 0.0f, 1.0f);

    double d_time = this->counter.elapsed_milliseconds();
    float f_time = static_cast<float>(d_time / 1000.0);
    this->counter.start();

    for (int i = 0; i < FLARE_COUNT; i++) {

        this->flares[i].update(f_time);
        if (this->flares[i].get_left() > 1.0f) {
            this->flares[i].reset_to_random();
        }
        this->flares[i].draw();

    }

    ::glFlush();

    if (this->img_binding) {
        this->img_binding->wait_async_data_completed();
    }

    //
    // small note here:
    //    GLUT is lying about the window size (at least under W8)
    //    Thus a small grey border will appear on the right and bottom of the image data.
    //    Instead of grey, it might also be image garbage from larger window sizes.
    //    Since this is a very simple test, we will ignore it for now.
    //    The final application should use a FBO to avoid this problem.
    //
    ::glReadBuffer(GL_BACK);
    ::glPixelStorei(GL_PACK_ALIGNMENT, 1);
    ::glReadPixels(0, 0, this->get_width(), this->get_height(), GL_RGB, GL_UNSIGNED_BYTE, this->pixdata);
    ::glPixelStorei(GL_PACK_ALIGNMENT, 4);

    if (this->img_binding) {
        try {
            this->img_binding->async_data_available();
        } catch(...) {
            fprintf(stderr, "Synchronization problems\n");
            // TODO: Should not happen!
            // This is a known issue in the current implementation, as on disconnect of the client, the corresponding encoder currently remains attached to the provider
            // (which also may result in degraded performance and memory leaks)
        }
    }

    this->on_display_end();

}


/*
 * test_render_window::on_reshape
 */
void test_render_window::on_reshape(int width, int height) {
    glut_window::on_reshape(width, height);

    if (this->img_binding) {
        this->img_binding->wait_async_data_abort();
        this->provider->remove_data_binding(this->img_binding);
        this->img_binding.reset();
    }

    this->pixdata.assert_size(this->get_width() * this->get_height() * 3 * 1);

    if (this->provider) {
        if (!this->pixdata.is_empty()) {
            this->img_binding = rivlib::raw_image_data_binding::create(
                this->pixdata, this->get_width(), this->get_height(),
                rivlib::image_colour_type::rgb, rivlib::image_data_type::byte,
                rivlib::image_orientation::bottom_up, this->get_width() * 3);
            THE_ASSERT(this->img_binding);
            this->provider->add_data_binding(this->img_binding);
        }
    }

}


/*
 * test_render_window::on_keyboard
 */
bool test_render_window::on_keyboard(unsigned char key, int x, int y) {
    if (glut_window::on_keyboard(key, x, y)) return true;

    switch (key) {
    case 27: // esc
        this->close();
        return true;
    }

    return false;

}


/*
 * test_render_window::on_user_message_proxy
 */
void test_render_window::on_user_message_proxy(unsigned int id, unsigned int size, const char* data, void *ctxt) {
    static_cast<test_render_window*>(ctxt)->on_user_message(id, size, data);
}


/*
 * test_render_window::on_user_message
 */
void test_render_window::on_user_message(unsigned int id, unsigned int size, const char* data) {
    switch (id) {

    case RIVLIB_USERMSG + 100: // rotate
        if (size >= 4) {
            this->view_rot -= reinterpret_cast<const float*>(data)[0];
            if (this->view_rot < 0.0f) {
                this->view_rot += 360.0f * ceil(-this->view_rot / 360.0f);
            }
            if (this->view_rot > 360.0f) {
                this->view_rot -= 360.0f * floor(this->view_rot / 360.0f);
            }
        }
        break;

    case RIVLIB_USERMSG + 101: { // Get Info

        double d_time = this->counter.query_millis();
        float f_time = static_cast<float>(d_time / 1000.0);

        std::string str = the::text::astring_builder::format("Server up time: %.2f", f_time);

        const char *data = str.c_str();
        unsigned int len = str.length(); // excluding zero terminator

        this->provider->broadcast_message(RIVLIB_USERMSG + 102, len, data);

    } break;

    }

}
