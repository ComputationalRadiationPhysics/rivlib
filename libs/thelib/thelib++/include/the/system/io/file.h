/*
 * include\the\system\io\file.h
 *
 * Copyright (C) 2012 TheLib Team (http://www.thelib.org/license)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of TheLib, TheLib Team, nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THELIB TEAM AS IS AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THELIB TEAM BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * File.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_FILE_H_INCLUDED
#define THE_FILE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#if defined(THE_LINUX)
#include <unistd.h>

// Enable large IO:
#define _FILE_OFFSET_BITS 64
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif /* _LARGEFILE64_SOURCE */

#include <climits>
#include <cstdio>

#include <fcntl.h>

#include <sys/stat.h>
#endif /* defined(THE_LINUX) */

#include "the/not_copyable.h"
#include "the/blob.h"
#include "the/stack_trace.h"
#include "the/string.h"
#include "the/types.h"


#if defined(_MSC_VER)
#pragma comment(lib, "shlwapi")
#endif /* defined(_MSC_VER) */


namespace the {
namespace system {
namespace io {

    /**
     * Instances of this class repsesent a file. The class provides unbuffered
     * read and write access to the file. The implementation uses 64 bit 
     * operations on all platforms to allow access to very large files.
     *
     * @author Christoph Mueller (christoph.mueller@vis.uni-stuttgart.de)
     */
    class file : public the::not_copyable {

    public:

        /**
         * Possible values for the access mode.
         *
         * 'read' means that the file will be opened for reading only.
         * 'write' means that the file will be opened for writing only.
         * 'read_write' means that the file will be opened for reading and 
         * writing.
         */
        enum class access {
#if defined(THE_WINDOWS)
            read = static_cast<int>(GENERIC_READ),
            write = static_cast<int>(GENERIC_WRITE),
            read_write = static_cast<int>(GENERIC_READ | GENERIC_WRITE)
#elif defined(THE_LINUX)
            read = O_RDONLY,
            write = O_WRONLY,
            read_write = O_RDWR
#else /* defined(THE_WINDOWS) */
#error "the::system::io::file must define access!"
#endif /* defined(THE_WINDOWS) */
        };

        /** This type is used for offsets when seeking in files. */
        typedef int64_t offset_type;

        /**
         * Specifies how the operating system should open a file.
         *
         * 'create_new' specifies that the operating system should create a new
         * file. The operation will fail if the file already exists.
         * 'create' specifies that the operating system should create a new file
         * or overwrite an existing one.
         * 'open' specifies that the operating system should open an existing 
         * file. The operation will fail if the file does not exist.
         * 'open_or_create' specifies that the operating system should open a 
         * file if it exists or create a new one if it does not exist.
         * 'truncate' indicates that the operating system should open an 
         * existing file and truncate its size to zero.
         */
        enum class mode {
#if defined(THE_WINDOWS)
            create_new = CREATE_NEW,
            create = CREATE_ALWAYS,
            open = OPEN_EXISTING,
            open_or_create = OPEN_ALWAYS,
            truncate = TRUNCATE_EXISTING
#elif defined(THE_LINUX)
            create_new = 1,
            create,
            open,
            open_or_create,
            truncate
#else /* defined(THE_WINDOWS) */
#error "the::system::io::file must define access!"
#endif /* defined(THE_WINDOWS) */
        };

        /** The type of the native file attribute structure. */
#if defined(THE_WINDOWS)
        typedef WIN32_FILE_ATTRIBUTE_DATA native_attribute_type;
#elif defined(THE_LINUX)
        typedef struct stat native_attribute_type;
#else /* defined(THE_WINDOWS) */
#error "the::system::io::file must define native_attribute_type!"
#endif /* defined(THE_WINDOWS) */

        /** The type of the native file handle. */
#if defined(THE_WINDOWS)
        typedef HANDLE native_handle_type;
#elif defined(THE_LINUX)
        typedef int native_handle_type;
#else /* defined(THE_WINDOWS) */
#error "the::system::io::file must define native_handle_type!"
#endif /* defined(THE_WINDOWS) */

