/*
 * provider.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "rivlib/provider.h"
#include "api_impl/provider_impl.h"
//#include "the/assert.h"
//#include "the/string.h"
#include "the/text/string_converter.h"
//#include "the/memory.h"
//#include "core_impl.h"

using namespace eu_vicci::rivlib;


/*
 * provider::create_provider
 */
provider::ptr provider::create(const char * name) {
    the::wstring n;
    if (name != nullptr) {
        the::text::string_converter::convert(n, name);
    }
    return new provider_impl(n);
}


/*
 * provider::create_provider
 */
provider::ptr provider::create(const wchar_t * name) {
    return new provider_impl(name);
}


/*
 * provider::provider
 */
provider::provider(void) {
    // intentionally empty
}


/*
 * provider::~provider
 */
provider::~provider(void) {
    // intentionally empty
}
