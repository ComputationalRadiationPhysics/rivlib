/*
 * src\graphics\directx\dwrite_interop_surface.cpp
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

#include "the/graphics/directx/dwrite_interop_surface.h"
#ifdef WITH_THE_DIRECTX11

#include "the/assert.h"
#include "the/memory.h"
#include "the/blob.h"
#include "the/stack_trace.h"

#include "the/system/system_string.h"


/*
 * the::graphics::directx::dwrite_interop_surface::dwrite_interop_surface
 */
the::graphics::directx::dwrite_interop_surface::dwrite_interop_surface(void)
        : base()  {
    THE_STACK_TRACE;
}


/*
 * the::graphics::directx::dwrite_interop_surface::~dwrite_interop_surface
 */
the::graphics::directx::dwrite_interop_surface::~dwrite_interop_surface(void) {
    THE_STACK_TRACE;
}


/*
 * the::graphics::directx::dwrite_interop_surface::create_text_format
 */
the::system::com_ptr<IDWriteTextFormat>
the::graphics::directx::dwrite_interop_surface::create_text_format(
        const wchar_t* fontFamilyName, FLOAT fontSize,
        DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle,
        DWRITE_FONT_STRETCH fontStretch, const wchar_t* localeName) {
    THE_STACK_TRACE;

    HRESULT hr = S_OK;                              // Result of API calls.
    blob locale;                             // The actual locale name.
    int localeLen = 0;                              // Length of 'locale'.
    the::system::com_ptr<IDWriteTextFormat> retval; // The new text format.

    if (this->dWriteFactory.is_null()) {
        throw new invalid_operation_exception(THE_SYSTEM_STRING_CONSTANT("The ")
            THE_SYSTEM_STRING_CONSTANT("dwrite_interop_surface has not been ")
            THE_SYSTEM_STRING_CONSTANT("initialised."), __FILE__, __LINE__);
    }

    /* 
     * If no locale is given, retrieve the name of the current user locale
     * and use this one. Otherwise, use the caller-defined locale name.
     */
    if (localeName == NULL) {
        localeLen = ::GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT,
            LOCALE_SNAME, NULL, 0);
        if (localeLen != 0) {
            locale.assert_size(localeLen * sizeof(wchar_t));
            localeLen = ::GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT,
                LOCALE_SNAME, locale.as<wchar_t>(), localeLen);
        } 
        if (localeLen == 0) {
            hr = HRESULT_FROM_WIN32(::GetLastError());
            THE_DIRECTX_TRACE_AND_THROW(hr, "Retrieving system locale failed "
                "with error code %d.\n", hr);
        }

    } else {
        localeLen = ::wcslen(localeName) + 1;
        locale.assert_size(localeLen * sizeof(wchar_t));
        ::wcscpy_s(locale.as<wchar_t>(), localeLen, localeName);
    }

    hr = this->dWriteFactory->CreateTextFormat(fontFamilyName, NULL, 
        fontWeight, fontStyle, fontStretch, fontSize, locale.as<wchar_t>(),
        &retval);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating text format failed with "
            "error code %d.\n", hr);
    }

    return retval;
}


/*
 * the::graphics::directx::dwrite_interop_surface::draw_text
 */
void the::graphics::directx::dwrite_interop_surface::draw_text(
        const wchar_t *text, IDWriteTextFormat *format, ID2D1Brush *brush,
        const D2D1_RECT_F *layoutRect, const bool acquireMutexAndinitateDraw) {
    THE_STACK_TRACE;

    HRESULT hr = S_OK;
    D2D1_RECT_F rect;
    system::com_ptr<ID2D1RenderTarget> renderTarget = *this;
    size_t len = (text != NULL) ? ::wcslen(text) : 0;

    /* Determine the layout rectangle if no user-defined one was specified. */
    if (layoutRect != NULL) {
        rect = *layoutRect;
    } else {
        D2D1_SIZE_F size = renderTarget->GetSize();
        rect = D2D1::RectF(0.0f, 0.0f, size.width, size.height);
    }

    if (acquireMutexAndinitateDraw) {
        THE_DIRECTX_TRACE_INFO("Acquiring keyed mutex in "
            "dwrite_interop_surface::draw_text...\n");
        hr = this->acquire_direct2d_sync();
        if (FAILED(hr)) {
            THE_DIRECTX_TRACE_AND_THROW(hr, "Acquiring keyed mutex for "
                "Direct2D failed with error code %d.", hr);
        }

        renderTarget->BeginDraw();
    } /* end if (acquireMutexAndinitateDraw) */

    renderTarget->SetTransform(::D2D1::IdentityMatrix());
    renderTarget->DrawText(text, len, format, rect, brush);

    if (acquireMutexAndinitateDraw) {
        hr = renderTarget->EndDraw();
        if (FAILED(hr)) {
            THE_DIRECTX_TRACE_AND_THROW(hr, "Finishing Direct2D drawing "
                "failed with error code %d.", hr);
        }

        hr = this->release_direct2d_sync();
        if (FAILED(hr)) {
            THE_DIRECTX_TRACE_AND_THROW(hr, "Releasing keyed mutex for "
                "Direct2D failed with error code %d.", hr);
        }
        THE_DIRECTX_TRACE_INFO("Released keyed mutex in "
            "dwrite_interop_surface::draw_text...\n");
    } /* end if (acquireMutexAndinitateDraw) */
}


/*
 * the::graphics::directx::dwrite_interop_surface::initialise
 */
void the::graphics::directx::dwrite_interop_surface::initialise(
        IDXGISwapChain *d3d11SwapChain) {
    THE_STACK_TRACE;
    base::initialise(d3d11SwapChain);
    this->alloc_dev_independent_res();
}


/*
 * the::graphics::directx::dwrite_interop_surface::alloc_dev_independent_res
 */
void the::graphics::directx::dwrite_interop_surface::alloc_dev_independent_res(
        void) {
    THE_STACK_TRACE;

    HRESULT hr = S_OK;

    THE_DIRECTX_TRACE_INFO("Creating DirectWrite factory...\n");
    hr = ::DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown **>(&dWriteFactory));
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating DirectWrite factory failed "
            "with error code %d", hr);
    }
}

#endif /* WITH_THE_DIRECTX11 */
