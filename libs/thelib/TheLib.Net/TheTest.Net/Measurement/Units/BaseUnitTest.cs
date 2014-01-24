/// <copyright file="BaseUnitTest.cs" company="TheLib Team">
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

using Microsoft.VisualStudio.TestTools.UnitTesting;
using The.Measurement.Dimensions;
using The.Measurement.Units;


namespace TheTest.Measurement.Units {

    [TestClass]
    public class BaseUnitTest {

        [TestMethod]
        public void TestAmpere() {
            Assert.AreEqual("A", SIUnits.Ampere.Symbol);
            Assert.IsTrue(SIUnits.Ampere.IsCoherent);
            Assert.AreEqual(1.0, SIUnits.Ampere.ConverterToCoherent.Convert(1.0));
            Assert.AreEqual(1.0, SIUnits.Ampere.ConverterToCoherent.Inverse.Convert(1.0));
            Assert.AreEqual(SIDimensions.ElectricCurrent, SIUnits.Ampere.Dimension);

            Assert.AreNotEqual(SIDimensions.AmountOfSubstance, SIUnits.Ampere.Dimension);
            Assert.AreNotEqual(SIDimensions.Length, SIUnits.Ampere.Dimension);
            Assert.AreNotEqual(SIDimensions.LuminousIntensity, SIUnits.Ampere.Dimension);
            Assert.AreNotEqual(SIDimensions.Mass, SIUnits.Ampere.Dimension);
            Assert.AreNotEqual(SIDimensions.Temperature, SIUnits.Ampere.Dimension);
            Assert.AreNotEqual(SIDimensions.Time, SIUnits.Ampere.Dimension);

            Assert.AreEqual(SIUnits.Ampere, SIUnits.Ampere);
            Assert.AreNotEqual(SIUnits.Candela, SIUnits.Ampere);
            Assert.AreNotEqual(SIUnits.Kelvin, SIUnits.Ampere);
            Assert.AreNotEqual(SIUnits.Kilogramme, SIUnits.Ampere);
            Assert.AreNotEqual(SIUnits.Metre, SIUnits.Ampere);
            Assert.AreNotEqual(SIUnits.Mole, SIUnits.Ampere);
            Assert.AreNotEqual(SIUnits.Second, SIUnits.Ampere);
        }

        [TestMethod]
        public void TestCandela() {
            Assert.AreEqual("cd", SIUnits.Candela.Symbol);
            Assert.IsTrue(SIUnits.Candela.IsCoherent);
            Assert.AreEqual(1.0, SIUnits.Candela.ConverterToCoherent.Convert(1.0));
            Assert.AreEqual(1.0, SIUnits.Candela.ConverterToCoherent.Inverse.Convert(1.0));
            Assert.AreEqual(SIDimensions.LuminousIntensity, SIUnits.Candela.Dimension);

            Assert.AreNotEqual(SIDimensions.AmountOfSubstance, SIUnits.Candela.Dimension);
            Assert.AreNotEqual(SIDimensions.ElectricCurrent, SIUnits.Candela.Dimension);
            Assert.AreNotEqual(SIDimensions.Length, SIUnits.Candela.Dimension);
            Assert.AreNotEqual(SIDimensions.Mass, SIUnits.Candela.Dimension);
            Assert.AreNotEqual(SIDimensions.Temperature, SIUnits.Candela.Dimension);
            Assert.AreNotEqual(SIDimensions.Time, SIUnits.Candela.Dimension);

            Assert.AreNotEqual(SIUnits.Ampere, SIUnits.Candela);
            Assert.AreEqual(SIUnits.Candela, SIUnits.Candela);
            Assert.AreNotEqual(SIUnits.Kelvin, SIUnits.Candela);
            Assert.AreNotEqual(SIUnits.Kilogramme, SIUnits.Candela);
            Assert.AreNotEqual(SIUnits.Metre, SIUnits.Candela);
            Assert.AreNotEqual(SIUnits.Mole, SIUnits.Candela);
            Assert.AreNotEqual(SIUnits.Second, SIUnits.Candela);
        }

