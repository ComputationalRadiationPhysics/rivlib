/*
 * include\the\graphics\directx\d3d11_rendering_behaviour.h
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

#ifndef THE_GRAPHICS_DIRECTX_D3D11_RENDERING_BEHAVIOUR_H_INCLUDED
#define THE_GRAPHICS_DIRECTX_D3D11_RENDERING_BEHAVIOUR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"
#ifdef WITH_THE_DIRECTX11

#include <list>

#include <Windows.h>
#include <D3D11.h>

#include "the/collections/array.h"

#include "the/system/com_exception.h"
#include "the/system/com_ptr.h"
#include "the/system/system_exception.h"
#include "the/system/system_string.h"


namespace the {
namespace graphics {
namespace directx {

    /**
     * Provides a basic implementation for a Direct3D 11 renderer with a swap
     * chain and (optionally) a depth/stencil buffer.
     *
     * Sub-classes should overwrite the draw() method to perform actual 
     * rendering.
     *
     * This class has been designed to use shared Direct3D devices such that
     * sharing resources between windows which are rendered by the same adapter
     * is possible.
     */
    class d3d11_rendering_behaviour : public the::not_copyable {

    public:

        /** The type to measure dimensions of the back buffer. */
        typedef unsigned int size_type;

        // TODO: implement this.
        template<template<class W, class R> class T>
        static std::list<std::list<std::shared_ptr<T> > >
        create_concurrency_groups(
            const std::list<std::shared_ptr<T> >& windows);

        /**
         * Cleans up all the work made in one_time_initialise().
         */
        static void one_time_finalise(void);

        /**
         * Performs one-time initialisation before the first instance of the
         * renderer can be created.
         *
         * @throws the::invalid_operation_exception If the initialisation has
         *                                          already been performed.
         * @throws the::system::com_exception In case the initialisation fails.
         * @throws the::system::system_exception In case the initialisation
         *                                       fails.
         */
        static void one_time_initialise(void);

        /** Dtor. */
        virtual ~d3d11_rendering_behaviour(void);

        /**
         * Render a new image into the back buffer.
         *
         * This method must not be called before the renderer has been
         * successfully initialised.
         *
         * The default implementation sets the render target view belonging to
         * the swap chain as active render target view.
         */
        virtual void draw(void);

        /**
         * Present the last rendered image by swapping the back and front 
         * buffers.
         *
         * This method must not be called before the renderer has been
         * successfully initialised.
         */
        virtual void present(void);

        /**
         * Resizes the swap chain.
         *
         * This method resizes the swap chain and its back buffer as well as the
         * render target and depth/stencil views. Furthermore, it sets a
         * viewport reflecting the new dimensions of the swap chain.
         *
         * @throws the::system::com_exception In case of an error.
         */
        virtual void resize(const size_type width, const size_t height);

    protected:

        /** Super class typedef. */
        typedef the::not_copyable base;

        /**
         * Retrieves the best-matching device for rendering to 'hWnd'.
         *
         * @param hWnd
         *
         * @return The device that is used best for rendering to 'hWnd',
         *         or NULL if no such device could be found.
         *
         * @throws the::argument_null_exception If 'hWnd' is NULL.
         * @throws the::invalid_operation_exception If one_time_initialise() has
         *                                          not been called successfully
         *                                          before.
         * @throws the::system::com_exception If finding the best-matching
         *                                    monitor for 'hWnd' failed.
         * @throws the::system::system_exception If any Direct3D or DXGI call
         *                                       fails.
         */
        static system::com_ptr<ID3D11Device> get_device_for_window(HWND hWnd);

        /** The DXGI factory used for creating all devices and swap chains. */
        static system::com_ptr<IDXGIFactory> dxgi_factory;

        /**
         * Initialises a new instance, but do not yet allocate resources.
         */
        d3d11_rendering_behaviour(void);

        /**
         * Convenience method for creating a depth/stencil view.
         *
         * @param format The format of the view. This parameter defaults to
         *               DXGI_FORMAT_D24_UNORM_S8_UINT.
         *
         * @throws the::system::com_exception If the creation of the view 
         *                                    failed.
         */
        void create_depth_stencil_view(const DXGI_FORMAT format
            = DXGI_FORMAT_D24_UNORM_S8_UINT);

        virtual void initialise(HWND hWnd);

        virtual void on_initialising(DXGI_SWAP_CHAIN_DESC& inOutSwapChainDesc);

        /** The depth/stencil view if any. */
        system::com_ptr<ID3D11DepthStencilView> depthStencilView;

        /** The device that the renderer uses. */
        system::com_ptr<ID3D11Device> device;

        /** The immediate context for issuing commands to the device. */
        system::com_ptr<ID3D11DeviceContext> immediateContext;

        /** The currently active render target view. */
        system::com_ptr<ID3D11RenderTargetView> renderTargetView;

        /** The swap chain associated with the render target view. */
        system::com_ptr<IDXGISwapChain> swapChain;

    private:

        /** A list of D3D devices. */
        typedef collections::array<system::com_ptr<ID3D11Device> > device_list;

        /**
         * Holds devices for all adapters found on the system. The idea of this
         * implementation is having all possible devices available and searching
         * the one that fits best for a given window. That way, resources can
         * be shared between windows (as long as they use the same adapter).
         */
        static device_list devices;

        /**
         * Creates a render target view for the swap chain.
         *
         * @throws the::system::com_exception In case of an error.
         */
        void create_rtv(void);

        /**
         * Updates the viewport to the specified size.
         */
        void update_viewport(const size_type width, const size_type height);
    };

} /* end namespace directx */
} /* end namespace graphics */
} /* end namespace the */

#include "the/graphics/directx/d3d11_rendering_behaviour.inl"

#endif /* WITH_THE_DIRECTX11 */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_GRAPHICS_DIRECTX_D3D11_RENDERING_BEHAVIOUR_H_INCLUDED */
