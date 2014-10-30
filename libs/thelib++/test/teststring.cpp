/*
 * teststring.cpp
 *
 * Copyright (c) 2012, TheLib Team (http://www.thelib.org/license)
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
 * teststring.cpp
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#include "stdafx.h"
#include "teststring.h"
#include "testhelper.h"

#include <stdarg.h>
#include <stdexcept>
#include "the/config.h"
#include "the/string.h"
#include "the/text/string_builder.h"
#include "the/text/string_converter.h"
#include "the/text/locale_utility.h"
//#include "vislib/stringTokeniser.h"

/**
 * Tests the char_utility
 */
void TestCharUtility(void);

/**
 * Tests the ANSI string
 */
void TestStringA(void);

/**
 * Tests the unicode string
 */
void TestStringW(void);

/**
 * Tests the string converter
 */
void TestStringConverter(void);

/**
 * Tests the ANSI stringBuffer
 */
void TestStringBuilderA(void);

/**
 * Tests the unicode stringBuffer
 */
void TestStringBuilderW(void);

/**
 * Tests the UTF8 encoding of strings
 */
void TestUTF8String(void);

///**
// * Tests the string tokeniser
// */
//void TestStringTokeniser(void);

/*
 * Tests the encoding of raw data in strings
 */
void TestStringRawDataEncoding(void);

/*
 * TestString
 */
void TestString(void) {
    using namespace the;

    TestCharUtility();

    TestStringA();
    TestStringW();

    TestStringConverter();
    TestUTF8String();

    TestStringRawDataEncoding();

    TestStringBuilderA();
    TestStringBuilderW();

    //TestStringTokeniser();
}


/*
 * TestCharUtility
 */
void TestCharUtility(void) {
    using the::text::char_utility;

    AssertTrue("'a' is Alpha", char_utility::is_alpha('a'));
    AssertTrue("'a' is Alpha", char_utility::is_alpha(L'a'));
    AssertFalse("'!' is not Alpha", char_utility::is_alpha('!'));
    AssertFalse("'!' is not Alpha", char_utility::is_alpha(L'!'));
    AssertTrue("'4' is Digit", char_utility::is_digit('4'));
    AssertTrue("'4' is Digit", char_utility::is_digit(L'4'));
    AssertFalse("'!' is not Digit", char_utility::is_digit('!'));
    AssertFalse("'!' is not Digit", char_utility::is_digit(L'!'));
    AssertTrue("'b' is lower case", char_utility::is_lower_case('b'));
    AssertTrue("'b' is lower case", char_utility::is_lower_case(L'b'));
    AssertFalse("'B' is not lower case", char_utility::is_lower_case('B'));
    AssertFalse("'B' is not lower case", char_utility::is_lower_case(L'B'));
    AssertTrue("' ' is space", char_utility::is_space(' '));
    AssertTrue("' ' is space", char_utility::is_space(L' '));
    AssertTrue("'\\t' is space", char_utility::is_space('\t'));
    AssertTrue("'\\t' is space", char_utility::is_space(L'\t'));
    AssertFalse("'!' is not space", char_utility::is_space('!'));
    AssertFalse("'!' is not space", char_utility::is_space(L'!'));
    AssertTrue("'C' is upper case", char_utility::is_upper_case('C'));
    AssertTrue("'C' is upper case", char_utility::is_upper_case(L'C'));
    AssertFalse("'c' is not upper case", char_utility::is_upper_case('c'));
    AssertFalse("'c' is not upper case", char_utility::is_upper_case(L'c'));

    AssertEqual("Lower case 'F' is 'f'", char_utility::to_lower_case('F'), 'f');
    AssertEqual("Lower case 'F' is 'f'", char_utility::to_lower_case(L'F'), L'f');
    AssertEqual("Lower case 'g' is 'g'", char_utility::to_lower_case('g'), 'g');
    AssertEqual("Lower case 'g' is 'g'", char_utility::to_lower_case(L'g'), L'g');
    AssertEqual("Lower case '.' is '.'", char_utility::to_lower_case('.'), '.');
    AssertEqual("Lower case '.' is '.'", char_utility::to_lower_case(L'.'), L'.');
    AssertEqual("Upper case ',' is ','", char_utility::to_upper_case(','), ',');
    AssertEqual("Upper case ',' is ','", char_utility::to_upper_case(L','), L',');
    AssertEqual("Upper case 'H' is 'H'", char_utility::to_upper_case('H'), 'H');
    AssertEqual("Upper case 'H' is 'H'", char_utility::to_upper_case(L'H'), L'H');
    AssertEqual("Upper case 'e' is 'E'", char_utility::to_upper_case('e'), 'E');
    AssertEqual("Upper case 'e' is 'E'", char_utility::to_upper_case(L'e'), L'E');

    AssertEqual("to_ansi conversion of 'a'", char_utility::to_ansi('a'), 'a');
    AssertEqual("to_ansi conversion of 'a'", char_utility::to_ansi(L'a'), 'a');
    AssertEqual("to_unicode conversion of 'a'", char_utility::to_unicode('a'), L'a');
    AssertEqual("to_unicode conversion of 'a'", char_utility::to_unicode(L'a'), L'a');

    // now test some special chars (will not result in relevant findings on Linux with UTF8-locale)
    char special;
    for (int i = 0; i < 256; i++) {
        special = static_cast<char>(i);
        if (!char_utility::is_alpha(special)) {
            continue;
        }
        printf("Char %c (%i): ", special, i);
        AssertEqual("ANSI-Unicode-ANSI roundtrip",
            char_utility::to_ansi(char_utility::to_unicode(special)), special);
    }

}


/*
 * TestStringA
 */
