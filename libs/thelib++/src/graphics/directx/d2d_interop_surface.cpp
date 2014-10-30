/*
 * src\graphics\directx\d2d_interop_surface.cpp
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

#include "the/graphics/directx/d2d_interop_surface.h"
#ifdef WITH_THE_DIRECTX11

#include <d3dcompiler.h>

#include "the/assert.h"
#include "the/memory.h"
#include "the/trace.h"

#include "the/not_implemented_exception.h"


/*
 * the::graphics::directx::d2d_interop_surface::d2d_interop_surface
 */
the::graphics::directx::d2d_interop_surface::d2d_interop_surface(void)
        : base()  {
    THE_STACK_TRACE;
}


/*
 * the::graphics::directx::d2d_interop_surface::~d2d_interop_surface
 */
the::graphics::directx::d2d_interop_surface::~d2d_interop_surface(void) {
    THE_STACK_TRACE;
    // Nothing needs to be release, because all resources are held using smart
    // pointers.
}


/*
 * the::graphics::directx::d2d_interop_surface::acquire_direct3d10_sync
 */
HRESULT the::graphics::directx::d2d_interop_surface::acquire_direct3d10_sync(
        const uint64_t key, const DWORD timeout)  throw() {
    THE_STACK_TRACE;
    return (this->d3d10Lock.is_not_null())
        ? this->d3d10Lock->AcquireSync(key, timeout)
        : E_POINTER;
}


/*
 * the::graphics::directx::d2d_interop_surface::acquire_direct3d11_sync
 */
HRESULT the::graphics::directx::d2d_interop_surface::acquire_direct3d11_sync(
        const uint64_t key, const DWORD timeout) throw() {
    THE_STACK_TRACE;
    return (this->d3d11Lock.is_not_null())
        ? this->d3d11Lock->AcquireSync(key, timeout)
        : E_POINTER;
}


/*
 * the::graphics::directx::d2d_interop_surface::create_brush
 */
the::system::com_ptr<ID2D1Brush>
the::graphics::directx::d2d_interop_surface::create_brush(
        const D2D1::ColorF& colour) {
    THE_STACK_TRACE;

    HRESULT hr = S_OK;
    the::system::com_ptr<ID2D1Brush> retval;

    /* Sanity check: initialisation must have been completed before. */
    if (this->d2dRenderTarget.is_null()) {
        throw null_pointer_exception("d2dRenderTarget", __FILE__, __LINE__);
    }

    hr = this->d2dRenderTarget->CreateSolidColorBrush(colour,
        reinterpret_cast<ID2D1SolidColorBrush **>(&retval));
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating brush failed with error "
            "code %d.", hr);
    }

    return retval;
}


/*
 * the::graphics::directx::d2d_interop_surface::draw
 */
void the::graphics::directx::d2d_interop_surface::draw(
        const bool acquireMutex) {
    THE_STACK_TRACE;
    HRESULT hr = S_OK;

    /* Sanity check: initialisation must have been completed before. */
    if (this->d3d11ImmediateContext.is_null()) {
        throw null_pointer_exception("d3d11ImmediateContext", __FILE__, 
            __LINE__);
    }

    /* Acquire lock for rendering if requested. */
    if (acquireMutex) {
        hr = this->acquire_direct3d11_sync();
        if (FAILED(hr)) {
            THE_DIRECTX_TRACE_AND_THROW(hr, "Acquiring keyed mutex for "
                "Direct3D 11 failed with error code %d.", hr);
        }
    }

    if (this->d3d11CmdList.is_null()) {
        // Lazily create command list (preserves states).
        this->update_rendering_cmdlist();
    }

    /* Render. */
    this->d3d11ImmediateContext->ExecuteCommandList(this->d3d11CmdList, TRUE);

    /* Release mutex, if previously acquired. */
    if (acquireMutex) {
        hr = this->release_direct3d11_sync();
        if (FAILED(hr)) {
            THE_DIRECTX_TRACE_AND_THROW(hr, "Releasing keyed mutex for "
                "Direct3D 11 failed with error code %d.", hr);
        }
    }
}


/*
 * the::graphics::directx::d2d_interop_surface::initialise
 */
