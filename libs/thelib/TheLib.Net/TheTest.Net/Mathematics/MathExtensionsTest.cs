/// <copyright file="MathExtensionsTest.cs" company="TheLib Team">
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
using The.Mathematics;

namespace TheTest.Mathematics {

    [TestClass]
    public class MathExtensionsTest {

        #region Clamp int
        [TestMethod]
        public void TestClampIntWithin() {
            int value = 50;
            int min = 10;
            int max = 100;
            Assert.AreEqual(value, value.Clamp(min, max));
        }

        [TestMethod]
        public void TestClampIntMin() {
            int value = 10;
            int min = 10;
            int max = 100;
            Assert.AreEqual(value, value.Clamp(min, max));
        }

        [TestMethod]
        public void TestClampIntLess() {
            int value = 1;
            int min = 10;
            int max = 100;
            Assert.AreEqual(min, value.Clamp(min, max));
        }

        [TestMethod]
        public void TestClampIntMax() {
            int value = 100;
            int min = 10;
            int max = 100;
            Assert.AreEqual(value, value.Clamp(min, max));
        }

        [TestMethod]
        public void TestClampIntMore() {
            int value = 200;
            int min = 10;
            int max = 100;
            Assert.AreEqual(max, value.Clamp(min, max));
        }
        #endregion

        #region Clamp float
        [TestMethod]
        public void TestClampFloatWithin() {
            float value = 50.0f;
            float min = 10.0f;
            float max = 100.0f;
            Assert.AreEqual(value, value.Clamp(min, max));
        }

        [TestMethod]
        public void TestClampFloatMin() {
            float value = 10.0f;
            float min = 10.0f;
            float max = 100.0f;
            Assert.AreEqual(value, value.Clamp(min, max));
        }

        [TestMethod]
        public void TestClampFloatLess() {
            float value = 1.0f;
            float min = 10.0f;
            float max = 100.0f;
            Assert.AreEqual(min, value.Clamp(min, max));
        }

        [TestMethod]
        public void TestClampFloatMax() {
            float value = 100.0f;
            float min = 10.0f;
            float max = 100.0f;
            Assert.AreEqual(value, value.Clamp(min, max));
        }

        [TestMethod]
        public void TestClampFloatMore() {
            float value = 200.0f;
            float min = 10.0f;
            float max = 100.0f;
            Assert.AreEqual(max, value.Clamp(min, max));
        }
        #endregion

        #region GCD
        [TestMethod]
        public void TestGreatestCommonDivisor() {
            int m = 54;
            int n = 24;
            int r = 6;
            Assert.AreEqual(r, MathExtensions.GreatestCommonDivisor(m, n));
        }
        #endregion

        #region LCM
        [TestMethod]
        public void LeastCommonMultiple() {
            int m = 4;
            int n = 6;
            int r = 12;
            Assert.AreEqual(r, MathExtensions.LeastCommonMultiple(m, n));
        }
        #endregion

        #region IsWithin int
        [TestMethod]
        public void TestIsWithinIntWithin() {
            int value = 50;
            int min = 10;
            int max = 100;
            Assert.IsTrue(value.IsWithin(min, max));
        }

        [TestMethod]
        public void TestIsWithinIntMin() {
            int value = 10;
            int min = 10;
            int max = 100;
            Assert.IsFalse(value.IsWithin(min, max));
        }

        [TestMethod]
        public void TestIsWithinIntMinIncluding() {
            int value = 10;
            int min = 10;
            int max = 100;
            Assert.IsTrue(value.IsWithin(min, max, true, true));
        }

        [TestMethod]
        public void TestIsWithinIntLess() {
            int value = 1;
            int min = 10;
            int max = 100;
            Assert.IsFalse(value.IsWithin(min, max));
        }


        [TestMethod]
        public void TestIsWithinIntLessIncluding() {
            int value = 1;
            int min = 10;
            int max = 100;
            Assert.IsFalse(value.IsWithin(min, max, true, true));
        }

        [TestMethod]
        public void TestIsWithinIntMax() {
            int value = 100;
            int min = 10;
            int max = 100;
            Assert.IsFalse(value.IsWithin(min, max));
        }

        [TestMethod]
        public void TestIsWithinIntMaxIncluding() {
            int value = 100;
            int min = 10;
            int max = 100;
            Assert.IsTrue(value.IsWithin(min, max, true, true));
        }

        [TestMethod]
        public void TestIsWithinIntMore() {
            int value = 200;
            int min = 10;
            int max = 100;
            Assert.IsFalse(value.IsWithin(min, max));
        }

        [TestMethod]
        public void TestIsWithinIntMoreIncluding() {
            int value = 200;
            int min = 10;
            int max = 100;
            Assert.IsFalse(value.IsWithin(min, max, true, true));
        }
        #endregion

        #region IsNearlyEqual
        [TestMethod]
        public void TestIsNearlyEqualFloat() {
            float m = 1.0f;
            float n = 1.0f;
            Assert.IsTrue(m.IsNearlyEqual(n));
        }

        [TestMethod]
        public void TestIsNearlyEqualDouble() {
            double m = 1.0;
            double n = 1.0;
            Assert.IsTrue(m.IsNearlyEqual(n));
        }

        [TestMethod]
        public void TestIsNearlyEqualDecimal() {
            decimal m = new decimal(1.0);
            decimal n = new decimal(1.0);
            Assert.IsTrue(m.IsNearlyEqual(n, new decimal(0.00001)));
        }

        [TestMethod]
        public void TestIsNearlyNotEqualFloat() {
            float m = 1.0f;
            float n = 2.0f;
            Assert.IsFalse(m.IsNearlyEqual(n));
        }

        [TestMethod]
        public void TestIsNearlyNotEqualDouble() {
            double m = 1.0;
            double n = 2.0;
            Assert.IsFalse(m.IsNearlyEqual(n));
        }

        [TestMethod]
        public void TestIsNearlyNotEqualDecimal() {
            decimal m = new decimal(1.0);
            decimal n = new decimal(2.0);
            Assert.IsFalse(m.IsNearlyEqual(n, new decimal(0.0)));
        }
        #endregion
    }
}