        /**
         * Possible origins for seek operations.
         *
         * 'begin' indicates that the seek operation should start at the begin 
         * of the file.
         * 'current' indicates that the seek operation should continue at the
         * current location of the file pointer.
         * 'end' indicates that the seek operation should start at the end of 
         * the file. Only negative offsets are reasonable in this case.
         */
        enum class seek_origin { 
#if defined(THE_WINDOWS)
            begin = FILE_BEGIN, 
            current = FILE_CURRENT, 
            end = FILE_END 
#elif defined(THE_LINUX)
            begin = SEEK_SET,
            current = SEEK_CUR,
            end = SEEK_END
#else /* defined(THE_WINDOWS) */
#error "the::system::io::file must define seek_origin!"
#endif /* defined(THE_WINDOWS) */
        };

        /**
         * Possible values for the share mode. 
         *
         * 'none' indicates that the file cannot be shared while it is open. The
         * object can use the file exclusively in this case.
         * 'read' indicates that other file handles with read access can be 
         * opened while the file is open.
         * 'write' indicates that other file handles with write access can be
         * opened while the file is open.
         * 'read_write' indicates that file handles with any access can be opened
         * while the file is open.
         */
        enum class share {
#if defined(THE_WINDOWS)
            none = 0, 
            read = FILE_SHARE_READ,
            write = FILE_SHARE_WRITE,
            read_write = FILE_SHARE_READ | FILE_SHARE_WRITE
#elif defined(THE_LINUX)
            none = 0, 
            read,
            write,
            read_write
#else /* defined(THE_WINDOWS) */
#error "the::system::io::file must define share!"
#endif /* defined(THE_WINDOWS) */
        };

        /** This type is used for size information of files. */
        typedef uint64_t size_type;

        /** The value of a native invalid file handle. */
        static const native_handle_type native_invalid_handle_value;

        // TODO: Move temp files to path class
        ///**
        // * Creates a temporary file. The file has 'AccessMode' 'READ_WRITE'.
        // * Because the name of the file is highly OS configuration dependent
        // * there is no way of opening the file a second time. Therefore the
        // * 'ShareMode' is 'SHARE_EXCLUSIVE'. The content of the temporary file
        // * will be held in memory as long as the cache size is sufficient. The
        // * file on secondary storage will be deleted when the file is closed.
        // * This will also happen if 'Open' is called on the returned object.
        // *
        // * @return A pointer to the 'File' object of the created temporary
        // *         file. This object is placed on the heap and the caller must
        // *         delete it when it is no longer needed (best practice is to
        // *         assign the returned pointer to a 'SmartPtr' object). The
        // *         return value is 'NULL' if there was an unexpected error.
        // *
        // * @throws io_exception in most error cases.
        // */
        //static File* CreateTempFile(void);
        //
        ///**
        // * Creates a file name for a temporary file
        // *
        // * @param outFn the string to receive the file name created
        // *
        // * @return outFn
        // */
        //static vislib::StringA& CreateTempFileName(vislib::StringA& outFn);

        ///**
        // * Creates a file name for a temporary file
        // *
        // * @param outFn the string to receive the file name created
        // *
        // * @return outFn
        // */
        //static vislib::StringW& CreateTempFileName(vislib::StringW& outFn);

        ///**
        // * Creates a file name for a temporary file
        // *
        // * @return the file name created
        // */
        //static inline vislib::StringA CreateTempFileNameA(void) {
        //    vislib::StringA s;
        //    return CreateTempFileName(s);
        //}

        ///**
        // * Creates a file name for a temporary file
        // *
        // * @return the file name created
        // */
        //static inline vislib::StringW CreateTempFileNameW(void) {
        //    vislib::StringW s;
        //    return CreateTempFileName(s);
        //}

        /**
         * Answer whether a file with the specified name exists.
         *
         * @param fileName Path to the file to be tested.
         *
         * @return true, if the specified file exists, false otherwise.
         */
        static bool exists(const char *fileName);