void the::graphics::directx::d2d_interop_surface::initialise(
        IDXGISwapChain *d3d11SwapChain) {
    THE_STACK_TRACE;

    if (d3d11SwapChain == NULL) {
        throw argument_null_exception("d3d11SwapChain", __FILE__, __LINE__);
    }

    this->alloc_dev_independent_res();
    this->alloc_dev_dependent_res(d3d11SwapChain);
    THE_ASSERT(this->d3d11Device.is_not_null());
    this->alloc_rendering_res();
}


/*
 * the::graphics::directx::d2d_interop_surface::prepare_resize
 */
void the::graphics::directx::d2d_interop_surface::prepare_resize(void) {
    THE_STACK_TRACE;
    // The command list holds references to the back buffer, so delete it in
    // order to allow the swap chain to resize.
    this->d3d11CmdList = NULL;
}


/*
 * the::graphics::directx::d2d_interop_surface::release_direct3d10_sync
 */
HRESULT the::graphics::directx::d2d_interop_surface::release_direct3d10_sync(
        const uint64_t key) throw() {
    THE_STACK_TRACE;
    return (this->d3d10Lock.is_not_null())
        ? this->d3d10Lock->ReleaseSync(key)
        : E_POINTER;
}


/*
 * the::graphics::directx::d2d_interop_surface::release_direct3d11_sync
 */
HRESULT the::graphics::directx::d2d_interop_surface::release_direct3d11_sync(
        const uint64_t key) throw() {
    THE_STACK_TRACE;
    return (this->d3d11Lock.is_not_null())
        ? this->d3d11Lock->ReleaseSync(key)
        : E_POINTER;
}


/*
 * the::graphics::directx::d2d_interop_surface::resize
 */
void the::graphics::directx::d2d_interop_surface::resize(
        IDXGISwapChain *d3d11SwapChain) {
    THE_STACK_TRACE;

    HRESULT hr = S_OK;

    if (d3d11SwapChain == NULL) {
        throw argument_null_exception("d3d11SwapChain", __FILE__, __LINE__);
    }

    this->d2dRenderTarget = NULL;
    this->d3d10Lock = NULL;
    this->d3d11Lock = NULL;
    this->d3d11ResView = NULL;
    this->d3d11Surface = NULL;

    this->alloc_dev_dependent_res(d3d11SwapChain);
    this->alloc_shader_res_view();
}


/*
 * the::graphics::directx::d2d_interop_surface::PIXEL_SHADER
 */
const char *the::graphics::directx::d2d_interop_surface::PIXEL_SHADER =
    "Texture2D tex : register(t0);" "\n"
    "SamplerState texSampler : register(s0);" "\n"
    "cbuffer PsParams : register(b0) {" "\n"
    "    float Opacity;" "\n"
    "    float3 Padding1;" "\n"
    "}" "\n"
    "float4 PsMain(" "\n"
    "        float4 position : SV_POSITION," "\n"
    "        float2 texCoords : TEXCOORD0) : SV_TARGET {" "\n"
    "    float4 colour = tex.Sample(texSampler, texCoords);" "\n"
    "    return float4(colour.rgb, colour.a );//* Opacity);" "\n"
    "}";


/*
 * the::graphics::directx::d2d_interop_surface::VERTEX_SHADER
 */
const char *the::graphics::directx::d2d_interop_surface::VERTEX_SHADER =
    "void VsMain(" "\n"
    "        out float4 outPosition: SV_POSITION," "\n"
    "        out float2 outTexCoords : TEXCOORD0," "\n"
    "        in float3 position : POSITION) {" "\n"
    "    outPosition = float4(2.0f * position, 1.0f);" "\n"
    "    outTexCoords.x = position.x + 0.5f;" "\n"
    "    outTexCoords.y = -position.y + 0.5f;" "\n"
    "}";


/*
 * the::graphics::directx::d2d_interop_surface::alloc_dev_dependent_res
 */
