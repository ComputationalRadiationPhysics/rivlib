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
#include "GL/freeglut.h"
#include "AntTweakBar.h"
#include "test_render_window.h"
#include "rivlib/vicci_middleware_broker.h"
#include "rivlib/ip_communicator.h"
#include "rivlib/simple_console_broker.h"


using namespace eu_vicci;
using namespace eu_vicci::riv_prov_test;


/*
 * glut_application::glut_application
 */
glut_application::glut_application(void) : wnd(), riv_core(NULL) {
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

    // setup the riv core
    this->riv_core = rivlib::core::create();
    if (!this->riv_core) {
        throw the::exception("rivlib core could not be created", __FILE__, __LINE__);
    }

}


/*
 * glut_application::~glut_application
 */
glut_application::~glut_application(void) {
    this->riv_core->shutdown();
    this->riv_core.reset();
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
#if defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0)
    CmdLineParserA::Option vicci_semiwa_disable("noviccisemiwa", "disables the use of the vicci semiwa",
        CmdLineParserA::Option::FLAG_UNIQUE, NULL);
    CmdLineParserA::Option vicci_host("viccihost", "specifies the host and port to the vicci semiwa host machine", 
        CmdLineParserA::Option::FLAG_UNIQUE,
        CmdLineParserA::Option::ValueDesc::ValueList(CmdLineParserA::Option::STRING_VALUE, "host", "host name or ip address")
        ->Add(CmdLineParserA::Option::INT_VALUE, "port", "IP port"));
    CmdLineParserA::Option vicci_jni_classpath("jniclasspath", "specifies a java class path entry for JNI (used by vicci semiwa)", 
        CmdLineParserA::Option::FLAG_NULL,
        CmdLineParserA::Option::ValueDesc::ValueList(CmdLineParserA::Option::STRING_VALUE, "path", "The JNI class path"));
    cmdPrsr.AddOption(&vicci_semiwa_disable);
    cmdPrsr.AddOption(&vicci_host);
    cmdPrsr.AddOption(&vicci_jni_classpath);
#endif /* defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0) */
    CmdLineParserA::Option ip_comm_diable("noipcomm", "disables the use of ip communication",
        CmdLineParserA::Option::FLAG_UNIQUE, NULL);
    CmdLineParserA::Option ip_comm_port("ipcommport", "specifies the port of the ip communication server on this host", 
        CmdLineParserA::Option::FLAG_UNIQUE,
        CmdLineParserA::Option::ValueDesc::ValueList(CmdLineParserA::Option::INT_VALUE, "port", "IP port"));
    CmdLineParserA::Option prov_name('n', "name", "specifies the name for the provider",
        CmdLineParserA::Option::FLAG_UNIQUE,
        CmdLineParserA::Option::ValueDesc::ValueList(CmdLineParserA::Option::STRING_VALUE, "name", "The name"));
    cmdPrsr.AddOption(&prov_name);
    cmdPrsr.AddOption(&ip_comm_diable);
    cmdPrsr.AddOption(&ip_comm_port);

    if (cmdPrsr.Parse(cmd_line.argc(), cmd_line.argv()) < 0) {
        fprintf(stderr, "Command line parsing error\n");
        return -1;
    }

    ::glutInit(&cmd_line.argc(), cmd_line.argv());
    ::glutInitWindowSize(1280, 720);
    ::glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    ::glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    ::TwInit(TW_OPENGL, NULL);
    ::TwGLUTModifiersFunc(glutGetModifiers);

#if defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0)
    if (vicci_semiwa_disable.GetFirstOccurrence() == NULL) {

        // connection configuration to the (java) middleware
        rivlib::broker::ptr vicci_semiwa = rivlib::vicci_middleware_broker::create();

        if (vicci_host.GetFirstOccurrence() != NULL) {
            CmdLineParserA::Argument *a1 = vicci_host.GetFirstOccurrence();
            a1 = cmdPrsr.NextArgument(a1);
            CmdLineParserA::Argument *a2 = cmdPrsr.NextArgument(a1);
            vicci_semiwa.get_as<rivlib::vicci_middleware_broker>()->set_vicci_semiwa_host(a1->GetValueString(), a2->GetValueInt());
        }

        if (vicci_jni_classpath.GetFirstOccurrence() != NULL) {
            for (CmdLineParserA::Argument *a = vicci_jni_classpath.GetFirstOccurrence(); a != NULL; a = vicci_jni_classpath.GetNextOccurrence(a)) {
                vicci_semiwa.get_as<rivlib::vicci_middleware_broker>()->add_java_class_path(a->GetValueString());
            }
        }

        this->riv_core->add_broker(vicci_semiwa);

    }

#endif /* defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0) */

    if (ip_comm_diable.GetFirstOccurrence() == NULL) {
        // setup ip connections configuration
        rivlib::ip_communicator::ptr ip_comm;
        if (ip_comm_port.GetFirstOccurrence() != NULL) {
            ip_comm = rivlib::ip_communicator::create(ip_comm_port.GetFirstOccurrence()->GetValueInt());
        } else {
            ip_comm = rivlib::ip_communicator::create(); // default ctor tries to autodetect everything
        }
        this->riv_core->add_communicator(ip_comm);
    }

    rivlib::broker::ptr sim_con_bro = rivlib::simple_console_broker::create();
    if (sim_con_bro) {
        this->riv_core->add_broker(sim_con_bro);
    }

    //this->wnd.reset(new glut_window("VICCI RIV Provider Test"));

    test_render_window *tstwnd = new test_render_window();
    ::glutReshapeWindow(
        1280 / 3,
        720  / 3);
    const char *name = "riv test";
    if (prov_name.GetFirstOccurrence() != NULL) {
        name = prov_name.GetFirstOccurrence()->GetValueString();
    }
    rivlib::provider::ptr prov(rivlib::provider::create(name));
    this->riv_core->add_provider(prov);
    tstwnd->set_rivlib_provider(prov);
    this->wnd.reset(tstwnd);

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
