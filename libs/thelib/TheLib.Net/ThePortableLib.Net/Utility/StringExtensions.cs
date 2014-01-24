/// <copyright file="StringExtensions.cs" company="TheLib Team">
/// Copyright (C) 2012 TheLib Team (http://thelib.org). All rights reserved. 
/// 
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
/// - Redistributions of source code must retain the above copyright notice,
///   this list of conditions and the following disclaimer.
/// - Redistributions in binary form must reproduce the above copyright notice,
///   this list of conditions and the following disclaimer in the documentation
///   and/or other materials provided with the distribution.
/// - Neither the name of TheLib, TheLib Team, nor the names of its
///   contributors may be used to endorse or promote products derived from this
///   software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THELIB TEAM AS IS AND ANY EXPRESS OR IMPLIED
/// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
/// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
/// EVENT SHALL THELIB TEAM BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
/// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
/// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
/// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
/// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
/// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/// </copyright> 
/// <author>Christoph Müller</author>

using System;
using System.Linq;


namespace The.Utility {

    /// <summary>
    /// This class provides extension methods for <see cref="System.String"/>s.
    /// </summary>
    public static class StringExtensions {

        /// <summary>
        /// Answer how many of the characters in <paramref name="str"/> fulfil
        /// the given predicate.
        /// </summary>
        /// <param name="str">The string to be tested. It is safe to pass
        /// <c>null</c>.</param>
        /// <param name="predicate">The predicate to be evaluated for each
        /// character.</param>
        /// <returns>The number of character which for
        /// <paramref name="predicate"/> returned true.</returns>
        /// <exception cref="System.ArgumentNullException">If
        /// <paramref name="predicate"/> is <c>null</c>.</exception>
        public static int CountIf(this string str, Func<char, bool> predicate) {
            if (predicate == null) {
                throw new ArgumentNullException("predicate");
            }

            int retval = 0;

            if (str != null) {
                foreach (char c in str) {
                    if (predicate(c)) {
                        ++retval;
                    }
                }
            }

            return retval;
        }

        /// <summary>
        /// Answer how many lower-case characters <paramref name="str"/> 
        /// contains.
        /// </summary>
        /// <param name="str">The string to be tested. It is safe to pass
        /// <c>null</c>.</param>
        /// <returns>The number of lower-case characters in 
        /// <paramref name="str"/>.</returns>
        public static int CountLowerCase(this string str) {
            return str.CountIf(char.IsLower);
        }

        /// <summary>
        /// Answer how many upper-case characters <paramref name="str"/> 
        /// contains.
        /// </summary>
        /// <param name="str">The string to be tested. It is safe to pass
        /// <c>null</c>.</param>
        /// <returns>The number of upper-case characters in 
        /// <paramref name="str"/>.</returns>
        public static int CountUpperCase(this string str) {
            return str.CountIf(char.IsUpper);
        }

        /// <summary>
        /// Answer how many white spaces are in <paramref name="str"/>.
        /// </summary>
        /// <remarks>
        /// A character is consideded as white space if
        /// <see cref="System.char.IsWhiteSpace"/> yields true for it.
        /// </remarks>
        /// <param name="str">The string to be tested. It is safe to pass
        /// <c>null</c>.</param>
        /// <returns>The number of white spaces in <paramref name="str"/>.
        /// </returns>
        public static int CountWhiteSpaces(this string str) {
            return str.CountIf(char.IsWhiteSpace);
        }

        /// <summary>
        /// Answer whether <paramref name="str"/> contains only lower-case
        /// characters.
        /// </summary>
        /// <param name="str">The string to be tested.</param>
        /// <returns><c>true</c> is <paramref name="str"/> only comprises
        /// lower-case characters, <c>false</c> otherwise.</returns>
        /// <exception cref="System.ArgumentNullException">If
        /// <paramref name="str"/> is <c>null</c>.</exception>
        public static bool IsAllLower(this string str) {
            if (str == null) {
                throw new ArgumentNullException("str");
            }
            return (str.CountLowerCase() == str.Length);
        }

        /// <summary>
        /// Answer whether <paramref name="str"/> contains only upper-case
        /// characters.
        /// </summary>
        /// <param name="str">The string to be tested.</param>
        /// <returns><c>true</c> is <paramref name="str"/> only comprises
        /// upper-case characters, <c>false</c> otherwise.</returns>
        /// <exception cref="System.ArgumentNullException">If
        /// <paramref name="str"/> is <c>null</c>.</exception>
        public static bool IsAllUpper(this string str) {
            if (str == null) {
                throw new ArgumentNullException("str");
            }
            return (str.CountUpperCase() == str.Length);
        }

