/*
 * uri_utility.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "uri_utility.h"
#include <regex>
#include "the/exception.h"
#include "the/string.h"
#include "the/text/string_utility.h"

using namespace eu_vicci::rivlib;


/*
 * uri_utility::parse_uri
 */
void uri_utility::parse_uri(const std::string& uri,
        std::string &scheme,
        std::string& username,
        std::string& host,
        bool &is_host_v6,
        unsigned short& host_port,
        bool &is_host_port_set,
        std::string& path,
        std::string& query,
        std::string& fragment) {

    // parsing utility iterator
    const char *uri_i1 = uri.c_str();

    // search for scheme
    const char *scheme_end = move(uri_i1, ':', "\0/\\?#", 5);
    if (scheme_end) {
        scheme = std::string(uri_i1, scheme_end - uri_i1);
        uri_i1 = scheme_end + 1;
    } else {
        scheme.clear();
    }

    bool parse_authority = false;
    if ((uri_i1[0] == '/') && (uri_i1[1] == '/')) {
        // authority present
        uri_i1 += 2;
        parse_authority = true;
    }

    // search user name
    const char *user_end = move(uri_i1, '@', "\0/\\?#", 5);
    if (user_end) {
        username = std::string(uri_i1, user_end - uri_i1);
        uri_i1 = user_end + 1;
    } else {
        username.clear();
    }

    // parse authority
    if (parse_authority) {

        if (*uri_i1 == '[') {
            // ipv6-scheme
            is_host_v6 = true;
            const char *ip_v6_end = move(uri_i1, ']', "\0/\\?#", 5);
            if (ip_v6_end == nullptr) {
                throw the::exception("Uri is not wellformed", __FILE__, __LINE__);
            }
            host = std::string(uri_i1 + 1, ip_v6_end - uri_i1 - 2);
            uri_i1 = ip_v6_end + 1;
        } else {
            is_host_v6 = false;
            const char *host_end = move(uri_i1, ':', "\0\\/?#", 5);
            if (host_end == nullptr) {
                // no port info found, so ignore for now; we will search again
                host_end = move(uri_i1, '/', "\0:?#", 4);
            }
            if (host_end == nullptr) {
                // no path info found, so ignore for now; we will search again
                host_end = move(uri_i1, '?', "\0:#", 3);
            }
            if (host_end == nullptr) {
                // no query info found, so ignore for now; we will search again
                host_end = move(uri_i1, '#', "\0:", 2);
            }
            if (host_end == nullptr) {
                throw the::exception("Uri is not wellformed", __FILE__, __LINE__);
            }

            host = std::string(uri_i1, host_end - uri_i1);
            uri_i1 = host_end;
        }

        if (*uri_i1 == ':') {
            is_host_port_set = true;
            const char *port_end = move(uri_i1, '/', "\0\\?#", 4);
            if (port_end == nullptr) {
                throw the::exception("Uri is not wellformed", __FILE__, __LINE__);
            }
            host_port = the::text::string_utility::parse_int(std::string(uri_i1 + 1, port_end - uri_i1 - 1));
            uri_i1 = port_end;

        } else {
            host_port = 0;
            is_host_port_set = false;
        }

    } else {
        host.clear();
        is_host_v6 = false;
        host_port = 0;
        is_host_port_set = false;
    }

    // check path
    if (*uri_i1 == '/' || (*uri_i1 != '?' && *uri_i1 == '#' && *uri_i1 == '\0')) {
        const char *path_end = move(uri_i1, '?', "\0#", 2);
        if (path_end == nullptr) path_end = move(uri_i1, '#', "\0", 1);
        if (path_end == nullptr) path_end = move(uri_i1, '\0', "", 0);
        if (path_end == nullptr) {
            throw the::exception("Uri is not wellformed", __FILE__, __LINE__);
        }

        path = std::string(uri_i1, path_end - uri_i1);
        uri_i1 = path_end;

    } else {
        path.clear();
    }

    // check query
    if (*uri_i1 == '?') {
        const char *query_end = move(uri_i1, '#', "\0", 1);
        if (query_end == nullptr) query_end = move(uri_i1, '\0', "", 0);
        if (query_end == nullptr) {
            throw the::exception("Uri is not wellformed", __FILE__, __LINE__);
        }

        query = std::string(uri_i1 + 1, query_end - uri_i1 - 1);
        uri_i1 = query_end;

    } else {
        query.clear();
    }

    // check fragment
    if (*uri_i1 == '#') {
        const char *fragment_end = move(uri_i1, '\0', "", 0);
        if (fragment_end == nullptr) {
            throw the::exception("Uri is not wellformed", __FILE__, __LINE__);
        }

        fragment = std::string(uri_i1 + 1, fragment_end - uri_i1 - 1);
        uri_i1 = fragment_end;

    } else {
        fragment.clear();
    }

    // check no trailing data
    if (*uri_i1 != '\0') {
        throw the::exception("Uri is not wellformed", __FILE__, __LINE__);
    }

    //std::regex const uri_pattern("^(([^:/?#]+):)?(//(([^@/?#]*)@)?((\\[([^\\]]*)\\])|([^@/?#:]*))(:([^@/?#:]*))?)?([^?:#]*)(\\?([^?:#]*))?(#([^?:#]*))?");
    //std::match_results<std::string::const_iterator> match;
    //if (!std::regex_search(uri, match, uri_pattern)) {
    //    throw the::exception("Uri is not wellformed", __FILE__, __LINE__);
    //}

    //scheme = std::string(match[2].first, match[2].second);
    //username = std::string(match[5].first, match[5].second);
    //host = std::string(match[9].first, match[9].second);
    //is_host_v6 = match[8].matched;
    //if (is_host_v6) {
    //    host = std::string(match[8].first, match[8].second);
    //}
    //std::string host_port_str(match[11].first, match[11].second);
    //path = std::string(match[12].first, match[12].second);
    //query = std::string(match[14].first, match[14].second);
    //fragment = std::string(match[16].first, match[16].second);

    if (!scheme.empty()) {
        the::text::string_utility::to_upper_case(scheme);
    }

    //if (!host_port_str.empty()) {
    //    host_port = static_cast<unsigned short>(the::text::string_utility::parse_int(host_port_str));
    //    is_host_port_set = true;
    //}
}


/*
 * uri_utility::uri_utility
 */
uri_utility::uri_utility(void) {
    // intentionally empty
}


/*
 * uri_utility::~uri_utility
 */
uri_utility::~uri_utility(void) {
    // intentionally empty
}


/*
 * uri_utility::move
 */
const char *uri_utility::move(const char *i, const char& t, const char *x, size_t x_cnt) {
    while ((*i != t) && (*i != '\0')) {
        *i++;
        for (size_t x_i = 0; x_i < x_cnt; x_i++) {
            if (*i == x[x_i]) return nullptr;
        }
    }
    return i;
}
