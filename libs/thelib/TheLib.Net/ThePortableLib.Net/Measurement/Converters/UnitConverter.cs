/// <copyright file="UnitConverter.cs" company="TheLib Team">
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
using System.Runtime.Serialization;


namespace The.Measurement.Converters {

    /// <summary>
    /// Defines the interface for converters between units and provides some
    /// shared functionality of converters, including the
    /// <see cref="UnitConverter.Identity"/> converter, which does nothing.
    /// </summary>
    [DataContract]
    [KnownType(typeof(ExponentialUnitConverter))]
    [KnownType(typeof(LogarithmicUnitConverter))]
    [KnownType(typeof(OffsetUnitConverter))]
    [KnownType(typeof(RationalUnitConverter))]
    [KnownType(typeof(ScaleUnitConverter))]
    [KnownType(typeof(UnitConverter.CompoundUnitConverter))]
    [KnownType(typeof(UnitConverter.IdentityUnitConverter))]
    public abstract class UnitConverter : IEquatable<UnitConverter> {

        #region Class Constructor
        /// <summary>
        /// Initialises the class.
        /// </summary>
        /// <remarks>
        /// The static constructor is guaranteed to be thread-safe according
        /// to http://msdn.microsoft.com/en-us/library/aa645612(VS.71).aspx.
        /// </remarks>
        static UnitConverter() {
            UnitConverter.Identity = new IdentityUnitConverter();
        }
        #endregion

        //#region Public Operators
        ///// <summary>
        ///// Constructs a new <see cref="UnitConverter"/> that represents the
        ///// concatenation of <paramref name="lhs"/> and <paramref name="rhs"/>
        ///// with <paramref name="lhs"/> being executed first.
        ///// </summary>
        ///// <param name="lhs"></param>
        ///// <param name="rhs"></param>
        ///// <returns></returns>
        //public static UnitConverter operator +(UnitConverter lhs,
        //        UnitConverter rhs) {
        //    return lhs.Concatenate(rhs);
        //}
        //#endregion

        #region Public Class Fields
        /// <summary>
        /// The identity converter, which does nothing.
        /// </summary>
        public static readonly UnitConverter Identity;
        #endregion

        #region Public Properties
        /// <summary>
        /// Gets a converter that inverts the conversion performed by
        /// this converter.
        /// </summary>
        /// <remarks>
        /// For a valid magnitude <c>m</c>, the following condition must
        /// hold (within the numeric accuracy of the underlying data type):
        /// <c>m == this.Inverse.Convert(this.Convert(m))</c>.
        /// </remarks>
        public abstract UnitConverter Inverse {
            get;
        }

        ///// <summary>
        ///// Gets whether this converter performs a coherent conversion, i.e.
        ///// does not introduce any constant except for one.
        ///// </summary>
        //bool IsCoherent {
        //    get;
        //}

        /// <summary>
        /// Gets whether the converter performs no conversion.
        /// </summary>
        /// <remarks>
        /// The default implementation checks whether this converter and
        /// <see cref="UnitConverter.Identity"/> are equal. Subclasses might
        /// want to provide a more performant implementation.
        /// </remarks>
        public virtual bool IsIdentity {
            get {
                return this.Equals(UnitConverter.Identity);
            }
        }

        /// <summary>
        /// Gets whether this converter performs a linear conversion.
        /// </summary>
        public abstract bool IsLinear {
            get;
        }

        /// <summary>
        /// Gets whether this converter performs a proportional conversion.
        /// </summary>
        public abstract bool IsProportional {
            get;
        }
        #endregion

        #region Public Methods
        /// <summary>
        /// Create a converter that represents a concatenation of this converter
        /// and <paramref name="next"/>.
        /// </summary>
        /// <param name="next">The converter that is to be executed after this
        /// one.</param>
        /// <returns>A converter representing the concatenated conversion.
        /// </returns>
        public virtual UnitConverter Concatenate(UnitConverter next) {
            if (this.Equals(UnitConverter.Identity)) {
                return next;
            } else if (next.Equals(UnitConverter.Identity)) {
                return this;
            } else {
                return new CompoundUnitConverter(this, next);
            }
        }

        /// <summary>
        /// Convert a given value.
        /// </summary>
        /// <param name="value">A valid quantity to be converted.</param>
        /// <returns>The converted value.</returns>
        public abstract double Convert(double quantity);

