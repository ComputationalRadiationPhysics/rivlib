/*
 * glut_window.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#include "stdafx.h"
//#ifdef THE_WINDOWS
#define GLH_EXT_SINGLE_FILE 1
//#endif
#include "glh/glh_extensions.h"
#include "riv_client_window.h"
//#include <GL/gl.h>
#include "AntTweakBar.h"
#include "the/text/string_builder.h"
#include "the/math/functions.h"
#include "rivlib/ip_communicator.h"
#include "rivlib/ip_utilities.h"
#include "the/system/threading/auto_lock.h"
#include "the/memory.h"

using namespace eu_vicci;
using namespace eu_vicci::riv_clnt_test;


/**
 * calls a delegate
 *
 * @param clientData Pointer to the delegate to be called
 */
static void TW_CALL riv_clnt_test_call_delegate(void *clientData) {
    the::delegate<void> *del = static_cast<the::delegate<void>*>(clientData);
    (*del)();
}


/*
 * riv_client_window::riv_client_window
 */
riv_client_window::riv_client_window(void) : glut_window("RIV Client"),
        rivlib::control_connection::listener(), rivlib::image_stream_connection::listener(), bar(nullptr),
        uri(the::text::astring_builder::format("riv://localhost:%u/riv%%20test", rivlib::ip_communicator::DEFAULT_PORT)),
        auto_connect(false), on_connect_clicked_delegate(), ctrl(nullptr), img_strm(nullptr), in_img_buf_lock(),
        in_img_dat(), in_img_width(0), in_img_height(0), tex(), tex_width(0), tex_height(0),
        on_rotate_clicked_delegate(), on_get_info_clicked_delegate() {

    this->on_connect_clicked_delegate = the::delegate<>(*this, &riv_client_window::on_connect_clicked);
    this->on_disconnect_clicked_delegate = the::delegate<>(*this, &riv_client_window::on_disconnect_clicked);
    this->on_rotate_clicked_delegate = the::delegate<>(*this, &riv_client_window::on_rotate_clicked);
    this->on_get_info_clicked_delegate = the::delegate<>(*this, &riv_client_window::on_get_info_clicked);

    ::TwSetCurrentWindow(this->get_wnd_id());
    this->bar = ::TwNewBar("tweakbar");
    ::TwDefine("tweakbar label='RIV Client' size='350 600' position='10 10' contained=true valueswidth=fit");
#ifdef _WIN32
    the::text::astring_builder set("tweakbar ");
    {
        DWORD col = ::GetSysColor(COLOR_INACTIVECAPTION);
        unsigned char r = GetRValue(col);
        unsigned char g = GetGValue(col);
        unsigned char b = GetBValue(col);
        set.append_formatted(" color='%u %u %u'", r, g, b);
    }
    {
        DWORD col = ::GetSysColor(COLOR_INACTIVECAPTIONTEXT);
        float r = static_cast<float>(GetRValue(col));
        float g = static_cast<float>(GetGValue(col));
        float b = static_cast<float>(GetBValue(col));
        if ((r + g + b / 3.0f) >= 128.0f) {
            set.append(" text=light");
        } else {
            set.append(" text=dark");
        }
    }
    set.append(" alpha=192");

    ::TwDefine(set.to_string().c_str());
#else /* _WIN32 */
    ::TwDefine("tweakbar color='220 220 220' alpha=192 text=dark");
#endif /* _WIN32 */

    ::TwAddButton(this->bar, "status", NULL, NULL, "label='Status'");
    ::TwAddSeparator(this->bar, "", "");
    ::TwAddVarRW(this->bar, "uri", TW_TYPE_STDSTRING, &this->uri, nullptr);
    ::TwAddButton(this->bar, "connect", riv_clnt_test_call_delegate, &this->on_connect_clicked_delegate, "label='connect'");
    ::TwAddButton(this->bar, "disconnect", riv_clnt_test_call_delegate, &this->on_disconnect_clicked_delegate, "label='disconnect'");
    ::TwAddSeparator(this->bar, NULL, NULL);
    ::TwAddButton(this->bar, "rotate", riv_clnt_test_call_delegate, &this->on_rotate_clicked_delegate, "label='Rotate'");
    ::TwAddSeparator(this->bar, NULL, NULL);
    ::TwAddButton(this->bar, "getInfo", riv_clnt_test_call_delegate, &this->on_get_info_clicked_delegate, "label='Get Info'");
    ::TwAddButton(this->bar, "infoLabel", NULL, NULL, "label=' '");
    ::TwAddSeparator(this->bar, NULL, NULL);

    this->ctrl = rivlib::control_connection::create();
    if (!this->ctrl) {
        throw the::exception("rivlib control_connection could not be created", __FILE__, __LINE__);
    }
    this->ctrl->add_listener(this);

    this->img_strm = rivlib::image_stream_connection::create();
    if (!this->img_strm) {
        throw the::exception("rivlib image_stream_connection could not be created", __FILE__, __LINE__);
    }
    this->img_strm->add_listener(this);

    this->on_ctrl_connection_status_changed();

    ::glEnable(GL_TEXTURE_2D);
    unsigned char demoTex[] = {
        0, 0, 0,
        255, 0, 0,
        0, 255, 0,
        255, 255, 255 };
    if (this->tex.Create(2, 2, false, demoTex, GL_RGB, GL_UNSIGNED_BYTE, GL_RGB, 0) != GL_NO_ERROR) {
        throw the::exception("Unable to initialize texture", __FILE__, __LINE__);
    }
    this->tex_width = 2;
    this->tex_height = 2;
    this->tex.Bind();
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    ::glBindTexture(GL_TEXTURE_2D, 0);
    ::glDisable(GL_TEXTURE_2D);

}


