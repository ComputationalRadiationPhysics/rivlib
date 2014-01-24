/// <copyright file="ExponentialUnitConverter.cs" company="TheLib Team">
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

namespace The.Measurement.Converters {

    /// <summary>
    /// A logarithmic converter.
    /// </summary>
    /// <remarks>
    /// Implementation is derived from JScience, see
    /// http://jscience.org/api/org/jscience/physics/amount/package-summary.html#package_description.
    /// </remarks>
    public sealed class LogarithmicUnitConverter : UnitConverter,
            IEquatable<LogarithmicUnitConverter> {

        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        /// <param name="base_">The logarithmic base, e.g.
        /// <see cref="System.Math.E"/>.</param>
        public LogarithmicUnitConverter(double base_) {
            this.base_ = base_;
            this.logOfBase = System.Math.Log(this.base_);
        }

        public double Base {
            get {
                return this.base_;
            }
        }

        /// <inheritdoc />
        public override UnitConverter Inverse {
            get {
                return new ExponentialUnitConverter(this.base_);
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
                return false;
            }
        }

        /// <inheritdoc />
        public override bool IsProportional {
            get {
                return false;
            }
        }

        /// <inheritdoc />
        public override double Convert(double quantity) {
            return (System.Math.Log(quantity) / this.logOfBase);
        }

        /// <inheritdoc />
        public override decimal Convert(decimal quantity) {
            return (decimal) this.Convert((double) quantity);
        }

        /// <inheritdoc />
        public bool Equals(LogarithmicUnitConverter other) {
            if (other == null) {
                return false;
            } else if (object.ReferenceEquals(this, other)) {
                return true;
            } else {
                return this.base_.Equals(other.base_);
            }
        }

        /// <inheritdoc />
        public override bool Equals(UnitConverter other) {
            return this.Equals(other as ExponentialUnitConverter);
        }

        /// <inheritdoc />
        public override int GetHashCode() {
            return (this.GetType().GetHashCode() ^ this.base_.GetHashCode());
        }

        private readonly double base_;
        private readonly double logOfBase;
    }
}
