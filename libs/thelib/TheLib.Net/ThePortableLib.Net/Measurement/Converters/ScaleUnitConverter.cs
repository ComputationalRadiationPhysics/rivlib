/// <copyright file="MultiplicationUnitConverter.cs" company="TheLib Team">
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


namespace The.Measurement.Converters {

    /// <summary>
    /// A converter that derives a new value by multiplying an existing unit
    /// with a factor.
    /// </summary>
    /// <remarks>
    /// <para>This converter is e.g. used for computing the results of SI 
    /// prefixes.</para>
    /// </remarks>
    public sealed class ScaleUnitConverter : UnitConverter,
            IEquatable<ScaleUnitConverter> {

        #region Public Constructors
        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        /// <param name="factor">The factor with which the quantity to be
        /// converted should be multiplied. This factor must be meaningful, i.e.
        /// not one, which would result in an identity conversion.</param>
        /// <exception cref="System.ArgumentException">If
        /// <paramref name="factor"/> is one.</exception>
        public ScaleUnitConverter(double factor) {
            if (factor == 1.0) {
                throw new ArgumentException(
                    Properties.Resources.ErrorConverterScaleIdentity,
                    "factor");
            }
            this.factor = factor;
        }
        #endregion

        #region Public Properties
        /// <summary>
        /// Gets the conversion factor that quantities are multiplied with.
        /// </summary>
        public double Factor {
            get {
                return this.factor;
            }
        }

        /// <inheritdoc />
        public override UnitConverter Inverse {
            get {
                if (this.inverse == null) {
                    this.inverse = new ScaleUnitConverter(1.0 / this.factor);
                }
                return this.inverse;
            }
        }

        /// <inheritdoc />
        public override bool IsIdentity {
            get {
                Debug.Assert(this.factor != 1.0);
                return false;
            }
        }

        /// <inheritdoc />
        public override bool IsLinear {
            get {
                return true;
            }
        }

        /// <inheritdoc />
        public override bool IsProportional {
            get {
                return true;
            }
        }
        #endregion

        #region Public Methods
        /// <inheritdoc />
        public override UnitConverter Concatenate(UnitConverter next) {
            ScaleUnitConverter n = next as ScaleUnitConverter;
            if (n != null) {
                // Concatenation can be simplified.
                double f = this.factor * n.factor;
                return (f == 1.0) ? UnitConverter.Identity
                    : new ScaleUnitConverter(f);
            } else {
                return base.Concatenate(next);
            }
        }

        /// <inheritdoc />
        public override double Convert(double quantity) {
            return (this.factor * quantity);
        }

        /// <inheritdoc />
        public override decimal Convert(decimal quantity) {
            return ((decimal) this.factor * quantity);
        }

        /// <inheritdoc />
        public bool Equals(ScaleUnitConverter other) {
            if (other == null) {
                return false;
            } else if (object.ReferenceEquals(this, other)) {
                return true;
            } else {
                return (this.factor == other.factor);
            }
        }

        /// <inheritdoc />
        public override bool Equals(UnitConverter other) {
            return this.Equals(other as ScaleUnitConverter);
        }

        /// <inheritdoc />
        public override int GetHashCode() {
            return (this.GetType().GetHashCode() ^ this.factor.GetHashCode());
        }
        #endregion

        #region Private Fields
        /// <summary>
        /// The factor that quantities are multiplied with during conversion.
        /// </summary>
        private readonly double factor;

        /// <summary>
        /// The inverse converter.
        /// </summary>
        /// <remarks>
        /// This attribute is computed lazily.
        /// </remarks>
        private ScaleUnitConverter inverse;
        #endregion
    }
}
