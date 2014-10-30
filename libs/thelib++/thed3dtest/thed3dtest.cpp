/*
 * thed3dtest.cpp
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


/**
 * Entry point of the application.
 *
 * @param hInstance
 * @param hPrevInstance
 * @param lpCmdLine
 * @param nCmdShow
 */
int CALLBACK _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
        LPTSTR lpCmdLine, int nCmdShow) {
    THE_STACK_TRACE;
    the::trace::get_instance().set_level(THE_TRCCHL_THELIB_COMMON + 16, the::trace::LEVEL_ALL);
    the::trace::get_instance().enable_debug_output(true);

    simple_rendering_window::one_time_initialise();

    simple_rendering_window wnd;
    wnd.initialise();

    while (simple_rendering_window::process_message()) {
        wnd.draw();
        wnd.present();
    }

    //std::list<std::shared_ptr<simple_rendering_window> > xxx;
    //auto www = simple_rendering_window::create<simple_rendering_window>(L"asdf", 0, 0, 100, 100);
    //xxx.push_back(www);
    //auto yyy = simple_rendering_window::create_concurrency_groups(xxx);

    simple_rendering_window::one_time_finalise();
    return 0;
}