void TestStringA(void) {
    using namespace the;
    using the::text::string_utility;

    astring s1;
    astring s2("Horst");
    astring s3(s2);
    astring s4(5, 'h');

    AssertEqual("Hash code is correct:", string_utility::hash_code(s2), 89503376u);

    AssertTrue("Default Constructor creates empty string", !::strcmp(s1.c_str(), ""));
    AssertTrue("C string constructor", !::strcmp(s2.c_str(), "Horst"));
    AssertTrue("Copy constructor", !::strcmp(s2.c_str(), s3.c_str()));
    AssertTrue("Character constructor", !::strcmp(s4.c_str(), "hhhhh"));

    AssertTrue("Test for empty string", string_utility::is_empty(s1));
    AssertFalse("Test for emtpy string", string_utility::is_empty(s2));

    AssertEqual<size_t>("\"Horst\" consists of 5 characters", s2.length(), 5);

    AssertEqual("\"Horst\"[0] is 'H'", s2[0], 'H');
    AssertEqual("\"Horst\"[4] is 't'", s2[4], 't');

    //  Array operator does no longer throw an exception!
    //   Asserts in Debug Builds!
    //   Returns nonsense in Release Builds!
    //  The 'at' method does throw an exception.
    try {
        s2.at(-1);
        AssertTrue("std::out_of_range at begin", false);
    } catch (std::out_of_range) {
        AssertTrue("std::out_of_range at begin", true);
    }
    
    try {
        s2.at(s2.length());
        AssertTrue("std::out_of_range at end", false);
    } catch (std::out_of_range) {
        AssertTrue("std::out_of_range at end", true);
    }

    AssertTrue("Test for inequality", s1 != s2);
    AssertTrue("Test for equality", s2 == s3);

    s1 = s2;
    AssertTrue("Assignment operator", !::strcmp(s1.c_str(), s2.c_str()));

    AssertTrue("\"Horst\" begins with \"H\"", string_utility::starts_with(s1, "H"));
    AssertTrue("\"Horst\" begins with \"Ho\"", string_utility::starts_with(s1, "Ho"));
    AssertTrue("\"Horst\" begins with \"Horst\"", string_utility::starts_with(s1, "Horst"));
    AssertFalse("\"Horst\" does not begin with \"Hu\"", string_utility::starts_with(s1, "Hu"));
    AssertTrue("\"Horst\" begins with 'H'", string_utility::starts_with(s1, 'H'));
    AssertTrue("\"Horst\" begins (insensitive) with 'h'", string_utility::starts_with(s1, 'h', false));
    AssertTrue("\"Horst\" begins (insensitive) with 'horst'", string_utility::starts_with(s1, 'h', false));
    AssertFalse("\"Horst\" does not begin (insensitive) with \"hu\"", string_utility::starts_with(s1, "hu", false));

    AssertTrue("\"Horst\" ends with \"t\"", string_utility::ends_with(s1, "t"));
    AssertTrue("\"Horst\" ends with \"st\"", string_utility::ends_with(s1, "st"));
    AssertFalse("\"Horst\" does not ends with \"go\"", string_utility::ends_with(s1, "go"));
    AssertTrue("\"Horst\" ends with 't'", string_utility::ends_with(s1, 't'));
    AssertTrue("\"Horst\" ends with 't'", string_utility::ends_with(s1, "Horst"));

    AssertEqual<size_t>("First 'o' in \"Horst\"", s1.find('o'), 1);
    AssertEqualOrShow<size_t>("First 'o' in \"Horst\"", string_utility::find(s1, 'O', 0, false), 1);
    AssertEqual<size_t>("First 'o' in \"Horst\" after 2", s1.find('o', 2), astring::npos);
    AssertEqual<size_t>("Last 'o' in \"Horst\"", s1.rfind('o'), 1);
    AssertEqual<size_t>("Last 't' in \"Horst\"", s1.rfind('t'), 4);
    AssertEqual<size_t>("First \"ors\" in \"Horst\"", s1.find("ors"), 1);
    AssertEqual<size_t>("First \"ors\" in \"Horst\" after 2", s1.find("ors", 2), astring::npos);
    AssertEqual<size_t>("First \"rs\" in \"Horst\" after 2", s1.find("rs", 2), 2);
    AssertEqual<size_t>("First \"ort\" in \"Horst\"", s1.find("ort"), astring::npos);
    AssertEqual<size_t>("Last \"ors\" in \"Horst\"", s1.rfind("ors"), 1);
    AssertEqualOrShow<size_t>("Last \"ors\" in \"Horst\"", string_utility::find_last(s1, "OrS", astring::npos, false), 1);
    AssertEqual<size_t>("Last \"orst\" in \"Horst\"", s1.rfind("orst"), 1);
    AssertEqual<size_t>("Last \"orst2\" in \"Horst\"", s1.rfind("orst2"), astring::npos);
    AssertEqual<size_t>("Last \"orst2\" in \"Horst\"", s1.rfind("orst2"), astring::npos);
    AssertEqual<size_t>("Last \"rst\" in \"Horst\"", s1.rfind("rst"), 2);
    AssertEqual<size_t>("Last \"rst\" in \"Horst\" before 1", s1.rfind("rst", 1), astring::npos);
    AssertEqual<size_t>("Last \"or\" in \"Horst\" before 2", s1.rfind("or", 2), 1);

    s4 = s2 + " und Hugo";
    AssertTrue("Concatenation", !::strcmp(s4.c_str(), "Horst und Hugo"));

    s2 += " und Hugo";
    AssertTrue("Assignment concatenation", !::strcmp(s2.c_str(), "Horst und Hugo"));

    string_utility::replace(s2, 'H', 'h');
    AssertTrue("Character replacement", !::strcmp(s2.c_str(), "horst und hugo"));

    s2 = "Horst";
    s2 += " und Hugo";
    AssertTrue("Append", !::strcmp(s2.c_str(), "Horst und Hugo"));
    s2 = "Heinz und " + s2; // not a real prepend. Use stringBuilder for that
    AssertTrue("Prepend", !::strcmp(s2.c_str(), "Heinz und Horst und Hugo"));

    s1 = "Horst";
    string_utility::to_lower_case(s1);
    AssertTrue("to_lower_case", !::strcmp(s1.c_str(), "horst"));
    string_utility::to_upper_case(s1);
    AssertTrue("to_upper_case", !::strcmp(s1.c_str(), "HORST"));

    s1 = "Horst";
    string_utility::trim_begin(s1, "oH");
    AssertTrue("\"Horst\" trim begin \"oH\"", !::strcmp(s1.c_str(), "rst"));

    s1 = "Horst";
    string_utility::trim_end(s1, "st");
    AssertTrue("\"Horst\" trim end \"st\"", !::strcmp(s1.c_str(), "Hor"));

    s1 = "Horst";
    string_utility::trim_end(s1, "rt");
    AssertTrue("\"Horst\" trim end \"rt\"", !::strcmp(s1.c_str(), "Hors"));

    s1 = "Horst";
    string_utility::trim(s1, "tsH");
    AssertTrue("\"Horst\" triming \"tsH\"", !::strcmp(s1.c_str(), "or"));

    s1 = "  Horst  ";
    string_utility::trim(s1);
    AssertTrue("\"Horst\" triming \"tsH\"", !::strcmp(s1.c_str(), "Horst"));

    s1 = "  Horst  ";
    string_utility::trim_begin(s1);
    AssertTrue("\"Horst\" triming \"tsH\"", !::strcmp(s1.c_str(), "Horst  "));

    s1 = "  Horst  ";
    string_utility::trim_end(s1);
    AssertTrue("\"Horst\" triming \"tsH\"", !::strcmp(s1.c_str(), "  Horst"));

    s1 = "Horst";
    s2 = s1.substr(1);
    AssertTrue("\"Horst\".Substring(1)", !::strcmp(s2.c_str(), "orst"));

    // Throws an exception
    //s1 = "Horst";
    //s2 = s1.substr(10);
    //AssertTrue("\"Horst\".Substring(10)", !::strcmp(s2.c_str(), ""));

    s1 = "Horst";
    s2 = s1.substr(1, 3);
    AssertTrue("\"Horst\".Substring(1, 3)", !::strcmp(s2.c_str(), "ors"));

    // Throws an exception
    //s1 = "Horst";
    //s2 = s1.substr(10, 3);
    //AssertTrue("\"Horst\".Substring(10, 3)", !::strcmp(s2.c_str(), ""));

    s1 = "Horst";
    s2 = s1.substr(1, 5);
    AssertTrue("\"Horst\".Substring(1, 5)", !::strcmp(s2.c_str(), "orst"));

    s1 = "Hors";
    s2 = s1 + 't';
    AssertTrue("Character concatenation", !::strcmp(s2.c_str(), "Horst"));

    s1 = "Hors";
    s1 += 't';
    AssertTrue("Character assignment concatenation", !::strcmp(s1.c_str(), "Horst"));

    s1 = "Hors";
    s1 += 't';
    AssertTrue("Character append", !::strcmp(s1.c_str(), "Horst"));

    s1 = "orst";
    s1.insert(0, 1, 'H'); // not a real prepend. Use stringBuilder for that
    AssertTrue("Character prepend", !::strcmp(s1.c_str(), "Horst"));

    s1 = "Horst Horst Horst";
    AssertEqual<size_t>("3 replacements.", string_utility::replace(s1, "Horst", "Hugo"), 3);
    AssertTrue("string replacement", !::strcmp(s1.c_str(), "Hugo Hugo Hugo"));
    AssertEqual<size_t>("0 replacements.", string_utility::replace(s1, "Horst", "Hugo"), 0);
    
    s1 = "Horst und Hugo";
    AssertEqual<size_t>("1 replacement.", string_utility::replace(s1, "und", "oder"), 1);
    AssertTrue("string replacement", !::strcmp(s1.c_str(), "Horst oder Hugo"));

    s1 = "Horst und Hugo";
    s1.erase(5);
    AssertTrue("remove(5)", !::strcmp(s1.c_str(), "Horst"));

    s1 = "Horst und Hugo";
    s1.erase(5, 4);
    AssertTrue("remove(5, 4)", !::strcmp(s1.c_str(), "Horst Hugo"));

    s1 = "Horst und Hugo";
    s1.erase(5, 9);
    AssertTrue("remove(5, 9)", !::strcmp(s1.c_str(), "Horst"));

    s1 = "Horst und Hugo";
    s1.erase(5, 8);
    AssertTrue("remove(5, 8)", !::strcmp(s1.c_str(), "Horsto"));

    s1 = "Horst und Hugo";
    string_utility::remove(s1, "und ");
    AssertTrue("remove \"und \"", !::strcmp(s1.c_str(), "Horst Hugo"));

    s1 = "Horst";
    s1.clear();
    AssertTrue("Cleared string is emtpy", string_utility::is_empty(s1));

#ifdef THE_WINDOWS
    AssertTrue("Resource 101 found.", string_utility::load_resource(s1, 101));
    AssertTrue("Load string from resource", !::strcmp(s1.c_str(), "This is a Test"));
    AssertTrue("Resource 102 found.", string_utility::load_resource(s1, 102));
    // test two included unicode, so we cannot test here
#endif /* THE_WINDOWS */

    s1 = "Horst Horst";
    string_utility::replace(s1, "orst", "ugo", 1);
    AssertTrue("Limited string replace", !::strcmp(s1.c_str(), "Hugo Horst"));

    s1 = "Horst Horst";
    string_utility::replace(s1, "orst", "ugo", string_utility::NO_LIMIT);
    AssertTrue("Unlimited string replace", !::strcmp(s1.c_str(), "Hugo Hugo"));

    s1 = "Horst Horst";
    string_utility::replace(s1, 'o', 'i', 1);
    AssertTrue("Limited char replace", !::strcmp(s1.c_str(), "Hirst Horst"));

    s1 = "Horst Horst";
    string_utility::replace(s1, 'o', 'i', string_utility::NO_LIMIT);
    AssertTrue("Unlimited char replace", !::strcmp(s1.c_str(), "Hirst Hirst"));

    s1 = "Hrst";
    s1.insert(1, 1, 'o');
    AssertTrue("Character insert at 1", !::strcmp(s1.c_str(), "Horst"));

    s1 = "orst";
    s1.insert(0, 1, 'H');
    AssertTrue("Character insert at 0", !::strcmp(s1.c_str(), "Horst"));

    s1 = "Hors";
    s1.insert(4, 1, 't');
    AssertTrue("Character insert at end", !::strcmp(s1.c_str(), "Horst"));

    s1 = "Ht";
    s1.insert(1, "ors");
    AssertTrue("string insert at 1", !::strcmp(s1.c_str(), "Horst"));

    s1 = "rst";
    s1.insert(0, "Ho");
    AssertTrue("string insert at 0", !::strcmp(s1.c_str(), "Horst"));

    s1 = "Hor";
    s1.insert(3, "st");
    AssertTrue("string insert at end", !::strcmp(s1.c_str(), "Horst"));

    s1 = "The quick brown fox jumps over the lazy dog.";
    s2 = s1;
    string_utility::rot48(s2);
    AssertNotEqual("Obfuscated is different", s1, s2);
    string_utility::rot48(s2);
    AssertEqual("Deobfuscated is equal to original", s1, s2);

    s1 = "Tier";
    s2 = "Tor";
    AssertEqual<size_t>("Levenshtein distance.", string_utility::levenshtein_distance(s1, s2), 2);

    s1 = "kitten";
    s2 = "sitting";
    AssertEqual<size_t>("Levenshtein distance.", string_utility::levenshtein_distance(s1,s2), 3);

    double d = string_utility::parse_double_invariant("-1.2");
    AssertTrue("Double parsed", abs(d - -1.2) < 0.0001);
    d = string_utility::parse_double_invariant("0.215e-10");
    AssertTrue("Double parsed", abs(d - 0.215e-10) < 0.0001);

    string_utility::double_to_string_invariant(s1, -150.21);
    AssertTrue("double_to_string_invariant", s1 == "-150.21");
    string_utility::double_to_string_invariant(s1, 0.02);
    AssertTrue("double_to_string_invariant", s1 == "0.02");
    string_utility::double_to_string_invariant(s1, 0.0);
    AssertTrue("double_to_string_invariant", s1 == "0");
    string_utility::double_to_string_invariant(s1, 0.0, string_utility::format_style::automatic, 0);
    AssertTrue("double_to_string_invariant", s1 == "0");
    string_utility::double_to_string_invariant(s1, 1025.0, string_utility::format_style::automatic, 5, 3);
    AssertTrue("double_to_string_invariant", s1 == "01030");
    string_utility::double_to_string_invariant(s1, -0.212, string_utility::format_style::automatic, 2, 1, 4);
    AssertTrue("double_to_string_invariant", s1 == "-00.2");
    string_utility::double_to_string_invariant(s1, 0.212, string_utility::format_style::automatic, 1, 7, 2);
    AssertTrue("double_to_string_invariant", s1 == "0.21");

    //printf("%e\n", 0.0001); // Debug check; but printf dependes on Locale
    string_utility::double_to_string_invariant(s1, 0.0001, string_utility::format_style::scientific);
    AssertTrue("double_to_string_invariant", s1 == "1e-4");
    //printf("%e\n", 10.0); // Debug check; but printf dependes on Locale
    string_utility::double_to_string_invariant(s1, 10.0, string_utility::format_style::scientific);
    AssertTrue("double_to_string_invariant", s1 == "1e1");
    //printf("%e\n", 3.0); // Debug check; but printf dependes on Locale
    string_utility::double_to_string_invariant(s1, 3.0, string_utility::format_style::scientific);
    AssertTrue("double_to_string_invariant", s1 == "3");
    //printf("%e\n", -205.0); // Debug check; but printf dependes on Locale
    string_utility::double_to_string_invariant(s1, -205.0, string_utility::format_style::scientific);
    AssertTrue("double_to_string_invariant", s1 == "-2.05e2");
    //printf("%e\n", -0.000000000071);
    //    // Debug check; but printf dependes on Locale
    string_utility::double_to_string_invariant(s1, -0.000000000071, string_utility::format_style::scientific);
    AssertTrue("double_to_string_invariant", s1 == "-7.1e-11");
    //printf("%e\n", 0.0); // Debug check; but printf dependes on Locale
    string_utility::double_to_string_invariant(s1, 0.0, string_utility::format_style::scientific);
    AssertTrue("double_to_string_invariant", s1 == "0");

    AssertEqual<size_t>("finding Hugo", string_utility::find_last("HugoHugoHugoHugo", "hUgO", the::astring::npos, false), 12);
    AssertEqual<size_t>("finding Hugo", string_utility::find_last("HugoHugoHugoHugo", "hUgO", 16, false), 12);
    AssertEqual<size_t>("finding Hugo", string_utility::find_last("HugoHugoHugoHugo", "hUgO", 15, false), 8);
    AssertEqual<size_t>("finding Hugo", string_utility::find_last("HugoHugoHugoHugo", "hUgO", 7, false), 0);

    s1 = "This\tstring\nContains\rspecial\\tCharacters.";
    s2 = s1;
    AssertTrue("Characters escaped", string_utility::escape_characters(s2, '\\', "\n\r\t", "nrt"));
    AssertTrue("Characters escaped", !::strcmp(s2.c_str(), "This\\tstring\\nContains\\rspecial\\\\tCharacters."));
    AssertTrue("Characters unescaped", string_utility::unescape_characters(s2, '\\', "\n\r\t", "nrt"));
    AssertTrue("Characters unescaped", !::strcmp(s2.c_str(), s1.c_str()));

    s1 = "This'string&Contains<All\"Special>Xml Entities";
    string_utility::encode_xml_entities(s1);
    AssertTrue("Xml Entities encoded", !::strcmp(s1.c_str(), "This&apos;string&amp;Contains&lt;All&quot;Special&gt;Xml Entities"));
    string_utility::decode_xml_entities(s1);
    AssertTrue("Xml Entities encoded", !::strcmp(s1.c_str(), "This'string&Contains<All\"Special>Xml Entities"));

}

