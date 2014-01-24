/// <copyright file="MemberInfoExtensions.cs" company="TheLib Team">
/// Copyright (C) 2013 TheLib Team (http://thelib.org). All rights reserved.
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
using System.Reflection;


namespace The.Utility {

    /// <summary>
    /// Extension methods for <see cref="System.Reflection.MemberInfo"/>.
    /// </summary>
    public static class MemberInfoExtensions {

        /// <summary>
        /// Gets all custom attributes of type <typeparamref name="T"/> that
        /// are attached to <paramref name="memberInfo"/>.
        /// </summary>
        /// <typeparam name="T">The type of the attribute to be retrieved.
        /// </typeparam>
        /// <param name="memberInfo">The member to get the attributes of.
        /// </param>
        /// <param name="inherit">Specifies whether inherited attributes should
        /// be retrieved or not. This defaults to <c>true</c>.</param>
        /// <returns>An array holding the attributes.</returns>
        public static T[] GetCustomAttributes<T>(this MemberInfo memberInfo,
                bool inherit = true) where T : Attribute {
                    if (memberInfo == null) {
                throw new ArgumentNullException("type");
            }

            object[] atts = memberInfo.GetCustomAttributes(typeof(T),
                inherit);
            return atts.Cast<T>().ToArray();
        }

        /// <summary>
        /// Gets a custom attribute of type <typeparamref name="T"/> that
        /// is attached to <paramref name="memberInfo"/>. If there are multiple
        /// attributes, the first is returned.
        /// </summary>
        /// <typeparam name="T">The type of the attribute to be retrieved.
        /// </typeparam>
        /// <param name="memberInfo">The member to get the attributes of.
        /// </param>
        /// <param name="inherit">Specifies whether inherited attributes should
        /// be retrieved or not. This defaults to <c>true</c>.</param>
        /// <returns>The first attribute of the specified member or <c>null</c>
        /// if <paramref name="memberInfo"/> does not have the specified 
        /// attribute.</returns>
        public static T GetCustomAttribute<T>(this MemberInfo memberInfo,
                bool inherit = true) where T : Attribute {
            T[] atts = MemberInfoExtensions.GetCustomAttributes<T>(memberInfo,
                inherit);
            return (atts.Length > 0) ? atts[0] : null;
        }

        /// <summary>
        /// Determine whether the member <paramref name="memberInfo"/> has one 
        /// or more custom attributes of type <typeparamref name="T"/>.
        /// </summary>
        /// <typeparam name="T">The type of the attribute to be retrieved.
        /// </typeparam>
        /// <param name="memberInfo">The member to get the attributes of.
        /// </param>
        /// <param name="inherit">Specifies whether inherited attributes should
        /// be retrieved or not. This defaults to <c>true</c>.</param>
        /// <returns><c>true</c> if the member has at least one of the specified
        /// attributes, <c>false</c> otherwise.</returns>
        /// <exception cref="System.ArgumentNullException">If
        /// <paramref name="type"/> is <c>null</c>.</exception>
        public static bool HasCustomAttribute<T>(this MemberInfo memberInfo,
                bool inherit = true) {
            return MemberInfoExtensions.HasCustomAttribute(memberInfo, 
                typeof(T), inherit);
        }

        /// <summary>
        /// Determine whether the member <paramref name="memberInfo"/> has one
        /// or more custom attributes of type <paramref name="attributeType"/>.
        /// </summary>
        /// <param name="memberInfo">The member to get the attributes of.
        /// </param>
        /// <param name="attributeType">The type of the attribute.</param>
        /// <param name="inherit">Specifies whether inherited attributes should
        /// be retrieved or not. This defaults to <c>true</c>.</param>
        /// <returns><c>true</c> if the member has at least one of the specified
        /// attributes, <c>false</c> otherwise.</returns>
        /// <exception cref="System.ArgumentNullException">If
        /// <paramref name="memberInfo"/> is <c>null</c>.</exception>
        /// <exception cref="System.ArgumentNullException">If
        /// <paramref name="attributeType"/> is <c>null</c>.</exception>
        public static bool HasCustomAttribute(this MemberInfo memberInfo,
                Type attributeType, bool inherit = true) {
            if (memberInfo == null) {
                throw new ArgumentNullException("memberInfo");
            }
            if (attributeType == null) {
                throw new ArgumentNullException("attributeType");
            }

            object[] atts = memberInfo.GetCustomAttributes(attributeType,
                inherit);
            return (atts.Length > 0);
        }
    }
}