/*
 * riv_client_window::~riv_client_window
 */
riv_client_window::~riv_client_window(void) {
    this->img_strm->remove_listener(this);
    if (this->img_strm->get_status() != rivlib::control_connection::status::not_connected) {
        this->img_strm->disconnect();
    }
    this->img_strm.reset();

    this->ctrl->remove_listener(this);
    if (this->ctrl->get_status() != rivlib::control_connection::status::not_connected) {
        this->ctrl->disconnect();
    }
    this->ctrl.reset();

    this->tex.Release();
}


/*
 * riv_client_window::set_status
 */
void riv_client_window::set_status(const char *msg) {
    the::astring str;
    if (msg == nullptr) {
        str = "tweakbar/status label='Status'";
    } else {
        the::text::astring_builder::format_to(str, "tweakbar/status label='%s'", msg);
    }
    ::TwDefine(str.c_str());
}


/*
 * riv_client_window::set_uri
 */
void riv_client_window::set_uri(const char *uri) {
    this->uri = uri;
}


/*
 * riv_client_window::on_connected
 */
void riv_client_window::on_connected(rivlib::control_connection::ptr comm) throw() {
    this->on_ctrl_connection_status_changed();
    printf("Ctrl connected.\n");

    // connected, now query available data channels
    this->ctrl->send(
        static_cast<unsigned int>(rivlib::message_id::query_data_channels),
        0, nullptr);

}


/*
 * riv_client_window::on_disconnected
 */
void riv_client_window::on_disconnected(rivlib::control_connection::ptr comm) throw() {
    this->on_ctrl_connection_status_changed();
    printf("Ctrl disconnected.\n");

    try {
        this->img_strm->disconnect(true);
    } catch(...) {
    }

}


/*
 * riv_client_window::on_error
 */
void riv_client_window::on_error(rivlib::control_connection::ptr comm, const char *msg) throw() {
    fprintf(stderr, "\nCtrl connection error: %s\n\n", msg);
    this->on_ctrl_connection_status_changed();
}


/*
 * riv_client_window::on_connected
 */
