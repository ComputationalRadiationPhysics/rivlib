/*
 * src\system\io\file.cpp
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
 * File.cpp
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#include "the/system/io/file.h"

#ifdef THE_WINDOWS
#include <Shlobj.h>
#include <Shlwapi.h>
#endif /* THE_WINDOWS */

#include "the/assert.h"
#include "the/argument_exception.h"
#include "the/invalid_operation_exception.h"
#include "the/not_implemented_exception.h"
#include "the/trace.h"

#include "the/system/io/io_exception.h"



//#ifndef _WIN32
//namespace vislib {
//namespace sys {
//
//    /**
//     * Internal helper class for temporary files.
//     */
//    class TmpFile : public File {
//    public:
//        friend class vislib::sys::File;
//
//        /** Close the file, if open. */
//        virtual void Close(void) {
//            if (this->hFile != NULL) {
//                ::fclose(this->hFile);
//                this->hFile = NULL;
//                this->handle = -1;
//            } else {
//                File::Close();
//            }
//        }
//
//    private:
//
//        /** Ctor. */
//        TmpFile(void) : File(), hFile(NULL) {
//            // intentionally empty
//        }
//
//        /** file stream object handle */
//        FILE *hFile;
//
//    };
//
//} /* end namespace sys */
//} /* end namespace vislib */
//#endif /* !_WIN32 */
//
//
///*
// * vislib::sys::File::CreateTempFile
// */
//vislib::sys::File* vislib::sys::File::CreateTempFile(void) {
//#ifdef _WIN32
//    vislib::StringA tempName;
//    File *retval = new File();
//    if ((retval->handle = ::CreateFileA(CreateTempFileName(tempName),
//            GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
//            FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, NULL))
//            == INVALID_HANDLE_VALUE) {
//        SAFE_DELETE(retval);
//        throw SystemException(__FILE__, __LINE__);
//    }
//
//#else /* _WIN32 */
//    TmpFile *retval = new TmpFile();
//    if ((retval->hFile = ::tmpfile()) == NULL) {
//        SAFE_DELETE(retval);
//        throw SystemException(__FILE__, __LINE__);
//    } else {
//        retval->handle = ::fileno(retval->hFile);
//    }
//
//#endif /* _WIN32 */
//
//    return retval;
//}
//
//
///*
// * vislib::sys::File::CreateTempFileName
// */
//vislib::StringA& vislib::sys::File::CreateTempFileName(vislib::StringA& outFn) {
//#ifdef _WIN32
//    const uint32_t BUFFER_SIZE = 4096;
//    uint32_t bufSize=BUFFER_SIZE;
//    char tempName[MAX_PATH];
//    char tempPath[BUFFER_SIZE];
//
//    GetTempPathA(bufSize, tempPath);
//    GetTempFileNameA(tempPath, "VIS", 0, tempName);
//
//    outFn = tempName;
//
//#else /* _WIN32 */
//    char buffer[L_tmpnam];
//    ::tmpnam(buffer);
//    outFn = buffer;
//
//#endif /* _WIN32 */
//
//    return outFn;
//}
//
//
///*
// * vislib::sys::File::CreateTempFileName
// */
//vislib::StringW& vislib::sys::File::CreateTempFileName(vislib::StringW& outFn) {
//#ifdef _WIN32
//    const uint32_t BUFFER_SIZE = 4096;
//    uint32_t bufSize=BUFFER_SIZE;
//    wchar_t tempName[MAX_PATH];
//    wchar_t tempPath[BUFFER_SIZE];
//
//    GetTempPathW(bufSize, tempPath);
//    GetTempFileNameW(tempPath, L"VIS", 0, tempName);
//
//    outFn = tempName;
//
//#else /* _WIN32 */
//    vislib::StringA outFnA;
//    CreateTempFileName(outFnA);
//    outFn = outFnA;
//
//#endif /* _WIN32 */
//
//    return outFn;
//}


/*
 * the::system::io::file::native_invalid_handle_value
 */
const the::system::io::file::native_handle_type 
the::system::io::file::native_invalid_handle_value
#if defined(THE_WINDOWS)
    = INVALID_HANDLE_VALUE;
#elif defined(THE_LINUX)
    = -1;
#else /* defined(THE_WINDOWS) */
#error "Define the::system::io::file::native_invalid_handle_value!"
#endif /* defined(THE_WINDOWS) */