        /// <summary>
        /// Computes the Levenshtein 
        /// (http://www.keldysh.ru/departments/dpt_10/lev.html) distance 
        /// between <paramref name="lhs"/>
        /// and <paramref name="rhs"/>.
        /// </summary>
        /// <remarks>
        /// This operation has a memory consumption of
        /// <c><paramref name="lhs"/>.Length * <paramref name="rhs"/>.Length</c>
        /// integers.
        /// </remarks>
        /// <param name="lhs">The expected string.</param>
        /// <param name="rhs">The actually  found string.</param>
        /// <param name="costAdd">The cost for adding a character. This defaults
        /// to 1.</param>
        /// <param name="costDelete">The cost for deleting a character. This
        /// defaults to 1.</param>
        /// <param name="costChange">The cost for replacing a character by
        /// another one. This defaults to 1.</param>
        /// <returns>The Levenshtein distance between the two strings.</returns>
        /// <exception cref="System.ArgumentNullException">If 
        /// <paramref name="lhs"/> is <c>null</c>, of if
        /// <paramref name="rhs"/> is <c>null</c>.</exception>
        public static int LevenshteinDistance(this string lhs, string rhs,
                int costAdd = 1, int costDelete = 1, int costChange = 1) {
            if (lhs == null) {
                throw new ArgumentNullException("lhs");
            }
            if (rhs == null) {
                throw new ArgumentNullException("rhs");
            }

            int[,] dist = new int[lhs.Length, rhs.Length];

            dist[0, 0] = 0;

            for (int i = 0; i <= lhs.Length; ++i) {
                dist[i, 0] = i * costAdd;
            }

            for (int i = 0; i <= rhs.Length; ++i) {
                dist[0, i] = i * costDelete;
            }

            for (int i = 1; i <= lhs.Length; ++i) {
                for (int j = 1; j <= rhs.Length; ++j) {
                    int[] cost = new int[] {
                        dist[i, j - 1] + costAdd,
                        (lhs[i - 1] == rhs[j - 1]) ? 0 : costChange,
                        dist[i - 1, j] + costDelete
                    };
                    dist[i, j] = cost.Min();
                }
            }

            return dist[lhs.Length, rhs.Length];
        }

        /// <summary>
        /// Returns a copy of <paramref name="str"/> which is guaranteed to
        /// start with a upper-case character.
        /// </summary>
        /// <remarks>
        /// For performance reasons, the string will not be cloned if it is
        /// <c>null</c> or empty, or if it already starts with a upper-case
        /// character.
        /// </remarks>
        /// <param name="str">The string to make the first character
        /// upper-case of. It is safe to pass <c>null</c>.</param>
        /// <returns>The string with the first character being upper-case.
        /// </returns>
        public static string ToCapitalised(this string str) {
            if (string.IsNullOrEmpty(str)) {
                return string.Empty;
            } else if (char.IsUpper(str[0])) {
                return str;
            } else {
                return char.ToUpper(str[0]) + str.Substring(1);
            }
        }

        /// <summary>
        /// Returns a reversed copy of a string.
        /// </summary>
        /// <param name="str">The string to be resersed. It is safe to
        /// pass <c>null</c>.</param>
        /// <returns>The reverse of <paramref name="str"/>.</returns>
        public static string ToReversed(this string str) {
            if (str != null) {
                char[] r = new char[str.Length];
                for (int i = 0; i < str.Length; ++i) {
                    r[i] = str[str.Length - i];
                }
                return new string(r);
            } else {
                return str;
            }
        }

        /// <summary>
        /// Returns a copy of <paramref name="str"/> which is guaranteed to
        /// start with a lower-case character.
        /// </summary>
        /// <remarks>
        /// For performance reasons, the string will not be cloned if it is
        /// <c>null</c> or empty, or if it already starts with a lower-case
        /// character.
        /// </remarks>
        /// <param name="str">The string to make the first character
        /// lower-case of. It is safe to pass <c>null</c>.</param>
        /// <returns>The string with the first character being lower-case.
        /// </returns>
        public static string ToUncapitalised(this string str) {
            if (string.IsNullOrEmpty(str)) {
                return str;
            } else if (char.IsLower(str[0])) {
                return str;
            } else {
                return char.ToLower(str[0]) + str.Substring(1);
            }
        }

        /// <summary>
        /// Untabifies a string by replacing all tabs with the requested number
        /// of spaces.
        /// </summary>
        /// <param name="str">The string to be untabified. It is safe to pass
        /// <c>null</c>.</param>
        /// <param name="tabs">The number of spaces to be added for each
        /// tab. This parameter defaults to 4.</param>
        /// <returns>The string with tabs replaced by the specified number
        /// of spaces.</returns>
        public static string ToUntabified(this string str, int tabs = 4) {
            if (string.IsNullOrEmpty(str)) {
                return str;
            } else {
                string spaces = new string(' ', tabs);
                return str.Replace("\t", spaces);
            }
        }

    }
}