        /**
         * Answer whether a file with the specified name exists.
         *
         * @param fileName Path to the file to be tested.
         *
         * @return true, if the specified file exists, false otherwise.
         */
        static bool exists(const wchar_t *fileName);

        /**
         * Answer the native file attributes for 'fileName'.
         *
         * @param outAttributes Receives the file attributes in case of success.
         * @param fileName      Path to the file to get the attribtues for.
         *
         * @throws io_exception If the operation failed.
         */
        static void get_attributes(native_attribute_type& outAttributes,
            const char *fileName);

        /**
         * Answer the native file attributes for 'fileName'.
         *
         * @param outAttributes Receives the file attributes in case of success.
         * @param fileName      Path to the file to get the attribtues for.
         *
         * @throws io_exception If the operation failed.
         */
        static void get_attributes(native_attribute_type& outAttributes,
            const wchar_t *fileName);

        /**
         * Answer the size of a file.
         *
         * @param fileName Path to the file
         *
         * @return The size of the file.
         *
         * @throws io_exception If the operation failed.
         */
        static size_type get_size(const char *fileName);

        /**
         * Answer the size of a file.
         *
         * @param fileName Path to the file
         *
         * @return The size of the file.
         *
         * @throws io_exception If the operation failed.
         */
        static size_type get_size(const wchar_t *fileName);

        // TODO: Move to directory class
        ///**
        // * Answer whether a file with the specified name is a directory.
        // *
        // * @param fileName Path to the file to be tested.
        // *
        // * @return true, if the specified file exists and is a directory,
        // *         false otherwise.
        // */
        //static bool IsDirectory(const char *fileName);

        // TODO: Move to directory class
        ///**
        // * Answer whether a file with the specified name is a directory.
        // *
        // * @param fileName Path to the file to be tested.
        // *
        // * @return true, if the specified file exists and is a directory,
        // *         false otherwise.
        // */
        //static bool IsDirectory(const wchar_t *fileName);

        /**
         * Answer whether a file with the specified name is a normal file (not 
         * a directory or any other special file system element).
         *
         * @param fileName Path to the file to be tested.
         *
         * @return true, if the specified file exists and is a normal file,
         *         false otherwise.
         */
        static bool is_file(const char *fileName) throw();

        /**
         * Answer whether a file with the specified name is a normal file (not 
         * a directory or any other special file system element).
         *
         * @param fileName Path to the file to be tested.
         *
         * @return true, if the specified file exists and is a normal file,
         *         false otherwise.
         */
        static bool is_file(const wchar_t *fileName) throw();

        /**
         * Rename the file 'oldName' to 'newName'.
         *
         * @param oldName The name of the file to be renamed.
         * @param newName The new name of the file.
         *
         * @throws io_exception If the operation failed.
         */
        static void move(const char *oldName, const char *newName);

        /**
         * Rename the file 'oldName' to 'newName'.
         *
         * @param oldName The name of the file to be renamed.
         * @param newName The new name of the file.
         *
         * @throws io_exception If the operation failed.
         */
        static void move(const wchar_t *oldName, const wchar_t *newName);

        /**
         * Opens the given file for reading and reads all content from it.
         *
         * @param outDst   Receives all data from the file. The content will be
         *                 overwritten. However, only the number of bytes 
         *                 returned are valid.
         * @param fileName The name of the file to be read.
         *
         * @return The number of bytes read, which is equal to the size of the
         *         file.
         *
         * @throws argument_null_exception If 'fileName' is NULL.
         * @throws io_exception            If the operation failed.
         */
        static size_type read_all_bytes(blob& outDst, 
            const char *fileName);

        /**
         * Opens the given file for reading and reads all content from it.
         *
         * @param outDst   Receives all data from the file. The content will be
         *                 overwritten. However, only the number of bytes 
         *                 returned are valid.
         * @param fileName The name of the file to be read.
         *
         * @return The number of bytes read, which is equal to the size of the
         *         file.
         *
         * @throws argument_null_exception If 'fileName' is NULL.
         * @throws io_exception            If the operation failed.
         */
        static size_type read_all_bytes(blob& outDst, 
            const wchar_t *fileName);

