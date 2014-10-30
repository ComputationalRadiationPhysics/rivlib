/*
 * include\the\graphics\directx\d3d11_rendering_behaviour.inl
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

#include "the/invalid_operation_exception.h"


/*
 * the::graphics::directx::d3d11_rendering_behaviour::create_concurrency_groups
 */
template<template<class W, class R> class T>
std::list<std::list<std::shared_ptr<T> > >
the::graphics::directx::d3d11_rendering_behaviour::create_concurrency_groups(
        const std::list<std::shared_ptr<T> >& windows) {
    THE_STACK_TRACE;
    typedef std::shared_ptr<T> window;
    typedef std::list<std::shared_ptr<T> > window_list;
    typedef window_list concurrency_group;

    std::list<concurrency_group> retval;

    if (d3d11_rendering_behaviour::devices.size() < 1) {
        THROW_THE_EXCEPTION(the::invalid_operation_exception,
            THE_SYSTEM_STRING_CONSTANT("d3d11_rendering_behaviour must have ")
            THE_SYSTEM_STRING_CONSTANT("been successfully initialised in ")
            THE_SYSTEM_STRING_CONSTANT("order to enumerate the available ")
            THE_SYSTEM_STRING_CONSTANT("graphics cards before it can sort ")
            THE_SYSTEM_STRING_CONSTANT("concurrency groups."));
    }

    THE_DIRECTX_TRACE_INFO("Sorting %u rendering windows according to the "
        "Direct3D device they use...\n", windows.size());
    for (window_list::const_iterator w = windows.begin();
            w != windows.end(); ++w) {
        auto wnd = *w;
        auto dev = wnd->device;

        for (std::list<concurrency_group>::iterator g = retval.begin();
                g != retval.end(); ++g) {
            if (g->front()->device == dev) {
                THE_DIRECTX_TRACE_INFO("Found existing concurrency group.\n");
                g->push_back(wnd);
                wnd = nullptr;
            }
        }

        if (wnd != nullptr) {
            THE_DIRECTX_TRACE_INFO("Creating new concurrency group.\n");
            concurrency_group cg;
            cg.push_back(wnd);
            retval.push_back(cg);
        }
    }

    return retval;
}