void the::graphics::directx::d2d_interop_surface::alloc_dev_dependent_res(
        IDXGISurface *dxgiSurface) {
    THE_STACK_TRACE;
    HRESULT hr = S_OK;
    D2D1_PIXEL_FORMAT pixelFormat;
    D2D1_RENDER_TARGET_PROPERTIES renderTargetProps;
    DXGI_SURFACE_DESC surfaceDesc;

    THE_ASSERT(this->d2dFactory != NULL);
    THE_ASSERT(this->d2dRenderTarget == NULL);
    THE_ASSERT(dxgiSurface != NULL);

    THE_DIRECTX_TRACE_INFO("Retrieving DXGI surface description...\n");
    hr = dxgiSurface->GetDesc(&surfaceDesc);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Retrieving DXGI surface description "
            "failed with error code %d.\n", hr);
    }

    pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, //surfaceDesc.Format, 
        D2D1_ALPHA_MODE_PREMULTIPLIED);
    renderTargetProps = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT, pixelFormat);

    THE_DIRECTX_TRACE_INFO("Creating DXGI surface render target...\n");
    hr = this->d2dFactory->CreateDxgiSurfaceRenderTarget(dxgiSurface, 
        &renderTargetProps, &this->d2dRenderTarget);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating surface render target "
            "failed with error code %d.\n", hr);
    }
}


/*
 * the::graphics::directx::d2d_interop_surface::alloc_dev_dependent_res
 */
void the::graphics::directx::d2d_interop_surface::alloc_dev_dependent_res(
        ID3D11Texture2D *d3d11Surface) {
    system::com_ptr<ID3D10Device1> d3d10Device;     // D3D 10 device object.
    system::com_ptr<IDXGIAdapter> dxgiAdapter;      // Owner of the surface.
    system::com_ptr<IDXGIDevice> dxgiDevice;        // DXGI device object.
    system::com_ptr<IDXGIResource> dxgiResource;    // DXGI surface resource.
    system::com_ptr<IDXGISurface1> dxgiSurface;     // Surface IF of resource.
    UINT d3d10DeviceFlags = 0;                      // Flags for D3D 10.
    HRESULT hr = S_OK;                              // Result of API calls.
    HANDLE hShared = NULL;                          // Shared surface handle.
                                                    // MUST NOT be closed!

    THE_ASSERT(d3d11Surface != NULL);
    this->d3d11Surface = d3d11Surface;

    // Get D3D 11 device, if not yet available.
    if (this->d3d11Device.is_null()) {
        THE_DIRECTX_TRACE_INFO("Retrieving D3D 11 device from shared "
            "surface...\n");
        this->d3d11Surface->GetDevice(&this->d3d11Device);
    }
    THE_ASSERT(this->d3d11Device.is_not_null());

    THE_DIRECTX_TRACE_INFO("Retrieving DXGI device from D3D device...\n");
    this->d3d11Device.query_interface(dxgiDevice);

    THE_DIRECTX_TRACE_INFO("Retrieving DXGI adapter from DXGI...\n");
    hr = dxgiDevice->GetAdapter(&dxgiAdapter);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Retrieving DXGI adapter from DXGI "
            "device failed with error code %d.\n", hr);
    }

    // Create a D3D 10.1 device on the same DXGI adapter that 'd3d11Surface' 
    // resides on.
    THE_DIRECTX_TRACE_INFO("Creating Direct3D 10.1 device...\n");
    d3d10DeviceFlags |= D3D10_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef THE_DEBUG
    d3d10DeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif /* THE_DEBUG */
        hr = ::D3D10CreateDevice1(dxgiAdapter, D3D10_DRIVER_TYPE_HARDWARE, NULL,
        d3d10DeviceFlags, D3D10_FEATURE_LEVEL_10_1, D3D10_1_SDK_VERSION,
        &d3d10Device);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating Direct3D 10.1 device failed "
            "with error code %d.\n", hr);
    }

    d3d10Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Get the DXGI surface from the shared surface in order to open the shared
    // handle for retrieving the keyed mutexes.
    THE_DIRECTX_TRACE_INFO("Querying IDXGIResource from shared surface...\n");
    this->d3d11Surface.query_interface(dxgiResource);

    THE_DIRECTX_TRACE_INFO("Getting shared handle from DXGI resource...\n");
    hr = dxgiResource->GetSharedHandle(&hShared);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Getting shared handle from DXGI "
            "resource failed with error code %d.\n", hr);
    }

    THE_DIRECTX_TRACE_INFO("Opening shared surface on Direct3D 10.1 "
        "device...\n");
    hr = d3d10Device->OpenSharedResource(hShared, IID_IDXGISurface1,
        reinterpret_cast<void **>(&dxgiSurface));
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Opening shared surface on Direct3D "
            "10.1 device failed with error code %d.\n", hr);
    }

    // Get the keyed mutexes for D3D 10.1 and D3D 11.
    THE_DIRECTX_TRACE_INFO("Getting keyed mutex for Direct3D 10.1...\n");
    dxgiSurface.query_interface(this->d3d10Lock);
    THE_DIRECTX_TRACE_INFO("Getting keyed mutex for Direct3D 11...\n");
    this->d3d11Surface.query_interface(this->d3d11Lock);

    // Proceed with standard non-shared resource initialisation.
    this->alloc_dev_dependent_res(dxgiSurface);
}


