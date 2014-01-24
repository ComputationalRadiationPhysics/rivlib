/*
 * rivclnttest.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#include "stdafx.h"
#include "glut_application.h"


/**
 * Application main entry point
 *
 * @param argc The number of command line arguments
 * @param argv The command line arguments
 *
 * @return The application exit code
 */
#if THE_WINDOWS
int _tmain(int argc, _TCHAR* argv[]) {
#else /* THE_WINDOWS */
int main(int argc, char *argv[]) {
#endif /* THE_WINDOWS */
    ::eu_vicci::riv_clnt_test::glut_application app;

    int ret_val = app.run(argc, argv);

    return ret_val;
}
