/// <copyright file="ConvertedUnit.cs" company="TheLib Team">
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
using The.Measurement.Converters;
using The.Measurement.Quantities;


namespace The.Measurement.Units {

    /// <summary>
    /// Represents a unit which of the values are obtained by transforming
    /// quantities of another unit using a
    /// <see cref="The.Measurement.Converters.UnitConverter"/>.
    /// </summary>
    /// <remarks>
    /// <para>There exists only a stronly typed variant of
    /// <see cref="ConvertedUnit"/>, because the computation of the
    /// <see cref="Dimension"/> measured by a unit relies on transformations
    /// not changing the quantity.</para>
    /// <para>It is legal to use <see cref="UnitConverter.Identity"/> as
    /// converter for the unit. In this case, the result will be an alias symbol
    /// for the underlying unit.</para>
    /// </remarks>
    /// <typeparam name="Q">The quantity that the unit measures.</typeparam>
    public class ConvertedUnit<Q> : Unit<Q>, IEquatable<ConvertedUnit<Q>> 
            where Q : Quantity<Q> {

        /// <summary>
        /// Initialise a new instance.
        /// </summary>
        /// <param name="symbol">The symbol of the new unit. This must be unique
        /// if <paramref name="registerSymbol"/> is <c>true</c>.</param>
        /// <param name="name">The localised name of the unit.</param>
        /// <param name="unit">The unit to be transformed.</param>
        /// <param name="converter">The converter to transform the unit.
        /// <see cref="UnitConverter.Identity"/> is allowed here for creating
        /// alias names.</param>
        /// <exception cref="System.ArgumentNullException">If the
        /// <paramref name="name"/> is <c>null</c>, or
        /// if <paramref name="unit"/> is <c>null</c>, or
        /// if <paramref name="converter"/> is <c>null</c>.</exception>
        /// <exception cref="System.ArgumentException">If another unit with the
        /// specified symbol was already registered and that previously
        /// created unit is not equal to this one.</exception>
        public ConvertedUnit(string symbol, string name, Unit unit,
                UnitConverter converter) : base(symbol, name) {
            /* Sanity checks. */
            if (unit == null) {
                throw new ArgumentNullException("unit");
            }
            if (converter == null) {
                throw new ArgumentNullException("converter");
            }

            // Dear maintainer,
            // you hit this assertion because you have added a new UnitConverter
            // that was not expected when this class was designed. 
            // This class relies on the fact that any UnitConverter cannot
            // change the quantity that is measured by a unit that is converted,
            // i.e. the underlying dimension must remain the same. Please think
            // about whether your new converter can ensure this (please also
            // have a look on how IsCoherent.get is implemented). If so, the
            // converter is safe and you should add it to the assertion below.
            // Otherwise, you should add a runtime check which raises an
            // exception if this converter is used to create a ConvertedUnit.
            Debug.Assert((converter is AdditionUnitConverter)
                || (converter is ConcatenationUnitConverter)
                || (converter is MultiplicationUnitConverter)
                || (converter == UnitConverter.Identity));

            this.converter = converter;
            this.unit = unit;

            /* 
             * ConvertedUnits should be registered by symbol, becuase they are
             * used to add alternate names to derived units via identity
             * conversion.
             */
            this.RegisterSymbol();
        }

        /// <inheridoc />
        public override UnitConverter ConversionToCoherent {
            get {
                if (this.unit.IsCoherent) {
                    return this.converter;
                } else {
                    return new ConcatenationUnitConverter(
                        this.unit.ConversionToCoherent, this.converter);
                }
            }
        }

        /// <inheridoc />
        public override Dimension Dimension {
            get {
                return this.unit.Dimension;
            }
        }

        /// <inheridoc />
        public override bool IsCoherent {
            get {
                return (UnitConverter.IsIdentity(this.converter)
                    && this.unit.IsCoherent);
            }
        }

        #region Public Methods
        /// <inheridoc />
        public bool Equals(ConvertedUnit<Q> other) {
            if (other == null) {
                return false;
            } else if (object.ReferenceEquals(this, other)) {
                return true;
            } else {
                return (this.converter.Equals(other.converter)
                    && this.unit.Equals(other.unit));
            }
        }

        /// <inheridoc />
        public override bool Equals(Unit other) {
            return this.Equals(other as ConvertedUnit<Q>);
        }
        #endregion

        #region Private Fields
        /// <summary>
        /// The <see cref="UnitConverter"/> that is used to transform quantities
        /// measuered in <see cref="unit"/> into this unit.
        /// </summary>
        private readonly UnitConverter converter;

        /// <summary>
        /// The <see cref="Unit"/> that this one is derived from via conversion.
        /// </summary>
        private readonly Unit unit;
        #endregion
    }
}
