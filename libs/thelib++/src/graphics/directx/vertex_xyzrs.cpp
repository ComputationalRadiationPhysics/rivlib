/*
 * src\graphics\directx\vertex_xyzrs.cpp
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

#include "the/graphics/directx/vertex_xyzrs.h"

#ifdef WITH_THE_DIRECTX

/*
 * the::graphics::directx::vertex_xyzrs_t::vertex_xyzrs_t
 */
the::graphics::directx::vertex_xyzrs_t::vertex_xyzrs_t(
        const position_coordinate_type x,
        const position_coordinate_type y,
        const position_coordinate_type z,
        const texcoord_coordinate_type r,
        const texcoord_coordinate_type s) {
    THE_STACK_TRACE;
    THE_ASSERT(sizeof(*this) == (sizeof(position_type)
        + sizeof(texcoord_type)));
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
    this->texcoords.x = r;
    this->texcoords.y = s;
}

#endif /* WITH_THE_DIRECTX */