/*
 * the::graphics::directx::d2d_interop_surface::alloc_dev_dependent_res
 */
void the::graphics::directx::d2d_interop_surface::alloc_dev_dependent_res(
        IDXGISwapChain *d3d11SwapChain) {
    THE_STACK_TRACE;
    system::com_ptr<ID3D11Texture2D>backBuffer;     // The back buffer.
    D3D11_TEXTURE2D_DESC backBufferDesc;    // Format of 'backBuffer'.
    HRESULT hr = S_OK;                      // Result of API calls

    THE_DIRECTX_TRACE_INFO("Getting back buffer...\n");
    hr = d3d11SwapChain->GetBuffer(0, IID_ID3D11Texture2D,
        reinterpret_cast<void **>(&backBuffer));
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Getting back buffer failed with "
            "error code %d.\n", hr);
    }

    THE_DIRECTX_TRACE_INFO("Getting back buffer description...\n");
    backBuffer->GetDesc(&backBufferDesc);
    backBufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET
        | D3D11_BIND_SHADER_RESOURCE;
    backBufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    backBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;
    backBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    // Get D3D 11 device, if not yet available.
    if (this->d3d11Device.is_null()) {
        THE_DIRECTX_TRACE_INFO("Retrieving D3D 11 device from back "
            "buffer...\n");
        backBuffer->GetDevice(&this->d3d11Device);
    }
    THE_ASSERT(this->d3d11Device.is_not_null());

    THE_DIRECTX_TRACE_INFO("Creating shared texture for Direct2D...\n");
    hr = this->d3d11Device->CreateTexture2D(&backBufferDesc, NULL, 
        &this->d3d11Surface);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating shared texture for "
            "Direct2D failed with error code %d.\n", hr);
    }
    THE_DIRECTX_SET_DEBUG_NAME(this->d3d11Surface, "Shared surface of "
        "d2d_interop_surface 0x%x", this);

    // Perform the rest of the initialisation like with a user-provided texture.
    this->alloc_dev_dependent_res(this->d3d11Surface);
}


/*
 * the::graphics::directx::d2d_interop_surface::alloc_dev_independent_res
 */
void the::graphics::directx::d2d_interop_surface::alloc_dev_independent_res(
        void) {
    THE_STACK_TRACE;
    HRESULT hr = S_OK;

    THE_DIRECTX_TRACE_INFO("Creating Direct2D factory...\n");
    hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,
        &this->d2dFactory);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating Direct2D factory failed "
            "with error code %d.\n", hr);
    }
}


/*
 * the::graphics::directx::d2d_interop_surface::alloc_rendering_res
 */
