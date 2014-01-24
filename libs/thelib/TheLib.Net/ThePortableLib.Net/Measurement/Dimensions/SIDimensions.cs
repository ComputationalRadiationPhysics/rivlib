/// <copyright file="SIDimensions.cs" company="TheLib Team">
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


namespace The.Measurement.Dimensions {

    /// <summary>
    /// This class provides constant instances of the dimensions of the SI base
    /// units.
    /// </summary>
    public static class SIDimensions {

        /// <summary>
        /// Initialises the class.
        /// </summary>
        /// <remarks>
        /// The static constructor is guaranteed to be thread-safe according
        /// to http://msdn.microsoft.com/en-us/library/aa645612(VS.71).aspx.
        /// </remarks>
        static SIDimensions() {
            SIDimensions.AmountOfSubstance = new Dimension('N');
            SIDimensions.ElectricCurrent = new Dimension('I');
            SIDimensions.Length = new Dimension('L');
            SIDimensions.LuminousIntensity = new Dimension('J');
            SIDimensions.Mass = new Dimension('M');
            SIDimensions.None = new Dimension('1');
            SIDimensions.Temperature = new Dimension('Θ');
            SIDimensions.Time = new Dimension('T');
        }

        /// <summary>
        /// Represents amount of substance (N).
        /// </summary>
        public static readonly Dimension AmountOfSubstance;

        /// <summary>
        /// Represents electric current (I).
        /// </summary>
        public static readonly Dimension ElectricCurrent;

        /// <summary>
        /// Represents length (L).
        /// </summary>
        public static readonly Dimension Length;

        /// <summary>
        /// Represents luminous intensity (J).
        /// </summary>
        public static readonly Dimension LuminousIntensity;

        /// <summary>
        /// Represents mass (M).
        /// </summary>
        public static readonly Dimension Mass;

        /// <summary>
        /// Represents "dimensionless".
        /// </summary>
        public static readonly Dimension None;

        /// <summary>
        /// Represents thermodynamic temperature (Θ).
        /// </summary>
        public static readonly Dimension Temperature;

        /// <summary>
        /// Represents time (T).
        /// </summary>
        public static readonly Dimension Time;
    }
}