        [TestMethod]
        public void TestKelvin() {
            Assert.AreEqual("K", SIUnits.Kelvin.Symbol);
            Assert.IsTrue(SIUnits.Kelvin.IsCoherent);
            Assert.AreEqual(1.0, SIUnits.Kelvin.ConverterToCoherent.Convert(1.0));
            Assert.AreEqual(1.0, SIUnits.Kelvin.ConverterToCoherent.Inverse.Convert(1.0));
            Assert.AreEqual(SIDimensions.Temperature, SIUnits.Kelvin.Dimension);

            Assert.AreNotEqual(SIDimensions.AmountOfSubstance, SIUnits.Kelvin.Dimension);
            Assert.AreNotEqual(SIDimensions.ElectricCurrent, SIUnits.Kelvin.Dimension);
            Assert.AreNotEqual(SIDimensions.Length, SIUnits.Kelvin.Dimension);
            Assert.AreNotEqual(SIDimensions.LuminousIntensity, SIUnits.Kelvin.Dimension);
            Assert.AreNotEqual(SIDimensions.Mass, SIUnits.Kelvin.Dimension);
            Assert.AreNotEqual(SIDimensions.Time, SIUnits.Kelvin.Dimension);

            Assert.AreNotEqual(SIUnits.Ampere, SIUnits.Kelvin);
            Assert.AreNotEqual(SIUnits.Candela, SIUnits.Kelvin);
            Assert.AreEqual(SIUnits.Kelvin, SIUnits.Kelvin);
            Assert.AreNotEqual(SIUnits.Kilogramme, SIUnits.Kelvin);
            Assert.AreNotEqual(SIUnits.Metre, SIUnits.Kelvin);
            Assert.AreNotEqual(SIUnits.Mole, SIUnits.Kelvin);
            Assert.AreNotEqual(SIUnits.Second, SIUnits.Kelvin);
        }

        [TestMethod]
        public void TestKilogramme() {
            Assert.AreEqual("kg", SIUnits.Kilogramme.Symbol);
            Assert.IsTrue(SIUnits.Kilogramme.IsCoherent);
            Assert.AreEqual(1.0, SIUnits.Kilogramme.ConverterToCoherent.Convert(1.0));
            Assert.AreEqual(1.0, SIUnits.Kilogramme.ConverterToCoherent.Inverse.Convert(1.0));
            Assert.AreEqual(SIDimensions.Mass, SIUnits.Kilogramme.Dimension);

            Assert.AreNotEqual(SIDimensions.AmountOfSubstance, SIUnits.Kilogramme.Dimension);
            Assert.AreNotEqual(SIDimensions.ElectricCurrent, SIUnits.Kilogramme.Dimension);
            Assert.AreNotEqual(SIDimensions.Length, SIUnits.Kilogramme.Dimension);
            Assert.AreNotEqual(SIDimensions.LuminousIntensity, SIUnits.Kilogramme.Dimension);
            Assert.AreNotEqual(SIDimensions.Temperature, SIUnits.Kilogramme.Dimension);
            Assert.AreNotEqual(SIDimensions.Time, SIUnits.Kilogramme.Dimension);

            Assert.AreNotEqual(SIUnits.Ampere, SIUnits.Kilogramme);
            Assert.AreNotEqual(SIUnits.Candela, SIUnits.Kilogramme);
            Assert.AreNotEqual(SIUnits.Kelvin, SIUnits.Kilogramme);
            Assert.AreEqual(SIUnits.Kilogramme, SIUnits.Kilogramme);
            Assert.AreNotEqual(SIUnits.Metre, SIUnits.Kilogramme);
            Assert.AreNotEqual(SIUnits.Mole, SIUnits.Kilogramme);
            Assert.AreNotEqual(SIUnits.Second, SIUnits.Kilogramme);
        }

        [TestMethod]
        public void TestMetre() {
            Assert.AreEqual("m", SIUnits.Metre.Symbol);
            Assert.IsTrue(SIUnits.Metre.IsCoherent);
            Assert.AreEqual(1.0, SIUnits.Metre.ConverterToCoherent.Convert(1.0));
            Assert.AreEqual(1.0, SIUnits.Metre.ConverterToCoherent.Inverse.Convert(1.0));
            Assert.AreEqual(SIDimensions.Length, SIUnits.Metre.Dimension);

            Assert.AreNotEqual(SIDimensions.AmountOfSubstance, SIUnits.Metre.Dimension);
            Assert.AreNotEqual(SIDimensions.ElectricCurrent, SIUnits.Metre.Dimension);
            Assert.AreNotEqual(SIDimensions.LuminousIntensity, SIUnits.Metre.Dimension);
            Assert.AreNotEqual(SIDimensions.Mass, SIUnits.Metre.Dimension);
            Assert.AreNotEqual(SIDimensions.Temperature, SIUnits.Metre.Dimension);
            Assert.AreNotEqual(SIDimensions.Time, SIUnits.Metre.Dimension);

            Assert.AreNotEqual(SIUnits.Ampere, SIUnits.Metre);
            Assert.AreNotEqual(SIUnits.Candela, SIUnits.Metre);
            Assert.AreNotEqual(SIUnits.Kelvin, SIUnits.Metre);
            Assert.AreNotEqual(SIUnits.Kilogramme, SIUnits.Metre);
            Assert.AreEqual(SIUnits.Metre, SIUnits.Metre);
            Assert.AreNotEqual(SIUnits.Mole, SIUnits.Metre);
            Assert.AreNotEqual(SIUnits.Second, SIUnits.Metre);
        }