        /**
         * Delete the file with the specified name.
         *
         * @param fileName The name of the file to be deleted.
         *
         * @throws io_exception If the operation failed.
         */
        static void remove(const char *fileName);

        /**
         * Delete the file with the specified name.
         *
         * @param fileName The name of the file to be deleted.
         *
         * @throws io_exception If the operation failed.
         */
        static void remove(const wchar_t *fileName);

        /**
         * Writes all bytes from 'buf' into the file with the specified name.
         *
         * @param fileName The name of the file to write to.
         * @param buf      The buffer that we will write from. This must be a
         *                 pointer to valid 'bufSize' bytes.
         * @param bufSize  The size of 'buf' in bytes.
         *
         * @throws argument_null_exception If 'fileName' or 'buf' is NULL.
         * @throws io_exception            If the operation failed.
         */
        static void write_all_bytes(const char *fileName, const void *buf, 
            const size_type bufSize);

        /**
         * Writes all bytes from 'buf' into the file with the specified name.
         *
         * @param fileName The name of the file to write to.
         * @param buf      The buffer that we will write from. This must be a
         *                 pointer to valid 'bufSize' bytes.
         * @param bufSize  The size of 'buf' in bytes.
         *
         * @throws argument_null_exception If 'fileName' or 'buf' is NULL.
         * @throws io_exception            If the operation failed.
         */
        static void write_all_bytes(const wchar_t *fileName, const void *buf, 
            const size_type bufSize);

        /**
         * Initialises a new instance.
         */
        file(void);

        /**
         * Dtor. 
         *
         * If the file is still open, it will be closed.
         */
        virtual ~file(void);

        /**
         * Close the file. 
         *
         * @throws io_exception If the operation failed.
         */
        virtual void close(void);

        /**
         * Forces all buffered data to be written. 
         *
         * @throws IOException
         */
        virtual void flush(void);

        /**
         * Answer the size of the file in bytes.
         *
         * @return The size of the file in bytes.
         *
         * @throws IOException If the file size cannot be retrieve, e. g. 
         *                     because the file has not been opened.
         */
        virtual size_type get_size(void) const;

        /**
         * Answer whether the file pointer is at the end of the file.
         *
         * @return true, if the eof flag is set, false otherwise.
         *
         * @throws IOException If the file is not open or the file pointer is at an
         *                     invalid position at the moment.
         */
        inline bool is_eof(void) const {
            return (this->tell() == this->get_size());
        }

        /**
         * Answer whether this file is open.
         *
         * @return true, if the file is open, false otherwise.
         */
        virtual bool is_open(void) const;

        /**
         * Opens a file.
         *
         * @param fileName   Path to the file to be opened.
         * @param fileMode   Specfies the creation disposition for the file.
         * @param fileAccess The access mode for the file to be opened. This 
         *                   parameter defaults to access::read_write.
         * @param fileShare  The share mode. This parameter defaults to
         *                   share::node. This parameter is ignored on Linux
         *                   systems.
         *
         * @throws argument_exception          If one of 'fileMode', 
         *                                     'fileAccess' or 'fileShare' are
         *                                     invalid.
         * @throws argument_null_exception     If 'fileName' is NULL.
         * @throws io_exception                If the operation failed.
         * @throws invalid_operation_exception If the file was already open.
         */
        virtual void open(const char *fileName, const mode fileMode,
            const access fileAccess = access::read_write, 
            const share fileShare = share::none);

