/// <copyright file="BaseUnit.cs" company="TheLib Team">
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
using The.Measurement.Converters;
using The.Measurement.Dimensions;
using The.Measurement.Quantities;


namespace The.Measurement.Units {

    /// <summary>
    /// This class represents a coherent base unit for the given quantity.
    /// </summary>
    /// <remarks>
    /// <para><see cref="BaseUnit"/>s are the building blocks of the whole
    /// unit system. The implementation provides the well-known base units of
    /// the Système international d'unités. Users are not allowed to provide
    /// their own base units.</para>
    /// <para>All base units are strongly typed via generics. It makes no sense
    /// to allow for covariance here, because this would break the whole idea
    /// of type checking.</para>
    /// <para>All <see cref="BaseUnit"/>s are registered in
    /// <see cref="Unit.All"/> and must have a unique symbol.</para>
    /// </remarks>
    /// <typeparam name="Q">The quantity that is measured by this unit.
    /// </typeparam>
    public sealed class BaseUnit<Q> : Unit<Q> where Q : Quantity<Q> {

        #region Public Properties
        /// <inheritdoc />
        public override Unit CoherentUnit {
            get {
                return this;
            }
        }

        /// <inheritdoc />
        public override UnitConverter ConverterToCoherent {
            get {
                return UnitConverter.Identity;
            }
        }

        /// <inheritdoc />
        public override Dimension Dimension {
            get {
                return this.dimension;
            }
        }

        /// <inheritdoc />
        public override bool IsCoherent {
            get {
                return true;
            }
        }

        public override string Name {
            get {
                return this.name;
            }
        }

        /// <inheritdoc />
        public override string Symbol {
            get {
                return this.symbol;
            }
        }
        #endregion

        #region Internal Constructors
        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        /// <remarks>
        /// <para>All base units are registered in the symbol dictionary of
        /// <see cref="Unit"/>. Therefore, the <paramref name="symbol"/>
        /// parameter must be unique.</para>
        /// </remarks>
        /// <param name="symbol">The symbol of the unit. This must be unique.
        /// </param>
        /// <param name="name">The localised name of the unit.</param>
        /// <param name="dimension">The dimension the unit measures</param>
        /// <exception cref="ArgumentNullException">If the
        /// <paramref name="symbol"/> or the <paramref name="dimension"/> are
        /// <c>null</c>.</exception>
        internal BaseUnit(string symbol, string name, Dimension dimension) {
            if (string.IsNullOrWhiteSpace(symbol)) {
                throw new ArgumentNullException("symbol");
            }
            if (dimension == null) {
                throw new ArgumentNullException("dimension");
            }

            this.dimension = dimension;
            this.name = name ?? symbol;
            this.symbol = symbol;
        }
        #endregion

        #region Private Fields
        /// <summary>
        /// Backing field for <see cref="Dimension"/> property.
        /// </summary>
        private readonly Dimension dimension;

        /// <summary>
        /// Backing field for <see cref="Name"/> property.
        /// </summary>
        private readonly string name;

        /// <summary>
        /// Backing field for <see cref="Symbol"/> property.
        /// </summary>
        private readonly string symbol;
        #endregion
    }
}