void TestStringW(void) {
    using namespace the;
    using the::text::string_utility;

    wstring s1;
    wstring s2(L"Horst");
    wstring s3(s2);
    wstring s4(5, L'h');

    AssertEqual("Hash code is correct:", string_utility::hash_code(s2), 89503376u);

    AssertTrue("Default Constructor creates empty string", !::wcscmp(s1.c_str(), L""));
    AssertTrue("C string constructor", !::wcscmp(s2.c_str(), L"Horst"));
    AssertTrue("Copy constructor", !::wcscmp(s2.c_str(), s3.c_str()));
    AssertTrue("Character constructor", !::wcscmp(s4.c_str(), L"hhhhh"));

    AssertTrue("Test for empty string", string_utility::is_empty(s1));
    AssertFalse("Test for emtpy string", string_utility::is_empty(s2));

    AssertEqual<size_t>("\"Horst\" consists of 5 characters", s2.length(), 5);

    AssertEqual("\"Horst\"[0] is 'H'", s2[0], L'H');
    AssertEqual("\"Horst\"[4] is 't'", s2[4], L't');

    //  Array operator does no longer throw an exception!
    //   Asserts in Debug Builds!
    //   Returns nonsense in Release Builds!
    //  The 'at' method does throw an exception.
    try {
        s2.at(-1);
        AssertTrue("std::out_of_range at begin", false);
    } catch (std::out_of_range) {
        AssertTrue("std::out_of_range at begin", true);
    }
    
    try {
        s2.at(s2.length());
        AssertTrue("std::out_of_range at end", false);
    } catch (std::out_of_range) {
        AssertTrue("std::out_of_range at end", true);
    }

    AssertTrue("Test for inequality", s1 != s2);
    AssertTrue("Test for equality", s2 == s3);

    s1 = s2;
    AssertTrue("Assignment operator", !::wcscmp(s1.c_str(), s2.c_str()));

    AssertTrue("\"Horst\" begins with \"H\"", string_utility::starts_with(s1, L"H"));
    AssertTrue("\"Horst\" begins with \"Ho\"", string_utility::starts_with(s1, L"Ho"));
    AssertTrue("\"Horst\" begins with \"Horst\"", string_utility::starts_with(s1, L"Horst"));
    AssertFalse("\"Horst\" does not begin with \"Hu\"", string_utility::starts_with(s1, L"Hu"));
    AssertTrue("\"Horst\" begins with 'H'", string_utility::starts_with(s1, L'H'));
    AssertTrue("\"Horst\" begins (insensitive) with 'h'", string_utility::starts_with(s1, L'h', false));
    AssertTrue("\"Horst\" begins (insensitive) with 'horst'", string_utility::starts_with(s1, L'h', false));
    AssertFalse("\"Horst\" does not begin (insensitive) with \"hu\"", string_utility::starts_with(s1, L"hu", false));

    AssertTrue("\"Horst\" ends with \"t\"", string_utility::ends_with(s1, L"t"));
    AssertTrue("\"Horst\" ends with \"st\"", string_utility::ends_with(s1, L"st"));
    AssertFalse("\"Horst\" does not ends with \"go\"", string_utility::ends_with(s1, L"go"));
    AssertTrue("\"Horst\" ends with 't'", string_utility::ends_with(s1, L't'));
    AssertTrue("\"Horst\" ends with 't'", string_utility::ends_with(s1, L"Horst"));

    AssertEqual<size_t>("First 'o' in \"Horst\"", s1.find(L'o'), 1);
    AssertEqualOrShow<size_t>("First 'o' in \"Horst\"", string_utility::find(s1, L'O', 0, false), 1);
    AssertEqual<size_t>("First 'o' in \"Horst\" after 2", s1.find(L'o', 2), astring::npos);
    AssertEqual<size_t>("Last 'o' in \"Horst\"", s1.rfind(L'o'), 1);
    AssertEqual<size_t>("First \"ors\" in \"Horst\"", s1.find(L"ors"), 1);
    AssertEqual<size_t>("First \"ors\" in \"Horst\" after 2", s1.find(L"ors", 2), astring::npos);
    AssertEqual<size_t>("First \"rs\" in \"Horst\" after 2", s1.find(L"rs", 2), 2);
    AssertEqual<size_t>("First \"ort\" in \"Horst\"", s1.find(L"ort"), astring::npos);
    AssertEqual<size_t>("Last \"ors\" in \"Horst\"", s1.rfind(L"ors"), 1);
    AssertEqualOrShow<size_t>("Last \"ors\" in \"Horst\"", string_utility::find_last(s1, L"OrS", astring::npos, false), 1);
    AssertEqual<size_t>("Last \"orst\" in \"Horst\"", s1.rfind(L"orst"), 1);
    AssertEqual<size_t>("Last \"orst2\" in \"Horst\"", s1.rfind(L"orst2"), astring::npos);
    AssertEqual<size_t>("Last \"rst\" in \"Horst\" before 1", s1.rfind(L"rst", 1), astring::npos);
    AssertEqual<size_t>("Last \"or\" in \"Horst\" before 2", s1.rfind(L"or", 2), 1);

    s4 = s2 + L" und Hugo";
    AssertTrue("Concatenation", !::wcscmp(s4.c_str(), L"Horst und Hugo"));

    s2 += L" und Hugo";
    AssertTrue("Assignment concatenation", !::wcscmp(s2.c_str(), L"Horst und Hugo"));

    string_utility::replace(s2, 'H', 'h');
    AssertTrue("Character replacement", !::wcscmp(s2.c_str(), L"horst und hugo"));

    s2 = L"Horst";
    s2 += L" und Hugo";
    AssertTrue("Append", !::wcscmp(s2.c_str(), L"Horst und Hugo"));
    s2.insert(0, L"Heinz und "); // not a real prepend. Use stringBuilder for that
    AssertTrue("Prepend", !::wcscmp(s2.c_str(), L"Heinz und Horst und Hugo"));

    s1 = L"Horst";
    string_utility::to_lower_case(s1);
    AssertTrue("to_lower_case", !::wcscmp(s1.c_str(), L"horst"));
    string_utility::to_upper_case(s1);
    AssertTrue("to_upper_case", !::wcscmp(s1.c_str(), L"HORST"));

    s1 = L"Horst";
    string_utility::trim_begin(s1, L"oH");
    AssertTrue("\"Horst\" trim begin \"oH\"", !::wcscmp(s1.c_str(), L"rst"));

    s1 = L"Horst";
    string_utility::trim_end(s1, L"st");
    AssertTrue("\"Horst\" trim end \"st\"", !::wcscmp(s1.c_str(), L"Hor"));

    s1 = L"Horst";
    string_utility::trim_end(s1, L"rt");
    AssertTrue("\"Horst\" trim end \"rt\"", !::wcscmp(s1.c_str(), L"Hors"));

    s1 = L"Horst";
    string_utility::trim(s1, L"tsH");
    AssertTrue("\"Horst\" triming \"tsH\"", !::wcscmp(s1.c_str(), L"or"));

    s1 = L"  Horst  ";
    string_utility::trim(s1);
    AssertTrue("\"Horst\" triming \"tsH\"", !::wcscmp(s1.c_str(), L"Horst"));

    s1 = L"  Horst  ";
    string_utility::trim_begin(s1);
    AssertTrue("\"Horst\" triming \"tsH\"", !::wcscmp(s1.c_str(), L"Horst  "));

    s1 = L"  Horst  ";
    string_utility::trim_end(s1);
    AssertTrue("\"Horst\" triming \"tsH\"", !::wcscmp(s1.c_str(), L"  Horst"));

    s1 = L"Horst";
    s2 = s1.substr(1);
    AssertTrue("\"Horst\".Substring(1)", !::wcscmp(s2.c_str(), L"orst"));

    // throws an exception
    //s1 = L"Horst";
    //s2 = s1.substr(10);
    //AssertTrue("\"Horst\".Substring(10)", !::wcscmp(s2.c_str(), L""));

    s1 = L"Horst";
    s2 = s1.substr(1, 3);
    AssertTrue("\"Horst\".Substring(1, 3)", !::wcscmp(s2.c_str(), L"ors"));

    // throws an exception
    //s1 = L"Horst";
    //s2 = s1.substr(10, 3);
    //AssertTrue("\"Horst\".Substring(10, 3)", !::wcscmp(s2.c_str(), L""));

    s1 = L"Horst";
    s2 = s1.substr(1, 5);
    AssertTrue("\"Horst\".Substring(1, 5)", !::wcscmp(s2.c_str(), L"orst"));

    s1 = L"Hors";
    s2 = s1 + L't';
    AssertTrue("Character concatenation", !::wcscmp(s2.c_str(), L"Horst"));

    s1 = L"Hors";
    s1 += L't';
    AssertTrue("Character assignment concatenation", !::wcscmp(s1.c_str(), L"Horst"));

    s1 = L"Hors";
    s1 += L't';
    AssertTrue("Character append", !::wcscmp(s1.c_str(), L"Horst"));

    s1 = L"orst";
    s1.insert(0, 1, L'H'); // not a real prepend. Use stringBuilder for that
    AssertTrue("Character prepend", !::wcscmp(s1.c_str(), L"Horst"));

    s1 = L"Horst Horst Horst";
    AssertEqual<size_t>("3 replacements.", string_utility::replace(s1, L"Horst", L"Hugo"), 3);
    AssertTrue("string replacement", !::wcscmp(s1.c_str(), L"Hugo Hugo Hugo"));
    AssertEqual<size_t>("0 replacements.", string_utility::replace(s1, L"Horst", L"Hugo"), 0);
    
    s1 = L"Horst und Hugo";
    AssertEqual<size_t>("1 replacement.", string_utility::replace(s1, L"und", L"oder"), 1);
    AssertTrue("string replacement", !::wcscmp(s1.c_str(), L"Horst oder Hugo"));

    s1 = L"Horst und Hugo";
    s1.erase(5);
    AssertTrue("remove(5)", !::wcscmp(s1.c_str(), L"Horst"));

    s1 = L"Horst und Hugo";
    s1.erase(5, 4);
    AssertTrue("remove(5, 4)", !::wcscmp(s1.c_str(), L"Horst Hugo"));

    s1 = L"Horst und Hugo";
    s1.erase(5, 9);
    AssertTrue("remove(5, 9)", !::wcscmp(s1.c_str(), L"Horst"));

    s1 = L"Horst und Hugo";
    s1.erase(5, 8);
    AssertTrue("remove(5, 8)", !::wcscmp(s1.c_str(), L"Horsto"));

    s1 = L"Horst und Hugo";
    string_utility::remove(s1, L"und ");
    AssertTrue("remove \"und \"", !::wcscmp(s1.c_str(), L"Horst Hugo"));

    s1 = L"Horst";
    s1.clear();
    AssertTrue("Cleared string is emtpy", string_utility::is_empty(s1));

#ifdef THE_WINDOWS
    AssertTrue("Resource 101 found.", string_utility::load_resource(s1, 101));
    AssertTrue("Load string from resource", !::wcscmp(s1.c_str(), L"This is a Test"));
    AssertTrue("Resource 102 found.", string_utility::load_resource(s1, 102));
    AssertTrue("Load string from resource (1/3)", 
        string_utility::starts_with(s1, L"This is a second Test with special Characters "));
    AssertEqual<unsigned int>("Load string from resource (2/3)", 
        s1[s1.length() - 2], 0x000020ac); // euro sign
    AssertEqual<unsigned int>("Load string from resource (2/3)", 
        s1[s1.length() - 1], 0x00003042); // hiragana A
#endif /* THE_WINDOWS */

    s1 = L"Horst Horst";
    string_utility::replace(s1, L"orst", L"ugo", 1);
    AssertTrue("Limited string replace", !::wcscmp(s1.c_str(), L"Hugo Horst"));

    s1 = L"Horst Horst";
    string_utility::replace(s1, L"orst", L"ugo", string_utility::NO_LIMIT);
    AssertTrue("Unlimited string replace", !::wcscmp(s1.c_str(), L"Hugo Hugo"));

    s1 = L"Horst Horst";
    string_utility::replace(s1, L'o', L'i', 1);
    AssertTrue("Limited char replace", !::wcscmp(s1.c_str(), L"Hirst Horst"));

    s1 = L"Horst Horst";
    string_utility::replace(s1, L'o', L'i', string_utility::NO_LIMIT);
    AssertTrue("Unlimited char replace", !::wcscmp(s1.c_str(), L"Hirst Hirst"));

    s1 = L"Hrst";
    s1.insert(1, 1, 'o');
    AssertTrue("Character insert at 1", !::wcscmp(s1.c_str(), L"Horst"));

    s1 = L"orst";
    s1.insert(0, 1, L'H');
    AssertTrue("Character insert at 0", !::wcscmp(s1.c_str(), L"Horst"));

    s1 = L"Hors";
    s1.insert(4, 1, L't');
    AssertTrue("Character insert at end", !::wcscmp(s1.c_str(), L"Horst"));

    s1 = L"Ht";
    s1.insert(1, L"ors");
    AssertTrue("string insert at 1", !::wcscmp(s1.c_str(), L"Horst"));

    s1 = L"rst";
    s1.insert(0, L"Ho");
    AssertTrue("string insert at 0", !::wcscmp(s1.c_str(), L"Horst"));

    s1 = L"Hor";
    s1.insert(3, L"st");
    AssertTrue("string insert at end", !::wcscmp(s1.c_str(), L"Horst"));

    s1 = L"The quick brown fox jumps over the lazy dog.";
    s2 = s1;
    string_utility::rot48(s2);
    AssertNotEqual("Obfuscated is different", s1, s2);
    string_utility::rot48(s2);
    AssertEqual("Deobfuscated is equal to original", s1, s2);

    s1 = L"Tier";
    s2 = L"Tor";
    AssertEqual<size_t>("Levenshtein distance.", string_utility::levenshtein_distance(s1, s2), 2);

    s1 = L"kitten";
    s2 = L"sitting";
    AssertEqual<size_t>("Levenshtein distance.", string_utility::levenshtein_distance(s1, s2), 3);

    double d = string_utility::parse_double_invariant(L"-1.2");
    AssertTrue("Double parsed", abs(d - -1.2) < 0.0001);
    d = string_utility::parse_double_invariant(L"0.215e-10");
    AssertTrue("Double parsed", abs(d - 0.215e-10) < 0.0001);

    string_utility::double_to_string_invariant(s1, -150.21);
    AssertTrue("double_to_string_invariant", s1 == L"-150.21");
    string_utility::double_to_string_invariant(s1, 0.02);
    AssertTrue("double_to_string_invariant", s1 == L"0.02");
    string_utility::double_to_string_invariant(s1, 0.0);
    AssertTrue("double_to_string_invariant", s1 == L"0");
    string_utility::double_to_string_invariant(s1, 0.0, string_utility::format_style::automatic, 0);
    AssertTrue("double_to_string_invariant", s1 == L"0");
    string_utility::double_to_string_invariant(s1, 1025.0, string_utility::format_style::automatic, 5, 3);
    AssertTrue("double_to_string_invariant", s1 == L"01030");
    string_utility::double_to_string_invariant(s1, -0.212, string_utility::format_style::automatic, 2, 1, 4);
    AssertTrue("double_to_string_invariant", s1 == L"-00.2");
    string_utility::double_to_string_invariant(s1, 0.212, string_utility::format_style::automatic, 1, 7, 2);
    AssertTrue("double_to_string_invariant", s1 == L"0.21");

    //printf("%e\n", 0.0001); // Debug check; but printf dependes on Locale
    string_utility::double_to_string_invariant(s1, 0.0001, string_utility::format_style::scientific);
    AssertTrue("double_to_string_invariant", s1 == L"1e-4");
    //printf("%e\n", 10.0); // Debug check; but printf dependes on Locale
    string_utility::double_to_string_invariant(s1, 10.0, string_utility::format_style::scientific);
    AssertTrue("double_to_string_invariant", s1 == L"1e1");
    //printf("%e\n", 3.0); // Debug check; but printf dependes on Locale
    string_utility::double_to_string_invariant(s1, 3.0, string_utility::format_style::scientific);
    AssertTrue("double_to_string_invariant", s1 == L"3");
    //printf("%e\n", -205.0); // Debug check; but printf dependes on Locale
    string_utility::double_to_string_invariant(s1, -205.0, string_utility::format_style::scientific);
    AssertTrue("double_to_string_invariant", s1 == L"-2.05e2");
    //printf("%e\n", -0.000000000071);
    //    // Debug check; but printf dependes on Locale
    string_utility::double_to_string_invariant(s1, -0.000000000071, string_utility::format_style::scientific);
    AssertTrue("double_to_string_invariant", s1 == L"-7.1e-11");
    //printf("%e\n", 0.0); // Debug check; but printf dependes on Locale
    string_utility::double_to_string_invariant(s1, 0.0, string_utility::format_style::scientific);
    AssertTrue("double_to_string_invariant", s1 == L"0");

    AssertEqual<size_t>("finding Hugo", string_utility::find_last(L"HugoHugoHugoHugo", L"hUgO", the::astring::npos, false), 12);
    AssertEqual<size_t>("finding Hugo", string_utility::find_last(L"HugoHugoHugoHugo", L"hUgO", 16, false), 12);
    AssertEqual<size_t>("finding Hugo", string_utility::find_last(L"HugoHugoHugoHugo", L"hUgO", 15, false), 8);
    AssertEqual<size_t>("finding Hugo", string_utility::find_last(L"HugoHugoHugoHugo", L"hUgO", 7, false), 0);

    s1 = L"This\tstring\nContains\rspecial\\tCharacters.";
    s2 = s1;
    AssertTrue("Characters escaped", string_utility::escape_characters(s2, L'\\', L"\n\r\t", L"nrt"));
    AssertTrue("Characters escaped", !::wcscmp(s2.c_str(), L"This\\tstring\\nContains\\rspecial\\\\tCharacters."));
    AssertTrue("Characters unescaped", string_utility::unescape_characters(s2, L'\\', L"\n\r\t", L"nrt"));
    AssertTrue("Characters unescaped", !::wcscmp(s2.c_str(), s1.c_str()));

    s1 = L"This'string&Contains<All\"Special>Xml Entities";
    string_utility::encode_xml_entities(s1);
    AssertTrue("Xml Entities encoded", !::wcscmp(s1.c_str(), L"This&apos;string&amp;Contains&lt;All&quot;Special&gt;Xml Entities"));
    string_utility::decode_xml_entities(s1);
    AssertTrue("Xml Entities encoded", !::wcscmp(s1.c_str(), L"This'string&Contains<All\"Special>Xml Entities"));

}



