using System;
using System.Diagnostics;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using The.Measurement.Quantities;
using The.Measurement.Units;

namespace TheTest.Measurement.Units {

    [TestClass]
    public class DerivedUnitTest {

        [TestMethod]
        public void TestAllSIUnits() {
            foreach (Unit u in SIUnits.All) {
                Debug.WriteLine(u);
            }
        }

        [TestMethod]
        public void TestSIDerivedQuantities() {
            Assert.AreEqual(SIUnits.SquareMetre, Area.DefaultUnit);
            Assert.AreEqual(SIUnits.CubicMetre, Volume.DefaultUnit);
            Assert.AreEqual(SIUnits.MetrePerSecond, Velocity.DefaultUnit);
            Assert.AreEqual(SIUnits.CubicMetrePerSecond, VolumetricFlow.DefaultUnit);
            Assert.AreEqual(SIUnits.MetrePerSecondSquared, Acceleration.DefaultUnit);
            Assert.AreEqual(SIUnits.MetrePerSecondCubed, Jerk.DefaultUnit); 
            Assert.AreEqual(SIUnits.MetrePerQuarticSecond, Snap.DefaultUnit);
        }
    }
}