void the::graphics::directx::d2d_interop_surface::alloc_rendering_res(void) {
    THE_STACK_TRACE;
    D3D11_BUFFER_DESC bd;                   // Describes new buffers.
    D3D11_BLEND_DESC blendStateDesc;        // Blend state configuration.
    DWORD dwShaderFlags = 0;                // Compiler flags for shader.
    HRESULT hr = S_OK;                      // Result of API calls.
    pixel_shader_constants initConsts;      // Initial constants.
    D3D11_SUBRESOURCE_DATA initData;        // Buffer initialisation data.
    system::com_ptr<ID3DBlob> msg;          // Messages of shader compiler.
    UINT offset = 0;                        // Offset of VB.
    D3D11_SAMPLER_DESC samplerDesc;         // Texture sampler config.
    system::com_ptr<ID3DBlob> shaderSource; // Shader source code.
    UINT stride = sizeof(vertex_type);      // Stride of VB.

    THE_ASSERT(this->d3d11Device.is_not_null());

    THE_DIRECTX_TRACE_INFO("Getting immediate context...\n");
    this->d3d11Device->GetImmediateContext(&this->d3d11ImmediateContext);
    THE_ASSERT(this->d3d11ImmediateContext.is_not_null());

    // Allocate and fill VB.
    vertex_type vertices[] = {
        vertex_type(-0.5f,  0.5f,  0.5f),
        vertex_type( 0.5f,  0.5f,  0.5f),
        vertex_type(-0.5f, -0.5f,  0.5f),
        vertex_type( 0.5f, -0.5f,  0.5f)
    };

    the::zero_memory(&bd);
    bd.Usage = D3D11_USAGE_IMMUTABLE;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    the::zero_memory(&initData);
    initData.pSysMem = vertices;

    THE_DIRECTX_TRACE_INFO("Allocating vertex buffer...\n");
    hr = this->d3d11Device->CreateBuffer(&bd, &initData,
        &this->d3d11VertexBuffer);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Allocating vertex buffer failed with "
            "error code %d.\n", hr);
    }
    THE_DIRECTX_SET_DEBUG_NAME(this->d3d11VertexBuffer, "Vertex buffer of "
        "d2d_interop_surface 0x%p", this);

    // Prepare the vertex shader.
    THE_DIRECTX_TRACE_INFO("Compiling vertex shader...\n");
    THE_ASSERT(shaderSource.is_null());
    THE_ASSERT(msg.is_null());
    hr = ::D3DCompile(VERTEX_SHADER, ::strlen(VERTEX_SHADER),
         "the::graphics::directx::d2d_interop_surface::VERTEX_SHADER",
         NULL, NULL, "VsMain", "vs_4_0", 0, 0, &shaderSource, &msg);
    if (FAILED(hr)) {
        if (msg.is_not_null()) {
            THE_DIRECTX_TRACE_ERROR(reinterpret_cast<char *>(
                msg->GetBufferPointer()));
        }
        THE_DIRECTX_TRACE_AND_THROW(hr, "Compiling vertex shader failed with "
            "error code %d.\n", hr);
    }

    THE_DIRECTX_TRACE_INFO("Creating vertex shader...\n");
    hr = this->d3d11Device->CreateVertexShader(shaderSource->GetBufferPointer(),
        shaderSource->GetBufferSize(), NULL, &this->d3d11VertexShader);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating vertex shader failed with "
            "error code %d.\n", hr);
    }
    THE_DIRECTX_SET_DEBUG_NAME(this->d3d11VertexShader, "Vertex shader of "
        "d2d_interop_surface 0x%p", this);

    // Create input layout.
    THE_DIRECTX_TRACE_INFO("Creating  input layout from vertex shader "
        "source...\n");
    hr = this->d3d11Device->CreateInputLayout(vertex_type::d3d11_input_layout,
        sizeof(vertex_type::d3d11_input_layout)
        / sizeof(*vertex_type::d3d11_input_layout),
        shaderSource->GetBufferPointer(), shaderSource->GetBufferSize(),
        &this->d3d11InputLayout);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating input layout from vertex "
            "shader source failed with error code %d.\n", hr);
    }
    THE_DIRECTX_SET_DEBUG_NAME(this->d3d11InputLayout, "Input layout of "
        "d2d_interop_surface 0x%p", this);

    // Prepare for re-use.
    msg = NULL;
    shaderSource = NULL;

    // Prepare the pixel shader.
    THE_DIRECTX_TRACE_INFO("Compiling pixel shader...\n");
    THE_ASSERT(shaderSource.is_null());
    THE_ASSERT(msg.is_null());
    hr = ::D3DCompile(PIXEL_SHADER, ::strlen(PIXEL_SHADER), 
        "the::graphics::directx::d2d_interop_surface:PIXEL_SHADER", NULL, NULL,
        "PsMain", "ps_4_0", 0, 0, &shaderSource, &msg);
    if (FAILED(hr)) {
        if (msg.is_not_null()) {
            THE_DIRECTX_TRACE_ERROR(reinterpret_cast<char *>(
                msg->GetBufferPointer()));
        }
        THE_DIRECTX_TRACE_AND_THROW(hr, "Compiling pixel shader failed with "
            "error code %d.\n", hr);
    }

    THE_DIRECTX_TRACE_INFO("Creating pixel shader...\n");
    hr = this->d3d11Device->CreatePixelShader(shaderSource->GetBufferPointer(),
        shaderSource->GetBufferSize(), NULL, &this->d3d11PixelShader);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating pixel shader failed with "
            "error code %d.\n", hr);
    }
    THE_DIRECTX_SET_DEBUG_NAME(this->d3d11PixelShader, "Pixel shader of "
        "d2d_interop_surface 0x%p", this);

    // Create D3D 11 resource view for shared texture.
    this->alloc_shader_res_view();

    // Allocate constant buffer for passing uniform parameters.
    THE_DIRECTX_TRACE_INFO("Creating constant buffer for pixel shader uniform "
        "parameters...\n");
    the::zero_memory(&bd);
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(pixel_shader_constants);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    the::zero_memory(&initConsts);
    initConsts.opacity = 1.0f;

    the::zero_memory(&initData);
    initData.pSysMem = &initConsts;
    initData.SysMemPitch = bd.ByteWidth;

    THE_DIRECTX_TRACE_INFO("Creating constant buffer for pixel shader uniform "
        "parameters...\n");
    hr = this->d3d11Device->CreateBuffer(&bd, &initData,
        &this->d3d11CbPsParams);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating constant buffer for pixel "
            "shader uniform parameters failed with error code %d.\n", hr);
    }
    THE_DIRECTX_SET_DEBUG_NAME(this->d3d11CbPsParams, "Constant buffer for "
        "pixel shader uniform variables of d2d_interop_surface 0x%p", this);

    // Allocate sampler state for reading shared surface.
    THE_DIRECTX_TRACE_INFO("Creating sampler state...\n");
    the::zero_memory(&samplerDesc);
    samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    hr = this->d3d11Device->CreateSamplerState(&samplerDesc,
        &this->d3d11SamplerState);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating sampler state failed with "
            "error code %d.\n", hr);
    }
    THE_DIRECTX_SET_DEBUG_NAME(this->d3d11SamplerState, "Sampler state of "
        "d2d_interop_surface 0x%p", this);

    // Allocate blend state for Porter-Duff over operator.
    THE_DIRECTX_TRACE_INFO("Creating blend state...\n");
    the::zero_memory(&blendStateDesc);
    blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
    blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    blendStateDesc.RenderTarget[0].RenderTargetWriteMask
        = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr = this->d3d11Device->CreateBlendState(&blendStateDesc,
        &this->d3d11BlendState);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating blend state failed with "
            "error code %d.\n", hr);
    }
    THE_DIRECTX_SET_DEBUG_NAME(this->d3d11BlendState, "Blend state of "
        "d2d_interop_surface 0x%p", this);
}