/*
 * TestStringConverter
 */
void TestStringConverter(void) {

    the::astring a1("Horst");
    the::wstring w1 = the::text::string_converter::to_w(a1);
    the::wstring w2(L"Hugo");
    the::astring a2 = the::text::string_converter::to_a(w2);

    AssertTrue("ANSI to wide character conversion constructor", !::wcscmp(w1.c_str(), L"Horst"));
    AssertTrue("Wide to ANSI character conversion constructor", !::strcmp(a2.c_str(), "Hugo"));

    w1.clear();
    a2.clear();

    w1 = the::text::string_converter::convert<the::wstring>(a1);
    a2 = the::text::string_converter::convert<the::astring>(w2.c_str(), 4);
    AssertTrue("ANSI to wide character conversion constructor", !::wcscmp(w1.c_str(), L"Horst"));
    AssertTrue("Wide to ANSI character conversion constructor", !::strcmp(a2.c_str(), "Hugo"));

    w1 = the::text::string_converter::convert<the::wstring>("Heinz");
    AssertTrue("ANSI to wide character conversion constructor", !::wcscmp(w1.c_str(), L"Heinz"));
    w1 = the::text::string_converter::convert<the::wstring>("Heinz", 4);
    AssertTrue("ANSI to wide character conversion constructor", !::wcscmp(w1.c_str(), L"Hein"));

    if (the::text::locale_utility::is_utf8_locale()) {

        // UTF-8 with 8 bytes but 5 characters!
        const char *utf8In = "[\x79\xC3\xA4\xE2\x82\xAC]\x00\x00\x00";
        AssertEqual<size_t>("Input string contains 8 byte of data", ::strlen(utf8In), 8);

        AssertNoException("Encode", the::text::string_converter::convert(w1, utf8In));
        AssertEqual<size_t>("Converted string contains 5 characters", w1.length(), 5);

        a2 = the::text::string_converter::to_a(w1);
        AssertEqual<size_t>("Re-Converted string contains 8 bytes", a2.length(), 8);

        AssertTrue("Re-Converted string is correct", a2.compare(utf8In) == 0);

    } else {

        for (unsigned int i = 128; i < 256; i++) {
            char c1 = static_cast<char>(i);
            if (!the::text::char_utility::is_alpha(c1)) continue;
            wchar_t w = the::text::string_converter::to_w(c1);
            char c2 = the::text::string_converter::to_a(w);

            AssertEqual("Round-trip of high-range ANSI character successful", c1, c2);
        }
        
    }

    the::text::astring_builder::format_to(a1, "test %hs string", THE_W2A(L"with"));
    AssertTrue("W2A works", !::strcmp(a1.c_str(), "test with string"));
    the::text::wstring_builder::format_to(w1, L"test %ls string", THE_A2W("with"));
    AssertTrue("A2W works", !::wcscmp(w1.c_str(), L"test with string"));

    the::text::astring_builder::format_to(a1, "test %hs string", THE_T2A(_T("also with")));
    AssertTrue("T2A works", !::strcmp(a1.c_str(), "test also with string"));
    the::text::wstring_builder::format_to(w1, L"test %ls string", THE_T2W(_T("also with")));
    AssertTrue("T2W works", !::wcscmp(w1.c_str(), L"test also with string"));

    // HAZARD! This only works with linux since UNICODE is never defined.
    // Otherwise "%s" would be interpreted as ANSI string (or multibyte) and
    // all would be lost. References:
    // http://www.cplusplus.com/reference/clibrary/cstdio/printf/  or
    // http://linux.about.com/library/cmd/blcmdl3_vswprintf.htm
    the::tstring t1;
    the::text::tstring_builder::format_to(t1, _T("test %s string"), THE_A2T("really with"));
    the::tstring t2(_T("test really with string"));
    AssertTrue("A2T works", !t2.compare(t1));
    the::text::tstring_builder::format_to(t1, _T("test %s string"), THE_W2T(L"also really with"));
    t2 = _T("test also really with string");
    AssertTrue("W2T works", !t2.compare(t1));

    //the::text::astring_builder::format_to(a1, "test %s string", AT2A(_T("also with")));
    //AssertTrue("T2A works", ::strcmp(a1.c_str(), "test with string"));
    //the::text::wstring_builder::format_to(w1, L"test %s string", T2W(_T("also with")));
    //AssertTrue("T2W works", ::wcscmp(w1.c_str(), L"test with string"));


    // The following VISlib-Tests are deprecated because they dependent on the
    // source file encoding and compiler support.

    ////the::text::string_converter::convert(a1, the::wstring(L"H‰llo Wˆrld"));
    ////the::text::string_converter::convert(w1, the::astring("H‰llo Wˆrld"));
    ////AssertTrue("assignment-conversion-test #3", !::strcmp(a1.c_str(), "H‰llo Wˆrld"));
    ////AssertTrue("assignment-conversion-test #4", !::wcscmp(w1.c_str(), L"H‰llo Wˆrld"));

    ////wchar_t wc = L'\x00e4';
    ////char ac = 0;
    ////CharTraitsW::convert(ac, wc);
    ////AssertEqual("Wide to ANSI character conversion.", ac, char('‰'));

    ////ac = char('ˆ');
    ////CharTraitsA::convert(wc, ac);
    ////AssertEqual("ANSI to wide character conversion.", wc, L'\x00f6');

}