void riv_client_window::on_connected(rivlib::image_stream_connection::ptr comm) throw() {
    this->on_ctrl_connection_status_changed();
    printf("Image stream connected.\n");

}


/*
 * riv_client_window::on_disconnected
 */
void riv_client_window::on_disconnected(rivlib::image_stream_connection::ptr comm) throw() {
    this->on_ctrl_connection_status_changed();
    printf("Image stream disconnected.\n");

}


/*
 * riv_client_window::on_error
 */
void riv_client_window::on_error(rivlib::image_stream_connection::ptr comm, const char *msg) throw() {
    fprintf(stderr, "\nImage stream connection error: %s\n\n", msg);
    this->on_ctrl_connection_status_changed();
}


/*
 * riv_client_window::on_image_data
 */
void riv_client_window::on_image_data(rivlib::image_stream_connection::ptr comm, uint32_t width, uint32_t height, const void* rgbpix) throw() {
    using namespace the::system::threading;
    if ((width <= 0) || (height <= 0)) return;
    {
        auto_lock<critical_section> lock(this->in_img_buf_lock);
        this->in_img_width = width;
        this->in_img_height = height;
        this->in_img_dat.assert_size(width * height * 3);
        ::memcpy(this->in_img_dat, rgbpix, width * height * 3);
    }
}


/*
 * riv_client_window::on_connected
 */
void riv_client_window::on_msg(rivlib::control_connection::ptr comm, unsigned int id, unsigned int size, const void *data) throw() {
    try {

        switch (id) {
        case static_cast<unsigned int>(rivlib::message_id::data_channels): {
            size_t pos = 0;
            if (size < pos + sizeof(uint32_t)) throw the::exception(__FILE__, __LINE__);
            uint32_t cnt = *the::as_at<uint32_t>(data, pos);
            pos += sizeof(uint32_t);

            printf("Provider publishes %u data channels:\n", static_cast<unsigned int>(cnt));

            std::string sel_chan_name;
            uint8_t sel_chan_qual = 0;
            uint16_t sel_chan_type = 0;
            uint16_t sel_chan_subtype = 0;

            for (uint32_t i = 0; i < cnt; ++i) {
                if (size < pos + sizeof(uint16_t)) throw the::exception(__FILE__, __LINE__);
                uint16_t namelen = *the::as_at<uint16_t>(data, pos);
                pos += sizeof(uint16_t);
                if (size < pos + namelen + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint8_t)) throw the::exception(__FILE__, __LINE__);
                std::string name(the::as_at<char>(data, pos), namelen);
                pos += namelen;
                uint16_t chan_type = *the::as_at<uint16_t>(data, pos);
                pos += sizeof(uint16_t);
                uint16_t chan_subtype = *the::as_at<uint16_t>(data, pos);
                pos += sizeof(uint16_t);
                uint8_t chan_quality = *the::as_at<uint8_t>(data, pos);
                pos += sizeof(uint8_t);

                printf("\t%s (%u,%u;%u)\n",
                    name.c_str(),
                    static_cast<unsigned int>(chan_type),
                    static_cast<unsigned int>(chan_subtype),
                    static_cast<unsigned int>(chan_quality));

                // select best image_stream channel
                if (static_cast<rivlib::data_channel_type>(chan_type) == rivlib::data_channel_type::image_stream) {
                    rivlib::data_channel_image_stream_subtype subtype = static_cast<rivlib::data_channel_image_stream_subtype>(chan_subtype);

                    // skip unsupported streams
                    // UPDATE: we now ask the lib to support channel subtypes
                    if (this->img_strm->is_supported(subtype)) {
                        // subtype is supported by this client o_O
                        if (sel_chan_qual < chan_quality) {
                            sel_chan_name = name;
                            sel_chan_type = chan_type;
                            sel_chan_subtype = chan_subtype;
                            sel_chan_qual = chan_quality;
                        }
                    }
                }
            }

            if (sel_chan_name.empty()) {
                printf("No suitable image_stream data channel available\n");

            } else {
                printf("Selected \"%s\" image_stream data channel\n", sel_chan_name.c_str());
                this->img_strm->disconnect(true);

                size_t uri_len = this->ctrl->make_data_channel_uri(sel_chan_name.c_str(), sel_chan_type, sel_chan_subtype, nullptr, 0);
                char *buf = new char[uri_len];
                uri_len = this->ctrl->make_data_channel_uri(sel_chan_name.c_str(), sel_chan_type, sel_chan_subtype, buf, uri_len);
                std::string uri(buf, uri_len);
                delete[] buf;

                this->img_strm->connect(uri.c_str());

            }

        } break;
        case RIVLIB_USERMSG + 102: { // Answer to "get info"
            // Playload is ANSI-String
            the::astring str(static_cast<const char*>(data), size);
            str.insert(0, "tweakbar/infoLabel label='");
            str.append("'");
            ::TwDefine(str.c_str());
        } break;
        default:
            printf("Ctrl Msg received: %u (%u bytes)\n", id, static_cast<unsigned int>(size));
            break;
        }

    } catch(the::exception ex) {
        fprintf(stderr, "Ctrl-Message error: %s (%s, %d)\n", ex.get_msg_astr(), ex.get_file(), ex.get_line());
        this->on_disconnect_clicked();
    } catch(...) {
        fprintf(stderr, "Ctrl-Message error: unexpected exception\n");
        this->on_disconnect_clicked();
    }
}


