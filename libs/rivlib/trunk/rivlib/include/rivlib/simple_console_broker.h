/*
 * rivlib API
 * simple_console_broker.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_SIMPLE_CONSOLE_BROKER_H_INCLUDED
#define VICCI_RIVLIB_SIMPLE_CONSOLE_BROKER_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "rivlib/common.h"
#include "rivlib/broker.h"


#ifdef __cplusplus

namespace eu_vicci {
namespace rivlib {


    /**
     * The simple console broker publishes riv url information to 'stdout'
     */
    class RIVLIB_API simple_console_broker : public broker {
    public:

        /**
         * Creates a new simple console broker
         *
         * @return A new simple console broker
         */
        static broker::ptr create(void);

        /** dtor */
        virtual ~simple_console_broker(void);

    protected:

        /** ctor */
        simple_console_broker(void);

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
#endif /* VICCI_RIVLIB_SIMPLE_CONSOLE_BROKER_H_INCLUDED */
