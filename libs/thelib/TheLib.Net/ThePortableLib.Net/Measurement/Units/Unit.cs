/// <copyright file="Unit.cs" company="TheLib Team">
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
using The.Measurement.Converters;
using The.Measurement.Dimensions;
using The.Measurement.Quantities;


namespace The.Measurement.Units {

    /// <summary>
    /// Defines the base of all units.
    /// </summary>
    /// <remarks>
    /// <para>This class is not intended to be used by end-users. It therefore
    /// is abstract and has no accessible constructor.</para>
    /// <para>All units that can be processed by TheLib.NET are derived from
    /// this class. There also exists a generic variant of this class, which
    /// provides more type safety regarding the quantities measured by the
    /// unit. This derived class should be used whenever possible.</para>
    /// <para>The design of the whole unit implementation is inspired by the
    /// Java Units of Measurement (http://code.google.com/p/unitsofmeasure/),
    /// but has some changes to reflect the capabilities of C#. E.g., the
    /// implementation at hand supports oerators. This, however, requires some
    /// kind of covariance between "strongly typed" units, which is not possible
    /// in C#. Therefore, this first unit base class is unsafe and allows us
    /// to implement operators.</para>
    /// </remarks>
    public abstract class Unit : IEquatable<Unit>, IFormattable {

        #region Public Operators
        /// <summary>
        /// Computes a <see cref="Unit"/> that is shifted by the given offset.
        /// </summary>
        /// <param name="lhs">The unit to be transformed.</param>
        /// <param name="rhs">The offset that values of the newly created unit
        /// have wrt. <paramref name="lhs"/>.</param>
        /// <returns>A new transformed unit.</returns>
        public static Unit operator +(Unit lhs, double rhs) {
            if (rhs == 0.0) {
                return lhs;
            } else {
                return lhs.Transform(new OffsetUnitConverter(rhs));
            }
        }

        /// <summary>
        /// Computes a <see cref="Unit"/> that is shifted by the given offset.
        /// </summary>
        /// <param name="lhs">The unit to be transformed.</param>
        /// <param name="rhs">The offset that values of the newly created unit
        /// have wrt. <paramref name="lhs"/>.</param>
        /// <returns>A new transformed unit.</returns>
        public static Unit operator -(Unit lhs, double rhs) {
            return (lhs + -rhs);
        }

        /// <summary>
        /// Computes a <see cref="Unit"/> that is scaled by the given factor.
        /// </summary>
        /// <param name="lhs">The scaling factor that values of the newly 
        /// created unit have wrt. <paramref name="rhs"/>.</param>
        /// <param name="rhs">The unit to be transformed.</param>
        /// <returns>A new transformed unit.</returns>
        public static Unit operator *(double lhs, Unit rhs) {
            if (lhs == 1.0) {
                return rhs;
            } else if (Unit.IsLongValue(lhs)) {
                return (((long) lhs) * rhs);
            } else {
                return rhs.Transform(new ScaleUnitConverter(lhs));
            }
        }

        // TODO: Documentation
        public static Unit operator *(long lhs, Unit rhs) {
            if (lhs == 1) {
                return rhs;
            } else {
                return rhs.Transform(new RationalUnitConverter(lhs, 1));
            }
        }

        // TODO: Documentation
        public static Unit operator *(int lhs, Unit rhs) {
            return (((long) lhs) * rhs);
        }

        // TODO: Documentation
        public static Unit operator *(SIPrefix lhs, Unit rhs) {
            return new AlternateUnit(lhs.Symbol + rhs.Symbol, null,
                lhs.Factor * rhs);
        }

        // TODO: Documentation
        public static Unit operator *(Unit lhs, Unit rhs) {
            if (lhs.Equals(SIUnits.One)) {
                return rhs;
            } else if (rhs.Equals(SIUnits.One)) {
                return lhs;
            } else {
                return ProductUnit.FromMultiplication(lhs, rhs);
            }
        }

