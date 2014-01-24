/// <copyright file="ProductUnit.cs" company="TheLib Team">
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
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using The.Measurement.Converters;
using The.Measurement.Dimensions;
using The.Measurement.Quantities;
using SM = System.Math;
using TM = The.Mathematics.MathExtensions;
using The.Utility;


namespace The.Measurement.Units {

    /// <summary>
    /// Represents a unit that is derived from other units by forming the
    /// product of powers of these other units.
    /// </summary>
    public sealed class ProductUnit : Unit, IEquatable<ProductUnit> {

        #region Nested Class Factor
        /// <summary>
        /// A factor that forms the product of a <see cref="ProductUnit"/>.
        /// </summary>
        public sealed class Factor : IEquatable<Factor>, IFormattable {

            #region Constructors
            /// <summary>
            /// Initialises a new instance.
            /// </summary>
            /// <param name="unit">The underlying unit.</param>
            /// <param name="power">The exponent of the power.</param>
            /// <param name="root">The root.</param>
            /// <exception cref="System.ArgumentNullException">If
            /// <paramref name="unit"/> is <c>null</c>.</exception>
            /// <exception cref="System.ArithmeticException">If
            /// <paramref name="root"/> is zero.</exception>
            public Factor(Unit unit, int power, int root) {
                if (unit == null) {
                    throw new ArgumentNullException("unit");
                }
                if (root == 0) {
                    // TODO: Message
                    throw new ArithmeticException();
                }
                this.Power = power;
                this.Root = root;
                this.Unit = unit;
            }
            #endregion

            #region Public Properties
            /// <summary>
            /// Gets the power exponent.
            /// </summary>
            public int Power {
                get;
                private set;
            }

            /// <summary>
            /// Gets the root exponent.
            /// </summary>
            public int Root {
                get;
                private set;
            }

            /// <summary>
            /// Gets the unit.
            /// </summary>
            public Unit Unit {
                get;
                private set;
            }
            #endregion

            #region Public Methods
            /// <inheritdoc />
            public bool Equals(Factor other) {
                if (other == null) {
                    return false;
                } else if (object.ReferenceEquals(this, other)) {
                    return true;
                } else {
                    return ((this.Power == other.Power)
                        && (this.Root == other.Root)
                        && (this.Unit.Equals(other.Unit)));
                }
            }

            /// <inheritdoc />
            public override bool Equals(object obj) {
                return this.Equals(obj as Factor);
            }

            /// <inheritdoc />
            public override int GetHashCode() {
                return (this.Unit.GetHashCode() ^ this.Power.GetHashCode()
                    ^ this.Root.GetHashCode());
            }

            /// <inheritdoc />
            public string ToString(string format, IFormatProvider formatProvider) {
                StringBuilder sb = new StringBuilder();

                sb.Append(Unit.ToString(null, formatProvider));

                if (this.Power != this.Root) {
                    sb.Append("^");

                    if (this.Root != 1) {
                        sb.Append("(");
                    }

                    sb.Append(this.Power.ToString(null, formatProvider));

                    if (this.Root != 1) {
                        sb.Append("/");
                        sb.Append(this.Root.ToString(null, formatProvider));
                        sb.Append(")");
                    }
                }

                return sb.ToString();
            }

            /// <inheritdoc />
            public override string ToString() {
                return this.ToString(null, null);
            }
            #endregion
        }
        #endregion

        #region Factory Methods
        #region From Division
        // TODO: Documentation
        public static Unit FromDivision(Unit lhs, Unit rhs) {
            List<Factor> factors = new List<Factor>();

            if (lhs is ProductUnit) {
                factors.AddRange(((ProductUnit) lhs).factors);
            } else {
                factors.Add(new Factor(lhs, 1, 1));
            }

            if (rhs is ProductUnit) {
                var fs = from f in ((ProductUnit) rhs).factors
                         select new Factor(f.Unit, -f.Power, f.Root);
                factors.AddRange(fs);
            } else {
                factors.Add(new Factor(rhs, -1, 1));
            }

            return ProductUnit.FromFactors(factors);
        }

        // TODO: Documentation
        public static Unit<Q> FromDivision<Q>(string symbol,
                string name, Unit lhs, Unit rhs)
                where Q : Quantity<Q> {
            return new AlternateUnit<Q>(symbol, name,
                ProductUnit.FromDivision(lhs, rhs));
        }
        #endregion

