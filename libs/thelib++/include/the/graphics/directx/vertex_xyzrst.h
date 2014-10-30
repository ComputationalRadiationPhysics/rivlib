/*
 * include\the\graphics\directx\vertex_xyzrst.h
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

#ifndef THE_GRAPHICS_DIRECTX_VERTEX_XYZRST_H_INCLUDED
#define THE_GRAPHICS_DIRECTX_VERTEX_XYZRST_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#ifdef WITH_THE_DIRECTX
#include <DirectXMath.h>

#include "the/graphics/directx/vertex_pos_tex_base.h"


namespace the {
namespace graphics {
namespace directx {

    /**
     * A three-dimensional vertex with two-dimensional texture coordinates.
     */
    typedef struct vertex_xyzrst_t : public vertex_pos_tex_base<
            DirectX::XMFLOAT3A, float, DXGI_FORMAT_R32G32B32_FLOAT,
            DirectX::XMFLOAT3A, float, DXGI_FORMAT_R32G32B32_FLOAT> {

        /**
         * Initialises a new instance.
         *
         * @param x The x-coordinate of the vertex.
         * @param y The y-coordinate of the vertex.
         * @param z The z-coordinate of the vertex.
         * @param z The w-coordinate of the vertex.
         * @param r The texture coordinate in x-direction.
         * @param s The texture coordiante in y-direction.
         * @param t The texture coordiante in z-direction.
         */
        struct vertex_xyzrst_t(const position_coordinate_type x = 0.0f,
            const position_coordinate_type y = 0.0f,
            const position_coordinate_type z = 0.0f,
            const texcoord_coordinate_type r = 0.0f,
            const texcoord_coordinate_type s = 0.0f,
            const texcoord_coordinate_type t = 0.0f);
    } vertex_xyzrts;

} /* end namespace directx */
} /* end namespace graphics */
} /* end namespace the */

#endif /* WITH_THE_DIRECTX */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_GRAPHICS_DIRECTX_VERTEX_XYZRST_H_INCLUDED */
