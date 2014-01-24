/*
 * include\the\graphics\directx\dwrite_interop_surface.h
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

#ifndef THE_GRAPHICS_DIRECTX_DWRITE_INTEROP_SURFACE_H_INCLUDED
#define THE_GRAPHICS_DIRECTX_DWRITE_INTEROP_SURFACE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"
#ifdef WITH_THE_DIRECTX11

#include <DWrite.h>

#include "the/graphics/directx/d2d_interop_surface.h"


namespace the {
namespace graphics {
namespace directx {

    /**
     * This class is an extension of the::graphics::directx::d2d_interop_surface
     * which additionally provides basic resources for rendering text with
     * DirectWrite.
     */
    class dwrite_interop_surface : public d2d_interop_surface {

    public:

        /**
         * Initialises a new instance.
         */
        dwrite_interop_surface(void);

        /** Dtor. */
        virtual ~dwrite_interop_surface(void);

        /**
         * Utility method for creating a text format.
         *
         * The surface must have been successfully initialised before this
         * method can be called.
         *
         * You can use the text format returned as long as the surface is not
         * being resized. The reason for that is that resizing the surface will
         * change the render target, which in turn invalidates all resources
         * created using the old render target.
         *
         * @param fontFamilyName
         * @param fontSize
         * @param fontWeight
         * @param fontStyle
         * @param fontStretch
         * @param localeName
         *
         * @return
         *
         * @throws the::system::com_exception
         */
        system::com_ptr<IDWriteTextFormat> create_text_format(
            const wchar_t* fontFamilyName,
            FLOAT fontSize,
            DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL,
            const wchar_t* localeName = NULL);

        /**
         * 
         *
         * @param text
         * @param format
         * @param brush
         * @param layoutRect
         * @param acquireMutexAndinitateDraw
         *
         * @throws the::system::com_exception
         */
        void draw_text(const wchar_t *text, IDWriteTextFormat *format,
            ID2D1Brush *brush, const D2D1_RECT_F *layoutRect = NULL,
            const bool acquireMutexAndinitateDraw = true);

        /**
         * Answer the DirectWrite factory that was used for acquiring 
         * DirectWrite resources.
         *
         * @return The DirectWrite factory used by the object.
         */
        inline system::com_ptr<IDWriteFactory> get_dwrite_factory(
                void) throw() {
            THE_STACK_TRACE;
            return this->dWriteFactory;
        }

        /**
         * Initialises the surface for the given Direct3D 11 swap chain.
         *
         * @param d3d11SwapChain The swap chain used by Direct3D 11.
         *
         * @throws the::argument_null_exception In case 'd3d11SwapChain' is a
         *                                      nullptr.
         * @throws the::system::com_exception In case the operation fails.
         */
        virtual void initialise(IDXGISwapChain *d3d11SwapChain);

    protected:

        /** Super class typedef. */
        typedef d2d_interop_surface base;

    private:

        /**
         * Allocates device-independent DirectWrite resources, which is 
         * primarily the factory.
         *
         * @throws the::system::com_exception  In case the operation fails.
         */
        void alloc_dev_independent_res(void);

        /** The DirectWrite factory that is used to allocate the resources. */
        system::com_ptr<IDWriteFactory> dWriteFactory;

    };

} /* end namespace directx */
} /* end namespace graphics */
} /* end namespace the */

#endif /* WITH_THE_DIRECTX11 */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_GRAPHICS_DIRECTX_DWRITE_INTEROP_SURFACE_H_INCLUDED */
