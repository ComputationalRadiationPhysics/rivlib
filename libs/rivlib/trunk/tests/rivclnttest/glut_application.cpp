/*
 * glut_application.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#include "stdafx.h"
#include "glh/glh_extensions.h"
#include "glut_application.h"
#include "the/system/cmd_line_provider.h"
#include "vislib/CmdLineParser.h"
#include "the/config.h"
#include "the/exception.h"
#include "the/system/io/ascii_file_buffer.h"
#include "visglut.h"
#include "AntTweakBar.h"
#include "riv_client_window.h"


using namespace eu_vicci;
using namespace eu_vicci::riv_clnt_test;


/**
 * Utility function copying std::strings
 *
 * @param destinationClientString The destination string
 * @param sourceLibraryString The source string
 */
static void TW_CALL CopyStdStringToClient(
        std::string& destinationClientString,
        const std::string& sourceLibraryString) {
  destinationClientString = sourceLibraryString;
}


/*
 * glut_application::glut_application
 */
glut_application::glut_application(void) : wnd() {
#ifdef _WIN32
    DWORD dd[4];
    DWORD pc = ::GetConsoleProcessList(dd, 4);
    if (pc == 1) {
        HWND con_wnd = ::GetConsoleWindow();
        HICON icon =(HICON)::LoadImage(
            ::GetModuleHandle(NULL),
            MAKEINTRESOURCE(101), IMAGE_ICON,
            ::GetSystemMetrics(SM_CXSMICON),
            ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTSIZE);
        ::PostMessage(con_wnd, WM_SETICON, ICON_SMALL, (LPARAM)icon);
        icon =(HICON)::LoadImage(
            ::GetModuleHandle(NULL),
            MAKEINTRESOURCE(101), IMAGE_ICON,
            ::GetSystemMetrics(SM_CXICON),
            ::GetSystemMetrics(SM_CYICON), LR_DEFAULTSIZE);
        ::PostMessage(con_wnd, WM_SETICON, ICON_BIG, (LPARAM)icon);
    }
#endif /* _WIN32 */
}


/*
 * glut_application::~glut_application
 */
glut_application::~glut_application(void) {
    // intentionally empty
}


/*
 * glut_application::run
 */
int glut_application::run(int argc, char **argv) {
    the::system::cmd_line_provider_a cmd_line(argc, argv);

    if (cmd_line.argc() >= 3) {
        if (std::string(cmd_line.argv()[1]).compare("--cmdlinefile") == 0) {
            the::system::io::ascii_file_buffer file;
            try {
                file.load(cmd_line.argv()[2]);
                if (file.size() > 0) {
                    cmd_line.create_cmd_line(argv[0], file[0].get());
                }
            } catch(...) {
            }
        }
    }

    using vislib::sys::CmdLineParserA;
    CmdLineParserA cmdPrsr;
    CmdLineParserA::Option host('h', "host", "The host uri",
        CmdLineParserA::Option::FLAG_UNIQUE,
        CmdLineParserA::Option::ValueDesc::ValueList(CmdLineParserA::Option::STRING_VALUE, "uri", "The host uri"));
    CmdLineParserA::Option connect('c', "connect", "Automatically connects to host after startup",
        CmdLineParserA::Option::FLAG_UNIQUE, NULL);
    cmdPrsr.AddOption(&host);
    cmdPrsr.AddOption(&connect);

    if (cmdPrsr.Parse(cmd_line.argc(), cmd_line.argv()) < 0) {
        fprintf(stderr, "Command line parsing error\n");
        return -1;
    }

    ::glutInit(&cmd_line.argc(), cmd_line.argv());
    ::glutInitWindowSize(1280, 720);
    ::glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    ::glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    ::TwInit(TW_OPENGL, NULL);
    ::TwCopyStdStringToClientFunc(CopyStdStringToClient);
    ::TwGLUTModifiersFunc(glutGetModifiers);


    //this->wnd.reset(new glut_window("VICCI RIV Client Test"));

    riv_client_window *wnd = new riv_client_window();
    if (host.GetFirstOccurrence() != NULL) {
        wnd->set_uri(host.GetFirstOccurrence()->GetValueString());
    }
    if (connect.GetFirstOccurrence() != NULL) {
        wnd->enable_auto_connect();
    }
    this->wnd.reset(wnd);

    ::glutMainLoop();

    return 0;
}


/*
 * glut_application::run
 */
int glut_application::run(int argc, wchar_t **argv) {
    vislib::sys::CmdLineProviderA cmdLine;
    for (int i = 0; i <= argc; i++) {
        cmdLine.Append(vislib::StringA(argv[i]));
    }
    return this->run(cmdLine.ArgC(), cmdLine.ArgV());
}
