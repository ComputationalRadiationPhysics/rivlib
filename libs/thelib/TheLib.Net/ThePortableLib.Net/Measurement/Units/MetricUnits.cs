/// <copyright file="MetricUnits.cs" company="TheLib Team">
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

using The.Measurement.Quantities;


namespace The.Measurement.Units {

    public sealed class MetricUnits : UnitSystem<MetricUnits> {

        /// <summary>
        /// Initialises the class.
        /// </summary>
        /// <remarks>
        /// The static constructor is guaranteed to be thread-safe according
        /// to http://msdn.microsoft.com/en-us/library/aa645612(VS.71).aspx.
        /// </remarks>
        static MetricUnits() {

            // TODO: Is this complete?

            MetricUnits.Are = new AlternateUnit<Area>(
                "a", "TODO",
                100 * SIUnits.SquareMetre);

            MetricUnits.Bar = new AlternateUnit<Pressure>(
                "bar", "TODO",
                10E5 * SIUnits.Pascal);

            MetricUnits.Celsius = new AlternateUnit<Temperature>(
                "°C", Properties.Resources.MetricUnitCelsius,
                SIUnits.Kelvin + 273.15);

            MetricUnits.Litre = new AlternateUnit<Volume>(
                "l", Properties.Resources.MetricUnitLitre,
                1E-3 * SIUnits.CubicMetre);

            MetricUnits.Tonne = new AlternateUnit<Mass>(
                "t", Properties.Resources.MetricUnitTonne,
                1E3 * SIUnits.Kilogramme);
        }

        /// <summary>
        /// Gets the unit are, which is 100 square metres.
        /// </summary>
        public static readonly Unit<Area> Are;

        /// <summary>
        /// Gets the unit bar, which is 10E5 Pascal.
        /// </summary>
        public static readonly Unit<Pressure> Bar;

        /// <summary>
        /// Gets the unit for degrees Celsius (°C).
        /// </summary>
        public static readonly Unit<Temperature> Celsius;

        /// <summary>
        /// Gets the unit for litres (l).
        /// </summary>
        public static readonly Unit<Volume> Litre;

        /// <summary>
        /// Gets the unit for a metric ton (t)
        /// </summary>
        public static readonly Unit<Mass> Tonne;

        /// <summary>
        /// Disallow instances.
        /// </summary>
        private MetricUnits() { }
    }
}
