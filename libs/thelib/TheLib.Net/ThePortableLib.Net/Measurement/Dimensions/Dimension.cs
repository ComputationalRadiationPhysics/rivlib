/// <copyright file="Dimension.cs" company="TheLib Team">
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
using System.Text;
using The.Measurement.Units;


namespace The.Measurement.Dimensions {

    /// <summary>
    /// Represents the dimension that a unit measures.
    /// </summary>
    /// <remarks>
    /// Predefined dimensions of the Système international d'unités can be found
    /// in <see cref="The.Measurement.Units.SIDimensions"/>.
    /// </remarks>
    public sealed class Dimension : IEquatable<Dimension> {

        #region Public Operators
        /// <summary>
        /// Forms the product of two <see cref="Dimension"/>s.
        /// </summary>
        /// <param name="lhs">The left hand side operand.</param>
        /// <param name="rhs">The right hand side operand.</param>
        /// <returns>The product of the to dimensions.</returns>
        public static Dimension operator *(Dimension lhs, Dimension rhs) {
            return new Dimension(lhs.definition * rhs.definition);
        }
        #endregion

        /// <summary>
        /// Gets the "dimensionless" dimension.
        /// </summary>
        public static Dimension None {
            get {
                return SIDimensions.None;
            }
        }

        #region Public Constructors
        /// <summary>
        /// Initialises a new dimension.
        /// </summary>
        /// <param name="symbol">The symbol of the dimension.</param>
        public Dimension(char symbol) {
            StringBuilder sb = new StringBuilder();
            sb.Append('[');
            sb.Append(symbol);
            sb.Append(']');
            this.definition = new PseudoUnit(sb.ToString());
        }
        #endregion

        #region Public Properties
        /// <summary>
        /// Gets the symbol of the dimension.
        /// </summary>
        public string Symbol {
            get {
                return this.definition.Symbol;
            }
        }
        #endregion

        #region Public Methods
        /// <inheritdoc />
        public bool Equals(Dimension other) {
            if (other == null) {
                return false;
            } else if (object.ReferenceEquals(this, other)) {
                return true;
            } else {
                return this.definition.Equals(other.definition);
            }
        }

        /// <inheritdoc />
        public override bool Equals(object obj) {
            return this.Equals(obj as Dimension);
        }

        /// <inheritdoc />
        public override int GetHashCode() {
            return (this.definition.GetHashCode()
                ^ this.GetType().GetHashCode());
        }

        /// <inheritdoc />
        public override string ToString() {
            return this.definition.ToString();
        }

        #region Transformations
        /// <summary>
        /// Gets the inverse of this dimension.
        /// </summary>
        /// <returns>The reciprocal dimension of this one.</returns>
        public Dimension Invert() {
            return new Dimension(this.definition.Invert());
        }

        /// <summary>
        /// Gets the <paramref name="n"/>th power of this dimension.
        /// </summary>
        /// <param name="n">The exponent.</param>
        /// <returns>The <paramref name="n"/>th power of this dimension.
        /// </returns>
        public Dimension Pow(int n) {
            return new Dimension(this.definition.Pow(n));
        }

        /// <summary>
        /// Gets the <paramref name="n"/>th root of this dimension.
        /// </summary>
        /// <param name="n">The root exponent.</param>
        /// <returns>The <paramref name="n"/>th root of this dimension.
        /// </returns>
        public Dimension Root(int n) {
            return new Dimension(this.definition.Root(n));
        }
        #endregion
        #endregion

        #region Nested Class PseudoUnit
        /// <summary>
        /// This class is used to provide the building blocks of the definition
        /// of <see cref="Dimension"/>s.
        /// </summary>
        private sealed class PseudoUnit : Unit {

            /// <summary>
            /// Initialises a new instance.
            /// </summary>
            /// <param name="symbol">The symbol of the dimension.</param>
            public PseudoUnit(string symbol) {
                Debug.Assert(!string.IsNullOrWhiteSpace(symbol));
                this.symbol = symbol;
            }

            /// <inheritdoc />
            public override Unit CoherentUnit {
                get {
                    throw new NotSupportedException();
                }
            }

            /// <inheritdoc />
            public override Converters.UnitConverter ConverterToCoherent {
                get {
                    throw new NotSupportedException(); 
                }
            }

            /// <inheritdoc />
            public override Dimension Dimension {
                get {
                    throw new NotSupportedException();
                }
            }

            /// <inheritdoc />
            public override bool IsCoherent {
                get {
                    throw new NotSupportedException();
                }
            }

            /// <inheritdoc />
            public override string Symbol {
                get {
                    return this.symbol;
                }
            }

            private readonly string symbol;
        };
        #endregion

        #region Private Constructors
        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        /// <param name="definition">The pseudo unit defining the dimension.
        /// </param>
        private Dimension(Unit definition) {
            Debug.Assert(definition != null);
            this.definition = definition;
        }
        #endregion

        #region Private Fields
        /// <summary>
        /// Each dimension is defined by a pseudo unit.
        /// </summary>
        /// <remarks>
        /// <para>This unit is used in order to allow all operations that
        /// are possible on units also possible on dimensions. This enables us
        /// to track the dimension of a derived unit the same way that the
        /// derived unit is defined.</para>
        /// </remarks>
        private readonly Unit definition;
        #endregion
    }
}
