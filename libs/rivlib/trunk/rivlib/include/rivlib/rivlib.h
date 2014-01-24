/*
 * rivlib.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_RIVLIB_H_INCLUDED
#define VICCI_RIVLIB_RIVLIB_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

/*
 * rivlib API header
 */

/*
 * Here all rivlib header files are included for convenience
 */

/*
 * common header files
 */
#include "rivlib/config.h"
#include "rivlib/common.h"
#include "rivlib/ip_utilities.h"

/*
 * server application header files
 */
#include "rivlib/core.h"
#include "rivlib/provider.h"
#include "rivlib/communicator.h"
#include "rivlib/broker.h"
#include "rivlib/image_data_types.h"
#include "rivlib/data_binding.h"
#include "rivlib/raw_image_data_binding.h"
#include "rivlib/ip_communicator.h"
#if defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0)
#include "rivlib/vicci_middleware_broker.h"
#endif /* defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0) */

/*
 * client application header files
 */
#include "rivlib/control_connection.h"
#include "rivlib/image_stream_connection.h"


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VICCI_RIVLIB_RIVLIB_H_INCLUDED */
