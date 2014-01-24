/// <copyright file="AdditionUnitConverter.cs" company="TheLib Team">
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
    /// Converts the quantities of one unit into another by adding a constant
    /// value.
    /// </summary>
    public sealed class OffsetUnitConverter : UnitConverter,
            IEquatable<OffsetUnitConverter> {

        #region Public Constructors
        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        /// <param name="offset">The amount that is added during conversion.
        /// </param>
        /// <exception cref="System.ArgumentException">If
        /// <paramref name="offset"/> is zero.</exception>
        public OffsetUnitConverter(double offset) {
            if (offset == 0.0) {
                throw new ArgumentException(
                    Properties.Resources.ErrorConverterOffsetIdentity,
                    "summand");
            }
            this.offset = offset;
        }
        #endregion

        #region Public Properties
        /// <inheritdoc />
        public override UnitConverter Inverse {
            get {
                if (this.inverse == null) {
                    this.inverse = new OffsetUnitConverter(-this.offset);
                }
                return this.inverse;
            }
        }


        /// <inheritdoc />
        public override bool IsIdentity {
            get {
                Debug.Assert(this.offset != 0.0);
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
                return false;
            }
        }

        /// <summary>
        /// Gets the offset that quantities are shifted by.
        /// </summary>
        public double Offset {
            get {
                return this.offset;
            }
        }
        #endregion

        #region Public Methods
        /// <inheritdoc />
        public override UnitConverter Concatenate(UnitConverter next) {
            OffsetUnitConverter n = next as OffsetUnitConverter;
            if (n != null) {
                // Concatenation can be simplified.
                double o = this.offset + n.offset;
                return (o == 0.0) ? UnitConverter.Identity
                    : new OffsetUnitConverter(o);
            } else {
                return base.Concatenate(next);
            }
        }

        /// <inheritdoc />
        public override double Convert(double quantity) {
            return (quantity + this.offset);
        }

        /// <inheritdoc />
        public override decimal Convert(decimal quantity) {
            return (quantity + (decimal) this.offset);
        }

        /// <inheritdoc />
        public bool Equals(OffsetUnitConverter other) {
            if (other == null) {
                return false;
            } else if (object.ReferenceEquals(this, other)) {
                return true;
            } else {
                return (this.offset == other.offset);
            }
        }

        /// <inheritdoc />
        public override bool Equals(UnitConverter other) {
            return this.Equals(other as OffsetUnitConverter);
        }

        /// <inheritdoc />
        public override int GetHashCode() {
            return (this.GetType().GetHashCode() ^ this.offset.GetHashCode());
        }
        #endregion

        #region Private Fields
        /// <summary>
        /// The inverse converter.
        /// </summary>
        /// <remarks>
        /// This attribute is computed lazily.
        /// </remarks>
        private OffsetUnitConverter inverse;

        /// <summary>
        /// The value that is added during conversion.
        /// </summary>
        private readonly double offset;
        #endregion
    }
}