/*
 * the::system::io::file::exists
 */
bool the::system::io::file::exists(const char *fileName) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    return (::PathFileExistsA(fileName) == TRUE);
    // GetLastError() holds more information in case of problem. who cares

#elif defined(THE_LINUX)
    try {
        native_attribute_type atts;
        file::get_attributes(atts, fileName);
        return true;
    } catch (...) {
        return false;
    }

#else /*  defined(THE_WINDOWS) */
#error "Implement the::system::io::file::exists!"
#endif /*  defined(THE_WINDOWS) */
}


/*
 * the::system::io::file::exists
 */
bool the::system::io::file::exists(const wchar_t *fileName) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    return (::PathFileExistsW(fileName) == TRUE);
    // GetLastError() holds more information in case of problem. who cares

#elif defined(THE_LINUX)
    return file::exists(THE_W2A(fileName));

#else /*  defined(THE_WINDOWS) */
#error "Implement the::system::io::file::exists!"
#endif /*  defined(THE_WINDOWS) */
}


/*
 * the::system::io::file::get_attributes
 */
void the::system::io::file::get_attributes(native_attribute_type& outAttributes,
        const char *fileName) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    if (!::GetFileAttributesExA(fileName, GetFileExInfoStandard,
            &outAttributes)) {
#elif defined(THE_LINUX)
    if (::stat(fileName, &outAttributes) != 0) {
#else /*  defined(THE_WINDOWS) */
#error "Implement the::system::io::file::get_attributes!"
#endif /*  defined(THE_WINDOWS) */
        throw io_exception(__FILE__, __LINE__);
    }
}


/*
 * the::system::io::file::get_attributes
 */
void the::system::io::file::get_attributes(native_attribute_type& outAttributes,
        const wchar_t *fileName) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    if (!::GetFileAttributesExW(fileName, GetFileExInfoStandard, 
            &outAttributes)) {
        throw io_exception(__FILE__, __LINE__);
    }

#elif defined(THE_LINUX)
    file::get_attributes(outAttributes, THE_W2A(fileName));

#else /*  defined(THE_WINDOWS) */
#error "Implement the::system::io::file::get_attributes!"
#endif /*  defined(THE_WINDOWS) */

}


/*
 * the::system::io::file::get_size
 */
the::system::io::file::size_type the::system::io::file::get_size(
        const char *fileName) {
    THE_STACK_TRACE;
    native_attribute_type atts;
    file::get_attributes(atts, fileName);

#if defined(THE_WINDOWS)
    return (static_cast<size_type>(atts.nFileSizeHigh) << 32)
        + static_cast<size_type>(atts.nFileSizeLow);
#elif defined(THE_LINUX)
    return static_cast<size_type>(atts.st_size);
#else /*  defined(THE_WINDOWS) */
#error "Implement the::system::io::file::get_size!"
#endif /*  defined(THE_WINDOWS) */
}


/*
 * the::system::io::file::get_size
 */
the::system::io::file::size_type the::system::io::file::get_size(
        const wchar_t *fileName) {
    THE_STACK_TRACE;
    native_attribute_type atts;
    file::get_attributes(atts, fileName);

#if defined(THE_WINDOWS)
    return (static_cast<size_type>(atts.nFileSizeHigh) << 32)
        + static_cast<size_type>(atts.nFileSizeLow);
#elif defined(THE_LINUX)
    return static_cast<size_type>(atts.st_size);
#else /*  defined(THE_WINDOWS) */
#error "Implement the::system::io::file::get_size!"
#endif /*  defined(THE_WINDOWS) */
}


//
///*
// * vislib::sys::File::Rename
// */
//bool vislib::sys::File::IsDirectory(const char *fileName) {
//#ifdef _WIN32
//    WIN32_FILE_ATTRIBUTE_DATA attrs;
//    if (::GetFileAttributesExA(fileName, GetFileExInfoStandard, &attrs) != 0) {
//        return (attrs.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
//    }
//    return false; // error on lookup. fileName is not accessable or valid
//
//#else /* _WIN32 */
//    struct stat buf;
//    int i = stat(fileName, &buf); 
//    return (i == 0) && S_ISDIR(buf.st_mode);
//
//#endif /* _WIN32 */
//}
//
//
///*
// * vislib::sys::File::Rename
// */
//bool vislib::sys::File::IsDirectory(const wchar_t *fileName) {
//#ifdef _WIN32
//    WIN32_FILE_ATTRIBUTE_DATA attrs;
//    if (::GetFileAttributesExW(fileName, GetFileExInfoStandard, &attrs) != 0) {
//        return (attrs.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
//    }
//    return false; // error on lookup. fileName is not accessable or valid
//
//#else /* _WIN32 */
//    struct stat buf;
//    int i = stat(W2A(fileName), &buf); 
//    return (i == 0) && S_ISDIR(buf.st_mode);
//
//#endif /* _WIN32 */
//}