        [TestMethod]
        public void TestMole() {
            Assert.AreEqual("mol", SIUnits.Mole.Symbol);
            Assert.IsTrue(SIUnits.Mole.IsCoherent);
            Assert.AreEqual(1.0, SIUnits.Mole.ConverterToCoherent.Convert(1.0));
            Assert.AreEqual(1.0, SIUnits.Mole.ConverterToCoherent.Inverse.Convert(1.0));
            Assert.AreEqual(SIDimensions.AmountOfSubstance, SIUnits.Mole.Dimension);

            Assert.AreNotEqual(SIDimensions.ElectricCurrent, SIUnits.Mole.Dimension);
            Assert.AreNotEqual(SIDimensions.Length, SIUnits.Mole.Dimension);
            Assert.AreNotEqual(SIDimensions.LuminousIntensity, SIUnits.Mole.Dimension);
            Assert.AreNotEqual(SIDimensions.Mass, SIUnits.Mole.Dimension);
            Assert.AreNotEqual(SIDimensions.Temperature, SIUnits.Mole.Dimension);
            Assert.AreNotEqual(SIDimensions.Time, SIUnits.Mole.Dimension);

            Assert.AreNotEqual(SIUnits.Ampere, SIUnits.Mole);
            Assert.AreNotEqual(SIUnits.Candela, SIUnits.Mole);
            Assert.AreNotEqual(SIUnits.Kelvin, SIUnits.Mole);
            Assert.AreNotEqual(SIUnits.Kilogramme, SIUnits.Mole);
            Assert.AreNotEqual(SIUnits.Metre, SIUnits.Mole);
            Assert.AreEqual(SIUnits.Mole, SIUnits.Mole);
            Assert.AreNotEqual(SIUnits.Second, SIUnits.Mole);
        }

        [TestMethod]
        public void TestSecond() {
            Assert.AreEqual("s", SIUnits.Second.Symbol);
            Assert.IsTrue(SIUnits.Second.IsCoherent);
            Assert.AreEqual(1.0, SIUnits.Second.ConverterToCoherent.Convert(1.0));
            Assert.AreEqual(1.0, SIUnits.Second.ConverterToCoherent.Inverse.Convert(1.0));
            Assert.AreEqual(SIDimensions.Time, SIUnits.Second.Dimension);

            Assert.AreNotEqual(SIDimensions.AmountOfSubstance, SIUnits.Second.Dimension);
            Assert.AreNotEqual(SIDimensions.ElectricCurrent, SIUnits.Second.Dimension);
            Assert.AreNotEqual(SIDimensions.Length, SIUnits.Second.Dimension);
            Assert.AreNotEqual(SIDimensions.LuminousIntensity, SIUnits.Second.Dimension);
            Assert.AreNotEqual(SIDimensions.Mass, SIUnits.Second.Dimension);
            Assert.AreNotEqual(SIDimensions.Temperature, SIUnits.Second.Dimension);

            Assert.AreNotEqual(SIUnits.Ampere, SIUnits.Second);
            Assert.AreNotEqual(SIUnits.Candela, SIUnits.Second);
            Assert.AreNotEqual(SIUnits.Kelvin, SIUnits.Second);
            Assert.AreNotEqual(SIUnits.Kilogramme, SIUnits.Second);
            Assert.AreNotEqual(SIUnits.Metre, SIUnits.Second);
            Assert.AreNotEqual(SIUnits.Mole, SIUnits.Second);
            Assert.AreEqual(SIUnits.Second, SIUnits.Second);
        }
    }
}
