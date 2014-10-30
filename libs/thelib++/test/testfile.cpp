/*
 * testfile.h  14.09.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#include "stdafx.h"
#include "testfile.h"

#include <climits>
#include <iostream>
#include <float.h>

#include "the/memory.h"

#include "the/system/io/file.h"
#include "the/system/io/io_exception.h"

#include "testhelper.h"

//#include "vislib/Bufferedfile.h"
//#include "vislib/Memmappedfile.h"
//#include "vislib/error.h"
//#include "vislib/Path.h"
//#include "vislib/IOException.h"
//#include "vislib/SystemMessage.h"
//#include "vislib/sysfunctions.h"
//#include "vislib/SystemInformation.h"
//#include "vislib/PerformanceCounter.h"

#ifdef _WIN32
#pragma warning ( disable : 4996 )
#define SNPRINTF _snprintf
#define FORMATINT64 "%I64u"
#else /* _WIN32 */
#define SNPRINTF snprintf
#define FORMATINT64 "%lu"
#endif /* _WIN32 */

using namespace the::system;
using namespace the::system::io;

//static const file::size_type BIGFILE_SIZE = 5368708992;
//static const file::size_type BIGFILE_LASTVAL = 671088623;
static const file::size_type BIGFILE_SIZE = 300000;
//static const file::size_type BIGFILE_LASTVAL = 37499;
static const file::size_type BIGFILE_LASTVAL = (BIGFILE_SIZE / 8) - 1;

static const char fname[] = "bigfile.bin";
static file::size_type BUF_SIZE = 1024 * 1024;  // TODO //SystemInformation::AllocationGranularity() * 2;

static void generateBigOne(file& f1) {
    file::size_type dataLeft = BIGFILE_SIZE;
    file::size_type splitPos = 0, val = 0, *tmp;
    char *buf = new char[static_cast<size_t>(BUF_SIZE)];
    //vislib::sys::PerformanceCounter *p = new vislib::sys::PerformanceCounter();

    file::size_type perf, ptotal = 0, numWritten;
    float minrate = FLT_MAX;
    float maxrate = 0.0f;
    float rate;

    if (file::exists(fname)) {
        return;
    }
    AssertNoException("Can generate big file", f1.open(fname, file::mode::create_new, file::access::read_write, file::share::none));
    // TODO //AssertTrue("AllocationGranularity is a multiple of 8", SystemInformation::AllocationGranularity() % 8 == 0);

    try {
        while(dataLeft > 0) {
            for (tmp = (file::size_type *)buf; tmp < (file::size_type*) (buf + BUF_SIZE); tmp++) {
                *tmp = val++;
            }
            //TODO p->SetMark();
            if (dataLeft < BUF_SIZE) {
                f1.write(buf, dataLeft);
                numWritten = dataLeft;
                dataLeft = 0;
            } else {
                f1.write(buf, splitPos);
                f1.write(buf + splitPos, BUF_SIZE - splitPos);
                dataLeft -= BUF_SIZE;
                numWritten = BUF_SIZE;
            }
            // TODO perf = p->Difference();
            // TODO ptotal += perf;
            // TODO rate = (float)numWritten/1024.0f / perf;
            // TODO if (rate < minrate) minrate = rate;
            // TODO if (rate > maxrate) maxrate = rate;
            splitPos = (++splitPos) % BUF_SIZE;
        }
        AssertTrue("Generate big testfile", true);
        //SNPRINTF(buf, static_cast<size_t>(BUF_SIZE - 1), "Linear writing: %03.1f MB/s min, %03.1f MB/s max, %03.1f MB/s average\n", minrate, maxrate, f1.GetSize() / 1024.0f / ptotal);
        std::cout << buf;
        f1.close();
    } catch (io_exception e) {
        AssertTrue("Generate big testfile", false);
        std::cout << e.get_msg_astr() << std::endl;
    }

    AssertNoException("Open big test file for reading.", f1.open(fname, file::mode::open, file::access::read));
    SNPRINTF(buf, static_cast<size_t>(BUF_SIZE - 1), "file size = " FORMATINT64 "", BIGFILE_SIZE);
    AssertEqual(buf, BIGFILE_SIZE, f1.get_size());
    f1.close();

    the::safe_array_delete(buf);
}


