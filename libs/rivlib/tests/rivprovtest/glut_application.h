/*
 * glut_application.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVPROVTEST_GLUT_APPLICATION_H_INCLUDED
#define VICCI_RIVPROVTEST_GLUT_APPLICATION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "glut_window.h"
#include "rivlib/core.h"


namespace eu_vicci {
namespace riv_prov_test {

    /**
     * Class managing a glut application
     */
    class glut_application {
    public:

        /** Ctor */
        glut_application(void);

        /** Dtor */
        ~glut_application(void);

        /**
         * Runs the application
         *
         * @param argc The number of command line arguments
         * @param argv The command line arguments
         *
         * @return The application exit code
         */
        int run(int argc, char **argv);

        /**
         * Runs the application
         *
         * @param argc The number of command line arguments
         * @param argv The command line arguments
         *
         * @return The application exit code
         */
        int run(int argc, wchar_t **argv);

    private:

        /** The window */
        std::shared_ptr<glut_window> wnd;

        /** The rivlib core */
        rivlib::core::ptr riv_core;

    };

} /* end namespace riv_prov_test */
} /* end namespace eu_vicci */

#endif /* VICCI_RIVPROVTEST_GLUT_APPLICATION_H_INCLUDED */