/*
 * TestUTF8String
 */
void TestUTF8String(void) {
    the::astring utf8;
    the::astring strA;
    the::wstring strW;

    AssertTrue("Encode", the::text::string_converter::convert_to_utf8(utf8, "This is a 7-Bit Ansi Test."));
    AssertTrue("UTF8 of \"This is a 7-Bit Ansi Test.\" correct.", utf8.compare("This is a 7-Bit Ansi Test.") == 0);

    AssertTrue("Encode", the::text::string_converter::convert_to_utf8(utf8, L"This is a 7-Bit Ansi Test."));
    AssertTrue("UTF8 of L\"This is a 7-Bit Ansi Test.\" correct.", utf8.compare("This is a 7-Bit Ansi Test.") == 0);

    AssertTrue("Decode", the::text::string_converter::convert_from_utf8(strA, utf8));
    AssertTrue("decoding of UTF8 \"This is a 7-Bit Ansi Test.\" to ansi correct.", strA.compare("This is a 7-Bit Ansi Test.") == 0);

    AssertTrue("Decode", the::text::string_converter::convert_from_utf8(strW, utf8));
    AssertTrue("decoding of UTF8 \"This is a 7-Bit Ansi Test.\" to unicode correct.", strW.compare(L"This is a 7-Bit Ansi Test.") == 0);

    const char *utf8In = "[\x79\xC3\xA4\xE2\x82\xAC]\x00\x00\x00";
    AssertEqual<size_t>("Input string contains 8 byte of data", ::strlen(utf8In), 8);

    AssertTrue("Encode", the::text::string_converter::convert_from_utf8(strW, utf8In));
    AssertEqual<size_t>("Converted string contains 5 characters", strW.length(), 5);

    AssertTrue("Encode", the::text::string_converter::convert_to_utf8(strA, strW));
    AssertEqual<size_t>("Re-Converted string contains 8 bytes", strA.length(), 8);

    AssertTrue("Re-Converted string is correct", strA.compare(utf8In) == 0);

    AssertTrue("Encode", the::text::string_converter::convert_from_utf8(strA, utf8In));
    AssertEqual<size_t>("Converted string contains 5 characters", strA.length(), 
        (the::text::locale_utility::is_utf8_locale()) ? 8 : 5);

    // The following VISlib-Tests are deprecated because they dependent on the
    // source file encoding and compiler support.

    ////AssertEqual("CharTraitsW::to_ansi", the::CharTraitsW::to_ansi(L'‰'), char('‰'));
    ////AssertEqual("CharTraitsA::to_unicode", the::CharTraitsA::to_unicode('÷'), L'÷');

    ////AssertEqual<unsigned int>("UTF8-Size of \"Horst\" is 6", the::UTF8Encoder::CalcUTF8Size("Horst"), 6);
    ////AssertEqual<unsigned int>("UTF8-Size of L\"Horst\" is 6", the::UTF8Encoder::CalcUTF8Size(L"Horst"), 6);
    ////AssertEqual<unsigned int>("UTF8-Size of \"B‰h\" is 5", the::UTF8Encoder::CalcUTF8Size("B‰h"), 5);
    ////AssertEqual<unsigned int>("UTF8-Size of L\"B‰h\" is 5", the::UTF8Encoder::CalcUTF8Size(L"B‰h"), 5);

    ////AssertEqual<int>("Size of UTF8 string is correct", the::UTF8Encoder::stringLength(utf8), int(strlen("This is a 7-Bit Ansi Test.")));

    ////unsigned char tst0[] = {'B', 0xc3, 0xa4, 'h', '!', 0x00};
    ////AssertTrue("Encode", the::UTF8Encoder::Encode(utf8, "B‰h!"));
    ////AssertEqual("UTF8 of \"B‰h!\" correct.", utf8, reinterpret_cast<const char*>(tst0));

    ////AssertEqual<int>("Size of UTF8 string is correct", the::UTF8Encoder::stringLength(utf8), int(strlen("B‰h!")));

    ////AssertTrue("Decode", the::UTF8Encoder::Decode(strA, utf8));
    ////AssertEqual("decoding of UTF8 \"B‰h!\" to ansi correct.", strA, "B‰h!");

    ////AssertTrue("Decode", the::UTF8Encoder::Decode(strW, utf8));
    ////AssertEqual("decoding of UTF8 \"B‰h!\" to unicode correct.", strW, L"B‰h!");

    ////unsigned char tst1[] = {0xc3, 0xa4, 0xc3, 0xb6, 0xc3, 0xbc, 0xc3, 0x84, 0xc3, 0x96, 0xc3, 0x9c, 0xc3, 0x9f, 0x00};
    ////AssertTrue("Encode", the::UTF8Encoder::Encode(utf8, L"‰ˆ¸ƒ÷‹ﬂ"));
    ////AssertEqual("UTF8 of L\"‰ˆ¸ƒ÷‹ﬂ\" correct.", utf8, reinterpret_cast<const char*>(tst1));

    ////AssertEqual<int>("Size of UTF8 string is correct", the::UTF8Encoder::stringLength(utf8), int(strlen("‰ˆ¸ƒ÷‹ﬂ")));

    ////AssertTrue("Encode", the::UTF8Encoder::Encode(utf8, "‰ˆ¸ƒ÷‹ﬂô"));
    ////AssertEqual("UTF8 of \"‰ˆ¸ƒ÷‹ﬂô\" correct.", utf8, reinterpret_cast<const char*>(tst1));

    ////AssertTrue("Encode", the::UTF8Encoder::Encode(utf8, "‰ˆ¸ƒ÷‹ﬂ"));
    ////tst1[14] = 0; // the ô-Character is destroied by the conversion from ansi to unicode with wsnprintf
    ////AssertEqual("UTF8 of \"‰ˆ¸ƒ÷‹ﬂ\" correct.", utf8, reinterpret_cast<const char*>(tst1));

    ////AssertTrue("Encode", the::UTF8Encoder::Encode(utf8, L"‰ˆ¸ƒ÷‹ﬂô"));

    ////AssertTrue("Decode", the::UTF8Encoder::Decode(strA, utf8));
    ////AssertEqual("decoding of UTF8 \"‰ˆ¸ƒ÷‹ﬂ\" to ansi correct.", strA, "‰ˆ¸ƒ÷‹ﬂ");

    ////AssertTrue("Decode", the::UTF8Encoder::Decode(strW, utf8));
    ////AssertEqual("decoding of UTF8 \"‰ˆ¸ƒ÷‹ﬂ\" to unicode correct.", strW, L"‰ˆ¸ƒ÷‹ﬂ");

}


/*
 * remove_firstTokeniser
 */