static file::size_type checkContent(char *buf, file::size_type pos, file::size_type chunk) {
    char *offset;
    file::size_type i, ret;

    for (offset = buf, i = pos / 8; offset < buf + chunk; offset += 8, i++) {
        ret = *(reinterpret_cast<file::size_type*>(offset)); // for better debugging
        if (ret != i) {
            AssertTrue("file contents comparison", false);
        }
        ret = i;
    }
    return ret;
}

static void testBigOne(file& f1) {
    //TODO vislib::sys::PerformanceCounter *p = new vislib::sys::PerformanceCounter();
    char *buf = new char[static_cast<size_t>(BUF_SIZE)];
    file::size_type numRead, perf, ptotal, pos, lastval;
    float minrate = 100000000.0f;
    float maxrate = 0.0f;
    float rate;

    f1.open(fname, file::mode::open);

    pos = ptotal = 0;
    try {
        // TODO p->SetMark();
        while((numRead = f1.read(buf, BUF_SIZE)) == BUF_SIZE) {
            // TODO perf = p->Difference();
            // TODO ptotal += perf;
            // TODO rate = (float)numRead/1024.0f / perf;
            // TODO if (rate < minrate) minrate=rate;
            // TODO if (rate > maxrate) maxrate=rate;

            lastval = checkContent(buf, pos, numRead);
            pos += numRead;

            // TODO p->SetMark();
        }
        if (numRead > 0) {
            // TODO perf = p->Difference();
            // TODO ptotal += perf;
            // TODO rate = (float)numRead/1024.0f / perf;
            // TODO if (rate < minrate) minrate=rate;
            // TODO if (rate > maxrate) maxrate=rate;
            lastval = checkContent(buf, pos, numRead);
            pos += numRead;
        }
    } catch (io_exception e) {
        SNPRINTF(buf, static_cast<size_t>(BUF_SIZE - 1), "Reading successful (" FORMATINT64 ")", pos);
        AssertTrue(buf, false);
    }
    SNPRINTF(buf, static_cast<size_t>(BUF_SIZE - 1), "Values consistent up to pos " FORMATINT64 "", BIGFILE_SIZE);
    AssertEqual(buf, pos, BIGFILE_SIZE);
    SNPRINTF(buf, static_cast<size_t>(BUF_SIZE - 1), "Last value = " FORMATINT64 "", BIGFILE_LASTVAL);
    AssertEqual(buf, lastval, BIGFILE_LASTVAL);
    // TODO SNPRINTF(buf, static_cast<size_t>(BUF_SIZE - 1), "Linear reading: %03.1f MB/s min, %03.1f MB/s max, %03.1f MB/s average\n", minrate, maxrate, f1.GetSize()/1024.0f / ptotal);
    std::cout << buf; 
    f1.close();
    delete[] buf;
}

static void removeBigOne(void) {
    AssertNoException("Delete test file", file::remove(fname));
}

