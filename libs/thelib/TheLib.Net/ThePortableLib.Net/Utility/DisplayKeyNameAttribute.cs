/// <copyright file="DisplayKeyNameAttribute.cs" company="TheLib Team">
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
using System.Collections.Generic;
using System.Reflection;


namespace The.Utility {

    /// <summary>
    /// This <see cref="Attribute"/> allows for annotating members with
    /// display names stored in a <see cref="IDictionary"/> or as properties
    /// of a static object.
    /// </summary>
    [AttributeUsage(AttributeTargets.Field | AttributeTargets.Property,
        AllowMultiple = false, Inherited = true)]
    public class DisplayKeyNameAttribute : Attribute {

        /// <summary>
        /// Initialise a new instance using the <see cref="String"/>-valued 
        /// static property with name <paramref name="keyName"/> of type
        /// <paramref name="staticPropertySource"/>
        /// </summary>
        /// <param name="keyName">The name of the property to be retrieved from
        /// <paramref name="staticPropertySource"/>.</param>
        /// <param name="staticPropertySource">The object to retrieve the value
        /// from.</param>
        /// <exception cref="ArgumentNullException">If
        /// <paramref name="keyName"/> is <c>null</c>, or if
        /// <paramref name="staticPropertySource"/> is <c>null</c>.</exception>
        public DisplayKeyNameAttribute(string keyName,
                Type staticPropertySource) {
            if (keyName == null) {
                throw new ArgumentNullException("keyName");
            }
            if (staticPropertySource == null) {
                throw new ArgumentNullException("staticPropertySource");
            }

            this.DictionarySource = null;
            this.KeyName = keyName;
            this.StaticPropertySource = staticPropertySource;
        }

        /// <summary>
        /// Initialise a new instance using the element with name 
        /// <paramref name="keyName"/> in the dictionary 
        /// <paramref name="dictionarySource"/>.
        /// </summary>
        /// <param name="keyName">The key to retrieve from the dictionary.
        /// </param>
        /// <param name="dictionarySource">The dictionary to retrieve the 
        /// values from</param>
        /// <exception cref="ArgumentNullException">If
        /// <paramref name="keyName"/> is <c>null</c>, or if
        /// <paramref name="dictionarySource"/> is <c>null</c>.</exception>
        public DisplayKeyNameAttribute(string keyName,
                IDictionary<string, string> dictionarySource) {
            if (keyName == null) {
                throw new ArgumentNullException("keyName");
            }
            if (dictionarySource == null) {
                throw new ArgumentNullException("dictionarySource");
            }

            this.DictionarySource = dictionarySource;
            this.KeyName = keyName;
            this.StaticPropertySource = null;
        }

        /// <summary>
        /// Gets the <see cref="IDictionary"/> to get the string from if 
        /// a dictionary has been set as source.
        /// </summary>
        /// <remarks>
        /// If this property is <c>null</c>, the property 
        /// <see cref="StaticPropertySource"/> must be non-<c>null</c>.
        /// </remarks>
        public IDictionary<string, string> DictionarySource {
            get;
            private set;
        }

        /// <summary>
        /// Gets the key name identifying the actual string resource to be
        /// used as display name.
        /// </summary>
        public string KeyName {
            get;
            private set;
        }

        /// <summary>
        /// Gets the <see cref="Type"/> descriptor object to retrieve the
        /// strings from if static properties are to be used as source.
        /// </summary>
        /// <remarks>
        /// If this property is <c>null</c>, the property 
        /// <see cref="DictionarySource"/> must be non-<c>null</c>.
        /// </remarks>
        public Type StaticPropertySource {
            get;
            private set;
        }

        /// <summary>
        /// Retrieve the actual value.
        /// </summary>
        /// <returns>The actual value from the dictionary or from the class.
        /// </returns>
        public string GetValue() {
            if (this.DictionarySource != null) {
                return this.DictionarySource[this.KeyName];

            } else if (this.StaticPropertySource != null) {
                PropertyInfo pi = this.StaticPropertySource.GetProperty(
                    this.KeyName, BindingFlags.Public | BindingFlags.NonPublic
                    | BindingFlags.Static);
                return pi.GetValue(null, null).ToString();

            } else {
                return this.KeyName;
            }
        }
    }
}
