/*
 * simple_rendering_window.h
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

#ifndef THED3DTEST_SIMPLE_RENDERING_WINDOW_H_INCLUDED
#define THED3DTEST_SIMPLE_RENDERING_WINDOW_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "the/graphics/rendering_window.h"
#include "the/graphics/win32_window_behaviour.h"
#include "the/graphics/window.h"

#include "the/graphics/directx/dwrite_interop_surface.h"
#include "the/graphics/directx/d3d11_rendering_behaviour.h"


class simple_rendering_window : public the::graphics::rendering_window<
        the::graphics::window<the::graphics::win32_window_behaviour>,
        the::graphics::directx::d3d11_rendering_behaviour> {

public:

    simple_rendering_window(void);

    virtual ~simple_rendering_window(void);

    using base::initialise;
    void initialise(void);

    virtual void draw(void);

protected:

    typedef the::graphics::rendering_window<
        the::graphics::window<the::graphics::win32_window_behaviour>,
        the::graphics::directx::d3d11_rendering_behaviour> base;

    virtual void on_close(void) throw();

    virtual void on_resized(const size_type width,
        const size_type height) throw();

    the::graphics::directx::dwrite_interop_surface dwSurface;
};

#endif /* THED3DTEST_SIMPLE_RENDERING_WINDOW_H_INCLUDED */