/*
 * the::graphics::directx::d2d_interop_surface::alloc_shader_res_view
 */
void the::graphics::directx::d2d_interop_surface::alloc_shader_res_view(void) {
    THE_STACK_TRACE;

    HRESULT hr = S_OK;                      // API call results.
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;// Resource view description.
    D3D11_TEXTURE2D_DESC texDesc;           // Format of D3D11 texture.

    THE_ASSERT(this->d3d11Device.is_not_null());
    THE_ASSERT(this->d3d11Surface.is_not_null());

    // Release possible old view.
    this->d3d11ResView = NULL;

    THE_DIRECTX_TRACE_INFO("Creating shader resource view...\n");
    this->d3d11Surface->GetDesc(&texDesc);
    the::zero_memory(&srvDesc);
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

    hr = this->d3d11Device->CreateShaderResourceView(this->d3d11Surface,
        &srvDesc, &this->d3d11ResView);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating shader resource view failed "
            "with error code %d.\n", hr);
    }
    THE_DIRECTX_SET_DEBUG_NAME(this->d3d11ResView, "Shader resource view for "
        "D3D 11 texture of d2d_interop_surface 0x%p", this);
}


/*
 * the::graphics::directx::d2d_interop_surface::update_rendering_cmdlist
 */
void the::graphics::directx::d2d_interop_surface::update_rendering_cmdlist(
        void) {
    THE_STACK_TRACE;

    THE_ASSERT(this->d3d11CbPsParams != NULL);
    THE_ASSERT(this->d3d11Device != NULL);
    THE_ASSERT(this->d3d11ImmediateContext != NULL);
    THE_ASSERT(this->d3d11InputLayout != NULL);
    THE_ASSERT(this->d3d11PixelShader != NULL);
    THE_ASSERT(this->d3d11ResView != NULL);
    THE_ASSERT(this->d3d11VertexBuffer != NULL);
    THE_ASSERT(this->d3d11VertexShader != NULL);

    static const UINT offset = 0;
    static const UINT stride = sizeof(vertex_type);

    UINT cntViewports = 1;
    system::com_ptr<ID3D11DeviceContext> defCtx;
    system::com_ptr<ID3D11DepthStencilView> dsv;
    ID3D11Buffer *dumbBuffer = NULL;
    ID3D11ShaderResourceView *dumbSrv = NULL;
    ID3D11RenderTargetView *dumbRtv = NULL;
    ID3D11SamplerState *dumbSS= NULL;
    HRESULT hr = S_OK;
    system::com_ptr<ID3D11RenderTargetView> rtv;
    D3D11_VIEWPORT viewport;

    // Allocate the command list for rendering the surface.
    hr = this->d3d11Device->CreateDeferredContext(0, &defCtx);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Creating deferred context failed with "
            "error code %d.\n", hr);
    }

    // Prepare input assembler.
    dumbBuffer = static_cast<ID3D11Buffer *>(this->d3d11VertexBuffer);
    defCtx->IASetVertexBuffers(0, 1, &dumbBuffer, &stride, &offset);
    defCtx->IASetInputLayout(this->d3d11InputLayout);
    defCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // Prepare vertex shader.
    defCtx->VSSetShader(this->d3d11VertexShader, NULL, 0);

    // Prepare pixel shader.
    defCtx->PSSetShader(this->d3d11PixelShader, NULL, 0);
    dumbBuffer = static_cast<ID3D11Buffer *>(this->d3d11CbPsParams);
    defCtx->PSSetConstantBuffers(0, 1, &dumbBuffer);
    dumbSrv = static_cast<ID3D11ShaderResourceView *>(this->d3d11ResView);
    defCtx->PSSetShaderResources(0, 1, &dumbSrv);
    dumbSS = static_cast<ID3D11SamplerState *>(this->d3d11SamplerState);
    //defCtx->PSSetSamplers(0, 1, &dumbSS);

    // Set rasteriser state.
    this->d3d11ImmediateContext->RSGetViewports(&cntViewports, &viewport);
    defCtx->RSSetViewports(1, &viewport);

    // Prepare output merger.
    defCtx->OMSetBlendState(this->d3d11BlendState, NULL, UINT_MAX);
    // TODO: That is really nasty...
    this->d3d11ImmediateContext->OMGetRenderTargets(1, &rtv, &dsv);
    dumbRtv = static_cast<ID3D11RenderTargetView *>(rtv);
    defCtx->OMSetRenderTargets(1, &dumbRtv, dsv);

    // Render.
    defCtx->Draw(4, 0);

    // Get a command list containing all the stuff done on 'defCtx'.
    hr = defCtx->FinishCommandList(TRUE, &this->d3d11CmdList);
    if (FAILED(hr)) {
        THE_DIRECTX_TRACE_AND_THROW(hr, "Finishing command list failed with "
            "error code %d.\n", hr);
    }
}

#endif /* WITH_THE_DIRECTX11 */
