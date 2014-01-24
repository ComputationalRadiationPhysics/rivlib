/// <copyright file="UnitSystem.cs" company="TheLib Team">
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
using System.Linq;
using System.Reflection;


namespace The.Measurement.Units {

    /// <summary>
    /// Base class for classes that representa system of units and provide 
    /// static instances of built-in units.
    /// </summary>
    /// <typeparam name="D">The type of the derived class.</typeparam>
    public abstract class UnitSystem<D> where D : UnitSystem<D> {

        // TODO: Dimension stuff is missing

        #region Class Constructor
        /// <summary>
        /// Initialises the class.
        /// </summary>
        /// <remarks>
        /// The static constructor is guaranteed to be thread-safe according
        /// to http://msdn.microsoft.com/en-us/library/aa645612(VS.71).aspx.
        /// </remarks>
        static UnitSystem() {
            UnitSystem<D>.SYMBOL_TO_UNIT = new Dictionary<string, Unit>();
            UnitSystem<D>.collectUnits();
        }
        #endregion

        #region Public Properties
        /// <summary>
        /// Gets all known <see cref="Unit"/>s.
        /// </summary>
        public static IEnumerable<Unit> All {
            get {
                return UnitSystem<D>.SYMBOL_TO_UNIT.Values;
            }
        }
        #endregion

        #region Public Methods
        /// <summary>
        /// Answer whether the given unit is part of the system of units.
        /// </summary>
        /// <param name="unit">The unit to be searched.</param>
        /// <returns><c>true</c> if the unit is part of the system of units,
        /// <c>false</c> otherwise.</returns>
        public static bool Contains(Unit unit) {
            Unit u;
            if (UnitSystem<D>.SYMBOL_TO_UNIT.TryGetValue(unit.Symbol, out u)) {
                return (object.ReferenceEquals(u, unit) || (u.Equals(unit)));
            } else {
                return false;
            }
        }

        /// <summary>
        /// Retrieves the unit with the given symbol from the known units of the
        /// system.
        /// </summary>
        /// <param name="symbol">The symbol to be searched.</param>
        /// <returns>The unit with the given symbol or <c>null</c> if no such
        /// unit could be found.</returns>
        public static Unit FromSymbol(string symbol) {
            try {
                lock (UnitSystem<D>.SYMBOL_TO_UNIT) {
                    return UnitSystem<D>.SYMBOL_TO_UNIT[symbol ?? string.Empty];
                }
            } catch {
                return null;
            }
        }
        #endregion

        #region Private Methods
        /// <summary>
        /// Collects all units from the derived class into
        /// <see cref="UnitSystem.units"/>.
        /// </summary>
        /// <remarks>
        /// <para>This method retrieves all public and static fields and 
        /// properties in the derived class that are derived from 
        /// <see cref="Unit"/> and adds them to the 
        /// <see cref="SYMBOL_TO_UNIT"/> dictionary. The symbol used is (in this
        /// order): The case-insensitive UCUM symbol if one was attached to the
        /// instance; the symbol of the unit itself.</para>
        /// <para>This method is not thread-safe as it is intended to be
        /// called only from class constructors, which are guaranteed to
        /// be thread-safe themselves.</para>
        /// </remarks>
        /// <exception cref="InvalidOperationException">If the derived class
        /// contains at least two units with the same symbol.</exception>
        private static void collectUnits() {
            var props = from u in typeof(D).GetProperties(BindingFlags.Public | BindingFlags.Static)
                        where typeof(Unit).IsAssignableFrom(u.PropertyType)
                        let v = u.GetValue(null, null) as Unit
                        where (v != null)
                        let a = UcumSymbolAttribute.Get(u)
                        select new { Unit = v, Symbol = a };

            foreach (var u in props) {
                if (u.Symbol != null) {
                    UnitSystem<D>.registerUnit(u.Symbol.CaseSensitiveSymbol,
                        u.Unit);
                } else {
                    UnitSystem<D>.registerUnit(u.Unit);
                }
            }

            var fields = from u in typeof(D).GetFields(BindingFlags.Public | BindingFlags.Static)
                         where typeof(Unit).IsAssignableFrom(u.FieldType)
                         let v = u.GetValue(null) as Unit
                         where (v != null)
                         let a = UcumSymbolAttribute.Get(u)
                         select new { Unit = v, Symbol = a };

            foreach (var u in fields) {
                if (u.Symbol != null) {
                    UnitSystem<D>.registerUnit(u.Symbol.CaseSensitiveSymbol,
                        u.Unit);
                } else {
                    UnitSystem<D>.registerUnit(u.Unit);
                }
            }
        }

        private static void registerUnit(string symbol, Unit unit) {
            try {
                UnitSystem<D>.SYMBOL_TO_UNIT.Add(symbol, unit);
            } catch (Exception ex) {
                throw new InvalidOperationException(string.Format(
                    Properties.Resources.ErrorDuplicateUnitSymbol,
                    unit.Symbol), ex);
            }
        }

        private static void registerUnit(Unit unit) {
            UnitSystem<D>.registerUnit(unit.Symbol, unit);
        }
        #endregion

        #region Private Fields
        /// <summary>
        /// Holds all known built-in units indexed by their symbol for searching
        /// for them.
        /// </summary>
        private static readonly Dictionary<string, Unit> SYMBOL_TO_UNIT;
        #endregion
    }
}
