/// <copyright file="IEnumerableExtensions.cs" company="TheLib Team">
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
using System.Collections.Generic;
using System.Linq;


namespace The.Utility {

    /// <summary>
    /// Contains extension methods for
    /// <see cref="System.Collections.IEnumerable"/> and
    /// <see cref="System.Collections.Generic.IEnumerable"/>.
    /// </summary>
    public static class IEnumerableExtensions {

        /// <summary>
        /// Answer whether <paramref name="e"/> contains no elements.
        /// </summary>
        /// <typeparam name="T">The type of the elements.</typeparam>
        /// <param name="e">The enumerable to be tested. It is safe to
        /// pass <c>null</c>.</param>
        /// <returns><c>true</c> if <paramref name="e"/> contains no elements
        /// or is <c>null</c>, <c>false</c> otherwise.</returns>
        public static bool IsEmpty<T>(this IEnumerable<T> e) {
            return ((e == null) || (e.Count() == 0));
        }

        /// <summary>
        /// Answer the symmetric difference between <paramref name="lhs"/> and
        /// <paramref name="rhs"/>.
        /// </summary>
        /// <typeparam name="T">The type of elements.</typeparam>
        /// <param name="lhs">The first enumeration.</param>
        /// <param name="rhs">The second enumeration.</param>
        /// <returns>The symmetric difference.</returns>
        /// <exception cref="System.ArgumentNullException">If 
        /// <paramref name="lhs"/> is <c>null</c>, or if
        /// <paramref name="rhs"/> is <c>null</c>.</exception>
        public static IEnumerable<T> SymmetricDifference<T>(
                this IEnumerable<T> lhs, IEnumerable<T> rhs) {
            if (lhs == null) {
                throw new ArgumentNullException("lhs");
            }
            if (rhs == null) {
                throw new ArgumentNullException("rhs");
            }
            return lhs.Except(rhs).Union(rhs.Except(lhs));
        }
    }
}
