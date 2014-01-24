/// <copyright file="SIPrefix.cs" company="TheLib Team">
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
/// <copyright file="SIPrefix.cs">
/// Copyright (C) 2012 Christoph Müller. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>

using System;
using System.Collections.Generic;
using System.Diagnostics;


namespace The.Measurement.Units {

    /// <summary>
    /// Represents a prefix in the the International System of Units.
    /// </summary>
    /// <remarks>
    /// <para>This class provides all standard SI prefixes as static instances.
    /// It is not possible to add any custom prefixes.</para>
    /// </remarks>
    public sealed class SIPrefix : IComparable<SIPrefix>, IEquatable<SIPrefix> {

        #region Class Constructor
        /// <summary>
        /// Initialises the class.
        /// </summary>
        /// <remarks>
        /// The static constructor is guaranteed to be thread-safe according
        /// to http://msdn.microsoft.com/en-us/library/aa645612(VS.71).aspx.
        /// </remarks>
        static SIPrefix() {
            SIPrefix.SYMBOL_TO_PREFIX = new Dictionary<string, SIPrefix>();

            SIPrefix.Yotta = new SIPrefix("Y", 
                Properties.Resources.SiPrefixYotta, 1E24);
            SIPrefix.Zetta = new SIPrefix("Z",
                Properties.Resources.SiPrefixZetta, 1E21);
            SIPrefix.Exa = new SIPrefix("E",
                Properties.Resources.SiPrefixExa, 1E18);
            SIPrefix.Peta = new SIPrefix("P",
                Properties.Resources.SiPrefixPeta, 1E15);
            SIPrefix.Tera = new SIPrefix("T",
                Properties.Resources.SiPrefixTera, 1E12);
            SIPrefix.Giga = new SIPrefix("G",
                Properties.Resources.SiPrefixGiga, 1E9);
            SIPrefix.Mega = new SIPrefix("M",
                Properties.Resources.SiPrefixMega, 1E6);
            SIPrefix.Kilo = new SIPrefix("k",
                Properties.Resources.SiPrefixKilo, 1E3);
            SIPrefix.Hecto = new SIPrefix("h",
                Properties.Resources.SiPrefixHecto, 1E2);
            SIPrefix.Deca = new SIPrefix("da",
                Properties.Resources.SiPrefixDeca, 1E1);
            SIPrefix.Deci = new SIPrefix("d",
                Properties.Resources.SiPrefixDeci, 1E-1);
            SIPrefix.Centi = new SIPrefix("c",
                Properties.Resources.SiPrefixCenti, 1E-2);
            SIPrefix.Milli = new SIPrefix("m",
                Properties.Resources.SiPrefixMilli, 1E-3);
            SIPrefix.Micro = new SIPrefix("µ",
                Properties.Resources.SiPrefixMicro, 1E-6);
            SIPrefix.Nano = new SIPrefix("n",
                Properties.Resources.SiPrefixNano, 1E-9);
            SIPrefix.Pico = new SIPrefix("p",
                Properties.Resources.SiPrefixPico, 1E-12);
            SIPrefix.Femto = new SIPrefix("f",
                Properties.Resources.SiPrefixFemto, 1E-15);
            SIPrefix.Atto = new SIPrefix("a",
                Properties.Resources.SiPrefixAtto, 1E-18);
            SIPrefix.Zepto = new SIPrefix("z",
                Properties.Resources.SiPrefixZepto, 1E-21);
            SIPrefix.Yocto = new SIPrefix("y",
                Properties.Resources.SiPrefixYocto, 1E-24);
        }
        #endregion

        #region Public Class Properties
        /// <summary>
        /// Gets all known <see cref="SIPrefix"/>es.
        /// </summary>
        public static IEnumerable<SIPrefix> All {
            get {
                return SIPrefix.SYMBOL_TO_PREFIX.Values;
            }
        }
        #endregion

        #region Public Class Methods
        /// <summary>
        /// Gets the SI prefix for the given symbol.
        /// </summary>
        /// <param name="symbol">The symbol of the prefix to search for.</param>
        /// <returns>The prefix with the given symbol or <c>null</c> if
        /// such a prefix was not found.</returns>
        public static SIPrefix FromSymbol(string symbol) {
            try {
                return SIPrefix.SYMBOL_TO_PREFIX[symbol];
            } catch {
                return null;
            }
        }
        #endregion

        #region SI standard prefixes
        /// <summary>
        /// The SI prefix "Yotta" which represents 1E24.
        /// </summary>
        public static readonly SIPrefix Yotta;

        /// <summary>
        /// The SI prefix "Zetta" which represents 1E21.
        /// </summary>
        public static readonly SIPrefix Zetta;

        /// <summary>
        /// The SI prefix "Exa" which represents 1E18.
        /// </summary>
        public static readonly SIPrefix Exa;

        /// <summary>
        /// The SI prefix "Peta" which represents 1E15.
        /// </summary>
        public static readonly SIPrefix Peta;

        /// <summary>
        /// The SI prefix "Tera" which represents 1E12.
        /// </summary>
        public static readonly SIPrefix Tera;

        /// <summary>
        /// The SI prefix "Giga" which represents 1E9.
        /// </summary>
        public static readonly SIPrefix Giga;

        /// <summary>
        /// The SI prefix "Mega" which represents 1E6.
        /// </summary>
        public static readonly SIPrefix Mega;