        #region FromFactors
        // TODO: Documentation
        public static Unit FromFactors(IEnumerable<Factor> factors) {
            if (factors == null) {
                throw new ArgumentNullException("factors");
            }

            var fs = from f in factors
                     group f by f.Unit into g
                     let r = g.Aggregate(1, (a, n) => a * n.Root)
                     let p = g.Sum(n => n.Power * (r / n.Root))
                     let gcd = (p != 0) ? TM.GreatestCommonDivisor(SM.Abs(p), r) : 1
                     select new Factor(g.Key, p / gcd, r / gcd);

            if (fs.Count() == 0) {
                // Result would be dimensionless.
                return SIUnits.One;

            } else if ((fs.Count() == 1)
                    && (fs.Single().Power == fs.Single().Root)) {
                // Result would be the single unit.
                return fs.Single().Unit;

            } else {
                return new ProductUnit(fs.ToArray());
            }
        }

        // TODO: Documentation
        public static Unit<Q>FromFactors<Q>(string symbol, string name,
                IEnumerable<Factor> factors)
                where Q : Quantity<Q> {
            return new AlternateUnit<Q>(symbol, name,
                ProductUnit.FromFactors(factors));
        }
        #endregion

        #region FromMultiplication
        // TODO: Documentation
        public static Unit FromMultiplication(Unit lhs, Unit rhs) {
            List<Factor> factors = new List<Factor>();

            if (lhs is ProductUnit) {
                factors.AddRange(((ProductUnit) lhs).factors);
            } else {
                factors.Add(new Factor(lhs, 1, 1));
            }

            if (rhs is ProductUnit) {
                factors.AddRange(((ProductUnit) rhs).factors);
            } else {
                factors.Add(new Factor(rhs, 1, 1));
            }

            return ProductUnit.FromFactors(factors);
        }

        // TODO: Documentation
        public static Unit<Q> FromMultiplication<Q>(string symbol,
                string name, Unit lhs, Unit rhs)
                where Q : Quantity<Q> {
            return new AlternateUnit<Q>(symbol, name,
                ProductUnit.FromMultiplication(lhs, rhs));
        }
        #endregion

        #region FromPower
        // TODO: Documentation
        public static Unit FromPower(Unit unit, int power) {
            ProductUnit pu = unit as ProductUnit;
            if (pu != null) {
                // Flatten wrapped product unit.
                var fs = from f in pu.factors
                         let np = power * f.Power
                         let gcd = TM.GreatestCommonDivisor(SM.Abs(np), f.Root)
                         select new Factor(f.Unit, np / gcd, f.Root / gcd);
                return ProductUnit.FromFactors(fs);

            } else if (power == 0) {
                // Result is dimensionless.
                return SIUnits.One;

            } else if (power == 1) {
                // Result is 'unit' itself.
                return unit;

            } else {
                // Result is power of 'unit'.
                return new ProductUnit(new Factor(unit, power, 1));
            }
        }

        // TODO: Documentation
        public static Unit<Q> FromPower<Q>(string symbol, string name,
                Unit unit, int power)
                where Q : Quantity<Q> {
            return new AlternateUnit<Q>(symbol, name,
                ProductUnit.FromPower(unit, power));
        }
        #endregion

        #region FromReciprocal
        // TODO: Documentation
        public static Unit FromReciprocal(Unit unit) {
            return ProductUnit.FromPower(unit, -1);
        }

        // TODO: Documentation
        public static Unit<Q> FromReciprocal<Q>(string symbol,
                string name, Unit unit)
                where Q : Quantity<Q> {
            return ProductUnit.FromPower<Q>(symbol, name, unit, -1);
        }
        #endregion

        #region FromRoot
        // TODO: Documentation
        public static Unit FromRoot(Unit unit, int root) {
            ProductUnit pu = unit as ProductUnit;
            if (pu != null) {
                var fs = from f in pu.factors
                         let nr = root * f.Root
                         let gcd = TM.GreatestCommonDivisor(SM.Abs(f.Power), nr)
                         select new Factor(f.Unit, f.Power / gcd, nr / gcd);
                return ProductUnit.FromFactors(fs);

            } else if (root == 1) {
                // Result is 'unit' itself.
                return unit;

            } else {
                return new ProductUnit(new Factor(unit, 1, root));
            }
        }