        /**
         * Opens a file.
         *
         * @param fileName   Path to the file to be opened.
         * @param fileMode   Specfies the creation disposition for the file.
         * @param fileAccess The access mode for the file to be opened. This 
         *                   parameter defaults to access::read_write.
         * @param fileShare  The share mode. This parameter defaults to
         *                   share::node. This parameter is ignored on Linux
         *                   systems.
         *
         * @throws argument_exception          If one of 'fileMode', 
         *                                     'fileAccess' or 'fileShare' are
         *                                     invalid.
         * @throws argument_null_exception     If 'fileName' is NULL.
         * @throws io_exception                If the operation failed.
         * @throws invalid_operation_exception If the file was already open.
         */
        virtual void open(const wchar_t *fileName, const mode fileMode,
            const access fileAccess = access::read_write, 
            const share fileShare = share::none);

        /**
         * Opens a file.
         *
         * @param fileName   Path to the file to be opened.
         * @param fileMode   Specfies the creation disposition for the file.
         * @param fileAccess The access mode for the file to be opened. This 
         *                   parameter defaults to access::read_write.
         * @param fileShare  The share mode. This parameter defaults to
         *                   share::node. This parameter is ignored on Linux
         *                   systems.
         *
         * @throws argument_exception          If one of 'fileMode', 
         *                                     'fileAccess' or 'fileShare' are
         *                                     invalid.
         * @throws io_exception                If the operation failed.
         * @throws invalid_operation_exception If the file was already open.
         */
        inline void open(const astring& fileName, const mode fileMode,
                const access fileAccess = access::read_write, 
                const share fileShare = share::none) {
            THE_STACK_TRACE;
            this->open(fileName.c_str(), fileMode, fileAccess, fileShare);
        }

        /**
         * Opens a file.
         *
         * @param fileName   Path to the file to be opened.
         * @param fileMode   Specfies the creation disposition for the file.
         * @param fileAccess The access mode for the file to be opened. This 
         *                   parameter defaults to access::read_write.
         * @param fileShare  The share mode. This parameter defaults to
         *                   share::node. This parameter is ignored on Linux
         *                   systems.
         *
         * @throws argument_exception          If one of 'fileMode', 
         *                                     'fileAccess' or 'fileShare' are
         *                                     invalid.
         * @throws io_exception                If the operation failed.
         * @throws invalid_operation_exception If the file was already open.
         */
        inline void open(const wstring& fileName, const mode fileMode,
                const access fileAccess = access::read_write, 
                const share fileShare = share::none) {
            THE_STACK_TRACE;
            this->open(fileName.c_str(), fileMode, fileAccess, fileShare);
        }

        /**
         * Read at most 'bufSize' bytes from the file into 'outBuf'.
         *
         * @param outBuf  The buffer to receive the data.
         * @param bufSize The size of 'outBuf' in bytes.
         *
         * @return The number of bytes actually read.
         *
         * @throws io_exception If the operation failed.
         */
        virtual size_type read(void *outBuf, const size_type bufSize);

        /**
         * Reads all bytes from the file into 'outDst'.
         *
         * @param outDst    The buffer that will receive the data. The buffer 
         *                  will be overwritten starting at 'dstOffset'.
         * @param dstOffset The offset in bytes into 'outDst' where the first
         *                  data from the file will be written. The data before
         *                  this position will not be touched. This parameter
         *                  defaults to 0, i.e. 'outDst' will be overwritten
         *                  completely.
         * @param from      Controls where the method starts reading, either
         *                  seek_origin::begin or seek_origin::current. Using
         *                  seek_origin::end is invalid. This parameter defaults
         *                  to seek_origin::begin.
         *
         * @return The number of bytes read. All data after this position in 
         *         'outDst' are invalid.
         *
         * @throws argument_exception If 'from' is invalid.
         * @throws io_exception       If the operation failed.
         */
        size_type read_all_bytes(blob& outDst, 
            const size_type dstOffset = 0, 
            const seek_origin from = seek_origin::begin);

        /**
         * Move the file pointer.
         *
         * If the file pointer is seeked beyond the end of file, the behaviour is 
         * undefined for read(), write(), tell() and is_eof().
         *
         * @param offset The offset in bytes.
         * @param from   The begin of the seek operation. This defaults to
         *               seek_origin::begin.
         *
         * @return The new offset in bytes of the file pointer from the begin of 
         *         the file.
         *
         * @throws io_exception If the file pointer could not be moved, e.g. 
         *                      because the file was not open or the offset was
         *                      invalid.
         */
        virtual size_type seek(const offset_type offset, 
            const seek_origin from = seek_origin::begin);

