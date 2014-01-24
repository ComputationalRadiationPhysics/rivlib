/// <copyright file="MetricUnitTest.cs" company="TheLib Team">
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
using The.Measurement.Quantities;
using The.Measurement.Units;

namespace TheTest.Measurement.Units {

    [TestClass]
    public class MetricUnitTest {

        [TestMethod]
        public void TestCelsius() {
            Assert.AreEqual("°C", MetricUnits.Celsius.Symbol);
            Assert.AreEqual(SIDimensions.Temperature, MetricUnits.Celsius.Dimension);
            Assert.AreEqual(SIUnits.Kelvin, MetricUnits.Celsius.CoherentUnit);
            // C = K - 273,15
            // K = C + 273,15
            Assert.AreEqual(273.15, MetricUnits.Celsius.ConverterToCoherent.Convert(0.0));
            Assert.AreEqual(-273.15, MetricUnits.Celsius.ConverterToCoherent.Inverse.Convert(0.0));

            Assert.IsFalse(MetricUnits.Celsius.IsCoherent);
        }

        [TestMethod]
        public void TestLitre() {
            Assert.AreEqual("l", MetricUnits.Litre.Symbol);
            //Assert.AreEqual(SIDimensions.Length^3 , MetricUnits.Litre.Dimension);
            Assert.AreEqual(Volume.DefaultUnit.Dimension, MetricUnits.Litre.Dimension);
            Assert.AreEqual(SIUnits.CubicMetre, MetricUnits.Litre.CoherentUnit);
            // 1l = 1/1000 m³
            Assert.AreEqual(1.0 / 1000.0, MetricUnits.Litre.ConverterToCoherent.Convert(1.0));
            // 1m³ = 1000l
            Assert.AreEqual(1000.0, MetricUnits.Litre.ConverterToCoherent.Inverse.Convert(1.0));

            Unit cdm = (SIPrefix.Deci * SIUnits.Metre).Pow(3);
            double oneCdm = cdm.ConverterToCoherent.Convert(1.0);
            double oneL = MetricUnits.Litre.ConverterToCoherent.Convert(1.0);
            Assert.IsTrue(oneCdm - oneL < 0.0000001);
        }

        [TestMethod]
        public void TestTon() {
            Assert.AreEqual("t", MetricUnits.Tonne.Symbol);
            Assert.AreEqual(SIDimensions.Mass, MetricUnits.Tonne.Dimension);
            Assert.AreEqual(SIUnits.Kilogramme, MetricUnits.Tonne.CoherentUnit);
            // 1t = 1000 kg
            Assert.AreEqual(1000.0, MetricUnits.Tonne.ConverterToCoherent.Convert(1.0));
            // 1kg = 1/1000 kg
            Assert.AreEqual(1.0 / 1000.0, MetricUnits.Tonne.ConverterToCoherent.Inverse.Convert(1.0));
        }
    }
}