        // TODO: Documentation
        public static Unit<Q> FromRoot<Q>(string symbol, string name,
                Unit unit, int root)
                where Q : Quantity<Q> {
            return new AlternateUnit<Q>(symbol, name,
                ProductUnit.FromRoot(unit, root));
        }
        #endregion
        #endregion

        #region Public Properties
        /// <inheritdoc />
        public override Unit CoherentUnit {
            get {
                if (this.coherentUnit == null) {
                    this.coherentUnit = SIUnits.One;
                    foreach (Factor f in this.factors) {
                        Unit u = f.Unit;
                        u = u.Pow(f.Power);
                        u = u.Root(f.Root);
                        this.coherentUnit *= u;
                    }
                }
                return this.coherentUnit;
            }
        }

        /// <inheritdoc />
        public override UnitConverter ConverterToCoherent {
            get {
                if (this.converterToCoherent == null) {
                    this.converterToCoherent = UnitConverter.Identity;
                    foreach (Factor f in this.factors) {
                        UnitConverter c = f.Unit.ConverterToCoherent;
                        int p = f.Power;

                        if (!c.IsLinear) {
                            // TODO: message that non-linear transform is not possible.
                            throw new InvalidOperationException();
                        }
                        if (f.Root != 1) {
                            // TODO: message that factional exp is not possible.
                            throw new InvalidOperationException();
                        }

                        if (p < 0) {
                            p = -p;
                            c = c.Inverse;
                        }

                        for (int i = 0; i < p; ++i) {
                            this.converterToCoherent = c.Concatenate(
                                this.converterToCoherent);
                        }
                    }
                }
                return this.converterToCoherent;
            }
        }

        /// <inheritdoc />
        public override Dimension Dimension {
            get {
                if (this.dimension== null) {
                    this.dimension = SIDimensions.None;

                    foreach (Factor f in this.factors) {
                        Dimension d = f.Unit.Dimension;
                        d = d.Pow(f.Power);
                        d = d.Root(f.Root);
                        this.dimension *= d;
                    }
                }

                return this.dimension;
            }
        }

        /// <inheritdoc />
        public override string Symbol {
            get {
                if (this.symbol == null) {
                    var relevantFactors = from f in this.factors
                                          where !f.Unit.Equals(SIUnits.One)
                                          select f;
                    this.symbol = string.Join(" * ", relevantFactors);
                }
                return this.symbol;
            }
        }
        #endregion

        #region Public Methods
        /// <inheritdoc />
        public bool Equals(ProductUnit other) {
            if (other == null) {
                return false;
            } else if (object.ReferenceEquals(this, other)) {
                return true;
            } else if (!base.Equals(other)) {
                return false;
            } else {
                return this.factors.SymmetricDifference(
                    other.factors).IsEmpty();
            }
        }

        /// <inheritdoc />
        public override bool Equals(Unit other) {
            return this.Equals(other as ProductUnit);
        }
        #endregion

        #region Constructors
        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        /// <remarks>
        /// <para>The constructor assumes that the factors used for the
        /// definition are correct and simplified. The factory methods
        /// ensure this.</para>
        /// </remarks>
        /// <param name="factors">The factors forming the product.</param>
        private ProductUnit(params Factor[] factors) {
            Debug.Assert(factors != null);
            Debug.Assert(factors.Length > 0);
            Debug.Assert((factors.Length > 1)
                || (factors.Single().Power != factors.Single().Root));
            this.factors = factors;
        }
        #endregion

        #region Private Fields
        /// <summary>
        /// The coherent equivalent of this unit.
        /// </summary>
        /// <remarks>
        /// This field is evaluated lazily.
        /// </remarks>
        private Unit coherentUnit;

        /// <summary>
        /// The converter to the coherent unit.
        /// </summary>
        /// <remarks>
        /// This field is evaluated lazily.
        /// </remarks>
        private UnitConverter converterToCoherent;

        /// <summary>
        /// The dimension measured by the unit.
        /// </summary>
        /// <remarks>
        /// This field is evaluated lazily.
        /// </remarks>
        private Dimension dimension;

        /// <summary>
        /// The factors that define the product.
        /// </summary>
        private readonly Factor[] factors;

        /// <summary>
        /// The symbol of the unit.
        /// </summary>
        /// <remarks>
        /// This field is evaluated lazily.
        /// </remarks>
        private string symbol;
        #endregion
    }
}