        // TODO: Documentation
        public static Unit operator /(Unit lhs, double rhs) {
            if (rhs == 1.0) {
                return lhs;
            } else if (Unit.IsLongValue(rhs)) {
                return (lhs / ((long) rhs));
            } else {
                return lhs.Transform(new ScaleUnitConverter(1.0 / rhs));
            }
        }

        // TODO: Documentation
        public static Unit operator /(Unit lhs, long rhs) {
            if (rhs == 1) {
                return lhs;
            } else {
                return lhs.Transform(new RationalUnitConverter(1, rhs));
            }
        }

        // TODO: Documentation
        public static Unit operator /(Unit lhs, int rhs) {
            return (lhs / ((long) rhs));
        }

        // TODO: Documentation
        public static Unit operator /(Unit lhs, Unit rhs) {
            return (lhs * rhs.Invert());
        }
        #endregion

        #region Public Properties
        /// <summary>
        /// Gets the coherent equivalent of this unit.
        /// </summary>
        public abstract Unit CoherentUnit {
            get;
        }

        /// <summary>
        /// Gets a <see cref="The.Measurement.Converters.UnitConverter"/> that 
        /// can convert quantities of this unit into quantities of 
        /// <see cref="Unit.CoherentUnit"/>.
        /// </summary>
        public abstract UnitConverter ConverterToCoherent {
            get;
        }

        /// <summary>
        /// Gets the <see cref="The.Measurement.Dimenions.Dimension"/> measured
        /// by this unit.
        /// </summary>
        public abstract Dimension Dimension {
            get;
        }

        /// <summary>
        /// Gets whether this unit is coherent.
        /// </summary>
        /// <remarks>
        /// The implementation of the base class tests whether this unit and
        /// <see cref="Unit.CoherentUnit"/> are equal. Derived classes may want
        /// to provide a more performant implementation.
        /// </remarks>
        public virtual bool IsCoherent {
            get {
                return this.Equals(this.CoherentUnit);
            }
        }

        /// <summary>
        /// Gets the human-readable, localised name of the unit.
        /// </summary>
        /// <remarks>
        /// The implementation of the base class returns the
        /// <see cref="Unit.Symbol"/>.
        /// </remarks>
        public virtual string Name {
            get {
                return this.Symbol;
            }
        }

        /// <summary>
        /// Gets the symbol of the unit.
        /// </summary>
        /// <remarks>
        /// The symbol should be unique for each unit. Systems of units might
        /// enforce this.
        /// </remarks>
        public abstract string Symbol {
            get;
        }
        #endregion

        #region Public Methods
        // TODO: Documentation
        //public virtual bool IsCompatible(Unit other) {
        //    if (object.ReferenceEquals(this, other) || this.Equals(other)) {
        //        return true;
        //    }

        //    if (this.Dimension.Equals(other.Dimension)) {
        //        return true;
        //    }

        //    // TODO

        //    return false;
        ////PhysicsDimension thisDimension = this.getDimension();
        ////PhysicsDimension thatDimension = ((PhysicsUnit)that).getDimension();
        ////if (thisDimension.equals(thatDimension)) return true;
        ////DimensionalModel model = DimensionalModel.getCurrent(); // Use dimensional analysis model.
        ////return model.getFundamentalDimension(thisDimension).equals(model.getFundamentalDimension(thatDimension));
        //}

        /// <summary>
        /// Indicates whether this object object is equal to another object of
        /// the same type.
        /// </summary>
        /// <remarks>
        /// <para>Equality of two units in this implementation is solely based
        /// on the symbol of the unit, which is assumed to be unique. Subclasses
        /// might want to add additional conditions for units being equal.</para>
        /// </remarks>
        /// <param name="other">The object to be compared.</param>
        /// <returns><c>true</c> if this object and <paramref name="other"/> are
        /// equal, <c>false</c> otherwise.</returns>
        public virtual bool Equals(Unit other) {
            if (other == null) {
                return false;
            } else if (object.ReferenceEquals(this, other)) {
                return true;
            } else {
                return this.Symbol.Equals(other.Symbol);
            }
        }

