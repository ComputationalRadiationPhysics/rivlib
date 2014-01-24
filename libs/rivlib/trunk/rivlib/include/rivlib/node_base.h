/*
 * rivlib API
 * node_base.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_NODE_BASE_H_INCLUDED
#define VICCI_RIVLIB_NODE_BASE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "rivlib/common.h"

#ifdef __cplusplus

namespace eu_vicci {
namespace rivlib {


    /**
     * Base class for rivlib internal node objects
     */
    class RIVLIB_API node_base {
    public:

        /** Dtor */
        virtual ~node_base(void);

    protected:

        /** Ctor */
        node_base(void);

    };

    
} /* end namespace rivlib */
} /* end namespace eu_vicci */

#else /* __cplusplus */

#error C API not implemented yet

#endif /* __cplusplus */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VICCI_RIVLIB_NODE_BASE_H_INCLUDED */