//void remove_firstTokeniser(void) {
//    printf("Testing stringTokeniser:\n");
//    
//    the::stringTokeniserA tokeniser("This is  a Test", " ");
//    if (!AssertTrue("Tokeniser HasNext", tokeniser.HasNext())) return;
//    AssertEqual("Token correct", tokeniser.Next(), "This");
//    if (!AssertTrue("Tokeniser HasNext", tokeniser.HasNext())) return;
//    AssertEqual("Token correct", tokeniser.Next(), "is");
//    if (!AssertTrue("Tokeniser HasNext", tokeniser.HasNext())) return;
//    AssertEqual("Token correct", tokeniser.Next(), "");
//    if (!AssertTrue("Tokeniser HasNext", tokeniser.HasNext())) return;
//    AssertEqual("Token correct", tokeniser.Next(), "a");
//    if (!AssertTrue("Tokeniser HasNext", tokeniser.HasNext())) return;
//    AssertEqual("Token correct", tokeniser.Next(), "Test");
//    if (!AssertFalse("Tokeniser HasNext NOT", tokeniser.HasNext())) return;
//
//    the::stringTokeniserA t1("ThisBLAHisBLAHaBLAHsecondBLAHTest", "BLAH");
//    if (!AssertTrue("Tokeniser HasNext", t1.HasNext())) return;
//    AssertEqual("Token correct", t1.Next(), "This");
//    if (!AssertTrue("Tokeniser HasNext", t1.HasNext())) return;
//    AssertEqual("Token correct", t1.Next(), "is");
//    if (!AssertTrue("Tokeniser HasNext", t1.HasNext())) return;
//    AssertEqual("Token correct", t1.Next(), "a");
//    if (!AssertTrue("Tokeniser HasNext", t1.HasNext())) return;
//    AssertEqual("Token correct", t1.Next(), "second");
//    if (!AssertTrue("Tokeniser HasNext", t1.HasNext())) return;
//    AssertEqual("Token correct", t1.Next(), "Test");
//    if (!AssertFalse("Tokeniser HasNext NOT", t1.HasNext())) return;
//
//    the::stringTokeniserA t2("Test with no match a all", "Hugo");
//    if (!AssertTrue("Tokeniser HasNext", t2.HasNext())) return;
//    AssertEqual("Token correct", t2.Next(), "Test with no match a all");
//    if (!AssertFalse("Tokeniser HasNext NOT", t2.HasNext())) return;
//}


/*
 * vaListHelper
 */
template<class T>
static void vaListHelper(const T *const fmt, ...) {
    va_list arglist;
    T goodStr;
    T sepStr(1, static_cast<typename T::value_type>('-'));
    the::text::string_converter::convert(goodStr, "H020060H");

    va_start(arglist, fmt);
    T str = the::text::string_builder<T>::formatVa(*fmt, arglist);
    va_end(arglist);
    AssertEqual("FormatVa", str, goodStr);

    str.clear();
    va_start(arglist, fmt);
    str = the::text::string_builder<T>::formatVa(fmt->c_str(), arglist);
    va_end(arglist);
    AssertEqual("FormatVa", str, goodStr);

    str.clear();
    va_start(arglist, fmt);
    the::text::string_builder<T>::formatVa_to(str, *fmt, arglist);
    va_end(arglist);
    AssertEqual("FormatVaTo", str, goodStr);

    the::text::string_builder<T> sb1;
    va_start(arglist, fmt);
    sb1.set_formattedVa(*fmt, arglist);
    va_end(arglist);
    AssertEqual("set_formattedVa", sb1.to_string(), goodStr);
    the::text::string_builder<T> sb2;
    va_start(arglist, fmt);
    sb2.set_formattedVa(fmt->c_str(), arglist);
    va_end(arglist);
    AssertEqual("set_formattedVa", sb2.to_string(), goodStr);

    goodStr = goodStr + sepStr + goodStr;
    sb1.prepend(sepStr);
    va_start(arglist, fmt);
    sb1.prepend_formattedVa(*fmt, arglist);
    va_end(arglist);
    AssertEqual("PrependFormattedVa", sb1.to_string(), goodStr);
    sb1 = sb2;
    sb1.prepend(sepStr);
    va_start(arglist, fmt);
    sb1.prepend_formattedVa(fmt->c_str(), arglist);
    va_end(arglist);
    AssertEqual("PrependFormattedVa", sb1.to_string(), goodStr);

    goodStr += sepStr;
    sb1 = sb2;
    sb1.append(sepStr);
    va_start(arglist, fmt);
    sb1.append_formattedVa(*fmt, arglist);
    sb1.append(sepStr);
    va_end(arglist);
    AssertEqual("append_formattedVa", sb1.to_string(), goodStr);
    sb1 = sb2;
    sb1.append(sepStr);
    va_start(arglist, fmt);
    sb1.append_formattedVa(fmt->c_str(), arglist);
    sb1.append(sepStr);
    va_end(arglist);
    AssertEqual("append_formattedVa", sb1.to_string(), goodStr);

    the::text::string_converter::convert(goodStr, "H020H020060H060H");
    sb1.clear();
    va_start(arglist, fmt);
    sb1.set_formattedVa(*fmt, arglist);
    va_end(arglist);
    va_start(arglist, fmt);
    sb1.insert_formattedVa(4, *fmt, arglist);
    va_end(arglist);
    AssertEqual("insert_formattedVa", sb1.to_string(), goodStr);
    sb1.clear();
    va_start(arglist, fmt);
    sb1.set_formattedVa(*fmt, arglist);
    va_end(arglist);
    va_start(arglist, fmt);
    sb1.insert_formattedVa(4, fmt->c_str(), arglist);
    va_end(arglist);
    AssertEqual("insert_formattedVa", sb1.to_string(), goodStr);

}


/*
 * TestStringBuilderA
 */
