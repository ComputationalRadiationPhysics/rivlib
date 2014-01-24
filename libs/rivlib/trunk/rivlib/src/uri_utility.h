/*
 * uri_utility.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_URI_UTILITY_H_INCLUDED
#define VICCI_RIVLIB_URI_UTILITY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include <string>


namespace eu_vicci {
namespace rivlib {


    /**
     * uri utility class
     */
    class uri_utility {
    public:

        /**
         * parses a riv-uri/url string
         *
         * @param uri The input uri
         * @param scheme Receives the scheme
         * @param username Receives the user name
         * @param host Receives the host name or address
         * @param is_host_v6 Is set to true if the host address is IPv6
         * @param host_port Receives the port
         * @param is_host_port_set Is set to true if the host port is set
         * @param path Receives the request path
         * @param query Receives the request query
         * @param fragment Receives the request fragment
         */
        static void parse_uri(const std::string& uri,
            std::string &scheme,
            std::string& username,
            std::string& host,
            bool &is_host_v6,
            unsigned short& host_port,
            bool &is_host_port_set,
            std::string& path,
            std::string& query,
            std::string& fragment);

    private:

        /** ctor */
        uri_utility(void);

        /** dtor */
        ~uri_utility(void);

        static const char *move(const char *i, const char& t, const char *x, size_t x_cnt);

    };

} /* end namespace rivlib */
} /* end namespace eu_vicci */

#endif /* VICCI_RIVLIB_URI_UTILITY_H_INCLUDED */
