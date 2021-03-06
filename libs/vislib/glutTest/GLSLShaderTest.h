/*
 * GLSLShaderTest.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef VISLIBTEST_GLSLSHADERTEST_H_INCLUDED
#define VISLIBTEST_GLSLSHADERTEST_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */


#include "AbstractGlutApp.h"
#include "vislib/types.h"
#include "vislib/CameraOpenGL.h"
#include "vislib/CameraRotate2DLookAt.h"
#include "vislib/Cursor2D.h"
#include "vislib/GLSLShader.h"
#include "vislib/InputModifiers.h"
#include "vislib/OpenGLVISLogo.h"


/*
 * Test for mono tiled display frustrum generation
 */
class GLSLShaderTest: public AbstractGlutApp {
public:
    GLSLShaderTest(void);
    virtual ~GLSLShaderTest(void);

    virtual int GLInit(void);
    virtual void GLDeinit(void);

    virtual void OnResize(unsigned int w, unsigned int h);
    virtual void Render(void);
    virtual bool OnKeyPress(unsigned char key, int x, int y);
    virtual void OnMouseEvent(int button, int state, int x, int y);
    virtual void OnMouseMove(int x, int y);
    virtual void OnSpecialKey(int key, int x, int y);

private:

    vislib::graphics::gl::CameraOpenGL camera;
    vislib::graphics::InputModifiers modkeys;
    vislib::graphics::Cursor2D cursor;
    vislib::graphics::CameraRotate2DLookAt rotator;
    vislib::graphics::gl::GLSLShader schade;
    vislib::graphics::gl::OpenGLVISLogo logo;
};

#endif /* VISLIBTEST_GLSLSHADERTEST_H_INCLUDED */
