/// <copyright file="TransformedUnit.cs" company="TheLib Team">
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


namespace The.Measurement.Units {

    /// <summary>
    /// Represents a <see cref="Unit"/> that is derived from another
    /// <see cref="Unit"/> by transformation using a
    /// <see cref="The.Measurement.Converters.UnitConverter"/>.
    /// </summary>
    /// <remarks>
    /// <para>Instances of <see cref="TransformedUnit"/> are e.g. used to
    /// define degrees Celsius, which is derived via a non-proportional
    /// transformation from Kelvin.</para>
    /// </remarks>
    public sealed class TransformedUnit : Unit, IEquatable<TransformedUnit> {

        // TODO: Documentation
        public TransformedUnit(Unit parentUnit, 
                UnitConverter transformationToParentUnit) {
            if (parentUnit == null) {
                throw new ArgumentNullException("unit");
            }
            if (transformationToParentUnit == null) {
                throw new ArgumentNullException("transformation");
            }
            // TODO: Assert 'transformation' != identity?

            //if (!parentUnit.isSI())
            //    throw new IllegalArgumentException("The parent unit: " + parentUnit
            //            + " is not a system unit");

            this.parentUnit = parentUnit;
            this.transformationToParentUnit = transformationToParentUnit;
        }

        #region Public Properties
        /// <inheritdoc />
        public override Unit CoherentUnit {
            get {
                return this.parentUnit.CoherentUnit;
            }
        }

        /// <inheritdoc />
        public override UnitConverter ConverterToCoherent {
            get {
                // Convert to parent unit first and let parent unit handle
                // conversion to coherent unit recursively.
                return this.transformationToParentUnit.Concatenate(
                    this.parentUnit.ConverterToCoherent);
            }
        }

        /// <inheritdoc />
        public override Dimensions.Dimension Dimension {
            get {
                return this.parentUnit.Dimension;
            }
        }

        /// <summary>
        /// Gets the unit that this one is derived from.
        /// </summary>
        public Unit ParentUnit {
            get {
                return this.parentUnit;
            }
        }

        /// <inheritdoc />
        public override string Symbol {
            get {
                return null;
            }
        }

        /// <summary>
        /// Gets the converter that converts values given in this
        /// unit to values given in <see cref="ParentUnit"/>.
        /// </summary>
        public UnitConverter TransformationToParentUnit {
            get {
                return this.transformationToParentUnit;
            }
        }
        #endregion

        #region Public Methods
        /// <inheritdoc />
        public bool Equals(TransformedUnit other) {
            if (other == null) {
                return false;
            } else if (object.ReferenceEquals(this, other)) {
                return true;
            } else {
                return (this.transformationToParentUnit.Equals(
                    other.transformationToParentUnit)
                    && this.parentUnit.Equals(other.parentUnit));
            }
        }

        /// <inheritdoc />
        public override bool Equals(Unit other) {
            return this.Equals(other as TransformedUnit);
        }

        /// <inheritdoc />
        public override bool Equals(object obj) {
            return this.Equals(obj as TransformedUnit);
        }

        /// <inheritdoc />
        public override int GetHashCode() {
            return (this.parentUnit.GetHashCode()
                + this.transformationToParentUnit.GetHashCode());
        }
        #endregion

        #region Private Fields
        /// <summary>
        /// Stores the transformation to parent unit values.
        /// </summary>
        private readonly UnitConverter transformationToParentUnit;

        /// <summary>
        /// The parent unit that this one is derived from.
        /// </summary>
        private readonly Unit parentUnit;
        #endregion
    }
}