/*
 * riv_client_window::on_display
 */
void riv_client_window::on_display(void) {
    using namespace the::system::threading;
    this->on_display_begin();

    ::glEnable(GL_TEXTURE_2D);
    {
        auto_lock<critical_section> lock(this->in_img_buf_lock);
        if ((this->in_img_width > 0) && (this->in_img_height > 0)) {

            //if ((this->tex_width != this->in_img_width) || (this->tex_height != this->in_img_height)) {
                this->tex.Release();
                ::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                this->tex.Create(this->in_img_width, this->in_img_height, false, this->in_img_dat,
                    GL_RGB, GL_UNSIGNED_BYTE, GL_RGB, 0);
                this->tex_width = this->in_img_width;
                this->tex_height = this->in_img_height;
                this->tex.Bind();
                ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
                ::glBindTexture(GL_TEXTURE_2D, 0);
            //} else {
            //    this->tex.Update(this->in_img_dat,  this->in_img_width, this->in_img_height,
            //        GL_RGB, GL_UNSIGNED_BYTE, GL_RGB, 0);
            //    this->tex.Bind();
            //    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            //    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            //    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            //    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            //    ::glBindTexture(GL_TEXTURE_2D, 0);
            //}

            this->in_img_width = 0;
            this->in_img_height = 0;
        }
    }

    ::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ::glMatrixMode(GL_PROJECTION);
    ::glLoadIdentity();
    double ww, wh;
    if (this->get_width() < this->get_height()) {
        double s = static_cast<double>(this->get_width()) / static_cast<double>(this->get_height());
        ww = 1.0;
        wh = 1.0 / s;
        ::glScaled(1.0, s, 1.0);
    } else {
        double s = static_cast<double>(this->get_height()) / static_cast<double>(this->get_width());
        ww = 1.0 / s;
        wh = 1.0;
        ::glScaled(s, 1.0, 1.0);
    }

    ::glMatrixMode(GL_MODELVIEW);
    ::glLoadIdentity();

    ::glScaled(0.95, 0.95, 1.0);

    this->tex.Bind();

    double txmin, txmax, tymin, tymax;
    double iw, ih;

    iw = 1.0;
    ih = static_cast<double>(this->tex_height) / static_cast<double>(this->tex_width);

    if ((iw / ih) > (ww / wh)) {
        ::glScaled(ww / iw, ww / iw, 1.0);
    } else {
        ::glScaled(wh / ih, wh / ih, 1.0);
    }

    txmin = 0.5 / static_cast<double>(this->tex_width);
    txmax = 1.0 - txmin;
    tymin = 0.5 / static_cast<double>(this->tex_height);
    tymax = 1.0 - tymin;

    ::glColor4ub(255, 255, 255, 255);
    ::glBegin(GL_QUADS);
    ::glTexCoord2d(txmin, tymax); ::glVertex2d(-iw, -ih);
    ::glTexCoord2d(txmin, tymin); ::glVertex2d(-iw,  ih);
    ::glTexCoord2d(txmax, tymin); ::glVertex2d( iw,  ih);
    ::glTexCoord2d(txmax, tymax); ::glVertex2d( iw, -ih);
    ::glEnd();

    ::glBindTexture(GL_TEXTURE_2D, 0);
    ::glDisable(GL_TEXTURE_2D);

    this->on_display_end();
}


