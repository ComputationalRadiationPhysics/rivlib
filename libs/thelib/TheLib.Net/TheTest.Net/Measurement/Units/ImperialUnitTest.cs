/// <copyright file="ImperialUnitTest.cs" company="TheLib Team">
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
using Microsoft.VisualStudio.TestTools.UnitTesting;
using The.Measurement.Units;
using The.Mathematics;

namespace TheTest.Measurement.Units {

    [TestClass]
    public class ImperialUnitTest {

        const double EPSILON = 0.00000001;

        [TestMethod]
        public void TestImperialLengthUnits() {
            // From UCUM §36
            const double INCH_IN_METRE = 0.02539998;
            const double FOOT_IN_INCH = 12.0;
            const double ROD_IN_FOOT = 16.5;
            const double CHAIN_IN_ROD = 4.0;
            const double YARD_IN_FOOT = 3.0;
            const double MILE_IN_FOOT = 5280.0;

            //Assert.AreEqual(SIUnits.Metre, ImperialUnits.Thou.CoherentUnit);
            //Assert.AreEqual(0.0000254, ImperialUnits.Thou.ConvertToCoherent(1.0));
            //Assert.AreEqual(1.0, ImperialUnits.Thou.ConvertFromCoherent(0.0000254));


            Assert.AreEqual(SIUnits.Metre, ImperialUnits.Inch.CoherentUnit);
            areNearlyEqual(INCH_IN_METRE, ImperialUnits.Inch.ConverterToCoherent.Convert(1.0));
            areNearlyEqual(1.0, ImperialUnits.Inch.ConverterToCoherent.Inverse.Convert(INCH_IN_METRE));

            Assert.AreEqual(SIUnits.Metre, ImperialUnits.Foot.CoherentUnit);
            areNearlyEqual(FOOT_IN_INCH * INCH_IN_METRE, ImperialUnits.Foot.ConverterToCoherent.Convert(1.0));
            areNearlyEqual(1.0, ImperialUnits.Foot.ConverterToCoherent.Inverse.Convert(FOOT_IN_INCH * INCH_IN_METRE));

            Assert.AreEqual(SIUnits.Metre, ImperialUnits.Yard.CoherentUnit);
            areNearlyEqual(YARD_IN_FOOT * FOOT_IN_INCH * INCH_IN_METRE, ImperialUnits.Yard.ConverterToCoherent.Convert(1.0));
            areNearlyEqual(1.0, ImperialUnits.Yard.ConverterToCoherent.Inverse.Convert(YARD_IN_FOOT * FOOT_IN_INCH * INCH_IN_METRE));

            Assert.AreEqual(SIUnits.Metre, ImperialUnits.Chain.CoherentUnit);
            areNearlyEqual(CHAIN_IN_ROD * ROD_IN_FOOT * FOOT_IN_INCH * INCH_IN_METRE, ImperialUnits.Chain.ConverterToCoherent.Convert(1.0));
            areNearlyEqual(1.0, ImperialUnits.Chain.ConverterToCoherent.Inverse.Convert(CHAIN_IN_ROD * ROD_IN_FOOT * FOOT_IN_INCH * INCH_IN_METRE));

            //Assert.AreEqual(SIUnits.Metre, ImperialUnits.Furlong.CoherentUnit);
            //Assert.AreEqual(201.168, ImperialUnits.Furlong.ConverterToCoherent.Convert(1.0));
            //Assert.AreEqual(1.0, ImperialUnits.Furlong.ConverterToCoherent.Inverse.Convert(201.168));

            Assert.AreEqual(SIUnits.Metre, ImperialUnits.Mile.CoherentUnit);
            areNearlyEqual(MILE_IN_FOOT * FOOT_IN_INCH * INCH_IN_METRE, ImperialUnits.Mile.ConverterToCoherent.Convert(1.0));
            areNearlyEqual(1.0, ImperialUnits.Mile.ConverterToCoherent.Inverse.Convert(MILE_IN_FOOT * FOOT_IN_INCH * INCH_IN_METRE));

            //Assert.AreEqual(SIUnits.Metre, ImperialUnits.League.CoherentUnit);
            //Assert.AreEqual(4828.032, ImperialUnits.League.ConverterToCoherent.Convert(1.0));
            //Assert.AreEqual(1.0, ImperialUnits.League.ConverterToCoherent.Inverse.Convert(4828.032));
        }

        [TestMethod]
        public void TestImperialMaritimeLengthUnits() {
            // From UCUM §36
            const double INCH_IN_METRE = 0.02539998;
            const double FOOT_IN_INCH = 12.0;
            const double FOOT_IN_METRE = INCH_IN_METRE * FOOT_IN_INCH;
            const double FATHOM_IN_FOOT = 6.0;
            const double NAUTICAL_MILE_IN_FOOT = 6080.0;

            Assert.AreEqual(SIUnits.Metre, ImperialUnits.Fathom.CoherentUnit);
            areNearlyEqual(FATHOM_IN_FOOT * FOOT_IN_METRE, ImperialUnits.Fathom.ConverterToCoherent.Convert(1.0));
            areNearlyEqual(1.0, ImperialUnits.Fathom.ConverterToCoherent.Inverse.Convert(FATHOM_IN_FOOT * FOOT_IN_METRE));

            //Assert.AreEqual(SIUnits.Metre, ImperialUnits.Cable.CoherentUnit);
            //Assert.AreEqual(185.3184, ImperialUnits.Cable.ConvertToCoherent(1.0));
            //Assert.AreEqual(1.0, ImperialUnits.Cable.ConvertFromCoherent(185.3184));

            Assert.AreEqual(SIUnits.Metre, ImperialUnits.NauticalMile.CoherentUnit);
            areNearlyEqual(NAUTICAL_MILE_IN_FOOT * FOOT_IN_METRE, ImperialUnits.NauticalMile.ConverterToCoherent.Convert(1.0));
            areNearlyEqual(1.0, ImperialUnits.NauticalMile.ConverterToCoherent.Inverse.Convert(NAUTICAL_MILE_IN_FOOT * FOOT_IN_METRE));
        }


        private static void areNearlyEqual(double lhs, double rhs) {
            Assert.IsTrue(lhs.IsNearlyEqual(rhs, EPSILON));
        }
    }
}