/*
 * the::system::io::file::is_file
 */
bool the::system::io::file::is_file(const char *fileName) throw() {
    THE_STACK_TRACE;
    native_attribute_type atts;
    try {
        file::get_attributes(atts, fileName);
#if defined(THE_WINDOWS)
        return ((atts.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0);
#elif defined(THE_LINUX)
        return (S_ISREG(atts.st_mode) != 0);
#else /* defined(THE_WINDOWS) */
#error "Implement the::system::io::file::is_file!"
#endif /* defined(THE_WINDOWS) */
    } catch (...) {
        // File does not exist.
        return false;
    }
}


/*
 * the::system::io::file::is_file
 */
bool the::system::io::file::is_file(const wchar_t *fileName) throw() {
    THE_STACK_TRACE;
    native_attribute_type atts;
    try {
        file::get_attributes(atts, fileName);
#if defined(THE_WINDOWS)
        return ((atts.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0);
#elif defined(THE_LINUX)
        return (S_ISREG(atts.st_mode) != 0);
#else /* defined(THE_WINDOWS) */
#error "Implement the::system::io::file::is_file!"
#endif /* defined(THE_WINDOWS) */
    } catch (...) {
        // File does not exist.
        return false;
    }
}


/*
 * the::system::io::file::read_all_bytes
 */
the::system::io::file::size_type the::system::io::file::read_all_bytes(
        blob& outDst, const char *fileName) {
    THE_STACK_TRACE;
    file file;
    size_type retval = 0;

    file.open(fileName, mode::open, access::read, share::read);
    retval = file.read_all_bytes(outDst, 0, seek_origin::begin);
    file.close();

    return retval;
}


/*
 * the::system::io::file::read_all_bytes
 */
the::system::io::file::size_type the::system::io::file::read_all_bytes(
        blob& outDst, const wchar_t *fileName) {
    THE_STACK_TRACE;
    file file;
    size_type retval = 0;

    file.open(fileName, mode::open, access::read, share::read);
    retval = file.read_all_bytes(outDst, 0, seek_origin::begin);
    file.close();

    return retval;
}


/*
 * the::system::io::file::move
 */
void the::system::io::file::move(const char *oldName, const char *newName) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    if (!::MoveFileA(oldName, newName)) {
#elif defined(THE_LINUX)
    if (::rename(oldName, newName) != 0) {
#else /* defined(THE_WINDOWS) */
#error "Implement the::system::io::file::move!"
#endif /* defined(THE_WINDOWS) */
        throw io_exception(__FILE__, __LINE__);
    }
}


/*
 * the::system::io::file::move
 */
void the::system::io::file::move(const wchar_t *oldName, const wchar_t *newName) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    if (!::MoveFileW(oldName, newName)) {
#elif defined(THE_LINUX)
    if (::rename(THE_W2A(oldName), THE_W2A(newName)) != 0) {
#else /* defined(THE_WINDOWS) */
#error "Implement the::system::io::file::move!"
#endif /* defined(THE_WINDOWS) */
        throw io_exception(__FILE__, __LINE__);
    }
}


/*
 * the::system::io::file::remove
 */
void the::system::io::file::remove(const char *fileName) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    if (!::DeleteFileA(fileName)) {
#elif defined(THE_LINUX)
    if (::remove(fileName) != 0) {
#else /* defined(THE_WINDOWS) */
#error "Implement the::system::io::file::remove!"
#endif /* defined(THE_WINDOWS) */
        throw io_exception(__FILE__, __LINE__);
    }
}


/*
 * the::system::io::file::remove
 */
void the::system::io::file::remove(const wchar_t *fileName) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    if (!::DeleteFileW(fileName)) {
#elif defined(THE_LINUX)
    if (::remove(THE_W2A(fileName)) != 0) {
#else /* defined(THE_WINDOWS) */
#error "Implement the::system::io::file::remove!"
#endif /* defined(THE_WINDOWS) */
        throw io_exception(__FILE__, __LINE__);
    }
}


/*
 * the::system::io::file::write_all_bytes
 */
void the::system::io::file::write_all_bytes(const char *fileName, 
        const void *buf, const size_type bufSize) {
    THE_STACK_TRACE;
    file file;

    file.open(fileName, mode::create, access::write, share::none);
    file.write_all_bytes(buf, bufSize, seek_origin::begin);
    file.close();
}


/*
 * the::system::io::file::write_all_bytes
 */
void the::system::io::file::write_all_bytes(const wchar_t *fileName, 
        const void *buf, const size_type bufSize) {
    THE_STACK_TRACE;
    file file;

    file.open(fileName, mode::create, access::write, share::none);
    file.write_all_bytes(buf, bufSize, seek_origin::begin);
    file.close();
}

/*
 * the::system::io::file::file
 */
the::system::io::file::file(void) : base(), 
        handle(native_invalid_handle_value) {
    THE_STACK_TRACE;
}


/*
 * the::system::io::file::~file
 */
the::system::io::file::~file(void) {
    THE_STACK_TRACE;
    try {
        if (this->is_open()) {
            this->close();
        }
    } catch (...) { /* Ignore this. */ }
}


/*
 * the::system::io::file::close
 */
void the::system::io::file::close(void) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    if (!::CloseHandle(this->handle)) {
#elif defined(THE_LINUX)
    if (::close(this->handle) != 0) {
#else /* defined(THE_WINDOWS) */
#error "Implement the::system::io::file::close!"
#endif /* defined(THE_WINDOWS) */
        throw io_exception(__FILE__, __LINE__);
    }

    this->handle = file::native_invalid_handle_value;
}


/*
 * the::system::io::file::flush
 */
void the::system::io::file::flush(void) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    if (!::FlushFileBuffers(this->handle)) {
#elif defined(THE_LINUX)
    if (::fsync(this->handle) != 0) {
#else /* defined(THE_WINDOWS) */
#error "Implement the::system::io::file::flush!"
#endif /* defined(THE_WINDOWS) */
        throw io_exception(__FILE__, __LINE__);
    }
}


/*
 * the::system::io::file::get_size
 */
the::system::io::file::size_type the::system::io::file::get_size(void) const {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    LARGE_INTEGER size;
    if (::GetFileSizeEx(this->handle, &size)) {
        return size.QuadPart;

#elif defined(THE_LINUX)
    struct stat64 stat;
    if (::fstat64(this->handle, &stat) == 0) {
        return stat.st_size;

#else /* defined(THE_WINDOWS) */
#error "Implement the::system::io::file::get_size!"
#endif /* defined(THE_WINDOWS) */

    } else {
        throw io_exception(__FILE__, __LINE__);
    }
}


/*
 * the::system::io::file::is_open
 */
bool the::system::io::file::is_open(void) const {
    THE_STACK_TRACE;
    return (this->handle != file::native_invalid_handle_value);
}


/*
 * the::system::io::file::open
 */
void the::system::io::file::open(const char *fileName, const mode fileMode,
            const access fileAccess, const share fileShare) {
    THE_STACK_TRACE;

    /* Sanity checks. */
    if (fileName == NULL) {
        throw argument_null_exception("fileName", __FILE__, __LINE__);
    }
    if (this->is_open()) {
        throw invalid_operation_exception(L"The file was already open.", 
            __FILE__, __LINE__);
    }


#if defined(THE_WINDOWS)
    DWORD creationDisposition = 0;
    DWORD desiredAccess = 0;
    DWORD shareMode = 0;

    this->to_native_params(creationDisposition, desiredAccess, shareMode,
        fileMode, fileAccess, fileShare);

    this->handle = ::CreateFileA(fileName, desiredAccess, shareMode, NULL, 
        creationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);

#elif defined(THE_LINUX)
    int oflag = this->to_native_flags(fileName, fileMode, fileAccess, 
        fileShare);

    this->handle = ::open(fileName, oflag, S_IRUSR | S_IWUSR | S_IRGRP 
        | S_IWGRP);

#else /* defined(THE_WINDOWS) */
#error "Implement the::system::io::file::open!"
#endif /* defined(THE_WINDOWS) */

    if (this->handle == native_invalid_handle_value) {
        throw io_exception(__FILE__, __LINE__);
    }
}



/*
 * the::system::io::file::open
 */
void the::system::io::file::open(const wchar_t *fileName, const mode fileMode,
            const access fileAccess, const share fileShare) {
    THE_STACK_TRACE;

    /* Sanity checks. */
    if (fileName == NULL) {
        throw argument_null_exception("fileName", __FILE__, __LINE__);
    }
    if (this->is_open()) {
        throw invalid_operation_exception(L"The file was already open.", 
            __FILE__, __LINE__);
    }

#if defined(THE_WINDOWS)
    DWORD creationDisposition = 0;
    DWORD desiredAccess = 0;
    DWORD shareMode = 0;

    this->to_native_params(creationDisposition, desiredAccess, shareMode,
        fileMode, fileAccess, fileShare);

    this->handle = ::CreateFileW(fileName, desiredAccess, shareMode, NULL, 
        creationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);

#elif defined(THE_LINUX)
    this->open(THE_W2A(fileName), fileMode, fileAccess, fileShare);

#else /* defined(THE_WINDOWS) */
#error "Implement the::system::io::file::open!"
#endif /* defined(THE_WINDOWS) */

    if (this->handle == native_invalid_handle_value) {
        throw io_exception(__FILE__, __LINE__);
    }
}


/*
 * the::system::io::file::read
 */
the::system::io::file::size_type the::system::io::file::read(void *outBuf,
        const size_type bufSize) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    DWORD readBytes;
    if (::ReadFile(this->handle, outBuf, static_cast<DWORD>(bufSize),
            &readBytes, NULL)) {
        return static_cast<size_type>(readBytes);

#elif defined(THE_LINUX)
    THE_ASSERT(bufSize < static_cast<size_type>(INT_MAX));
    int readBytes = ::read(this->handle, outBuf, bufSize);
    if (readBytes != -1) {
        return static_cast<size_type>(readBytes);

#else /* defined(THE_WINDOWS) */
#error "Implement the::system::io::file::get_size!"
#endif /* defined(THE_WINDOWS) */

    } else {
        throw io_exception(__FILE__, __LINE__);
    }
}


/*
 * the::system::io::file::size_type
 */
the::system::io::file::size_type the::system::io::file::read_all_bytes(
        blob& outDst, const size_type dstOffset, 
        const seek_origin from) {
    THE_STACK_TRACE;
    byte *cursor = NULL;
    size_type read = 0;
    size_type remaining = this->get_size();

    /* Seek to the requested start position. */
    switch (from) {
        case seek_origin::begin:
            this->seek_to_begin();
            break;

        case seek_origin::current:
            // Nothing to do.
            break;

        default:
            throw argument_exception("from", __FILE__, __LINE__);
    }

    remaining -= this->tell();
    outDst.assert_size(static_cast<size_t>(remaining + dstOffset), 
        (dstOffset > 0));
    cursor = outDst.as_at<byte>(static_cast<size_t>(dstOffset));

    while (remaining > 0) {
        read = this->read(cursor, remaining);
        remaining -= read;
        cursor += read;
    }

    return (cursor - outDst.as_at<byte>(static_cast<size_t>(dstOffset)));
}


/*
 * the::system::io::file::seek
 */
the::system::io::file::size_type the::system::io::file::seek(
        const offset_type offset, const seek_origin from) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    LARGE_INTEGER o;
    LARGE_INTEGER n;
    o.QuadPart = offset; 
    if (::SetFilePointerEx(this->handle, o, &n, static_cast<DWORD>(from))) {
        return n.QuadPart;

#elif defined(THE_LINUX)
    off64_t n = ::lseek64(this->handle, offset, static_cast<int>(from));
    if (n != static_cast<off64_t>(-1)) {
        return n;

#else /* defined(THE_WINDOWS) */
#error "Implement the::system::io::file::seek!"
#endif /* defined(THE_WINDOWS) */

    } else {
        throw io_exception(__FILE__, __LINE__);
    }
}


/*
 * the::system::io::file::tell
 */
the::system::io::file::size_type the::system::io::file::tell(void) const {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    LARGE_INTEGER o;
    LARGE_INTEGER n;
    o.QuadPart = 0; 
    if (::SetFilePointerEx(this->handle, o, &n, FILE_CURRENT)) {
        return n.QuadPart;

#elif defined(THE_LINUX)
    off64_t n = ::lseek64(this->handle, 0, SEEK_CUR);
    if (n != static_cast<off64_t>(-1)) {
        return n;

#else /* defined(THE_WINDOWS) */
#error "Implement the::system::io::file::seek!"
#endif /* defined(THE_WINDOWS) */

    } else {
        throw io_exception(__FILE__, __LINE__);
    }
}


/*
 * the::system::io::file::write
 */
the::system::io::file::size_type the::system::io::file::write(const void *buf,
        const size_type bufSize) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    DWORD writtenBytes;
    if (::WriteFile(this->handle, buf, static_cast<DWORD>(bufSize), 
            &writtenBytes, NULL)) {
        return writtenBytes;

#elif defined(THE_LINUX)
    THE_ASSERT(bufSize < static_cast<size_type>(INT_MAX));
    int writtenBytes = ::write(this->handle, buf, static_cast<int>(bufSize));
    if (writtenBytes != -1) {
        return writtenBytes;

#else /* defined(THE_WINDOWS) */
#error "Implement the::system::io::file::write!"
#endif /* defined(THE_WINDOWS) */

    } else {
        throw io_exception(__FILE__, __LINE__);
    }
}


/*
 * the::system::io::file::write_all_bytes
 */
void the::system::io::file::write_all_bytes(const void *buf,
        const size_type bufSize, const seek_origin from) {
    THE_STACK_TRACE;
    if (buf == NULL) {
        throw argument_null_exception("buf", __FILE__, __LINE__);
    }

    this->seek(0, from);
    const byte *cursor = static_cast<const byte *>(buf);
    const byte *end = cursor + bufSize;
    size_type written = 0;

    while (cursor < end) {
        written = this->write(cursor, end - cursor);
        cursor += written;
    }
}


#ifdef THE_WINDOWS
/*
 * the::system::io::file::to_native_params
 */
void the::system::io::file::to_native_params(DWORD& outCreationDisposition,
        DWORD& outDesiredAccess, DWORD& outShareMode, const mode fileMode, 
        const access fileAccess, const share fileShare) {
    THE_STACK_TRACE;

    switch (fileMode) {
        case mode::create_new:
        case mode::create:
        case mode::open:
        case mode::open_or_create:
        case mode::truncate:
            outCreationDisposition = static_cast<DWORD>(fileMode);
            break;

        default:
            throw argument_exception("fileMode", __FILE__, __LINE__);
    }

    switch (fileAccess) {
        case access::read:
        case access::write:
        case access::read_write:
            outDesiredAccess = static_cast<DWORD>(fileAccess);
            break;

        default:
            throw argument_exception("fileAccess", __FILE__, __LINE__);
    }

    switch (fileShare) {
        case share::none:
        case share::read:
        case share::write:
        case share::read_write:
            outShareMode = static_cast<DWORD>(fileShare);
            break;

        default:
            throw argument_exception("fileShare", __FILE__, __LINE__);

    }
}
#endif /* THE_WINDOWS */


#ifdef THE_LINUX
/*
 * the::system::io::file::to_native_flags
 */
int the::system::io::file::to_native_flags(const char *fileName, 
        const mode fileMode, const access fileAccess, const share fileShare) {
    THE_STACK_TRACE;

    int retval = O_LARGEFILE | O_SYNC;
    // TODO: consider O_DIRECT for disabling OS-Caching
    bool fileExists = file::exists(fileName);

    switch (fileMode) {
        case mode::create_new:
            if (fileExists) {
                throw io_exception(EEXIST, __FILE__, __LINE__);
            }
            retval |= O_CREAT;
            break;

        case mode::create:
            retval |= (fileExists) ? O_TRUNC : O_CREAT;
            break;

        case mode::open:
            if (!fileExists) {
                throw io_exception(ENOENT, __FILE__, __LINE__);
            }
            break;

        case mode::open_or_create:
            if (!fileExists) {
                retval |= O_CREAT;
            }
            break;

        case mode::truncate:
            if (!fileExists) {
                throw io_exception(ENOENT, __FILE__, __LINE__);
            }
            retval |= O_TRUNC;
            break;

        default:
            throw argument_exception("fileMode", __FILE__, __LINE__);
    }

    switch (fileAccess) {
        case access::read:
        case access::write:
        case access::read_write:
            retval = static_cast<uint32_t>(fileAccess);
            break;

        default:
            throw argument_exception("fileAccess", __FILE__, __LINE__);
    }

    return retval;
}
#endif /* THE_LINUX */
