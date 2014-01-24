/// <copyright file="AlternateUnit.cs" company="TheLib Team">
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
using System.Diagnostics;
using System.Reflection;
using The.Measurement.Converters;
using The.Measurement.Dimensions;
using The.Measurement.Quantities;
using The.Utility;


namespace The.Measurement.Units {

    /// <summary>
    /// Represents a "renamed" version of another unit.
    /// </summary>
    /// <remarks>
    /// <para>Equality of two <see cref="AlternateUnit"/>s is solely based
    /// on the equality of the <see cref="AlternateUnit.ParentUnit"/>s.</para>
    /// <para>There is also a generic version of <see cref="AlternateUnit"/>,
    /// which allows for attaching quantities to exisiting units. For the
    /// generic variant, a new symbol is optional. For this class, it is not
    /// as this would not make any sense.</para>
    /// <para>It is hugly that the implementation of alternate units is
    /// replicated, but I do not have better idea at the moment.</para>
    /// </remarks>
    public sealed class AlternateUnit : Unit, IEquatable<AlternateUnit> {

        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        /// <param name="symbol">The alternate symbol for
        /// <paramref name="parentUnit"/>.</param>
        /// <param name="name">The alternate name for
        /// <paramref name="parentUnit"/>. This can be <c>null</c>.</param>
        /// <param name="parentUnit">The unit to be wrapped.</param>
        /// <exception cref="System.ArgumentNullException">If
        /// <paramref name="symbol"/> is <c>null</c>, or if
        /// <paramref name="parentUnit"/> is <c>null</c>.</exception>
        public AlternateUnit(string symbol, string name, Unit parentUnit) {
            /* Sanity checks. */
            if (string.IsNullOrWhiteSpace(symbol)) {
                throw new ArgumentNullException("symbol");
            }
            if (parentUnit == null) {
                throw new ArgumentNullException("unit");
            }

            this.name = name ?? symbol;
            this.parentUnit = AlternateUnit.Flatten(parentUnit);
            this.symbol = symbol;
        }

        #region Public Properties
        /// <inheritdoc />
        public override Unit CoherentUnit {
            get {
                return this.ParentUnit.CoherentUnit;
            }
        }

        /// <inheritdoc />
        public override UnitConverter ConverterToCoherent {
            get {
                return this.ParentUnit.ConverterToCoherent;
            }
        }

        /// <inheritdoc />
        public override Dimension Dimension {
            get {
                return this.ParentUnit.Dimension;
            }
        }

        /// <inheritdoc />
        public override bool IsCoherent {
            get {
                return this.parentUnit.IsCoherent;
            }
        }

        /// <inheritdoc />
        public override string Name {
            get {
                return this.name;
            }
        }

        /// <inheritdoc />
        public Unit ParentUnit {
            get {
                return this.parentUnit;
            }
        }

        /// <inheritdoc />
        public override string Symbol {
            get {
                return this.symbol;
            }
        }
        #endregion

        public bool Equals(AlternateUnit other) {
            if (other == null) {
                return false;
            } else if (object.ReferenceEquals(this, other)) {
                return true;
            } else {
                // TODO: Semantik so OK?
                return this.ParentUnit.Equals(other.ParentUnit);
            }
        }

        public override bool Equals(Unit other) {
            // TODO: Semantik so OK?
            return (this.Equals(other as AlternateUnit)
                || this.ParentUnit.Equals(other));
        }

        // TODO Hash und Equals()?

        /// <summary>
        /// If <paramref name="unit"/> is a <see cref="AlternateUnit"/> (or its
        /// stronly typed variant), return its
        /// <see cref="AlternateUnit.ParentUnit"/>. Otherwise, return 
        /// <paramref name="unit"/>.
        /// </summary>
        /// <param name="unit"></param>
        /// <returns></returns>
        internal static Unit Flatten(Unit unit) {
            if (unit is AlternateUnit) {
                return ((AlternateUnit) unit).ParentUnit;

            } else if (unit.GetType().IsGenericInstantiationOf(
                    typeof(AlternateUnit<>))) {
                PropertyInfo pi = unit.GetType().GetProperty("ParentUnit");
                Debug.Assert(pi != null);
                Debug.Assert(pi.PropertyType.IsAssignableFrom(typeof(Unit)));
                Debug.Assert(pi.CanRead);
                return (Unit) pi.GetValue(unit, null);

            } else {
                return unit;
            }
        }