        /// <summary>
        /// The SI prefix "Kilo" which represents 1E3.
        /// </summary>
        public static readonly SIPrefix Kilo;

        /// <summary>
        /// The SI prefix "Hecto" which represents 1E2.
        /// </summary>
        public static readonly SIPrefix Hecto;

        /// <summary>
        /// The SI prefix "Deca" which represents 1E1.
        /// </summary>
        public static readonly SIPrefix Deca;

        /// <summary>
        /// The SI prefix "Deci" which represents 1E-1.
        /// </summary>
        public static readonly SIPrefix Deci;

        /// <summary>
        /// The SI prefix "Centi" which represents 1E-2.
        /// </summary>
        public static readonly SIPrefix Centi;

        /// <summary>
        /// The SI prefix "Milli" which represents 1E-3.
        /// </summary>
        public static readonly SIPrefix Milli;

        /// <summary>
        /// The SI prefix "Micro" which represents 1E-6.
        /// </summary>
        public static readonly SIPrefix Micro;

        /// <summary>
        /// The SI prefix "Nano" which represents 1E9.
        /// </summary>
        public static readonly SIPrefix Nano;

        /// <summary>
        /// The SI prefix "Pico" which represents 1E-12.
        /// </summary>
        public static readonly SIPrefix Pico;

        /// <summary>
        /// The SI prefix "Femto" which represents 1E-15.
        /// </summary>
        public static readonly SIPrefix Femto;

        /// <summary>
        /// The SI prefix "Atto" which represents 1E-18.
        /// </summary>
        public static readonly SIPrefix Atto;

        /// <summary>
        /// The SI prefix "Zepto" which represents  1E-21.
        /// </summary>
        public static readonly SIPrefix Zepto;

        /// <summary>
        /// The SI prefix "Yocto" which represents 1E-24.
        /// </summary>
        public static readonly SIPrefix Yocto;
        #endregion

        #region Public Properties
        /// <summary>
        /// Gets the factor of the prefix.
        /// </summary>
        public double Factor { 
            get; 
            private set; 
        }

        /// <summary>
        /// Gets the name of the prefix.
        /// </summary>
        public string Name { 
            get; 
            private set; 
        }

        /// <summary>
        /// Gets the symbol of the prefix.
        /// </summary>
        public string Symbol {
            get;
            private set;
        }
        #endregion

        #region Public Methods
        /// <summary>
        /// Compares this prefix with another prefix.
        /// </summary>
        /// <param name="other">The object to be compared.</param>
        /// <returns>A negative number if this object is less than
        /// <paramref name="other"/>, zero if both are equal, and
        /// larger than zero if this object is larger than
        /// <paramref name="other"/>.</returns>
        public int CompareTo(SIPrefix other) {
            return (other != null)
                ? System.Math.Sign(this.Factor - other.Factor)
                : 1;
        }

        /// <summary>
        /// Indicates whether this prefix is equal to another prefix.
        /// </summary>
        /// <param name="other">The object to be compared.</param>
        /// <returns><c>true</c> if this object and <paramref name="rhs"/> are
        /// equal, <c>false</c> otherwise.</returns>
        public bool Equals(SIPrefix other) {
            if (other == null) {
                return false;
            } else if (object.ReferenceEquals(this, other)) {
                return true;
            } else {
                Debug.Assert((this.Factor == other.Factor) 
                    || (this.Symbol != other.Symbol));
                Debug.Assert((this.Factor == other.Factor)
                    || (this.Name != other.Name));
                return (this.Factor == other.Factor);
            }
        }

        /// <inheritdoc />
        public override bool Equals(object obj) {
            return this.Equals(obj as SIPrefix);
        }

        /// <inheritdoc />
        public override int GetHashCode() {
            return this.Factor.GetHashCode() ^ this.GetType().GetHashCode();
        }

        /// <inheritdoc />
        public override string ToString() {
            return this.Name;
        }
        #endregion

        /// <summary>
        /// Holds all known prefixes indexed by their symbol for searching 
        /// for them.
        /// </summary>
        private static readonly Dictionary<string, SIPrefix> SYMBOL_TO_PREFIX;

        #region Constructors
        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        /// <param name="symbol">The symbol fo the prefix.</param>
        /// <param name="name">The name of the prefix.</param>
        /// <param name="factor">The factor of the prefix.</param>
        /// <exception cref="System.ArgumentNullException">If
        /// <paramref name="symbol"/> is <c>null</c> or empty.</exception>
        /// <exception cref="System.ArgumentNullException">If
        /// <paramref name="name"/> is <c>null</c> or empty.</exception>
        /// <exception cref="System.ArgumentException">If a prefix with 
        /// the same symbol has already been created.</exception>
        private SIPrefix(string symbol, string name, double factor) {
            if (string.IsNullOrWhiteSpace(symbol)) {
                throw new ArgumentNullException("symbol");
            }
            if (string.IsNullOrWhiteSpace("name")) {
                throw new ArgumentNullException("name");
            }

            this.Symbol = symbol;
            this.Name = name;
            this.Factor = factor;

            // No locking is required here, because all this stuff is
            // initialised statically and we do not allow inheritance.
            SIPrefix.SYMBOL_TO_PREFIX.Add(this.Symbol, this);
        }
        #endregion
    }
}