/*
 * riv_client_window::on_keyboard
 */
bool riv_client_window::on_keyboard(unsigned char key, int x, int y) {
    if (glut_window::on_keyboard(key, x, y)) return true;

    switch (key) {
    case 27: // esc
        this->close();
        return true;
    }

    return false;

}


/*
 * riv_client_window::on_connect_clicked
 */
void riv_client_window::on_close(void) {
    ::TwDeleteBar(this->bar);
    this->bar = nullptr;
    glut_window::on_close();
}


/*
 * riv_client_window::on_reshape
 */
void riv_client_window::on_reshape(int width, int height) {
    glut_window::on_reshape(width, height);
}


/*
 * riv_client_window::on_idle
 */
void riv_client_window::on_idle(void) {
    if (this->auto_connect) {
        this->auto_connect = false;
        this->on_connect_clicked();
    }
    glut_window::on_idle();
}


/*
 * riv_client_window::on_connect_clicked
 */
void riv_client_window::on_connect_clicked(void) {
    this->ctrl->connect(this->uri.c_str());
    this->img_strm->disconnect(false);
    this->on_ctrl_connection_status_changed();
}


/*
 * riv_client_window::on_disconnect_clicked
 */
void riv_client_window::on_disconnect_clicked(void) {
    this->ctrl->disconnect(false);
    this->img_strm->disconnect(false);
    this->on_ctrl_connection_status_changed();
}


/*
 * riv_client_window::on_ctrl_connection_status_changed
 */
void riv_client_window::on_ctrl_connection_status_changed(void) {
    const char *ctrl_state = "unknown";
    const char *img_state = "unknown";

    switch (this->ctrl->get_status()) {
    case rivlib::control_connection::status::connected:
        ctrl_state = "connected";
        break;
    case rivlib::control_connection::status::connecting:
        ctrl_state = "connecting";
        break;
    case rivlib::control_connection::status::not_connected:
        ctrl_state = "not connected";
        break;
    case rivlib::control_connection::status::disconnecting:
        ctrl_state = "disconnecting";
        break;
    }

    switch (this->img_strm->get_status()) {
    case rivlib::image_stream_connection::status::connected:
        img_state = "connected";
        break;
    case rivlib::image_stream_connection::status::connecting:
        img_state = "connecting";
        break;
    case rivlib::image_stream_connection::status::not_connected:
        img_state = "not connected";
        break;
    case rivlib::image_stream_connection::status::disconnecting:
        img_state = "disconnecting";
        break;
    }

    this->set_status(the::text::astring_builder::format("Ctrl: %s / Img: %s", ctrl_state, img_state).c_str());
}


/*
 * riv_client_window::on_rotate_clicked
 */
void riv_client_window::on_rotate_clicked(void) {
    float angle = 45.0f;
    this->ctrl->send(RIVLIB_USERMSG + 100, 4, &angle);
}


/*
 * riv_client_window::on_get_info_clicked
 */
void riv_client_window::on_get_info_clicked(void) {
    this->ctrl->send(RIVLIB_USERMSG + 101, 0, nullptr);
}
