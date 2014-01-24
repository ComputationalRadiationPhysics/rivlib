/// <copyright file="UcumSymbolAttribute.cs" company="TheLib Team">
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
using System.Reflection;
using The.Utility;


namespace The.Measurement.Units {

    /// <summary>
    /// This attribute is used to attach UCUM (http://unitsofmeasure.org/)
    /// symbols to exisiting units.
    /// </summary>
    /// <remarks>
    /// <para>UCUM symbols should be attached to all fields and properties
    /// in <see cref="UcumUnits"/>. Other systems of units should not use
    /// this attribute directly, because the UCUM symbols overwrite the
    /// "normal" unit symbol for indexing in <see cref="UnitSystem"/>-derived
    /// classes.</para>
    /// <para>This attribute should also be attached to prefix instances that
    /// are supported in UCUM, e.g. <see cref="SIPrefix"/>.</para>
    /// </remarks>
    [AttributeUsage(AttributeTargets.Property | AttributeTargets.Field,
        AllowMultiple = false)]
    public sealed class UcumSymbolAttribute : Attribute {

        /// <summary>
        /// Gets the <see cref="UcumSymbolAttribute"/> from the given member
        /// if there is any.
        /// </summary>
        /// <param name="mi">The property or field to get the attribute 
        /// for.</param>
        /// <returns>The attribute, or <c>null</c> if no attribute was
        /// attached.</returns>
        public static UcumSymbolAttribute Get(MemberInfo mi) {
            return (UcumSymbolAttribute) Attribute.GetCustomAttribute(mi,
                typeof(UcumSymbolAttribute), false);
        }

        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        /// <remarks>
        /// The order of the parameters has been chosen to match the order in 
        /// the UCUM tables. From a software engineering point of view, it would
        /// have been better to have the print symbol as last and optional
        /// parameter, but this makes the use of the attribute error-prone.
        /// </remarks>
        /// <param name="printSymbol">The suggested formatted print symbol. This
        /// parameter can be <c>null</c>.</param>
        /// <param name="caseSensitiveSymbol">The case-sensitive symbol.</param>
        /// <param name="caseInsenitiveSymbol">The case-insensitive 
        /// symbol.</param>
        /// <exception cref="System.ArgumentNullException">If
        /// <paramref name="caseSensitiveSymbol"/> is empty or <c>null</c>, or
        /// if <paramref name="caseInsenitiveSymbol"/> is empty or <c>null</c>.
        /// </exception>
        /// <exception cref="System.ArgumentException">If
        /// <paramref name="caseInsenitiveSymbol"/> is not all upper case.
        /// </exception>
        public UcumSymbolAttribute(string printSymbol, 
                string caseSensitiveSymbol, string caseInsenitiveSymbol) {
            if (string.IsNullOrWhiteSpace(caseSensitiveSymbol)) {
                throw new ArgumentNullException("caseSensitiveSymbol");
            }
            if (string.IsNullOrWhiteSpace(caseInsenitiveSymbol)) {
                throw new ArgumentNullException("caseInsenitiveSymbol");
            }
            if (caseInsenitiveSymbol.CountIf(UcumSymbolAttribute.isValidCiChar)
                    != caseInsenitiveSymbol.Length) {
                throw new ArgumentException(
                    Properties.Resources.ErrorInvalidUcumCiSymbol,
                    "caseInsenitiveSymbol");
            }
            this.CaseSensitiveSymbol = caseSensitiveSymbol;
            this.CaseInsensitiveSymbol = caseInsenitiveSymbol;
            this.PrintSymbol = printSymbol;
        }

        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        /// <param name="caseSensitiveSymbol">The case-sensitive symbol.</param>
        /// <param name="caseInsenitiveSymbol">The case-insensitive 
        /// symbol.</param>
        /// <exception cref="System.ArgumentNullException">If
        /// <paramref name="caseSensitiveSymbol"/> is empty or <c>null</c>, or
        /// if <paramref name="caseInsenitiveSymbol"/> is empty or <c>null</c>.
        /// </exception>
        /// <exception cref="System.ArgumentException">If
        /// <paramref name="caseInsenitiveSymbol"/> is not all upper case.
        /// </exception>
        public UcumSymbolAttribute(string caseSensitiveSymbol,
            string caseInsenitiveSymbol) 
            : this(null, caseSensitiveSymbol, caseInsenitiveSymbol) { }

        /// <summary>
        /// Gets the case-insensitive symbol.
        /// </summary>
        public string CaseInsensitiveSymbol {
            get;
            private set;
        }

        /// <summary>
        /// Gets the case-sensitive symbol.
        /// </summary>
        public string CaseSensitiveSymbol {
            get;
            private set;
        }

        /// <summary>
        /// Gets the print symbol.
        /// </summary>
        public string PrintSymbol {
            get;
            private set;
        }

        /// <summary>
        /// Answer whether <paramref name="c"/> is a valid character for a
        /// case-insensitive UCUM symbol.
        /// </summary>
        /// <param name="c">The character to be tested.</param>
        /// <returns><c>true</c> if the character is valid, <c>false</c>
        /// otherwise.</returns>
        private static bool isValidCiChar(char c) {
            return (char.IsUpper(c) || (c == '[') || (c == '_') || (c == ']'));
        }
    }
}
