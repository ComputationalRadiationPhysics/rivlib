/*
 * include\the\graphics\directx\d2d_interop_surface.h
 *
 * Copyright (C) 2012 TheLib Team (http://www.thelib.org/license)
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

#ifndef THE_GRAPHICS_DIRECTX_D2D_INTEROP_SURFACE_H_INCLUDED
#define THE_GRAPHICS_DIRECTX_D2D_INTEROP_SURFACE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"
#ifdef WITH_THE_DIRECTX11

#include <Windows.h>
#include <DXGI.h>
#include <D2D1.h>
#include <D3D10.h>
#include <D3D11.h>
#include <DirectXMath.h>

#include "the/force_inline.h"
#include "the/not_copyable.h"
#include "the/stack_trace.h"
#include "the/types.h"

#include "the/graphics/directx/debug.h"
#include "the/graphics/directx/vertex_xyz.h"

#include "the/system/com_exception.h"
#include "the/system/com_ptr.h"


namespace the {
namespace graphics {
namespace directx {

    /**
     * Direct3D 11 and Direct2D cannot share a DXGI surface, i.e. it is not 
     * possible to draw (using DXGI) on the back buffer of the D3D device. The
     * solution for that is a shared surface (texture) that D2D can draw to and
     * that is finally composited over the 3D rendering. This class implements
     * such a surface and manages all the resources that are required to create
     * and to composit it.
     *
     * The approach is described at http://msdn.microsoft.com/en-us/library/windows/desktop/ee913554(v=vs.85).aspx.
     */
    class d2d_interop_surface : public the::not_copyable {

    public:

        /**
         * Initialises a new instance, but does not yet allocate the GPU
         * resources.
         */
        d2d_interop_surface(void);

        /** Dtor. */
        virtual ~d2d_interop_surface(void);

        /**
         * Acquires the keyed mutex of the shared surface for use with 
         * Direct2D, which is equivalent to acquiring a Direct3D 10 lock.
         *
         * This method must only be called after the surface has been 
         * sucessfully initialised. Otherwise, the lock will not be available
         * and the method will return E_POINTER.
         *
         * @param key
         * @param timeout
         *
         * @return S_OK in case of success, an error code otherwise.
         */
        THE_FORCE_INLINE HRESULT acquire_direct2d_sync(const uint64_t key = 0,
                const DWORD timeout = INFINITE) throw() {
            THE_STACK_TRACE;
            return this->acquire_direct3d10_sync(key, timeout);
        }

        /**
         * Acquires the keyed mutex of the shared surface for use with 
         * Direct3D 10.
         *
         * This method must only be called after the surface has been 
         * sucessfully initialised. Otherwise, the lock will not be available
         * and the method will return E_POINTER.
         *
         * @param key
         * @param timeout
         *
         * @return S_OK in case of success, an error code otherwise.
         */
        HRESULT acquire_direct3d10_sync(const uint64_t key = 0,
            const DWORD timeout = INFINITE) throw();

        /**
         * Acquires the keyed mutex of the shared surface for use with 
         * Direct3D 11.
         *
         * This method must only be called after the surface has been 
         * sucessfully initialised. Otherwise, the lock will not be available
         * and the method will return E_POINTER.
         *
         * @param key
         * @param timeout
         *
         * @return S_OK in case of success, an error code otherwise.
         */
        HRESULT acquire_direct3d11_sync(const uint64_t key = 0,
            const DWORD timeout = INFINITE) throw();

        /**
         * Convenience method for clearing the render target.
         */
        //void clear(const D2D1::ColorF& colour);

        /**
         * Convenience method for creating a solid colour brush.
         *
         * This method must only be called after the surface has been
         * successfully initialised. Otherwise, it will raise an exception.
         *
         * You can use the brush returned as long as the surface is not
         * being resized. The reason for that is that resizing the surface will
         * change the render target, which in turn invalidates all resources
         * created using the old render target.
         *
         * @param colour The colour of the brush to be created.
         *
         * @return A brush with the specified colour, which is valid for this
         *         render target (as long as it is not resized).
         *
         * @throws the::null_pointer_exception In case the render target has not
         *                                     been initialised before.
         * @throws the::system::com_exception In case the resource allocation
         *                                    failed.
         */
        system::com_ptr<ID2D1Brush> create_brush(const D2D1::ColorF& colour);

        /**
         * Convenience method for creating a solid colour brush.
         *
         * This method must only be called after the surface has been
         * successfully initialised. Otherwise, it will raise an exception.
         *
         * You can use the brush returned as long as the surface is not
         * being resized. The reason for that is that resizing the surface will
         * change the render target, which in turn invalidates all resources
         * created using the old render target.
         *
         * @param colour The colour of the brush to be created.
         *
         * @return A brush with the specified colour, which is valid for this
         *         render target (as long as it is not resized).
         *
         * @throws the::null_pointer_exception In case the render target has not
         *                                     been initialised before.
         * @throws the::system::com_exception In case the resource allocation
         *                                    failed.
         */
        inline system::com_ptr<ID2D1Brush> create_brush(
                const D2D1::ColorF::Enum colour) {
            THE_STACK_TRACE;
            return this->create_brush(D2D1::ColorF(colour));
        }

        /**
         * Draws the shared surface using Direct3D 11.
         *
         * This method must only be called after the surface has been
         * successfully initialised. Otherwise, it will raise an exception.
         *
         * The method will acquire the keyed mutex for Direct3D 11 by itself
         * if not specified otherwise. If the caller disables automatic 
         * acquisition of the lock, he must ensure that the lock was acquired
         * before this method is called and that it is held until the method
         * returns.
         *
         * Rendering will use a previously recorded command list and 
         * automatically restore the device state before returning.
         *
         * @param acquireMutex If true, the method will acquire the keyed mutex
         *                     fore Direct3D 11 before rendering and release it
         *                     afterwards.
         *
         * @throws the::null_pointer_exception If the surface has not been 
         *                                     completely initialised before and
         *                                     at least one of the required 
         *                                     resources is NULL.
         * @throws the::system::com_exception If acquiring or releaseing the 
         *                                    keyed mutex failed.
         */
        void draw(const bool acquireMutex = true);

        /**
         * Answer the Direct2D factory that was used for acquiring Direct2D
         * resources.
         *
         * @return The Direct2D factory used by the object.
         */
        inline system::com_ptr<ID2D1Factory> get_d2d_factory(void) throw() {
            THE_STACK_TRACE;
            return this->d2dFactory;
        }

        ///**
        // * Answer the opacity that is applied on the shared surface when
        // * rendering it using Direct3D 11.
        // *
        // * @return The opacity for rendering.
        // */
        //inline float get_opacity(void) const throw() {
        //    THE_STACK_TRACE;
        //    return this->psParams.opacity;
        //}

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

        /**
         * Prepares the object for resizing the underlying swap chain.
         *
         * This operation must release all outstanding references that the
         * object (and all derived objects) hold to the back buffer of the swap
         * chain, because the swap chain can only be resized once all of these
         * references have been released.
         */
        virtual void prepare_resize(void);

        /**
         * Releases a previously acquired keyed mustes of the shared surface
         * for using it with Direct2D.
         *
         * This method must only be called after the surface has been 
         * sucessfully initialised. Otherwise, the lock will not be available
         * and the method will return E_POINTER.
         *
         * @param key
         *
         * @return S_OK in case of success, an error code otherwise.
         */
        THE_FORCE_INLINE HRESULT release_direct2d_sync(
                const uint64_t key = 0) throw() {
            THE_STACK_TRACE;
            return this->release_direct3d10_sync(key);
        }

        /**
         * Releases a previously acquired keyed mustes of the shared surface
         * for using it with Direct3D 10.
         *
         * This method must only be called after the surface has been 
         * sucessfully initialised. Otherwise, the lock will not be available
         * and the method will return E_POINTER.
         *
         * @param key
         *
         * @return S_OK in case of success, an error code otherwise.
         */
        HRESULT release_direct3d10_sync(const uint64_t key = 0) throw();

        /**
         * Releases a previously acquired keyed mustes of the shared surface
         * for using it with Direct3D 11.
         *
         * This method must only be called after the surface has been 
         * sucessfully initialised. Otherwise, the lock will not be available
         * and the method will return E_POINTER.
         *
         * @param key
         *
         * @return S_OK in case of success, an error code otherwise.
         */
        HRESULT release_direct3d11_sync(const uint64_t key = 0) throw();

        /**
         * Resizes the surface to match the dimensions of the swap chain.
         *
         * @param d3d11SwapChain The swap chain used by Direct3D 11.
         *
         * @throws the::argument_null_exception In case 'd3d11SwapChain' is a
         *                                      nullptr.
         * @throws the::system::com_exception In case the operation fails.
         */
        void resize(IDXGISwapChain *d3d11SwapChain);

        // cast

        /**
         * Convert the surface into a Direct2D render target.
         *
         * @return The Direct2D render target.
         */
        inline operator system::com_ptr<ID2D1RenderTarget>& (void) throw() {
            THE_STACK_TRACE;
            return this->d2dRenderTarget;
        }

        /**
         * Convert the surface into a Direct3D 11 texture.
         *
         * @return The Direct3D 11 texture.
         */
        inline operator system::com_ptr<ID3D11Texture2D>& (void) throw() {
            THE_STACK_TRACE;
            return this->d3d11Surface;
        }

    protected:

        /** Super class typedef. */
        typedef the::not_copyable base;

    private:

        /**
         * The vertex format used for drawing the shared surface on a D3D 11 
         * device.
         */
        typedef vertex_xyz vertex_type;

        /** 
         * The per-frame parameter for the pixel shader when rendering the 
         * interop surface.
         */
        typedef __declspec(align(16)) struct pixel_shader_constants_t {
            float opacity;
            DirectX::XMFLOAT3 Padding1;
        } pixel_shader_constants;

        /** The pixel shader used for rendering the surface with D3D 11. */
        static const char *PIXEL_SHADER;

        /** The vertex shader used for rendering the surface with D3D 11. */
        static const char *VERTEX_SHADER;

        /*
         * Allocates the device-dependent D2D resources for the given  DXGI
         * (D3D 10.1-compatible) surface.
         *
         * Note: The outcome of all alloc_dev_dependent_res() overloads is the
         * same. The only difference is the starting point, i.e. some of the
         * methods do more work while others are based on work the caller did
         * before.
         *
         * @param dxgiSurface The DXGI surface to allocate the D2D resource for.
         *
         * @throws the::system::com_exception In case the operation fails.
         */
        void alloc_dev_dependent_res(IDXGISurface *dxgiSurface);

        /**
         * Allocates the D3D 10.1 interoperability resources using the shared
         * D3D 11 texture 'surface' and afterwards creates the D2D resources
         * using the DXGI surface that is begin created in this method.
         *
         * The texture 'd3d11Surface' must have been created in a format that is
         * suitable for D3D 10.1/11 interoperability. Most notably, 'MiscFlags'
         * must include D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX and 'BindFlags'
         * must include D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE.
         *
         * Note: The outcome of all alloc_dev_dependent_res() overloads is the
         * same. The only difference is the starting point, i.e. some of the
         * methods do more work while others are based on work the caller did
         * before.
         *
         * @param d3d11Surface The surface used to present the result to
         *                     Direct3D 11.
         *
         * @throws the::system::com_exception In case the operation fails.
         */
        void alloc_dev_dependent_res(ID3D11Texture2D *d3d11Surface);

        /**
         * Allocates the D3D 11 shared surface compatible with the back buffer
         * of the given swap chain and afterwards allocates all other
         * device-depentent D3D and D2D resources.
         *
         * Note: The outcome of all alloc_dev_dependent_res() overloads is the
         * same. The only difference is the starting point, i.e. some of the
         * methods do more work while others are based on work the caller did
         * before.
         * 
         * @param d3d11SwapChain The swap chain used by Direct3D 11.
         *
         * @throws system::com_exception In case the operation fails.
         */
        void alloc_dev_dependent_res(IDXGISwapChain *d3d11SwapChain);

        /**
         * Allocates device-independent D2D resources, which is primarily the
         * factory.
         *
         * @throws the::system::com_exception In case the operation fails.
         */
        void alloc_dev_independent_res(void);

        /**
         * Allocates the D3D 11 resources that are required for rendering the
         * shared texture in D3D.
         *
         * @throws the::system::com_exception In case the operation fails.
         */
        void alloc_rendering_res(void);

        /**
         * (Re-) Creates the shader resource view for D3D 11.
         *
         * The D3D 11 device and the D3D 11 surface must have been set before.
         * It is safe to call the method even if the shader resource view is
         * not NULL.
         *
         * This method is called for initialising the surface and when resizing
         * it (after the textures have been re-created).
         *
         * @throws the::system::com_exception In case the operation fails.
         */
        void alloc_shader_res_view(void);

        /**
         * Update the command list for rendering the surface.
         *
         * @throws the::system::com_exception In case the operation fails.
         */
        void update_rendering_cmdlist(void);

        // TOOD: Most of the members are probably useless, because we never 
        // need them after the command list was created during initialisation.

        /** The D2D factory that is used to allocate the resources. */
        system::com_ptr<ID2D1Factory> d2dFactory;

        /** The D2D render target. */
        system::com_ptr<ID2D1RenderTarget> d2dRenderTarget;

        /** This lock must be acquired while using D3D 10 and D2D. */
        system::com_ptr<IDXGIKeyedMutex> d3d10Lock;

        /** The blend state for drawing the interop surface in D3D 11. */
        system::com_ptr<ID3D11BlendState> d3d11BlendState;

        /** Constant buffer for 'psParams'. */
        system::com_ptr<ID3D11Buffer> d3d11CbPsParams;

        /** The command list for rendering the interop surface on D3d 11. */
        system::com_ptr<ID3D11CommandList> d3d11CmdList;

        /** The Direct3D 11 device used for rendering the interop surface. */
        system::com_ptr<ID3D11Device> d3d11Device;

        /** The immediate context used to render the interop surface. */
        system::com_ptr<ID3D11DeviceContext> d3d11ImmediateContext;

        /** 
         * The vertex input layout of the proxy geometry used for rendering the 
         * interop surface.
         */
        system::com_ptr<ID3D11InputLayout> d3d11InputLayout;

        /** This lock must be acquired while using D3D 11. */
        system::com_ptr<IDXGIKeyedMutex> d3d11Lock;

        /** The pixel shader used to render the interop surface. */
        system::com_ptr<ID3D11PixelShader> d3d11PixelShader;

        /**
         * The resource view of 'd3d11Surface' for rendering the interop 
         * surface. 
         */
        system::com_ptr<ID3D11ShaderResourceView> d3d11ResView;

        /** The sampler state accessing for 'd3d11Surface'. */
        system::com_ptr<ID3D11SamplerState> d3d11SamplerState;

        /** The shared texture of the interop surface in D3D 11. */ 
        system::com_ptr<ID3D11Texture2D> d3d11Surface;

        /** The proxy geometry for rendering the interop surface. */
        system::com_ptr<ID3D11Buffer> d3d11VertexBuffer;

        /** The vertex shader for rendering the interop surface. */
        system::com_ptr<ID3D11VertexShader> d3d11VertexShader;
    };

} /* end namespace directx */
} /* end namespace graphics */
} /* end namespace the */

#endif /* WITH_THE_DIRECTX11 */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_GRAPHICS_DIRECTX_D2D_INTEROP_SURFACE_H_INCLUDED */
