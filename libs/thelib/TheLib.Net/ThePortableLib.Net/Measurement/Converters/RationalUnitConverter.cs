/// <copyright file="RationalUnitConverter.cs" company="TheLib Team">
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
using The.Mathematics;


namespace The.Measurement.Converters {

    /// <summary>
    /// 
    /// </summary>
    /// <remarks>
    /// Implementation is derived from JScience, see
    /// http://jscience.org/api/org/jscience/physics/amount/package-summary.html#package_description.
    /// </remarks>
    public sealed class RationalUnitConverter : UnitConverter,
            IEquatable<RationalUnitConverter> {

        public RationalUnitConverter(long dividend, long divisor) {
            if (dividend == divisor) {
                // TODO: message
                throw new ArgumentException();
            }
            if (divisor == 0) {
                // TODO: message
                throw new ArgumentException();
            }
            this.dividend = dividend;
            this.divisor = divisor;
        }

        public long Dividend {
            get {
                return this.dividend;
            }
        }

        public long Divisor {
            get {
                return this.divisor;
            }
        }

        /// <inheritdoc />
        public override UnitConverter Inverse {
            get {
                if (this.inverse == null) {
                    long s = System.Math.Sign(this.Dividend);
                    this.inverse = new RationalUnitConverter(s * this.Divisor,
                        s * this.Dividend);
                }
                return this.inverse;
            }
        }

        /// <inheritdoc />
        public override bool IsIdentity {
            get {
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

        /// <inheritdoc />
        public override UnitConverter Concatenate(UnitConverter next) {
            RationalUnitConverter n = next as RationalUnitConverter;
            if (n != null) {
                // Concatenation can be simplified.
                long newDividend = this.Dividend * n.Dividend;
                long newDivisor = this.Divisor * n.Divisor;
                long gcd = MathExtensions.GreatestCommonDivisor(newDividend, newDivisor);
                newDividend /= gcd;
                newDivisor /= gcd;

                if ((newDividend == 1) && (newDivisor == 1)) {
                    return UnitConverter.Identity;
                } else {
                    return new RationalUnitConverter(newDividend, newDivisor);
                }

            } else {
                return base.Concatenate(next);
            }
        }

        /// <inheritdoc />
        public override double Convert(double quantity) {
            return ((double) this.dividend * quantity) 
                / (double) this.divisor;
        }

        /// <inheritdoc />
        public override decimal Convert(decimal quantity) {
            return ((decimal) this.dividend * quantity)
                / (decimal) this.divisor;
        }

        /// <inheritdoc />
        public bool Equals(RationalUnitConverter other) {
            if (other == null) {
                return false;
            } else if (object.ReferenceEquals(this, other)) {
                return true;
            } else {
                return (this.dividend.Equals(other.dividend)
                    && this.divisor.Equals(other.divisor));
            }
        }

        /// <inheritdoc />
        public override bool Equals(UnitConverter other) {
            return this.Equals(other as RationalUnitConverter);
        }

        /// <inheritdoc />
        public override int GetHashCode() {
            return (this.GetType().GetHashCode() ^ (this.dividend.GetHashCode()
                + this.divisor.GetHashCode()));
        }

        private readonly long dividend;
        private readonly long divisor;

        /// <summary>
        /// The inverse converter.
        /// </summary>
        /// <remarks>
        /// This attribute is computed lazily.
        /// </remarks>
        private RationalUnitConverter inverse;
    }
}