        #region Private Fields
        /// <summary>
        /// The alternate name of the unit.
        /// </summary>
        private readonly string name;

        /// <summary>
        /// The original unit wrapped by this one.
        /// </summary>
        private readonly Unit parentUnit;

        /// <summary>
        /// The alternate symbol of the unit.
        /// </summary>
        private readonly string symbol;
        #endregion
    }


    /// <summary>
    /// Represents a "renamed" version of another unit.
    /// </summary>
    /// <remarks>
    /// <para>Equality of two <see cref="AlternateUnit"/>s is solely based
    /// on the equality of the <see cref="AlternateUnit.ParentUnit"/>s.</para>
    /// <para><see cref="AlternateUnits"/> can also be used to "attach"
    /// typesafety to other units or to relabel their quantity. The user is
    /// resposible for creating only meaningful instances.</para>
    /// </remarks>
    /// <typeparam name="Q"></typeparam>
    public sealed class AlternateUnit<Q> : Unit<Q>, IEquatable<AlternateUnit<Q>>
            where Q : Quantity<Q> {

        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        /// <param name="symbol">The alternate symbol for
        /// <paramref name="parentUnit"/>. This can be <c>null</c>.</param>
        /// <param name="name">The alternate name for
        /// <paramref name="parentUnit"/>. This can be <c>null</c>.</param>
        /// <param name="parentUnit">The unit to be wrapped.</param>
        /// <exception cref="System.ArgumentNullException">If
        /// <paramref name="symbol"/> is <c>null</c>.</exception>
        public AlternateUnit(string symbol, string name, Unit parentUnit) {
            /* Sanity checks. */
            // Note: 'symbol' can be null here! This is on purpose.
            if (parentUnit == null) {
                throw new ArgumentNullException("unit");
            }

            this.name = name ?? symbol;
            this.parentUnit = AlternateUnit.Flatten(parentUnit);
            this.symbol = symbol;
        }

        public AlternateUnit(Unit parentUnit) : this(null, null, parentUnit) { }

        #region Public Properties
        /// <inheritdoc />
        public override Unit CoherentUnit {
            get {
                return this.ParentUnit.CoherentUnit;
            }
        }

        /// <inheritdoc />
        public override UnitConverter ConverterToCoherent {
            get {
                return this.ParentUnit.ConverterToCoherent;
            }
        }

        /// <inheritdoc />
        public override Dimension Dimension {
            get {
                return this.ParentUnit.Dimension;
            }
        }

        /// <inheritdoc />
        public override bool IsCoherent {
            get {
                return this.parentUnit.IsCoherent;
            }
        }

        /// <summary>
        /// Gets the alternate unit name or the name of the underlying
        /// parent unit if no alternate name was given.
        /// </summary>
        public override string Name {
            get {
                return this.name ?? this.parentUnit.Name;
            }
        }

        /// <summary>
        /// Gets the parent unit that this unit is an alternate version
        /// of.
        /// </summary>
        public Unit ParentUnit {
            get {
                return this.parentUnit;
            }
        }

        /// <summary>
        /// Gets the alternate unit symbol or the symbol of the underlying
        /// parent unit if no alternate symbol was given.
        /// </summary>
        public override string Symbol {
            get {
                return this.symbol ?? this.parentUnit.Symbol;
            }
        }
        #endregion

        public bool Equals(AlternateUnit<Q> other) {
            if (other == null) {
                return false;
            } else if (object.ReferenceEquals(this, other)) {
                return true;
            } else {
                // TODO: Semantik so OK?
                return this.ParentUnit.Equals(other.ParentUnit);
            }
        }

        public override bool Equals(Unit other) {
            // TODO: Semantik so OK?
            return (this.Equals(other as AlternateUnit<Q>) 
                || this.ParentUnit.Equals(other));
        }

        // TODO Hash und Equals()?

        #region Private Fields
        /// <summary>
        /// The alternate name of the unit.
        /// </summary>
        private readonly string name;

        /// <summary>
        /// The original unit wrapped by this one.
        /// </summary>
        private readonly Unit parentUnit;

        /// <summary>
        /// The alternate symbol of the unit.
        /// </summary>
        private readonly string symbol;
        #endregion
    }
}
