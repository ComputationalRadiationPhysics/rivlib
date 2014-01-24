/*
 * rivlib API
 * communicator.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_COMMUNICATOR_H_INCLUDED
#define VICCI_RIVLIB_COMMUNICATOR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "rivlib/common.h"
#include "rivlib/api_ptr.h"
#include "rivlib/provider.h"


#ifdef __cplusplus

namespace eu_vicci {
namespace rivlib {


    /** forward declaration */
    class RIVLIB_API communicator;

    /*
     * force generation of template class for dll interface
     * http://support.microsoft.com/default.aspx?scid=KB;EN-US;168958
     */
    RIVLIB_APIEXT template class RIVLIB_API api_ptr<communicator>;

    /**
     * The base class for communication connections
     */
    class RIVLIB_API communicator {
    public:

        /** The pointer type to be used by the applications */
        typedef api_ptr<communicator> ptr;

        /** dtor */
        virtual ~communicator(void);

        /**
         * Answer the number of public uris the specified provider will be
         * available through this communicator
         *
         * @param prov The provider
         *
         * @return The number of public uris
         */
        virtual size_t count_public_uris(provider::ptr prov) = 0;

        /**
         * Copies one public uri to 'buf' under which the specified provider
         * will be available through this communicator
         *
         * @param prov The provider
         * @param idx The zero-based index of the public uri to be returned
         * @param buf The buffer to write the uri to. If 'nullptr' no data
         *            will be written
         * @param buflen The number of bytes that can safely be written to
         *            'buf'
         *
         * @return If 'buf' is nullptr, returns the length of the requested
         *         uri in bytes/characters. If 'buf' is not nullptr, return
         *         the number of bytes/characters written.
         */
        virtual size_t public_uri(provider::ptr prov, size_t idx, char *buf, size_t buflen) = 0;

    protected:

        /** ctor */
        communicator(void);

    private:

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */

#else /* __cplusplus */

#error C API not implemented yet

#endif /* __cplusplus */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VICCI_RIVLIB_COMMUNICATOR_H_INCLUDED */
