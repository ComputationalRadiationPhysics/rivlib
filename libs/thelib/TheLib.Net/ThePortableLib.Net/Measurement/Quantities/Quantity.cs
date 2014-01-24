/// <copyright file="Quantity.cs" company="TheLib Team">
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
using The.Measurement.Units;


namespace The.Measurement.Quantities {

    /// <summary>
    /// Represents a property that can exist as a magnitude or multitude of a
    /// unit of measurements.
    /// </summary>
    /// <remarks>
    /// This class does not provide any compile-time checks for compatibility
    /// of the used units. In case of an error, instances of this class will
    /// raise a runtime exception.
    /// </remarks>
    public class Quantity {

        #region Public Constructors
        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        /// <param name="value">The magnitude.</param>
        /// <param name="unit">The unit.</param>
        /// <exception cref="System.ArgumentNullException">If
        /// <paramref name="unit"/> is <c>null</c>.</exception>
        public Quantity(double value, Unit unit) {
            if (unit == null) {
                throw new ArgumentNullException("unit");
            }

            this.Value = value;
            this.Unit = unit;
        }
        #endregion

        #region Public Properties
        /// <summary>
        /// Gets the value of the quantity.
        /// </summary>
        public virtual double Value {
            get;
            protected set;
        }

        /// <summary>
        /// Gets the <see cref="IUnit"/> used to measure the quantity.
        /// </summary>
        public Unit Unit {
            get;
            protected set;
        }
        #endregion

    }

    /// <summary>
    /// Represents a property that can exist as a magnitude or multitude of a
    /// unit of measurements.
    /// </summary>
    /// <typeparam name="Q">The type of the quantity.</typeparam>
    public class Quantity<Q> : Quantity, IComparable<Quantity<Q>>,
            IEquatable<Quantity<Q>> where Q : Quantity<Q> {

        //#region Public Operators
        // TODO
        //public static Quantity<Q> operator +(Quantity<Q> lhs, Quantity<Q> rhs) {
        //}
        //#endregion

        #region Public Constructors
        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        /// <param name="value">The magnitude.</param>
        /// <param name="unit">The unit.</param>
        /// <exception cref="System.ArgumentNullException">If
        /// <paramref name="unit"/> is <c>null</c>.</exception>
        public Quantity(double value, Unit<Q> unit) : base(value, unit) { }
        #endregion

        #region Public Properties
        /// <summary>
        /// Gets the <see cref="IUnit"/> used to measure the quantity.
        /// </summary>
        public new Unit<Q> Unit {
            get {
                return (Unit<Q>) base.Unit;
            }
            protected set {
                base.Unit = value;
            }
        }
        #endregion

        #region Public Methods
        /// <inheritdoc />
        public int CompareTo(Quantity<Q> other) {
            if (other == null) {
                return 1;
            } else {
                double t = this.Unit.ConverterToCoherent.Convert(this.Value);
                double o = this.Unit.ConverterToCoherent.Convert(other.Value);
                return System.Math.Sign(t - o);
            }
        }

        /// <inheritdoc />
        public bool Equals(Quantity<Q> other) {
            return (this.CompareTo(other) == 0);
        }

        /// <inheritdoc />
        public override bool Equals(object obj) {
            return this.Equals(obj as Quantity<Q>);
        }

        /// <inheritdoc />
        public override int GetHashCode() {
            //double v = this.Unit.ConvertToCoherent(this.Value);
            // TODO
            return base.GetHashCode();
        }

        /// <inheritdoc />
        public override string ToString() {
            return this.Value + " [" + this.Unit + "]";
        }
        #endregion
    }
}