        /**
         * Move the file pointer to the begin of the file.
         *
         * @return The new offset of the file pointer from the beginning of the
         *         file. This should be zero...
         *
         * @throws io_exception If the file pointer could not be moved, e.g. 
         *                      because the file was not open or the offset was
         *                      invalid.
         */
        inline size_type seek_to_begin(void) {
            THE_STACK_TRACE;
            return this->seek(static_cast<offset_type>(0), seek_origin::begin);
        }

        /**
         * Move the file pointer to the end of the file.
         *
         * @return The new offset of the file pointer from the beginning of the
         *         file. This should be the size of the file.
         *
         * @throws io_exception If the file pointer could not be moved, e.g. 
         *                      because the file was not open or the offset was
         *                      invalid.
         */
        inline size_type seek_to_end(void) {
            THE_STACK_TRACE;
            return this->seek(static_cast<offset_type>(0), seek_origin::end);
        }

        /**
         * Returns the position of the current file pointer.
         *
         * @return The position of the file pointer in bytes from the beginning
         *         of the file.
         *
         * @throws io_exception If the operation failed.
         */
        virtual size_type tell(void) const;

        /**
         * Write 'bufSize' bytes from 'buf' to the file.
         *
         * @param buf     Pointer to the data to be written.
         * @param bufSize The number of bytes to be written.
         *
         * @return The number of bytes actually written.
         *
         * @throws io_exception If the operation failed.
         */
        virtual size_type write(const void *buf, const size_type bufSize);

        /**
         * Writes all bytes from 'buf' into the file.
         *
         * @param buf     The buffer that we will write from. This must be a
         *                pointer to valid 'bufSize' bytes.
         * @param bufSize The size of 'buf' in bytes.
         * @param from    Controls where the method starts writing in the file.
         *                This parameter defaults to seek_origin::begin.
         *
         * @throws argument_null_exception If 'buf' is NULL.
         * @throws io_exception            If the operation failed.
         */
        void write_all_bytes(const void *buf, const size_type bufSize,
            const seek_origin from = seek_origin::begin);

    protected:

        /** Super class typedef. */
        typedef the::not_copyable base;

#ifdef THE_WINDOWS
        /**
         * Converts the parameters of open() into native WIN32 API values.
         *
         * @param outCreationDisposition Receives the value for the creation
         *                               disposition.
         * @param outDesiredAccess       Receives the value for the desired
         *                               access.
         * @param outShareMode           Receives the sharing parameter.
         * @param fileMode               The requested mode.
         * @param fileAccess             The requested access.
         * @param fileShare              The requested share.
         * 
         * @throws argument_exception If one of the input parameters has an 
         *                            invalid value that cannot be translated.
         */
        void to_native_params(DWORD& outCreationDisposition, 
            DWORD& outDesiredAccess, DWORD& outShareMode,
            const mode fileMode, const access fileAccess, 
            const share fileShare);
#endif /* THE_WINDOWS */

#ifdef THE_LINUX
        /**
         * Converts the parameters of open() into POSIX API flags.
         *
         * @param fileMode               The requested mode.
         * @param fileAccess             The requested access.
         * @param fileShare              The requested share. This parameter
         *                               currently has no effect.
         *
         * @throws argument_exception If one of the input parameters has an 
         *                            invalid value that cannot be translated.
         * @throws io_exception       If 'fileMode' requires an existing file,
         *                            but the file does not exist or vice versa.
         */
        int to_native_flags(const char *fileName, const mode fileMode, 
            const access fileAccess, const share fileShare);
#endif /* THE_LINUX */

        /** The native file handle. */
        native_handle_type handle;

    };

} /* end namespace io */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_FILE_H_INCLUDED */