        /// <inheritdoc />
        public override bool Equals(object obj) {
            return this.Equals(obj as Unit);
        }

        // TODO: Documentation
        public virtual UnitConverter GetConverterTo(Unit other) {
            if (object.ReferenceEquals(this, other) || (this.Equals(other))) {
                return UnitConverter.Identity;
            }

            if (this.CoherentUnit.Equals(other.CoherentUnit)) {
                UnitConverter thisToCoherent = this.ConverterToCoherent;
                UnitConverter otherFromCoherent = this.ConverterToCoherent.Inverse;
                return thisToCoherent.Concatenate(otherFromCoherent);

            } else {
                // TODO
                throw new ArgumentException();
            }

            // TODO
            //@Override
            //public final UnitConverter getConverterTo(Unit<Q> that) throws UnconvertibleException {
            //    if ((this == that) || this.equals(that)) return PhysicsConverter.IDENTITY; // Shortcut.
            //    Unit<Q> thisSystemUnit = this.getSystemUnit();
            //    Unit<Q> thatSystemUnit = that.getSystemUnit();
            //    if (!thisSystemUnit.equals(thatSystemUnit)) return getConverterToAny(that); 
            //    UnitConverter thisToSI= this.getConverterToSI();
            //    UnitConverter thatToSI= that.getConverterTo(thatSystemUnit);
            //    return thatToSI.inverse().concatenate(thisToSI);    
            //}

            //@Override
            //public final UnitConverter getConverterToAny(Unit<?> that) throws IncommensurableException,
            //        UnconvertibleException {
            //    if (!isCompatible(that))
            //        throw new IncommensurableException(this + " is not compatible with " + that);
            //    PhysicsUnit thatPhysics = (PhysicsUnit)that; // Since both units are compatible they must be both physics units.
            //    DimensionalModel model = DimensionalModel.getCurrent();
            //    PhysicsUnit thisSystemUnit = this.getSystemUnit();
            //    UnitConverter thisToDimension = model.getDimensionalTransform(thisSystemUnit.getDimension()).concatenate(this.getConverterToSI());
            //    PhysicsUnit thatSystemUnit = thatPhysics.getSystemUnit();
            //    UnitConverter thatToDimension = model.getDimensionalTransform(thatSystemUnit.getDimension()).concatenate(thatPhysics.getConverterToSI());
            //    return thatToDimension.inverse().concatenate(thisToDimension);
            //}
        }

        /// <inheritdoc />
        public override int GetHashCode() {
            return this.Symbol.GetHashCode();
        }

        /// <inheritdoc />
        public string ToString(string format, IFormatProvider formatProvider) {
            if (string.IsNullOrWhiteSpace(format)) {
                format = "G";
            }
            format = format.ToUpper().Substring(0, 1);

            switch (format) {
                case "G":
                case "S":
                    return this.Symbol ?? base.ToString();

                case "N":
                    return this.Name ?? base.ToString();

                default:
                    throw new FormatException();
            }
        }

        /// <inheritdoc />
        public override string ToString() {
            return this.ToString(null, null);
        }

        #region Transformations
        /// <summary>
        /// Gets the inverse <c><see cref="SIUnits.One"/> / this</c> of the
        /// unit.
        /// </summary>
        /// <returns>The reciprocal unit of this one.</returns>
        public virtual Unit Invert() {
            if (this.Equals(SIUnits.One)) {
                return this;
            } else {
                return ProductUnit.FromDivision(SIUnits.One, this);
            }
        }

