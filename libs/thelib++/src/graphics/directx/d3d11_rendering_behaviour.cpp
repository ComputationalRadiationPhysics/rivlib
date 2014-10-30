/*
 * src\graphics\directx\d3d11_rendering_behaviour.cpp
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

#include "the/graphics/directx/d3d11_rendering_behaviour.h"
#ifdef WITH_THE_DIRECTX11

#include "the/argument_null_exception.h"
#include "the/invalid_operation_exception.h"
#include "the/memory.h"
#include "the/stack_trace.h"

#include "the/graphics/directx/debug.h"


#include "the/not_implemented_exception.h"


/*
 * the::graphics::directx::d3d11_rendering_behaviour::one_time_finalise
 */
void the::graphics::directx::d3d11_rendering_behaviour::one_time_finalise(
        void) {
    THE_STACK_TRACE;
    d3d11_rendering_behaviour::devices.clear(true);
}


/*
 * the::graphics::directx::d3d11_rendering_behaviour::one_time_initialise
 */
void the::graphics::directx::d3d11_rendering_behaviour::one_time_initialise(
        void) {
    THE_STACK_TRACE;

    system::com_ptr<IDXGIAdapter> adapter;      // Current GPU in iteration.
    system::com_ptr<ID3D11Device> device;       // D3D device for current GPU.
    UINT deviceFlags = 0;                       // D3D device creation flags.
    D3D_FEATURE_LEVEL featureLevel;             // Features of current device.
    HRESULT hr = S_OK;                          // Result of API calls.
    system::com_ptr<ID3D11DeviceContext> immediateContext;  // IC of 'device'.

    if (d3d11_rendering_behaviour::dxgi_factory != NULL) {
        throw invalid_operation_exception(THE_SYSTEM_STRING_CONSTANT("The ")
            THE_SYSTEM_STRING_CONSTANT("one-time initialisation of ")
            THE_SYSTEM_STRING_CONSTANT("d3d11_rendering_behaviour has already ")
            THE_SYSTEM_STRING_CONSTANT("been performed."), __FILE__, __LINE__);
    }

    /* Create DXGI factory. */
    hr = ::CreateDXGIFactory1(IID_IDXGIFactory1, reinterpret_cast<void **>(
        &d3d11_rendering_behaviour::dxgi_factory));
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating DXGI factory failed with "
            "error code %d.", hr);
    }

#if (defined(DEBUG) || defined(_DEBUG))
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif /* (defined(DEBUG) || defined(_DEBUG)) */

    for (UINT a = 0; SUCCEEDED(hr); ++a) {
        hr = d3d11_rendering_behaviour::dxgi_factory->EnumAdapters(a, &adapter);
        if (SUCCEEDED(hr)) {
            hr = ::D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL,
                deviceFlags, NULL, 0, D3D11_SDK_VERSION, &device,
                &featureLevel, &immediateContext);
        }
        if (SUCCEEDED(hr)) {
            THE_DIRECTX_SET_DEBUG_NAME(device, "d3d11_rendering_behaviour "
                "device #%d", a);
        }
        if (SUCCEEDED(hr)) {
            d3d11_rendering_behaviour::devices.push_back(device);
        }

        // Release smart pointer for re-use in next iteration.
        device = NULL;
        immediateContext = NULL;
        adapter = NULL;
    }

    if (FAILED(hr) && (hr != DXGI_ERROR_NOT_FOUND)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating Direct3D 11 devices failed "
            "with error code %d.", hr);
    }
}


/*
 * the::graphics::directx::d3d11_rendering_behaviour::~d3d11_rendering_behaviour
 */
the::graphics::directx::d3d11_rendering_behaviour::~d3d11_rendering_behaviour(
        void) {
    THE_STACK_TRACE;
}


/*
 * the::graphics::directx::d3d11_rendering_behaviour::draw
 */
