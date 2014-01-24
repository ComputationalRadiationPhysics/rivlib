/// <copyright file="SIPrefixTest.cs" company="TheLib Team">
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


namespace TheTest.Measurement.Units {

    [TestClass]
    public class SIPrefixTest {

        [TestMethod]
        public void TestCompareToNull() {
            Assert.IsTrue(SIPrefix.Yotta.CompareTo(null) > 0);
        }

        [TestMethod]
        public void TestCompareToSelf() {
            Assert.IsTrue(SIPrefix.Yotta.CompareTo(SIPrefix.Yotta) == 0);
        }

        [TestMethod]
        public void TestCompareToLess() {
            Assert.IsTrue(SIPrefix.Yotta.CompareTo(SIPrefix.Zetta) > 0);
        }

        [TestMethod]
        public void TestCompareToMore() {
            Assert.IsTrue(SIPrefix.Zetta.CompareTo(SIPrefix.Yotta) < 0);
        }

        [TestMethod]
        public void TestConstantOrder() {
            Assert.IsTrue(SIPrefix.Yotta.CompareTo(SIPrefix.Zetta) > 0);
            Assert.IsTrue(SIPrefix.Zetta.CompareTo(SIPrefix.Exa) > 0);
            Assert.IsTrue(SIPrefix.Exa.CompareTo(SIPrefix.Peta) > 0);
            Assert.IsTrue(SIPrefix.Peta.CompareTo(SIPrefix.Tera) > 0);
            Assert.IsTrue(SIPrefix.Tera.CompareTo(SIPrefix.Giga) > 0);
            Assert.IsTrue(SIPrefix.Giga.CompareTo(SIPrefix.Mega) > 0);
            Assert.IsTrue(SIPrefix.Mega.CompareTo(SIPrefix.Kilo) > 0);
            Assert.IsTrue(SIPrefix.Kilo.CompareTo(SIPrefix.Hecto) > 0);
            Assert.IsTrue(SIPrefix.Hecto.CompareTo(SIPrefix.Deca) > 0);
            Assert.IsTrue(SIPrefix.Deca.CompareTo(SIPrefix.Deci) > 0);
            Assert.IsTrue(SIPrefix.Deci.CompareTo(SIPrefix.Centi) > 0);
            Assert.IsTrue(SIPrefix.Centi.CompareTo(SIPrefix.Milli) > 0);
            Assert.IsTrue(SIPrefix.Milli.CompareTo(SIPrefix.Micro) > 0);
            Assert.IsTrue(SIPrefix.Micro.CompareTo(SIPrefix.Nano) > 0);
            Assert.IsTrue(SIPrefix.Nano.CompareTo(SIPrefix.Pico) > 0);
            Assert.IsTrue(SIPrefix.Pico.CompareTo(SIPrefix.Femto) > 0);
            Assert.IsTrue(SIPrefix.Femto.CompareTo(SIPrefix.Atto) > 0);
            Assert.IsTrue(SIPrefix.Atto.CompareTo(SIPrefix.Zepto) > 0);
            Assert.IsTrue(SIPrefix.Zepto.CompareTo(SIPrefix.Yocto) > 0);
        }

        [TestMethod]
        public void TestEquals() {
            Assert.IsTrue(SIPrefix.Yotta.Equals(SIPrefix.Yotta));
            Assert.IsFalse(SIPrefix.Yotta.Equals(SIPrefix.Zetta));
            Assert.IsFalse(SIPrefix.Yotta.Equals(new object()));
            Assert.IsFalse(SIPrefix.Yotta.Equals(null));
        }

        [TestMethod]
        public void TestFromSymbol() {
            Assert.AreEqual(SIPrefix.Yotta, SIPrefix.FromSymbol("Y"));
            Assert.AreEqual(SIPrefix.Zetta, SIPrefix.FromSymbol("Z"));
            Assert.AreEqual(SIPrefix.Exa, SIPrefix.FromSymbol("E"));
            Assert.AreEqual(SIPrefix.Peta, SIPrefix.FromSymbol("P"));
            Assert.AreEqual(SIPrefix.Tera, SIPrefix.FromSymbol("T"));
            Assert.AreEqual(SIPrefix.Giga, SIPrefix.FromSymbol("G"));
            Assert.AreEqual(SIPrefix.Mega, SIPrefix.FromSymbol("M"));
            Assert.AreEqual(SIPrefix.Kilo, SIPrefix.FromSymbol("k"));
            Assert.AreEqual(SIPrefix.Hecto, SIPrefix.FromSymbol("h"));
            Assert.AreEqual(SIPrefix.Deca, SIPrefix.FromSymbol("da"));
            Assert.AreEqual(SIPrefix.Deci, SIPrefix.FromSymbol("d"));
            Assert.AreEqual(SIPrefix.Centi, SIPrefix.FromSymbol("c"));
            Assert.AreEqual(SIPrefix.Milli, SIPrefix.FromSymbol("m"));
            Assert.AreEqual(SIPrefix.Micro, SIPrefix.FromSymbol("µ"));
            Assert.AreEqual(SIPrefix.Nano, SIPrefix.FromSymbol("n"));
            Assert.AreEqual(SIPrefix.Pico, SIPrefix.FromSymbol("p"));
            Assert.AreEqual(SIPrefix.Femto, SIPrefix.FromSymbol("f"));
            Assert.AreEqual(SIPrefix.Atto, SIPrefix.FromSymbol("a"));
            Assert.AreEqual(SIPrefix.Zepto, SIPrefix.FromSymbol("z"));
            Assert.IsNull(SIPrefix.FromSymbol("horst"));
        }

        [TestMethod]
        public void TestHashCode() {
            foreach (SIPrefix p in SIPrefix.All) {
                foreach (SIPrefix q in SIPrefix.All) {
                    Assert.IsTrue(p.Equals(q) || (p.GetHashCode() != q.GetHashCode()));
                }
            }
        }
    }
}
