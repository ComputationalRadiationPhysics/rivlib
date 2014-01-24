/*
 * simple_rendering_window.cpp
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

#include "stdafx.h"
#include "simple_rendering_window.h"


/*
 * simple_rendering_window::simple_rendering_window
 */
simple_rendering_window::simple_rendering_window(void) : base() {
    THE_STACK_TRACE;
}


/*
 * simple_rendering_window::~simple_rendering_window
 */
simple_rendering_window::~simple_rendering_window(void) {
    THE_STACK_TRACE;
}


/*
 * simple_rendering_window::draw
 */
void simple_rendering_window::draw(void) {
    THE_STACK_TRACE;
    base::draw();
    the::system::com_ptr<ID2D1Brush> textBrush;
    the::system::com_ptr<IDWriteTextFormat> textFormat;
    float clearColour[] = { 0.2f, 0.2f, 0.8f, 1.0f };
    if (this->renderTargetView != NULL) {
        this->immediateContext->ClearRenderTargetView(this->renderTargetView,
            clearColour);

        textBrush = this->dwSurface.create_brush(D2D1::ColorF::White);
        textFormat = this->dwSurface.create_text_format(L"Segoe UI", 64);

        the::system::com_ptr<ID2D1RenderTarget> rt = this->dwSurface;
        this->dwSurface.acquire_direct2d_sync();
        rt->BeginDraw();
        rt->Clear(D2D1::ColorF(D2D1::ColorF::Crimson));
        this->dwSurface.draw_text(L"THE lib", textFormat, textBrush, NULL, false);
        rt->EndDraw();
        this->dwSurface.release_direct2d_sync();

        this->dwSurface.draw();
    }
}


/*
 * simple_rendering_window::initialise
 */
void simple_rendering_window::initialise(void) {
    THE_STACK_TRACE;
    base::initialise(L"THE lib", 64, 64, 1024, 1024);
    //this->create_depth_stencil_view();
    this->dwSurface.initialise(this->swapChain);
    base::show();
}


/*
 * simple_rendering_window::on_close
 */
void simple_rendering_window::on_close(void) throw() {
    THE_STACK_TRACE;
    ::PostQuitMessage(0);
}


/*
 * simple_rendering_window::on_resized
 */
void simple_rendering_window::on_resized(const size_type width,
        const size_type height) throw() {
    THE_STACK_TRACE;
    this->dwSurface.prepare_resize();
    base::on_resized(width, height);
    this->dwSurface.resize(this->swapChain);
}