static void runTests(file& f1) {
    const file::size_type TEXT_SIZE = 26;
    char writeBuffer[TEXT_SIZE + 1] = "abcdefghijklmnopqrstuvwxyz";
    char readBuffer[TEXT_SIZE];
    the::blob rs;

    std::cout << "Clean up possible old trash ..." << std::endl;
    ::remove("horst.txt");
    ::remove("hugo.txt");
    
    AssertFalse("\"horst.txt\" does not exist", file::exists("horst.txt"));
    AssertFalse("L\"horst.txt\" does not exist", file::exists(L"horst.txt"));

    AssertException("Cannot open not existing file", f1.open("horst.txt", file::mode::open, file::access::read_write, file::share::read_write), io_exception);

    AssertNoException("Can create file", f1.open("horst.txt", file::mode::create, file::access::read_write, file::share::read_write));
    AssertTrue("File is open now", f1.is_open());
    f1.close();
    AssertFalse("Closing file", f1.is_open());

    AssertException("file::mode::create_new cannot overwrite existing file", f1.open("horst.txt", file::mode::create_new, file::access::read_write, file::share::read_write), io_exception);
    AssertNoException("file::mode::open can open existing file", f1.open("horst.txt", file::mode::open, file::access::write, file::share::none));
    AssertTrue("file is open now", f1.is_open());
    AssertEqual("Is at begin of file", f1.tell(), static_cast<file::size_type>(0));

    AssertNoException("Writing to file", f1.write_all_bytes(writeBuffer, TEXT_SIZE));
    AssertTrue("Is EOF", f1.is_eof());

    AssertEqual("Seek to begin", f1.seek_to_begin(), static_cast<file::size_type>(0));
    AssertEqual("Is at begin", f1.tell(), static_cast<file::size_type>(0));
    AssertFalse("Is not EOF", f1.is_eof());

    AssertException("Cannot read on file::access::write", f1.read(readBuffer, TEXT_SIZE), io_exception);

    AssertNoException("Closing file", f1.close());
    AssertFalse("File closed", f1.is_open());

    AssertNoException("Open file for reading", f1.open("horst.txt", file::mode::open, file::access::read, file::share::none));
    AssertTrue("file is open now", f1.is_open());
    AssertEqual("Is at begin of file", f1.tell(), static_cast<file::size_type>(0));
    AssertNotEqual("Seek to end", f1.seek_to_end(), static_cast<file::size_type>(0));
    AssertTrue("Is at end", f1.is_eof());


    AssertException("Cannot write to file::access::read", f1.write(readBuffer, TEXT_SIZE), io_exception);


    AssertEqual("Seek to 2", f1.seek(2, file::seek_origin::begin), static_cast<file::size_type>(2));
    AssertEqual("Is at 2", f1.tell(), static_cast<file::size_type>(2));
    AssertEqual("Reading from 2", f1.read(readBuffer, 1), static_cast<file::size_type>(1));
    AssertEqual("Read correct character", readBuffer[0], 'c');

    AssertEqual("file size", f1.get_size(), TEXT_SIZE);

    AssertNoException("Closing file", f1.close());
    AssertFalse("File closed", f1.is_open());

    AssertNoException("Renaming file", file::move("horst.txt", "hugo.txt"));
    AssertFalse("Old file does not exist", file::exists("horst.txt"));
    AssertTrue("New file exists", file::exists("hugo.txt"));

    AssertNoException("Opening file for read/write", f1.open(L"hugo.txt", file::mode::open, file::access::read_write, file::share::read));
    AssertEqual("Reading 9 characters", f1.read(readBuffer, 9), static_cast<file::size_type>(9));
    AssertEqual("Read correct characters", ::memcmp(readBuffer, writeBuffer, 9), 0);
    AssertEqual("Seek to 2", f1.seek(2, file::seek_origin::begin), static_cast<file::size_type>(2));
    AssertEqual("Reading 9 characters", f1.read(readBuffer, 9), static_cast<file::size_type>(9));
    AssertEqual("Read correct characters", ::memcmp(readBuffer, writeBuffer + 2, 9), 0);
    
    AssertEqual("Seek to begin", f1.seek_to_begin(), static_cast<file::size_type>(0));
    AssertEqual("Reading 7 characters", f1.read(readBuffer, 7), static_cast<file::size_type>(7));
    AssertEqual("Read correct characters", ::memcmp(readBuffer, writeBuffer, 7), 0);
    AssertEqual("Reading additionally 5 characters", f1.read(readBuffer, 5), static_cast<file::size_type>(5));
    AssertEqual("Read correct characters", ::memcmp(readBuffer, writeBuffer + 7, 5), 0);

    AssertEqual("Seek to 1", f1.seek(1, file::seek_origin::begin), static_cast<file::size_type>(1));
    AssertEqual("Writing 3 characters to file", f1.write(writeBuffer, 3), static_cast<file::size_type>(3));
    AssertEqual("Reading after new characters ", f1.read(readBuffer, 1), static_cast<file::size_type>(1));
    AssertEqual("Read 'e' after new characters", readBuffer[0], 'e');
    AssertEqual("Seek to begin", f1.seek_to_begin(), static_cast<file::size_type>(0));
    AssertEqual("Reading 5 characters", f1.read(readBuffer, 5), static_cast<file::size_type>(5));
    AssertEqual("New file @0 = 'a'", readBuffer[0], 'a');
    AssertEqual("New file @1 = 'a'", readBuffer[1], 'a');
    AssertEqual("New file @2 = 'b'", readBuffer[2], 'b');
    AssertEqual("New file @3 = 'c'", readBuffer[3], 'c');
    AssertEqual("New file @4 = 'e'", readBuffer[4], 'e');

    AssertEqual("Seek to begin", f1.seek_to_begin(), static_cast<file::size_type>(0));
    AssertEqual("Writing 9 characters to file", f1.write(writeBuffer, 9), static_cast<file::size_type>(9));

    AssertEqual("Seek to begin", f1.seek_to_begin(), static_cast<file::size_type>(0));
    AssertEqual("Writing 5 characters to file", f1.write(writeBuffer, 5), static_cast<file::size_type>(5));
    AssertEqual("Reading 4 after new characters ", f1.read(readBuffer, 4), static_cast<file::size_type>(4));
    AssertEqual("Read correct characters", ::memcmp(readBuffer, writeBuffer + 5, 4), 0);

    AssertNoException("Closing file", f1.close());
    AssertFalse("File closed", f1.is_open());

    AssertNoException("Open file for reading", f1.open("hugo.txt", file::mode::open, file::access::read, file::share::read));
    AssertEqual("file size", f1.get_size(), static_cast<file::size_type>(26)); 
    AssertNoException("Closing file", f1.close());
    AssertFalse("File closed", f1.is_open());

    AssertNoException("Truncate file", f1.open("hugo.txt", file::mode::truncate, file::access::write, file::share::none));
    AssertEqual("File truncated", f1.get_size(), static_cast<file::size_type>(0));
    AssertEqual("Writing to file", f1.write(writeBuffer, TEXT_SIZE), TEXT_SIZE);
    AssertTrue("Is EOF", f1.is_eof());
    AssertNoException("Closing file", f1.close());
    AssertFalse("File closed", f1.is_open());

    AssertNoException("Open file for reading", f1.open("hugo.txt", file::mode::open, file::access::read, file::share::read));
    AssertEqual("Is at beginn", f1.tell(), static_cast<file::size_type>(0));
    AssertEqual("Reading all file content", f1.read_all_bytes(rs), TEXT_SIZE);
    AssertEqual("Read correct characters", ::memcmp(rs.as<char>(), writeBuffer, TEXT_SIZE), 0);
    AssertEqual("Seek to 2", f1.seek(2), static_cast<file::size_type>(2));
    rs.enforce_size(2, true);
    AssertEqual("Destination buffer resized", rs.size(), static_cast<size_t>(2));
    AssertEqual("Reading all file content", f1.read_all_bytes(rs, 2, file::seek_origin::current), TEXT_SIZE - 2);
    AssertEqual("Read correct characters", ::memcmp(rs.as<char>(), writeBuffer, TEXT_SIZE), 0);

    AssertNoException("Closing file", f1.close());
    AssertFalse("File closed", f1.is_open());


    file::remove("hugo.txt");
    AssertFalse("\"hugo.txt\" was deleted", file::exists("hugo.txt"));

    AssertTrue("All tests complete", 1);
}


