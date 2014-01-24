/*
 * rivlib API
 * ip_communicator.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_IP_COMMUNICATOR_H_INCLUDED
#define VICCI_RIVLIB_IP_COMMUNICATOR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "rivlib/common.h"
#include "rivlib/communicator.h"


#ifdef __cplusplus

namespace eu_vicci {
namespace rivlib {


    /**
     * The communication connection using ip protocols
     */
    class RIVLIB_API ip_communicator : public communicator {
    public:

        /**
         * Creates an ip_communicator (TCP)
         *
         * @param port The port to listen on (TCP)
         */
        static communicator::ptr create(unsigned short port = DEFAULT_PORT);

        /** dtor */
        virtual ~ip_communicator(void);

        ///**
        // * magic id header to be used when establishing connections
        // * The size of the struct must be 16 bytes.
        // */
        //typedef struct _magic_id_t {

        //    /** The id string { 'R', 'I', 'V', 0x13, 0x57, 0x9B, 0xDF, 0x00 } */
        //    unsigned char id_str[8];

        //    /** test dword 0x12345678 */
        //    unsigned int tst_dword;

        //    /** test float 2.71828175f */
        //    float tst_float;

        //} magic_id;


        /** The defaul port for ip_communicator server socket */
        static const unsigned short DEFAULT_PORT;

        ///**
        // * Answer all uris which can be used to specify the given 'name' in
        // * the context of this communicator. The container 'out_uris' is
        // * not cleaned by the method, instead names are only appended.
        // *
        // * @param pro The provider to be reachable through this
        // *            communicator. Must not be null.
        // * @param out_uris A vector container to receive all the uris the
        // *            specified 'name' will be available in the context of
        // *            this communicator.
        // */
        //virtual void public_uris(const provider *pro,
        //    std::vector<std::string>& out_uris);

    protected:

        /** ctor */
        ip_communicator(void);

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
#endif /* VICCI_RIVLIB_IP_COMMUNICATOR_H_INCLUDED */