void the::graphics::directx::d3d11_rendering_behaviour::draw(void) {
    THE_STACK_TRACE;
    THE_ASSERT(this->immediateContext != NULL);

    ID3D11DepthStencilView *dsv = this->depthStencilView;
    ID3D11RenderTargetView *rtv = this->renderTargetView;

    this->immediateContext->OMSetRenderTargets(1, &rtv, dsv);
}


/*
 * the::graphics::directx::d3d11_rendering_behaviour::present
 */
void the::graphics::directx::d3d11_rendering_behaviour::present(void) {
    THE_STACK_TRACE;
    THE_ASSERT(this->swapChain != NULL);
    this->swapChain->Present(0, 0);
}


/*
 * the::graphics::directx::d3d11_rendering_behaviour::resize
 */
void the::graphics::directx::d3d11_rendering_behaviour::resize(
        const size_type width, const size_t height) {
    THE_STACK_TRACE;

    HRESULT hr = S_OK;
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    DXGI_SWAP_CHAIN_DESC swapChainDesc;

    if (this->immediateContext != NULL) {
        THE_ASSERT(this->swapChain != NULL);

        /* Get old swap chain and depth/stencil format. */
        hr = this->swapChain->GetDesc(&swapChainDesc);
        if (FAILED(hr)) {
            THE_DIRECTX_TRACE_AND_THROW(hr, "Retrieving swap chain description "
                "failed with error code %d.", hr);
        }

        if (this->depthStencilView != NULL) {
            this->depthStencilView->GetDesc(&dsvDesc);
        } else {
            the::zero_memory(&dsvDesc);
        }

        /* Release old render target view. This must be done before resizing. */
        this->immediateContext->OMSetRenderTargets(0, NULL, NULL);
        this->renderTargetView = NULL;
        this->depthStencilView = NULL;

        /* Resize the swap chain. */
        hr = this->swapChain->ResizeBuffers(swapChainDesc.BufferCount,
            width, height, swapChainDesc.BufferDesc.Format,
            swapChainDesc.Flags);
        if (FAILED(hr)) {
            THE_DIRECTX_TRACE_AND_THROW(hr, "Resizing swap chain failed with "
                "error code %d.", hr);
        }

        /* Re-create the render target view. */
        this->create_rtv();

        /* Re-create the depth/stencil view. */
        if (dsvDesc.Format != 0) {
            this->create_depth_stencil_view(dsvDesc.Format);
        }

        /* Update the viewport. */
        this->update_viewport(width, height);
    }
}


/*
 * the::graphics::directx::d3d11_rendering_behaviour::get_device_for_window
 */
the::system::com_ptr<ID3D11Device>
the::graphics::directx::d3d11_rendering_behaviour::get_device_for_window(
        HWND hWnd) {
    THE_STACK_TRACE;

    HRESULT hr = S_OK;                          // Result of API calls.
    device_list::enumerator it;                 // Enumerator of D3D devices.
    HMONITOR hMonitor = NULL;                   // Monitor showing window.
    DXGI_OUTPUT_DESC outputDesc;                // Description of output.

    /* Sanity checks. */
    if (d3d11_rendering_behaviour::dxgi_factory == NULL) {
        throw invalid_operation_exception(THE_SYSTEM_STRING_CONSTANT("The ")
            THE_SYSTEM_STRING_CONSTANT("DXGI factory is not available. Did ")
            THE_SYSTEM_STRING_CONSTANT("you forget to perform the one-time ")
            THE_SYSTEM_STRING_CONSTANT("initialisation of the renderer?"),
            __FILE__, __LINE__);
    }
    if (hWnd == NULL) {
        throw argument_null_exception("hWnd", __FILE__, __LINE__);
    }

    /* Get pseudo handle for monitor covered by our window. */
    hMonitor = ::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    if (hMonitor == NULL) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

    /* Test all devices whether they are currently displaying the window. */
    it = d3d11_rendering_behaviour::devices.get_enumerator();
    while (it.has_next()) {
        system::com_ptr<ID3D11Device>& d3dDevice = it.next();
        system::com_ptr<IDXGIAdapter> dxgiAdapter;
        system::com_ptr<IDXGIDevice> dxgiDevice;

        /* Convert D3D device into DXGI device. */
        try {
            d3dDevice.query_interface(dxgiDevice);
        } catch (system::com_exception e) {
            hr = e.get_error_code();
        }

        /* Get the adapter that the device was created from. */
        if (SUCCEEDED(hr)) {
            hr = dxgiDevice->GetAdapter(&dxgiAdapter);
        }

        /* Search whether any of the adapter's outputs matches. */
        if (SUCCEEDED(hr)) {
            for (UINT o = 0; SUCCEEDED(hr); ++o) {
                system::com_ptr<IDXGIOutput> dxgiOutput;
                hr = dxgiAdapter->EnumOutputs(o, &dxgiOutput);
                if (SUCCEEDED(hr)) {
                    hr = dxgiOutput->GetDesc(&outputDesc);
                }
                if (SUCCEEDED(hr)) {
                    if (outputDesc.Monitor == hMonitor) {
                        return d3dDevice;
                    }
                }
            } /* end for (UINT o = 0; SUCCEEDED(hr) ... */

            if (hr == DXGI_ERROR_NOT_FOUND) {
                // This is an expected error, so ignore it.
                hr = S_OK;
            }
        }
    } /* end while (it.has_next()) */

    return NULL;
}