void TestStringBuilderA(void) {
    using the::astring;
    using the::text::astring_builder;

    astring s1, s2;

    astring_builder sb1('x', 4), sb2;
    sb1.insert_formatted(2, "%d", 101);

    AssertEqual<size_t>("Formatted string of correct size", sb1.length(), 7);
    AssertTrue("Formatted string correct", ::strcmp(sb1.to_string().c_str(), "xx101xx") == 0);

    s1 = astring_builder::format("Horst");
    AssertTrue("Format \"Horst\"", !::strcmp(s1.c_str(), "Horst"));

    astring_builder::format_to(s1, "Horst%d", 2);
    AssertTrue("Format \"Horst2\"", !::strcmp(s1.c_str(), "Horst2"));

    // sadly locale dependent
    //s1.Format("Horst%d %04.1f", 2, 2.0f);
    //AssertTrue("Format \"Horst2 02.0\"", !::strcmp(s1.c_str(), "Horst2 02.0"));
    sb1.set_formatted("Horst %ls", L"Hugo");
    sb1.to_string(s1);
    AssertTrue("Format \"Horst Hugo\"", !::strcmp(s1.c_str(), "Horst Hugo"));

    s1 = "Hugo %d";
    sb1.set_formatted(&s1, 3);
    s1 = sb1.to_string();
    AssertTrue("Format with vislib format string", !::strcmp(s1.c_str(), "Hugo 3"));

    AssertTrue("Sb2 is empty", sb2.is_empty());
    AssertFalse("Sb1 is not empty", sb1.is_empty());

    AssertTrue("sb2 is not sb1", sb1 != sb2);

    sb2 = sb1;
    AssertTrue("sb2 is now sb1", sb1 == sb2);
    sb2 = sb2;
    AssertTrue("sb2 remains sb1", sb1 == sb2);
    sb2.clear();
    AssertTrue("Sb2 is empty", sb2.is_empty());
    AssertTrue("sb2 is not sb1", sb1 != sb2);
    sb2 = sb1.to_string();
    AssertTrue("sb2 is now sb1", sb1 == sb2);

    sb1.clear();
    sb2.clear();
    s1.clear();
    s2.clear();
    // now test remaining methods of stringBuilder systematically

    s1 = "Hugo%.3dHorst";
    s2 = astring_builder::format(&s1, 50);
    AssertTrue("Format", !::strcmp(s2.c_str(), "Hugo050Horst"));
    s2.clear();
    astring_builder::format_to(s2, &s1, 50);
    AssertTrue("Format", !::strcmp(s2.c_str(), "Hugo050Horst"));
    sb1 = 'x';
    AssertTrue("Assign", !::strcmp(sb1.to_string().c_str(), "x"));
    sb2 = "hugo";
    AssertTrue("Assign", !::strcmp(sb2.to_string().c_str(), "hugo"));
    sb2 += sb1;
    AssertTrue("Append", !::strcmp(sb2.to_string().c_str(), "hugox"));
    sb1 += sb2.to_string();
    AssertTrue("Append", !::strcmp(sb1.to_string().c_str(), "xhugox"));
    sb1 += 'y';
    AssertTrue("Append", !::strcmp(sb1.to_string().c_str(), "xhugoxy"));
    sb1 += "ungeloest";
    AssertTrue("Append", !::strcmp(sb1.to_string().c_str(), "xhugoxyungeloest"));
    AssertEqual("Element operator", sb1[0], 'x');
    AssertEqual("Element operator", sb1[sb1.length() - 1], 't');
    sb1.append('!', 2);
    AssertTrue("Append", !::strcmp(sb1.to_string().c_str(), "xhugoxyungeloest!!"));
    sb1.append(":-)");
    AssertTrue("Append", !::strcmp(sb1.to_string().c_str(), "xhugoxyungeloest!!:-)"));
    s1 = "__";
    sb1.append(s1);
    AssertTrue("Append", !::strcmp(sb1.to_string().c_str(), "xhugoxyungeloest!!:-)__"));
    sb2.set('q', 3);
    AssertTrue("Set", !::strcmp(sb2.to_string().c_str(), "qqq"));
    sb2.append("QQQ", 2);
    AssertTrue("Append", !::strcmp(sb2.to_string().c_str(), "qqqQQ"));
    sb1.append(sb2);
    AssertTrue("Append", !::strcmp(sb1.to_string().c_str(), "xhugoxyungeloest!!:-)__qqqQQ"));
    sb2.set(sb1);
    AssertTrue("Set", !::strcmp(sb1.to_string().c_str(), sb2.to_string().c_str()));
    sb1.trim_begin("Qx");
    AssertTrue("trim_begin", !::strcmp(sb1.to_string().c_str(), "hugoxyungeloest!!:-)__qqqQQ"));
    sb1.trim_end("qQh_):-!");
    AssertTrue("trim_end", !::strcmp(sb1.to_string().c_str(), "hugoxyungeloest"));
    sb1.trim("tsuh");
    AssertTrue("trim", !::strcmp(sb1.to_string().c_str(), "goxyungeloe"));
    sb2.set(sb1.to_string());
    AssertTrue("Set", !::strcmp(sb2.to_string().c_str(), sb1.to_string().c_str()));
    sb1.set("Hugo");
    AssertTrue("Set", !::strcmp(sb1.to_string().c_str(), "Hugo"));
    sb1.append_formatted("%4d", 2);
    AssertTrue("append_formatted", !::strcmp(sb1.to_string().c_str(), "Hugo   2"));
    s1 = "%3d";
    sb1.append_formatted(&s1, 1);
    AssertTrue("append_formatted", !::strcmp(sb1.to_string().c_str(), "Hugo   2  1"));
    AssertEqual<size_t>("replace", sb1.replace('2', '1'), 1);
    AssertTrue("replace", !::strcmp(sb1.to_string().c_str(), "Hugo   1  1"));
    AssertEqual<size_t>("replace", sb1.replace('1', "3", 1), 1);
    AssertTrue("replace", !::strcmp(sb1.to_string().c_str(), "Hugo   3  1"));
    AssertEqual<size_t>("replace", sb1.replace(astring("2"), "3", 1), 0);
    AssertTrue("replace", !::strcmp(sb1.to_string().c_str(), "Hugo   3  1"));
    AssertEqual<size_t>("replace", sb1.replace(astring("1"), astring("3"), 2), 1);
    AssertTrue("replace", !::strcmp(sb1.to_string().c_str(), "Hugo   3  3"));
    AssertEqual<size_t>("replace", sb1.replace(astring("3"), 'x', 2), 2);
    AssertTrue("replace", !::strcmp(sb1.to_string().c_str(), "Hugo   x  x"));
    AssertEqual<size_t>("replace", sb1.replace(" x", astring("4")), 2);
    AssertTrue("replace", !::strcmp(sb1.to_string().c_str(), "Hugo  4 4"));
    AssertEqual<size_t>("replace", sb1.replace('4', astring(" ! ")), 2);
    AssertTrue("replace", !::strcmp(sb1.to_string().c_str(), "Hugo   !   ! "));
    AssertEqual<size_t>("replace", sb1.replace("! ", '>'), 2);
    AssertTrue("replace", !::strcmp(sb1.to_string().c_str(), "Hugo   >  >"));
    AssertEqual<size_t>("replace", sb1.replace("Hugo", "    Horst"), 1);
    AssertTrue("replace", !::strcmp(sb1.to_string().c_str(), "    Horst   >  >"));
    AssertEqual<size_t>("replace", sb1.replace("Horst", 1, "XYZ", 2, 0), 1);
    AssertTrue("replace", !::strcmp(sb1.to_string().c_str(), "    XYorst   >  >"));
    sb1.remove(14, 2);
    AssertTrue("remove", !::strcmp(sb1.to_string().c_str(), "    XYorst   >>"));
    sb1.remove(5, 1);
    AssertTrue("remove", !::strcmp(sb1.to_string().c_str(), "    Xorst   >>"));
    sb1.remove(12);
    AssertTrue("remove", !::strcmp(sb1.to_string().c_str(), "    Xorst   "));
    astring linux_temporary("%d");
    sb1.insert_formatted(6, &linux_temporary, 123);
    AssertTrue("insert_formatted", !::strcmp(sb1.to_string().c_str(), "    Xo123rst   "));
    sb2 = "!";
    sb1.insert(2, sb2);
    AssertTrue("Insert", !::strcmp(sb1.to_string().c_str(), "  !  Xo123rst   "));
    sb1.insert(9, 'X', 2);
    AssertTrue("Insert", !::strcmp(sb1.to_string().c_str(), "  !  Xo12XX3rst   "));
    sb1.insert(10, "Hugo");
    AssertTrue("Insert", !::strcmp(sb1.to_string().c_str(), "  !  Xo12XHugoX3rst   "));
    sb1.insert(0, astring("  "));
    AssertTrue("Insert", !::strcmp(sb1.to_string().c_str(), "    !  Xo12XHugoX3rst   "));
    sb2 = sb1;
    sb1.insert(sb1.length(), astring("!"));
    AssertTrue("Insert", !::strcmp(sb1.to_string().c_str(), "    !  Xo12XHugoX3rst   !"));
    AssertEqual<size_t>("find", sb1.find('X'), 7);
    AssertEqual<size_t>("find", sb1.find("Y"), -1);
    AssertEqual<size_t>("find", sb1.find("Hugo", 1), 12);
    AssertEqual<size_t>("find", sb1.find(astring("X"), 8), 11);
    sb1 = sb2;
    sb1.trim_begin();
    AssertTrue("trim_begin", !::strcmp(sb1.to_string().c_str(), "!  Xo12XHugoX3rst   "));
    sb1 = sb2;
    sb1.trim_end();
    AssertTrue("trim_end", !::strcmp(sb1.to_string().c_str(), "    !  Xo12XHugoX3rst"));
    sb1 = sb2;
    sb1.trim();
    AssertTrue("trim", !::strcmp(sb1.to_string().c_str(), "!  Xo12XHugoX3rst"));
    sb2 = "?";
    sb1.prepend(sb2);
    AssertTrue("Prepend", !::strcmp(sb1.to_string().c_str(), "?!  Xo12XHugoX3rst"));
    sb1.prepend("What");
    AssertTrue("Prepend", !::strcmp(sb1.to_string().c_str(), "What?!  Xo12XHugoX3rst"));
    sb1.prepend(" _", 1);
    AssertTrue("Prepend", !::strcmp(sb1.to_string().c_str(), " What?!  Xo12XHugoX3rst"));
    sb1.prepend('#', 2);
    AssertTrue("Prepend", !::strcmp(sb1.to_string().c_str(), "## What?!  Xo12XHugoX3rst"));
    sb1.prepend(astring("Horst"));
    AssertTrue("Prepend", !::strcmp(sb1.to_string().c_str(), "Horst## What?!  Xo12XHugoX3rst"));
    linux_temporary = "%.4d";
    sb1.prepend_formatted(&linux_temporary, 1);
    AssertTrue("PrependFormatted", !::strcmp(sb1.to_string().c_str(), "0001Horst## What?!  Xo12XHugoX3rst"));
    sb1.prepend_formatted("%.2d", 5);
    AssertTrue("PrependFormatted", !::strcmp(sb1.to_string().c_str(), "050001Horst## What?!  Xo12XHugoX3rst"));

    sb1 = astring_builder("HorstHugo", 5);
    AssertTrue("Ctor", !::strcmp(sb1.to_string().c_str(), "Horst"));
    sb1 = astring_builder("HorstHugo");
    AssertTrue("Ctor", !::strcmp(sb1.to_string().c_str(), "HorstHugo"));
    sb1 = astring_builder(astring("Hugo"));
    AssertTrue("Ctor", !::strcmp(sb1.to_string().c_str(), "Hugo"));
    sb1.append("Horst");
    sb1.insert(2, "X");
    sb2 = astring_builder(sb1);
    AssertTrue("Ctor", !::strcmp(sb1.to_string().c_str(), "HuXgoHorst"));

    s1 = "H%.3d%.3dH";
    vaListHelper(&s1, 20, 60);

}


/*
 * TestStringBuilderW
 */
