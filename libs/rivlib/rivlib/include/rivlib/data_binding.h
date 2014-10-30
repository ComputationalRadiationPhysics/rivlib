/*
 * rivlib API
 * data_binding.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_DATA_BINDING_H_INCLUDED
#define VICCI_RIVLIB_DATA_BINDING_H_INCLUDED
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
    class RIVLIB_API data_binding;
    
    /*
     * force generation of template class for dll interface
     * http://support.microsoft.com/default.aspx?scid=KB;EN-US;168958
     */
    RIVLIB_APIEXT template class RIVLIB_API api_ptr<data_binding>;

    /**
     * The base class for data connections
     */
    class RIVLIB_API data_binding {
    public:

        /** The pointer type to be used by the applications */
        typedef api_ptr<data_binding> ptr;

        /**
         * Informs the data_binding that new data is now available.
         * The provider object will start reading this data asynchronously,
         * if possible.
         */
        virtual void async_data_available(void) = 0;

        /**
         * Answer whether or not asynchronous reading operations are performed
         * on the current image data.
         */
        virtual bool is_async_operation_running(void) = 0;

        /**
         * Blocks the calling thread until the asynchronous read operation on
         * the current data has been completed (if any is currently
         * performed).
         */
        virtual void wait_async_data_completed(void) = 0;

        /**
         * Aborts any asynchronous reading operations on the data. This
         * methode blocks until all such operations are aborted.
         */
        virtual void wait_async_data_abort(void) = 0;

        /** dtor */
        virtual ~data_binding(void);

    protected:

        /** ctor */
        data_binding(void);

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
#endif /* VICCI_RIVLIB_DATA_BINDING_H_INCLUDED */