void TestFile(void) {
    AssertEqual("Testsize_type % 8 == 0", static_cast<int64_t>(BIGFILE_SIZE) % static_cast<int64_t>(8), static_cast<int64_t>(0));

    try {
        ::TestBaseFile();
  //      ::TestBufferedFile();
        //::TestMemmappedFile();
    } catch (io_exception e) {
        std::cout << e.get_msg_astr() << std::endl;
    }
}


void TestBaseFile(void) {
    file f1;
    std::cout << std::endl << "Tests for the::system::io::file" << std::endl;
    ::runTests(f1);

    ::generateBigOne(f1);
    ::testBigOne(f1);
    ::removeBigOne();
}


//void TestBufferedfile(void) {
//    Bufferedfile f1;
//    std::cout << std::endl << "Tests for Bufferedfile" << std::endl;
//    ::runTests(f1);
//
//    f1.SetBufferSize(8);
//    std::cout << std::endl << "Tests for Bufferedfile, buffer size 8" << std::endl;
//    ::runTests(f1);
//
//	::generateBigOne(f1);
//	::testBigOne(f1);	
//    ::removeBigOne();
//}
//
//void TestMemmappedfile(void) {
//	Memmappedfile f1;
//	std::cout << std::endl << "Tests for Memmappedfile" << std::endl;
//	::runTests(f1);
//	
//	::generateBigOne(f1);
//	char *buf = new char[static_cast<size_t>(BUF_SIZE)];
//
//	SNPRINTF(buf, static_cast<size_t>(BUF_SIZE - 1), "Testing with views of "FORMATINT64" (machine default)\n", f1.GetViewSize());
//	std::cout << buf;
//	::testBigOne(f1);
//
//	f1.SetViewSize(8 * 1024 * 1024);
//	SNPRINTF(buf, static_cast<size_t>(BUF_SIZE - 1), "Testing with views of "FORMATINT64"\n", f1.GetViewSize());
//	std::cout << buf;
//	::testBigOne(f1);
//    ::removeBigOne();
//}
//
//
//void TestPath(void) {
//    using namespace vislib;
//    using namespace vislib::sys;
//    using namespace std;
//
//    try {
//        cout << "Working directory \"" << static_cast<const char *>(Path::GetCurrentDirectoryA()) << "\"" << endl;
//
//        cout << "Resolve \"~\" " << Path::Resolve("~") << endl;
//        cout << "Resolve \"~/\" " << Path::Resolve("~/") << endl;
//        cout << "Resolve \"~/heinz\" " << Path::Resolve("~/heinz") << endl;
//        cout << "Resolve \"~heinz\" " << Path::Resolve("~heinz") << endl;
//        cout << "Resolve \"./~\" " << Path::Resolve("./~") << endl;
//
//        cout << "Resolve \"horst\" " << Path::Resolve("horst") << endl;
//        cout << "Resolve \"/horst\" " << Path::Resolve("/horst") << endl;
//        cout << "Resolve \"/horst/\" " << Path::Resolve("/horst/") << endl;
//        cout << "Resolve \"//horst/\" " << Path::Resolve("//horst/") << endl;
//
//#ifdef _WIN32
//        cout << "Resolve \"horst\" " << Path::Resolve("horst") << endl;
//        cout << "Resolve \"\\horst\" " << Path::Resolve("\\horst") << endl;
//        cout << "Resolve \"C:\\horst\" " << Path::Resolve("C:\\horst") << endl;
//        cout << "Resolve \"\\horst\\\" " << Path::Resolve("\\horst\\") << endl;
//        cout << "Resolve \"\\\\horst\\\" " << Path::Resolve("\\\\horst\\") << endl;
//        cout << "Resolve \"~\\\" " << Path::Resolve("~\\") << endl;
//        cout << "Resolve \"~\\heinz\" " << Path::Resolve("~\\heinz") << endl;
//        cout << "Resolve \"~heinz\" " << Path::Resolve("~heinz") << endl;
//        cout << "Resolve \".\\~\" " << Path::Resolve(".\\~") << endl;
//
//        AssertTrue("Path \"\\hugo\" is relative", Path::IsRelative("\\hugo"));
//        AssertTrue("Path \"\\\\hugo\" is absolute", Path::IsAbsolute("\\\\hugo"));
//
//        AssertEqual("Canonicalise \"horst\\..\\hugo\"", Path::Canonicalise("horst\\..\\hugo"), StringA("\\hugo"));
//        AssertEqual("Canonicalise \"\\horst\\..\\hugo\"", Path::Canonicalise("\\horst\\..\\hugo"), StringA("\\hugo"));
//        AssertEqual("Canonicalise \"\\..\\horst\\..\\hugo\"", Path::Canonicalise("\\..\\horst\\..\\hugo"), StringA("\\hugo"));
//        AssertEqual("Canonicalise \"\\..\\horst\\..\\..\\hugo\"", Path::Canonicalise("\\..\\horst\\..\\..\\hugo"), StringA("\\hugo"));
//        AssertEqual("Canonicalise \"horst\\.\\hugo\"", Path::Canonicalise("horst\\.\\hugo"), StringA("horst\\hugo"));
//        AssertEqual("Canonicalise \"horst\\\\hugo\"", Path::Canonicalise("horst\\\\hugo"), StringA("horst\\hugo"));
//        AssertEqual("Canonicalise \"horst\\\\\\hugo\"", Path::Canonicalise("horst\\\\\\hugo"), StringA("horst\\hugo"));
//        AssertEqual("Canonicalise \"\\horst\\hugo\"", Path::Canonicalise("\\horst\\hugo"), StringA("\\horst\\hugo"));
//        AssertEqual("Canonicalise \"\\\\horst\\hugo\"", Path::Canonicalise("\\\\horst\\hugo"), StringA("\\\\horst\\hugo"));
//        AssertEqual("Canonicalise \"\\\\\\horst\\hugo\"", Path::Canonicalise("\\\\\\horst\\hugo"), StringA("\\\\horst\\hugo"));
//#else /* _WIN32 */
//        AssertEqual("Canonicalise \"horst/../hugo\"", Path::Canonicalise("horst/../hugo"), StringA("/hugo"));
//        AssertEqual("Canonicalise \"/horst/../hugo\"", Path::Canonicalise("/horst/../hugo"), StringA("/hugo"));
//        AssertEqual("Canonicalise \"/../horst/../hugo\"", Path::Canonicalise("/../horst/../hugo"), StringA("/hugo"));
//        AssertEqual("Canonicalise \"/../horst/../../hugo\"", Path::Canonicalise("/../horst/../../hugo"), StringA("/hugo"));
//        AssertEqual("Canonicalise \"horst/./hugo\"", Path::Canonicalise("horst/./hugo"), StringA("horst/hugo"));
//        AssertEqual("Canonicalise \"horst//hugo\"", Path::Canonicalise("horst//hugo"), StringA("horst/hugo"));
//        AssertEqual("Canonicalise \"horst///hugo\"", Path::Canonicalise("horst///hugo"), StringA("horst/hugo"));
//
//#endif /* _WIN32 */
//
//        // testing compare methods
//#ifdef _WIN32
//        AssertTrue("Compare \"C:\\horst\\\" == \"C:\\..\\HoRsT\\\"", Path::Compare<vislib::CharTraitsA>("C:\\horst\\", "C:\\..\\HoRsT\\"));
//        AssertTrue("Compare \"\\horst\\\" == \"\\..\\HoRsT\\\"", Path::Compare<vislib::CharTraitsA>("\\horst\\", "\\..\\HoRsT\\"));
//        AssertFalse("Compare \"Path::Resolve(hugo)\" != \"\\hugo\"", Path::Compare<vislib::CharTraitsA>(Path::Resolve("hugo"), "\\hugo"));
//        AssertTrue("Compare \"Path::Resolve(hugo)\" == \"hugo\"", Path::Compare<vislib::CharTraitsA>(Path::Resolve("hugo"), "hugo"));
//
//        AssertTrue("Compare \"C:\\horst\\\" == \"C:\\..\\HoRsT\\\"", Path::Compare<vislib::CharTraitsW>(L"C:\\horst\\", L"C:\\..\\HoRsT\\"));
//        AssertTrue("Compare \"\\horst\\\" == \"\\..\\HoRsT\\\"", Path::Compare<vislib::CharTraitsW>(L"\\horst\\", L"\\..\\HoRsT\\"));
//        AssertFalse("Compare \"Path::Resolve(hugo)\" != \"\\hugo\"", Path::Compare<vislib::CharTraitsW>(Path::Resolve(L"hugo"), L"\\hugo"));
//        AssertTrue("Compare \"Path::Resolve(hugo)\" == \"hugo\"", Path::Compare<vislib::CharTraitsW>(Path::Resolve(L"hugo"), L"hugo"));
//#else /* _WIN32 */
//        AssertTrue("Compare \"/horst/\" == \"/../horst/\"", Path::Compare<vislib::CharTraitsA>("/horst/", "/../horst/"));
//        AssertFalse("Compare \"/horst\" != \"/Horst\"", Path::Compare<vislib::CharTraitsA>("/horst", "/Horst"));
//        AssertFalse("Compare \"Path::Resolve(hugo)\" != \"/hugo\"", Path::Compare<vislib::CharTraitsA>(Path::Resolve("hugo"), "/hugo"));
//        AssertTrue("Compare \"Path::Resolve(hugo)\" == \"hugo\"", Path::Compare<vislib::CharTraitsA>(Path::Resolve("hugo"), "hugo"));
//
//        AssertTrue("Compare \"/horst/\" == \"/../horst/\"", Path::Compare<vislib::CharTraitsW>(L"/horst/", L"/../horst/"));
//        AssertFalse("Compare \"/horst\" != \"/Horst\"", Path::Compare<vislib::CharTraitsW>(L"/horst", L"/Horst"));
//        AssertFalse("Compare \"Path::Resolve(hugo)\" != \"/hugo\"", Path::Compare<vislib::CharTraitsW>(Path::Resolve(L"hugo"), L"/hugo"));
//        AssertTrue("Compare \"Path::Resolve(hugo)\" == \"hugo\"", Path::Compare<vislib::CharTraitsW>(Path::Resolve(L"hugo"), L"hugo"));
//#endif /* _WIN32 */
//        
//    } catch (SystemException e) {
//        cout << e.GetMsgA() << endl;
//    }
//
//#ifdef _WIN32
//    vislib::StringA p = vislib::sys::Path::FindExecutablePath("cmd.exe");
//    cout << "Executable found at: " << p.PeekBuffer() << endl;
//    p = vislib::sys::Path::FindExecutablePath("notepad.exe");
//    cout << "Executable found at: " << p.PeekBuffer() << endl;
//    p = vislib::sys::Path::FindExecutablePath("iexplore.exe");
//    cout << "Executable found at: " << p.PeekBuffer() << endl;
//    p = vislib::sys::Path::FindExecutablePath("calc.exe");
//    cout << "Executable found at: " << p.PeekBuffer() << endl;
//    p = vislib::sys::Path::FindExecutablePath("subwcrev.exe");
//    cout << "Executable found at: " << p.PeekBuffer() << endl;
//    p = vislib::sys::Path::FindExecutablePath("test.exe");
//    cout << "Executable found at: " << p.PeekBuffer() << endl;
//    vislib::StringW w = vislib::sys::Path::FindExecutablePath(L"cmd.exe");
//    cout << "Executable found at: " << vislib::StringA(w).PeekBuffer() << endl;
//    w = vislib::sys::Path::FindExecutablePath(L"notepad.exe");
//    cout << "Executable found at: " << vislib::StringA(w).PeekBuffer() << endl;
//    w = vislib::sys::Path::FindExecutablePath(L"iexplore.exe");
//    cout << "Executable found at: " << vislib::StringA(w).PeekBuffer() << endl;
//    w = vislib::sys::Path::FindExecutablePath(L"calc.exe");
//    cout << "Executable found at: " << vislib::StringA(w).PeekBuffer() << endl;
//    w = vislib::sys::Path::FindExecutablePath(L"subwcrev.exe");
//    cout << "Executable found at: " << vislib::StringA(w).PeekBuffer() << endl;
//    w = vislib::sys::Path::FindExecutablePath(L"test.exe");
//    cout << "Executable found at: " << vislib::StringA(w).PeekBuffer() << endl;
//#else /* _WIN32 */
//    vislib::StringA p = vislib::sys::Path::FindExecutablePath("xterm");
//    cout << "Executable found at: " << p.PeekBuffer() << endl;
//    p = vislib::sys::Path::FindExecutablePath("bash");
//    cout << "Executable found at: " << p.PeekBuffer() << endl;
//#endif /* _WIN32 */
//
//}
