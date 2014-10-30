/*
 * java_vm_config.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_JAVA_VM_CONFIG_H_INCLUDED
#define VICCI_RIVLIB_JAVA_VM_CONFIG_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "rivlib/common.h"

#if defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0)

#include <vector>
#include <string>


namespace eu_vicci {
namespace rivlib {

    /**
     * Implementation of the core class
     */
    class java_vm_config {
    public:

        /** ctor */
        java_vm_config(void);

        /** dtor */
        ~java_vm_config(void);

        /**
         * Removes all class path information
         */
        inline void clear_class_paths(void) {
            this->class_paths.clear();
        }

        /**
         * Adds one class path entry
         *
         * @param path The new class path entry
         */
        inline void add_class_path(std::string path) {
            this->class_paths.push_back(path);
        }

        /**
         * Answer the vector of class paths
         *
         * @return The vector of class paths
         */
        inline const std::vector<std::string>& get_class_paths(void) const {
            return this->class_paths;
        }

    private:

        /** The java class paths */
        std::vector<std::string> class_paths;

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */

#endif /* defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0) */

#endif /* VICCI_RIVLIB_JAVA_VM_CONFIG_H_INCLUDED */