void TestStringBuilderW(void) {
    using the::wstring;
    using the::text::wstring_builder;

    wstring s1, s2;

    wstring_builder sb1(L'x', 4), sb2;
    sb1.insert_formatted(2, L"%d", 101);

    AssertEqual<size_t>("Formatted string of correct size", sb1.length(), 7);
    AssertTrue("Formatted string correct", ::wcscmp(sb1.to_string().c_str(), L"xx101xx") == 0);

    s1 = wstring_builder::format(L"Horst");
    AssertTrue("Format \"Horst\"", !::wcscmp(s1.c_str(), L"Horst"));

    wstring_builder::format_to(s1, L"Horst%d", 2);
    AssertTrue("Format \"Horst2\"", !::wcscmp(s1.c_str(), L"Horst2"));

    // sadly locale dependent
    //s1.Format("Horst%d %04.1f", 2, 2.0f);
    //AssertTrue("Format \"Horst2 02.0\"", !::wcscmp(s1.c_str(), "Horst2 02.0"));
    sb1.set_formatted(L"Horst %hs", "Hugo");
    sb1.to_string(s1);
    AssertTrue("Format \"Horst Hugo\"", !::wcscmp(s1.c_str(), L"Horst Hugo"));

    s1 = L"Hugo %d";
    sb1.set_formatted(&s1, 3);
    s1 = sb1.to_string();
    AssertTrue("Format with vislib format string", !::wcscmp(s1.c_str(), L"Hugo 3"));

    AssertTrue("Sb2 is empty", sb2.is_empty());
    AssertFalse("Sb1 is not empty", sb1.is_empty());

    AssertTrue("sb2 is not sb1", sb1 != sb2);

    sb2 = sb1;
    AssertTrue("sb2 is now sb1", sb1 == sb2);
    sb2 = sb2;
    AssertTrue("sb2 remains sb1", sb1 == sb2);
    sb2.clear();
    AssertTrue("Sb2 is empty", sb2.is_empty());
    AssertTrue("sb2 is not sb1", sb1 != sb2);
    sb2 = sb1.to_string();
    AssertTrue("sb2 is now sb1", sb1 == sb2);

    sb1.clear();
    sb2.clear();
    s1.clear();
    s2.clear();
    // now test remaining methods of stringBuilder systematically

    s1 = L"Hugo%.3dHorst";
    s2 = wstring_builder::format(&s1, 50);
    AssertTrue("Format", !::wcscmp(s2.c_str(), L"Hugo050Horst"));
    s2.clear();
    wstring_builder::format_to(s2, &s1, 50);
    AssertTrue("Format", !::wcscmp(s2.c_str(), L"Hugo050Horst"));
    sb1 = L'x';
    AssertTrue("Assign", !::wcscmp(sb1.to_string().c_str(), L"x"));
    sb2 = L"hugo";
    AssertTrue("Assign", !::wcscmp(sb2.to_string().c_str(), L"hugo"));
    sb2 += sb1;
    AssertTrue("Append", !::wcscmp(sb2.to_string().c_str(), L"hugox"));
    sb1 += sb2.to_string();
    AssertTrue("Append", !::wcscmp(sb1.to_string().c_str(), L"xhugox"));
    sb1 += L'y';
    AssertTrue("Append", !::wcscmp(sb1.to_string().c_str(), L"xhugoxy"));
    sb1 += L"ungeloest";
    AssertTrue("Append", !::wcscmp(sb1.to_string().c_str(), L"xhugoxyungeloest"));
    AssertEqual("Element operator", sb1[0], L'x');
    AssertEqual("Element operator", sb1[sb1.length() - 1], L't');
    sb1.append(L'!', 2);
    AssertTrue("Append", !::wcscmp(sb1.to_string().c_str(), L"xhugoxyungeloest!!"));
    sb1.append(L":-)");
    AssertTrue("Append", !::wcscmp(sb1.to_string().c_str(), L"xhugoxyungeloest!!:-)"));
    s1 = L"__";
    sb1.append(s1);
    AssertTrue("Append", !::wcscmp(sb1.to_string().c_str(), L"xhugoxyungeloest!!:-)__"));
    sb2.set(L'q', 3);
    AssertTrue("Set", !::wcscmp(sb2.to_string().c_str(), L"qqq"));
    sb2.append(L"QQQ", 2);
    AssertTrue("Append", !::wcscmp(sb2.to_string().c_str(), L"qqqQQ"));
    sb1.append(sb2);
    AssertTrue("Append", !::wcscmp(sb1.to_string().c_str(), L"xhugoxyungeloest!!:-)__qqqQQ"));
    sb2.set(sb1);
    AssertTrue("Set", !::wcscmp(sb1.to_string().c_str(), sb2.to_string().c_str()));
    sb1.trim_begin(L"Qx");
    AssertTrue("trim_begin", !::wcscmp(sb1.to_string().c_str(), L"hugoxyungeloest!!:-)__qqqQQ"));
    sb1.trim_end(L"qQh_):-!");
    AssertTrue("trim_end", !::wcscmp(sb1.to_string().c_str(), L"hugoxyungeloest"));
    sb1.trim(L"tsuh");
    AssertTrue("trim", !::wcscmp(sb1.to_string().c_str(), L"goxyungeloe"));
    sb2.set(sb1.to_string());
    AssertTrue("Set", !::wcscmp(sb2.to_string().c_str(), sb1.to_string().c_str()));
    sb1.set(L"Hugo");
    AssertTrue("Set", !::wcscmp(sb1.to_string().c_str(), L"Hugo"));
    sb1.append_formatted(L"%4d", 2);
    AssertTrue("append_formatted", !::wcscmp(sb1.to_string().c_str(), L"Hugo   2"));
    s1 = L"%3d";
    sb1.append_formatted(&s1, 1);
    AssertTrue("append_formatted", !::wcscmp(sb1.to_string().c_str(), L"Hugo   2  1"));
    AssertEqual<size_t>("replace", sb1.replace(L'2', L'1'), 1);
    AssertTrue("replace", !::wcscmp(sb1.to_string().c_str(), L"Hugo   1  1"));
    AssertEqual<size_t>("replace", sb1.replace(L'1', L"3", 1), 1);
    AssertTrue("replace", !::wcscmp(sb1.to_string().c_str(), L"Hugo   3  1"));
    AssertEqual<size_t>("replace", sb1.replace(wstring(L"2"), L"3", 1), 0);
    AssertTrue("replace", !::wcscmp(sb1.to_string().c_str(), L"Hugo   3  1"));
    AssertEqual<size_t>("replace", sb1.replace(wstring(L"1"), wstring(L"3"), 2), 1);
    AssertTrue("replace", !::wcscmp(sb1.to_string().c_str(), L"Hugo   3  3"));
    AssertEqual<size_t>("replace", sb1.replace(wstring(L"3"), L'x', 2), 2);
    AssertTrue("replace", !::wcscmp(sb1.to_string().c_str(), L"Hugo   x  x"));
    AssertEqual<size_t>("replace", sb1.replace(L" x", wstring(L"4")), 2);
    AssertTrue("replace", !::wcscmp(sb1.to_string().c_str(), L"Hugo  4 4"));
    AssertEqual<size_t>("replace", sb1.replace(L'4', wstring(L" ! ")), 2);
    AssertTrue("replace", !::wcscmp(sb1.to_string().c_str(), L"Hugo   !   ! "));
    AssertEqual<size_t>("replace", sb1.replace(L"! ", L'>'), 2);
    AssertTrue("replace", !::wcscmp(sb1.to_string().c_str(), L"Hugo   >  >"));
    AssertEqual<size_t>("replace", sb1.replace(L"Hugo", L"    Horst"), 1);
    AssertTrue("replace", !::wcscmp(sb1.to_string().c_str(), L"    Horst   >  >"));
    AssertEqual<size_t>("replace", sb1.replace(L"Horst", 1, L"XYZ", 2, 0), 1);
    AssertTrue("replace", !::wcscmp(sb1.to_string().c_str(), L"    XYorst   >  >"));
    sb1.remove(14, 2);
    AssertTrue("remove", !::wcscmp(sb1.to_string().c_str(), L"    XYorst   >>"));
    sb1.remove(5, 1);
    AssertTrue("remove", !::wcscmp(sb1.to_string().c_str(), L"    Xorst   >>"));
    sb1.remove(12);
    AssertTrue("remove", !::wcscmp(sb1.to_string().c_str(), L"    Xorst   "));
    wstring linux_temporary(L"%d");
    sb1.insert_formatted(6, &linux_temporary, 123);
    AssertTrue("insert_formatted", !::wcscmp(sb1.to_string().c_str(), L"    Xo123rst   "));
    sb2 = L"!";
    sb1.insert(2, sb2);
    AssertTrue("Insert", !::wcscmp(sb1.to_string().c_str(), L"  !  Xo123rst   "));
    sb1.insert(9, L'X', 2);
    AssertTrue("Insert", !::wcscmp(sb1.to_string().c_str(), L"  !  Xo12XX3rst   "));
    sb1.insert(10, L"Hugo");
    AssertTrue("Insert", !::wcscmp(sb1.to_string().c_str(), L"  !  Xo12XHugoX3rst   "));
    sb1.insert(0, wstring(L"  "));
    AssertTrue("Insert", !::wcscmp(sb1.to_string().c_str(), L"    !  Xo12XHugoX3rst   "));
    sb2 = sb1;
    sb1.insert(sb1.length(), wstring(L"!"));
    AssertTrue("Insert", !::wcscmp(sb1.to_string().c_str(), L"    !  Xo12XHugoX3rst   !"));
    AssertEqual<size_t>("find", sb1.find(L'X'), 7);
    AssertEqual<size_t>("find", sb1.find(L"Y"), -1);
    AssertEqual<size_t>("find", sb1.find(L"Hugo", 1), 12);
    AssertEqual<size_t>("find", sb1.find(wstring(L"X"), 8), 11);
    sb1 = sb2;
    sb1.trim_begin();
    AssertTrue("trim_begin", !::wcscmp(sb1.to_string().c_str(), L"!  Xo12XHugoX3rst   "));
    sb1 = sb2;
    sb1.trim_end();
    AssertTrue("trim_end", !::wcscmp(sb1.to_string().c_str(), L"    !  Xo12XHugoX3rst"));
    sb1 = sb2;
    sb1.trim();
    AssertTrue("trim", !::wcscmp(sb1.to_string().c_str(), L"!  Xo12XHugoX3rst"));
    sb2 = L"?";
    sb1.prepend(sb2);
    AssertTrue("Prepend", !::wcscmp(sb1.to_string().c_str(), L"?!  Xo12XHugoX3rst"));
    sb1.prepend(L"What");
    AssertTrue("Prepend", !::wcscmp(sb1.to_string().c_str(), L"What?!  Xo12XHugoX3rst"));
    sb1.prepend(L" _", 1);
    AssertTrue("Prepend", !::wcscmp(sb1.to_string().c_str(), L" What?!  Xo12XHugoX3rst"));
    sb1.prepend(L'#', 2);
    AssertTrue("Prepend", !::wcscmp(sb1.to_string().c_str(), L"## What?!  Xo12XHugoX3rst"));
    sb1.prepend(wstring(L"Horst"));
    AssertTrue("Prepend", !::wcscmp(sb1.to_string().c_str(), L"Horst## What?!  Xo12XHugoX3rst"));
    linux_temporary = L"%.4d";
    sb1.prepend_formatted(&linux_temporary, 1);
    AssertTrue("PrependFormatted", !::wcscmp(sb1.to_string().c_str(), L"0001Horst## What?!  Xo12XHugoX3rst"));
    sb1.prepend_formatted(L"%.2d", 5);
    AssertTrue("PrependFormatted", !::wcscmp(sb1.to_string().c_str(), L"050001Horst## What?!  Xo12XHugoX3rst"));

    sb1 = wstring_builder(L"HorstHugo", 5);
    AssertTrue("Ctor", !::wcscmp(sb1.to_string().c_str(), L"Horst"));
    sb1 = wstring_builder(L"HorstHugo");
    AssertTrue("Ctor", !::wcscmp(sb1.to_string().c_str(), L"HorstHugo"));
    sb1 = wstring_builder(wstring(L"Hugo"));
    AssertTrue("Ctor", !::wcscmp(sb1.to_string().c_str(), L"Hugo"));
    sb1.append(L"Horst");
    sb1.insert(2, L"X");
    sb2 = wstring_builder(sb1);
    AssertTrue("Ctor", !::wcscmp(sb1.to_string().c_str(), L"HuXgoHorst"));

    s1 = L"H%.3d%.3dH";
    vaListHelper(&s1, 20, 60);

}


/*
 * TestStringRawDataEncoding
 */
void TestStringRawDataEncoding(void) {
    unsigned char rawData[256];
    for (int i = 0; i < 256; ++i) rawData[i] = static_cast<unsigned char>(i);
    const char *strHexData = 
        "000102030405060708090a0b0c0d0e0f"
        "101112131415161718191a1b1c1d1e1f"
        "202122232425262728292a2b2c2d2e2f"
        "303132333435363738393a3b3c3d3e3f"
        "404142434445464748494a4b4c4d4e4f"
        "505152535455565758595a5b5c5d5e5f"
        "606162636465666768696a6b6c6d6e6f"
        "707172737475767778797a7b7c7d7e7f"
        "808182838485868788898a8b8c8d8e8f"
        "909192939495969798999a9b9c9d9e9f"
        "a0a1a2a3a4a5a6a7a8a9aaabacadaeaf"
        "b0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
        "c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"
        "d0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
        "e0e1e2e3e4e5e6e7e8e9eaebecedeeef"
        "f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff";
    const char *strBase64Data = 
        "AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJC"
        "UmJygpKissLS4vMDEyMzQ1Njc4OTo7PD0+P0BBQkNERUZHSElK"
        "S0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3"
        "BxcnN0dXZ3eHl6e3x9fn+AgYKDhIWGh4iJiouMjY6PkJGSk5SV"
        "lpeYmZqbnJ2en6ChoqOkpaanqKmqq6ytrq+wsbKztLW2t7i5ur"
        "u8vb6/wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t/g"
        "4eLj5OXm5+jp6uvs7e7v8PHy8/T19vf4+fr7/P3+/w==";

    unsigned char backData[256];

    the::astring str;

    the::text::string_utility::to_hex_string(str, rawData, 256);
    AssertEqual<size_t>("Encoded data string size correct", str.length(), 512);
    AssertTrue("Encoded data correct", strcmp(str.c_str(), strHexData) == 0);
    AssertEqual<size_t>("Encoded data size correct", the::text::string_utility::hex_string_length(str), 256);
    AssertTrue("Decoding successful", the::text::string_utility::from_hex_string(backData, 256, str));
    AssertTrue("Decoded data correct", ::memcmp(backData, rawData, 256) == 0);

    str.clear();
    for (int i = 0; i < 256; i++) backData[i] = 0;

    the::text::string_utility::to_base64_string(str, rawData, 256);
    AssertEqual<size_t>("Encoded data string size correct", str.length(), 344);
    AssertTrue("Encoded data correct", strcmp(str.c_str(), strBase64Data) == 0);
    AssertEqual<size_t>("Encoded data size correct", the::text::string_utility::base64_string_length(str), 256);
    AssertTrue("Decoding successful", the::text::string_utility::from_base64_string(backData, 256, str));
    AssertTrue("Decoded data correct", ::memcmp(backData, rawData, 256) == 0);

}
