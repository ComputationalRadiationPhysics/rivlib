/// <copyright file="DimensionTest.cs" company="TheLib Team">
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
using The.Measurement.Dimensions;
using The.Measurement.Units;


namespace TheTest.Measurement.Units {

    [TestClass]
    public class DimensionTest {

        [TestMethod]
        public void TestAmountOfSubstance() {
            Assert.AreEqual("[N]", SIDimensions.AmountOfSubstance.Symbol);

            Assert.AreEqual(SIDimensions.AmountOfSubstance, SIDimensions.AmountOfSubstance);
            Assert.AreNotEqual(SIDimensions.ElectricCurrent, SIDimensions.AmountOfSubstance);
            Assert.AreNotEqual(SIDimensions.Length, SIDimensions.AmountOfSubstance);
            Assert.AreNotEqual(SIDimensions.LuminousIntensity, SIDimensions.AmountOfSubstance);
            Assert.AreNotEqual(SIDimensions.Mass, SIDimensions.AmountOfSubstance);
            Assert.AreNotEqual(SIDimensions.Temperature, SIDimensions.AmountOfSubstance);
            Assert.AreNotEqual(SIDimensions.Time, SIDimensions.AmountOfSubstance);
        }

        [TestMethod]
        public void TestElectricCurrent() {
            Assert.AreEqual("[I]", SIDimensions.ElectricCurrent.Symbol);

            Assert.AreNotEqual(SIDimensions.AmountOfSubstance, SIDimensions.ElectricCurrent);
            Assert.AreEqual(SIDimensions.ElectricCurrent, SIDimensions.ElectricCurrent);
            Assert.AreNotEqual(SIDimensions.Length, SIDimensions.ElectricCurrent);
            Assert.AreNotEqual(SIDimensions.LuminousIntensity, SIDimensions.ElectricCurrent);
            Assert.AreNotEqual(SIDimensions.Mass, SIDimensions.ElectricCurrent);
            Assert.AreNotEqual(SIDimensions.Temperature, SIDimensions.ElectricCurrent);
            Assert.AreNotEqual(SIDimensions.Time, SIDimensions.ElectricCurrent);
        }

        [TestMethod]
        public void TestLength() {
            Assert.AreEqual("[L]", SIDimensions.Length.Symbol);

            Assert.AreNotEqual(SIDimensions.AmountOfSubstance, SIDimensions.Length);
            Assert.AreNotEqual(SIDimensions.ElectricCurrent, SIDimensions.Length);
            Assert.AreEqual(SIDimensions.Length, SIDimensions.Length);
            Assert.AreNotEqual(SIDimensions.LuminousIntensity, SIDimensions.Length);
            Assert.AreNotEqual(SIDimensions.Mass, SIDimensions.Length);
            Assert.AreNotEqual(SIDimensions.Temperature, SIDimensions.Length);
            Assert.AreNotEqual(SIDimensions.Time, SIDimensions.Length);
        }

        [TestMethod]
        public void TestLuminousIntensity() {
            Assert.AreEqual("[J]", SIDimensions.LuminousIntensity.Symbol);

            Assert.AreNotEqual(SIDimensions.AmountOfSubstance, SIDimensions.LuminousIntensity);
            Assert.AreNotEqual(SIDimensions.ElectricCurrent, SIDimensions.LuminousIntensity);
            Assert.AreNotEqual(SIDimensions.Length, SIDimensions.LuminousIntensity);
            Assert.AreEqual(SIDimensions.LuminousIntensity, SIDimensions.LuminousIntensity);
            Assert.AreNotEqual(SIDimensions.Mass, SIDimensions.LuminousIntensity);
            Assert.AreNotEqual(SIDimensions.Temperature, SIDimensions.LuminousIntensity);
            Assert.AreNotEqual(SIDimensions.Time, SIDimensions.LuminousIntensity);
        }

        [TestMethod]
        public void TestMass() {
            Assert.AreEqual("[M]", SIDimensions.Mass.Symbol);

            Assert.AreNotEqual(SIDimensions.AmountOfSubstance, SIDimensions.Mass);
            Assert.AreNotEqual(SIDimensions.ElectricCurrent, SIDimensions.Mass);
            Assert.AreNotEqual(SIDimensions.Length, SIDimensions.Mass);
            Assert.AreNotEqual(SIDimensions.LuminousIntensity, SIDimensions.Mass);
            Assert.AreEqual(SIDimensions.Mass, SIDimensions.Mass);
            Assert.AreNotEqual(SIDimensions.Temperature, SIDimensions.Mass);
            Assert.AreNotEqual(SIDimensions.Time, SIDimensions.Mass);
        }

        [TestMethod]
        public void TestTemperature() {
            Assert.AreEqual("[Θ]", SIDimensions.Temperature.Symbol);

            Assert.AreNotEqual(SIDimensions.AmountOfSubstance, SIDimensions.Temperature);
            Assert.AreNotEqual(SIDimensions.ElectricCurrent, SIDimensions.Temperature);
            Assert.AreNotEqual(SIDimensions.Length, SIDimensions.Temperature);
            Assert.AreNotEqual(SIDimensions.LuminousIntensity, SIDimensions.Temperature);
            Assert.AreNotEqual(SIDimensions.Mass, SIDimensions.Temperature);
            Assert.AreEqual(SIDimensions.Temperature, SIDimensions.Temperature);
            Assert.AreNotEqual(SIDimensions.Time, SIDimensions.Temperature);
        }

        [TestMethod]
        public void TestTime() {
            Assert.AreEqual("[T]", SIDimensions.Time.Symbol);

            Assert.AreNotEqual(SIDimensions.AmountOfSubstance, SIDimensions.Time);
            Assert.AreNotEqual(SIDimensions.ElectricCurrent, SIDimensions.Time);
            Assert.AreNotEqual(SIDimensions.Length, SIDimensions.Time);
            Assert.AreNotEqual(SIDimensions.LuminousIntensity, SIDimensions.Time);
            Assert.AreNotEqual(SIDimensions.Mass, SIDimensions.Time);
            Assert.AreNotEqual(SIDimensions.Temperature, SIDimensions.Time);
            Assert.AreEqual(SIDimensions.Time, SIDimensions.Time);
        }
    }
}