/*
 * the::graphics::directx::d3d11_rendering_behaviour::dxgi_factory
 */
the::system::com_ptr<IDXGIFactory>
the::graphics::directx::d3d11_rendering_behaviour::dxgi_factory;


/*
 * the::graphics::directx::d3d11_rendering_behaviour::d3d11_rendering_behaviour
 */
the::graphics::directx::d3d11_rendering_behaviour::d3d11_rendering_behaviour(
        void) : base() {
    THE_STACK_TRACE;
}


/*
 * the::graphics::directx::d3d11_rendering_behaviour::create_depth_stencil_view
 */
void the::graphics::directx::d3d11_rendering_behaviour
        ::create_depth_stencil_view(const DXGI_FORMAT format) {
    THE_STACK_TRACE;
    THE_ASSERT(this->device != NULL);
    THE_ASSERT(this->swapChain != NULL);

    system::com_ptr<ID3D11Texture2D> tex;       // Texture for view.
    D3D11_TEXTURE2D_DESC texDesc;               // Description of 'tex'.
    DXGI_SWAP_CHAIN_DESC swapChainDesc;         // Current swap chain props.
    HRESULT hr = S_OK;                          // Results of API calls.

    /* Release old view. */
    this->depthStencilView = NULL;

    /* Get swap chain dimensions. */
    hr = this->swapChain->GetDesc(&swapChainDesc);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Retrieving swap chain description "
            "failed with error code %d.", hr);
    }

    /* Create texture for depth/stencil view. */
    the::zero_memory(&texDesc);
    texDesc.Width = swapChainDesc.BufferDesc.Width;
    texDesc.Height = swapChainDesc.BufferDesc.Height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = format;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    hr = this->device->CreateTexture2D(&texDesc, NULL, &tex);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating depth/stencil buffer texture "
            "failed with error code %d.", hr);
    }

    /* Create the depth/stencil view. */
    hr = this->device->CreateDepthStencilView(tex, NULL, 
        &this->depthStencilView);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating depth/stencil view failed "
            "with error code %d.", hr);
    }
}


/*
 *the::graphics::directx::d3d11_rendering_behaviour::initialise
 */