        /// <summary>
        /// Convert a given value.
        /// </summary>
        /// <param name="value">A valid quantity to be converted.</param>
        /// <returns>The converted value.</returns>
        public abstract decimal Convert(decimal quantity);

        /// <inheritdoc />
        public abstract bool Equals(UnitConverter other);
        #endregion

        #region Protected Internal Constructors
        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        /// <remarks>
        /// The visibility of the default constructor ensures that only this
        /// assembly provides converters. This way, we can ensure that all
        /// assumptions made (e.g. a converter never yielding an identity
        /// conversion) hold.
        /// </remarks>
        internal protected UnitConverter() { }
        #endregion

        #region Nested Class CompoundConverter
        [DataContract]
        private sealed class CompoundUnitConverter : UnitConverter,
                IEquatable<CompoundUnitConverter> {

            public CompoundUnitConverter(UnitConverter first,
                    UnitConverter second) {
                if (first == null) {
                    throw new ArgumentNullException("first");
                }
                if (second == null) {
                    throw new ArgumentNullException("second");
                }
                this.first = first;
                this.second = second;
            }

            /// <inheritdoc />
            public override UnitConverter Inverse {
                get {
                    if (this.inverse == null) {
                        this.inverse = new CompoundUnitConverter(
                            this.second.Inverse, this.first.Inverse);
                    }
                    return this.inverse;
                }
            }

            /// <inheritdoc />
            public override bool IsIdentity {
                get {
                    return (this.first.IsIdentity && this.second.IsIdentity);
                }
            }

            /// <inheritdoc />
            public override bool IsLinear {
                get {
                    return (this.first.IsLinear && this.second.IsLinear);
                }
            }

            /// <inheritdoc />
            public override bool IsProportional {
                get {
                    return (this.first.IsProportional 
                        && this.second.IsProportional);
                }
            }

            /// <inheritdoc />
            public override double Convert(double quantity) {
                return this.second.Convert(this.first.Convert(quantity));
            }

            /// <inheritdoc />
            public override decimal Convert(decimal quantity) {
                return this.second.Convert(this.first.Convert(quantity));
            }

            /// <inheritdoc />
            public bool Equals(CompoundUnitConverter other) {
                if (other == null) {
                    return false;
                } else if (object.ReferenceEquals(this, other)) {
                    return true;
                } else {
                    return (this.first.Equals(other.first)
                        && this.second.Equals(other.second));
                }
            }

            /// <inheritdoc />
            public override bool Equals(UnitConverter other) {
                return this.Equals(other as CompoundUnitConverter);
            }

            /// <inheritdoc />
            public override int GetHashCode() {
                return (this.first.GetHashCode() + this.second.GetHashCode());
            }

            #region Private Fields
            /// <summary>
            /// The inverse converter.
            /// </summary>
            /// <remarks>
            /// This attribute is computed lazily.
            /// </remarks>
            [IgnoreDataMember]
            private CompoundUnitConverter inverse;

            /// <summary>
            /// The conversion to be performed first.
            /// </summary>
            [DataMember]
            private readonly UnitConverter first;

            /// <summary>
            /// The conversion to be performed second.
            /// </summary>
            [DataMember]
            private readonly UnitConverter second;
            #endregion
        }
        #endregion

        #region Nested Class IdentityConverter
        /// <summary>
        /// The identity converter which does nothing.
        /// </summary>
        [DataContract]
        private sealed class IdentityUnitConverter : UnitConverter,
                IEquatable<IdentityUnitConverter> {

            /// <inheritdoc />
            public override UnitConverter Inverse {
                get {
                    return this;
                }
            }

            /// <inheritdoc />
            public override bool IsIdentity {
                get {
                    return true;
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
                return next;
            }

            /// <inheritdoc />
            public override double Convert(double quantity) {
                return quantity;
            }

            /// <inheritdoc />
            public override decimal Convert(decimal quantity) {
                return quantity;
            }

            /// <inheritdoc />
            public bool Equals(IdentityUnitConverter other) {
                if (other == null) {
                    return false;
                } else {
                    return true;
                }
            }

            /// <inheritdoc />
            public override bool Equals(UnitConverter other) {
                return this.Equals(other as IdentityUnitConverter);
            }

            /// <inheritdoc />
            public override int GetHashCode() {
                return this.GetType().GetHashCode();
            }
        }
        #endregion
    }
}
