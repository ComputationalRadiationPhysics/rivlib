/*
 * glut_window.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#include "stdafx.h"
#include "glut_window.h"
#include "visglut.h"
#include "GL/gl.h"
#include "AntTweakBar.h"
#include "the/math/functions.h"


using namespace eu_vicci;
using namespace eu_vicci::riv_prov_test;


/*
 * glut_window::wnd_cnt
 */
unsigned int glut_window::wnd_cnt = 0;


/*
 * glut_window::glut_window
 */
glut_window::glut_window(const char* caption) : wnd_id(0), wnd_opened(false) {
    this->wnd_id = ::glutCreateWindow(caption);

    ::glutSetWindowData(this);

#ifdef VISGLUT_EXTENSIONS
    ::glutSetWindowIconI(101);
#endif /* VISGLUT_EXTENSIONS */

    ::glutDisplayFunc(&glut_window::glut_display);
    ::glutReshapeFunc(&glut_window::glut_reshape);
    ::glutKeyboardFunc(&glut_window::glut_keyboard);
    ::glutSpecialFunc(&glut_window::glut_special_keyboard);
    ::glutMouseFunc(&glut_window::glut_mouse_button);
    ::glutMotionFunc(&glut_window::glut_mouse_motion);
    ::glutPassiveMotionFunc(&glut_window::glut_mouse_motion);
    ::glutIdleFunc(&glut_window::glut_idle);
    ::glutCloseFunc(&glut_window::glut_close);
}


/*
 * glut_window::~glut_window
 */
glut_window::~glut_window(void) {
    this->close();
}


/*
 * glut_window::close
 */
void glut_window::close(void) {
    if (this->wnd_opened) {
        ::glutDestroyWindow(this->wnd_id);
        // 'wnd_opened' will be set to false by the corresponding callback
    }
}


/*
 * glut_window::on_display
 */
void glut_window::on_display(void) {

    this->on_display_begin();

    ::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Implement in derived class

    this->on_display_end();

}


/*
 * glut_window::on_display_begin
 */
void glut_window::on_display_begin(void) {
    if (!this->wnd_opened) {
        this->wnd_opened = true;
        glut_window::wnd_cnt++;
    }
    ::TwSetCurrentWindow(this->wnd_id);
}


/*
 * glut_window::on_display_end
 */
void glut_window::on_display_end(void) {
    ::TwDraw();
    ::glutSwapBuffers();
}


/*
 * glut_window::on_reshape
 */
void glut_window::on_reshape(int width, int height) {
    ::TwSetCurrentWindow(this->wnd_id);

    this->width = the::math::maximum(0, width);
    this->height = the::math::maximum(0, height);

    ::glViewport(0, 0, this->width, this->height);

    ::TwWindowSize(this->width, this->height);

}


/*
 * glut_window::on_mouse_button
 */
bool glut_window::on_mouse_button(int button, int state, int x, int y) {
    ::TwSetCurrentWindow(this->wnd_id);
    if (::TwEventMouseButtonGLUT(button, state, x, y)) return true;

    // Implement in derived class

    return false;
}


/*
 * glut_window::on_mouse_motion
 */
bool glut_window::on_mouse_motion(int x, int y) {
    ::TwSetCurrentWindow(this->wnd_id);
    if (::TwEventMouseMotionGLUT(x, y)) return true;

    // Implement in derived class

    return false;

}


/*
 * glut_window::on_keyboard
 */
bool glut_window::on_keyboard(unsigned char key, int x, int y) {
    ::TwSetCurrentWindow(this->wnd_id);
    if (::TwEventKeyboardGLUT(key, x, y)) return true;

    // Implement in derived class

    return false;

}


/*
 * glut_window::on_special_keyboard
 */
bool glut_window::on_special_keyboard(int key, int x, int y) {
    ::TwSetCurrentWindow(this->wnd_id);
    if (::TwEventSpecialGLUT(key, x, y)) return true;

    // Implement in derived class

    return false;

}


/*
 * glut_window::on_idle
 */
void glut_window::on_idle(void) {
    ::TwSetCurrentWindow(this->wnd_id);

    // redisplay for continuous redraw
    ::glutPostRedisplay();
}


/*
 * glut_window::on_close
 */
void glut_window::on_close(void) {
    if (this->wnd_opened) {
        this->wnd_opened = false;
        glut_window::wnd_cnt--;
        if (glut_window::wnd_cnt == 0) {
            ::glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
            ::TwTerminate(); // because the last window closes
        }
    }
}


/*
 * glut_window::glut_display
 */
void glut_window::glut_display(void) {
    glut_window *wnd = static_cast<glut_window*>(::glutGetWindowData());
    wnd->on_display();
}


/*
 * glut_window::glut_reshape
 */
void glut_window::glut_reshape(int width, int height) {
    glut_window *wnd = static_cast<glut_window*>(::glutGetWindowData());
    wnd->on_reshape(width, height);
}


/*
 * glut_window::glut_mouse_button
 */
void glut_window::glut_mouse_button(int button, int state, int x, int y) {
    glut_window *wnd = static_cast<glut_window*>(::glutGetWindowData());
    wnd->on_mouse_button(button, state, x, y);
}


/*
 * glut_window::glut_mouse_motion
 */
void glut_window::glut_mouse_motion(int x, int y) {
    glut_window *wnd = static_cast<glut_window*>(::glutGetWindowData());
    wnd->on_mouse_motion(x, y);
}


/*
 * glut_window::glut_keyboard
 */
void glut_window::glut_keyboard(unsigned char key, int x, int y) {
    glut_window *wnd = static_cast<glut_window*>(::glutGetWindowData());
    wnd->on_keyboard(key, x, y);
}


/*
 * glut_window::glut_special_keyboard
 */
void glut_window::glut_special_keyboard(int key, int x, int y) {
    glut_window *wnd = static_cast<glut_window*>(::glutGetWindowData());
    wnd->on_special_keyboard(key, x, y);
}


/*
 * glut_window::glut_idle
 */
void glut_window::glut_idle(void) {
    glut_window *wnd = static_cast<glut_window*>(::glutGetWindowData());
    wnd->on_idle();
}


/*
 * glut_window::glut_close
 */
void glut_window::glut_close(void) {
    glut_window *wnd = static_cast<glut_window*>(::glutGetWindowData());
    wnd->on_close();
}