void the::graphics::directx::d3d11_rendering_behaviour::initialise(HWND hWnd) {
    THE_STACK_TRACE;

    HRESULT hr = S_OK;
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    RECT wndRect;

    /* Sanity checks. */
    if (this->device != NULL) {
        throw invalid_operation_exception(THE_SYSTEM_STRING_CONSTANT("The ")
            THE_SYSTEM_STRING_CONSTANT("renderer has already been ")
            THE_SYSTEM_STRING_CONSTANT("initialised."), __FILE__, __LINE__);
    }
    THE_ASSERT(this->immediateContext == NULL);
    THE_ASSERT(this->renderTargetView == NULL);
    THE_ASSERT(this->swapChain == NULL);

    /* Get the device that is best for the window. */
    this->device = d3d11_rendering_behaviour::get_device_for_window(hWnd);
    if (this->device == NULL) {
        throw exception(THE_SYSTEM_STRING_CONSTANT("The renderer could not ")
            THE_SYSTEM_STRING_CONSTANT("find an appropriate Direct3D device ")
            THE_SYSTEM_STRING_CONSTANT("for the given window. This should ")
            THE_SYSTEM_STRING_CONSTANT("never happen."), __FILE__, __LINE__);
    }

    /* Get the immediate context of the device. */
    this->device->GetImmediateContext(&this->immediateContext);

    /* Get the window extents to create a matching swap chain. */
    if (!::GetWindowRect(hWnd, &wndRect)) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

    /* Create a swap chain. */
    THE_ASSERT(wndRect.right >= wndRect.left);
    THE_ASSERT(wndRect.bottom >= wndRect.top);
    the::zero_memory(&swapChainDesc);
    swapChainDesc.BufferDesc.Width = wndRect.right - wndRect.left;
    swapChainDesc.BufferDesc.Height = wndRect.bottom - wndRect.top;
    //swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    //swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    //swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    ////swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
    swapChainDesc.Flags = 0;

    /* Allow sub-classes customising the swap chain. */
    this->on_initialising(swapChainDesc);

    hr = d3d11_rendering_behaviour::dxgi_factory->CreateSwapChain(this->device,
        &swapChainDesc, &this->swapChain);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating swap chain failed with error "
            "code %d.", hr);
    }

    /* Create the render target view for the swap chain. */
    this->create_rtv();
    this->update_viewport(swapChainDesc.BufferDesc.Width,
        swapChainDesc.BufferDesc.Height);
}


/*
 * the::graphics::directx::d3d11_rendering_behaviour::on_initialising
 */
void the::graphics::directx::d3d11_rendering_behaviour::on_initialising(
        DXGI_SWAP_CHAIN_DESC& inOutSwapChainDesc) {
    THE_STACK_TRACE;
}


/*
 * the::graphics::directx::d3d11_rendering_behaviour::devices
 */
the::graphics::directx::d3d11_rendering_behaviour::device_list
the::graphics::directx::d3d11_rendering_behaviour::devices;


/*
 * the::graphics::directx::d3d11_rendering_behaviour::create_render_target_view
 */
void the::graphics::directx::d3d11_rendering_behaviour::create_rtv(void) {
    THE_STACK_TRACE;
    THE_ASSERT(this->device != NULL);
    THE_ASSERT(this->swapChain != NULL);
    THE_ASSERT(this->renderTargetView == NULL);

    system::com_ptr<ID3D11Texture2D> backBuffer;
    HRESULT hr = S_OK;

    hr = this->swapChain->GetBuffer(0, IID_ID3D11Texture2D,
        reinterpret_cast<void **>(&backBuffer));
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Retrieving back buffer from swap "
            "chain failed with error code %d.", hr);
    }

    hr = this->device->CreateRenderTargetView(backBuffer, NULL,
        &this->renderTargetView);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating render target view failed "
            "with error code %d.", hr);
    }
}


/*
 * the::graphics::directx::d3d11_rendering_behaviour::update_viewport
 */
void the::graphics::directx::d3d11_rendering_behaviour::update_viewport(
        const size_type width, const size_type height) {
    THE_STACK_TRACE;
    THE_ASSERT(this->immediateContext != NULL);

    D3D11_VIEWPORT viewport;

    the::zero_memory(&viewport);
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0;
    viewport.MaxDepth = 1.0;

    this->immediateContext->RSSetViewports(1, &viewport);
}

#endif /* WITH_THE_DIRECTX11 */
