/*
 * rivlib API
 * broker.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_BROKER_H_INCLUDED
#define VICCI_RIVLIB_BROKER_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "rivlib/common.h"
#include "rivlib/api_ptr.h"


#ifdef __cplusplus

namespace eu_vicci {
namespace rivlib {
    
    /** forward declaration */
    class RIVLIB_API broker;
    
    /*
     * force generation of template class for dll interface
     * http://support.microsoft.com/default.aspx?scid=KB;EN-US;168958
     */
    RIVLIB_APIEXT template class RIVLIB_API api_ptr<broker>;

    /**
     * The base class for broker connections
     */
    class RIVLIB_API broker {
    public:

        /** The pointer type to be used by the applications */
        typedef api_ptr<broker> ptr;

        /** dtor */
        virtual ~broker(void);

    protected:

        /** ctor */
        broker(void);

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
#endif /* VICCI_RIVLIB_BROKER_H_INCLUDED */