        /// <summary>
        /// Gets the <paramref name="n"/>th power of this unit.
        /// </summary>
        /// <param name="n">The exponent.</param>
        /// <returns>The <paramref name="n"/>th power of this unit.</returns>
        public virtual Unit Pow(int n) {
            if (n > 0) {
                // TODO: Does this make sense?
                return (this * this.Pow(n - 1));
            } else if (n < 0) {
                return (SIUnits.One / this.Pow(-n));
            } else {
                return SIUnits.One;
            }
        }

        /// <summary>
        /// Gets the <paramref name="n"/>th root of this unit.
        /// </summary>
        /// <param name="n">The root exponent.</param>
        /// <returns>The <paramref name="n"/>th root of this unit.</returns>
        public virtual Unit Root(int n) {
            if (n < 0) {
                return (SIUnits.One / this.Root(-n));
            } else {
                // Construction of ProductUnit will raise exception if root 
                // is zero.
                return ProductUnit.FromRoot(this, n);
            }
        }

        /// <summary>
        /// Create a unit that is derived by transformation from this one.
        /// </summary>
        /// <param name="transformation"></param>
        /// <returns></returns>
        public Unit Transform(UnitConverter transformation) {
            if (transformation == null) {
                throw new ArgumentNullException("transformation");
            }

            UnitConverter xform = transformation.Concatenate(
                this.ConverterToCoherent);
            return (xform.IsIdentity)
                ? this.CoherentUnit
                : new TransformedUnit(this.CoherentUnit, xform);
        }
        #endregion
        #endregion

        /// <summary>
        /// Determines whether <paramref name="value"/> would fit into a
        /// <see cref="System.Long"/> variable, i.e. whether a cast will not
        /// cut off significant information.
        /// </summary>
        /// <param name="value">The value to be tested.</param>
        /// <returns><c>true</c> if <paramref name="value"/> can be safely
        /// stored as a <see cref="System.Long"/>, <c>false</c>
        /// otherwise.</returns>
        protected static bool IsLongValue(double value) {
            if ((value < long.MinValue) || (value > long.MaxValue)) {
                return false;
            } else {
                return (System.Math.Floor(value) == value);
            }
        }

        #region Protected Internal Constructors
        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        /// <remarks>
        /// <para>End users are not allowed to derive directly from this 
        /// class.</para>
        protected internal Unit() { }
        #endregion
    }


    /// <summary>
    /// The stronly typed variant of <see cref="Unit"/>.
    /// </summary>
    /// <typeparam name="Q"></typeparam>
    public abstract class Unit<Q> : Unit where Q : Quantity<Q> {

        #region Public Operators
        ///// <summary>
        ///// Computes a <see cref="Unit"/> that is shifted by the given offset.
        ///// </summary>
        ///// <param name="lhs">The unit to be transformed.</param>
        ///// <param name="rhs">The offset that values of the newly created unit
        ///// have wrt. <paramref name="lhs"/>.</param>
        ///// <returns>A new transformed unit.</returns>
        //public static Unit<Q> operator +(Unit<Q> lhs, double rhs) {
        //    return new AlternateUnit<Q>((Unit) lhs + rhs);
        //}

        //public static Unit<Q> operator -(Unit<Q> lhs, double rhs) {
        //    return new AlternateUnit<Q>((Unit) lhs - rhs);
        //}

        //public static Unit<Q> operator *(double lhs, Unit<Q> rhs) {
        //    return new AlternateUnit<Q>(lhs * (Unit) rhs);
        //}

        // TODO: Documentation
        public static Unit<Q> operator *(SIPrefix lhs, Unit<Q> rhs) {
            return new AlternateUnit<Q>(lhs.Symbol + rhs.Symbol, null,
                lhs.Factor * rhs);
        }

        //public static Unit<Q> operator /(Unit<Q> lhs, double rhs) {
        //    return new AlternateUnit<Q>((Unit) lhs / rhs);
        //}
        #endregion

        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        /// <remarks>
        /// <para>End users are not allowed to derive directly from this 
        /// class.</para>
        protected internal Unit() : base() { }
    }
}
