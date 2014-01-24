/// <copyright file="ImperialUnits.cs" company="TheLib Team">
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

using The.Measurement.Converters;
using The.Measurement.Quantities;


namespace The.Measurement.Units {

    /// <summary>
    /// Provides constant instances of British Imperial Units.
    /// </summary>
    public sealed class ImperialUnits : UnitSystem<ImperialUnits> {

        #region Class Constructor
        /// <summary>
        /// Initialise the class.
        /// </summary>
        static ImperialUnits() {

            // TODO: Units are missing here


            #region UCUM 4.4 §36 British Imperial lengths
            ImperialUnits.Inch = new AlternateUnit<Length>(
                "in", Properties.Resources.ImperialUnitInch,
                new TransformedUnit(SIPrefix.Centi * SIUnits.Metre,
                new RationalUnitConverter(2539998, 1000000)));

            ImperialUnits.Foot = new AlternateUnit<Length>(
                "ft", Properties.Resources.ImperialUnitFoot,
                12 * ImperialUnits.Inch);

            ImperialUnits.Rod = new AlternateUnit<Length>(
                "rd", Properties.Resources.ImperialUnitRod,
                new TransformedUnit(ImperialUnits.Foot,
                new RationalUnitConverter(33, 2)));

            ImperialUnits.Chain = new AlternateUnit<Length>(
                "ch", Properties.Resources.ImperialUnitChain,
                4 * ImperialUnits.Rod);

            ImperialUnits.Link = new AlternateUnit<Length>(
                "li", Properties.Resources.ImperialUnitLink,
                ImperialUnits.Chain / 100);

            ImperialUnits.Fathom = new AlternateUnit<Length>(
                "ftm", Properties.Resources.ImperialUnitFathom,
                6 * ImperialUnits.Foot);

            ImperialUnits.Pace = new AlternateUnit<Length>(
                "pc", "TODO",
                new TransformedUnit(ImperialUnits.Foot,
                new RationalUnitConverter(5, 20)));

            ImperialUnits.Yard = new AlternateUnit<Length>(
                "yd", Properties.Resources.ImperialUnitYard,
                3 * ImperialUnits.Foot);

            ImperialUnits.Mile = new AlternateUnit<Length>(
                "mi", Properties.Resources.ImperialUnitMile,
                5280 * ImperialUnits.Foot);

            ImperialUnits.NauticalMile = new AlternateUnit<Length>(
                "nmi", Properties.Resources.ImperialUnitNauticalMile,
                6080 * ImperialUnits.Foot);

            ImperialUnits.Knot= new AlternateUnit<Velocity>(
                "kn", "TODO",
                ImperialUnits.NauticalMile / UcumUnits.Hour);

            ImperialUnits.Acre = new AlternateUnit<Area>(
                "acr", Properties.Resources.ImperialUnitAcre,
                4840 * ImperialUnits.Yard.Pow(2));
            #endregion

            #region UCUM 4.4 §38 British Imperial volumes
            ImperialUnits.Gallon = new AlternateUnit<Volume>(
                "gal", Properties.Resources.ImperialUnitGallon,
                454609 * MetricUnits.Litre / 100000);

            ImperialUnits.Peck = new AlternateUnit<Volume>(
                "pk", "TODO",
                2 * ImperialUnits.Gallon);

            ImperialUnits.Bushel = new AlternateUnit<Volume>(
                "bu", "TODO",
                4 * ImperialUnits.Peck);

            ImperialUnits.Quart = new AlternateUnit<Volume>(
                "qt", Properties.Resources.ImperialUnitQuart,
                4 * ImperialUnits.Gallon);

            ImperialUnits.Pint = new AlternateUnit<Volume>(
                "pt", Properties.Resources.ImperialUnitPint,
                ImperialUnits.Quart / 2);

            ImperialUnits.Gill = new AlternateUnit<Volume>(
                "gi", Properties.Resources.ImperialUnitGill,
                ImperialUnits.Pint / 4);

            ImperialUnits.FluidOunce = new AlternateUnit<Volume>(
                "fl oz", Properties.Resources.ImperialUnitFluidOunce,
                ImperialUnits.Gill / 5);

            ImperialUnits.FluidDrachm = new AlternateUnit<Volume>(
                "ʒ", Properties.Resources.ImperialUnitFluidDrachm,
                ImperialUnits.FluidOunce / 8);

            ImperialUnits.Minim = new AlternateUnit<Volume>(
                "♏", Properties.Resources.ImperialUnitMinim,
                ImperialUnits.FluidDrachm / 60);
            #endregion


            #region Wikipedia
            //#region Length Units
            //ImperialUnits.Thou = DerivedUnit.FromScale<Length>("th",
            //    Properties.Resources.ImperialUnitThou,
            //    0.0000254, SIUnits.Metre);

            //ImperialUnits.Inch = DerivedUnit.FromScale<Length>("in",
            //    Properties.Resources.ImperialUnitInch,
            //    0.0254, SIUnits.Metre);

            //ImperialUnits.Foot = DerivedUnit.FromScale<Length>("ft",
            //    Properties.Resources.ImperialUnitFoot,
            //    0.3048, SIUnits.Metre);

            //ImperialUnits.Yard = DerivedUnit.FromScale<Length>("yd",
            //    Properties.Resources.ImperialUnitYard,
            //    0.9144, SIUnits.Metre);

            //ImperialUnits.Chain = DerivedUnit.FromScale<Length>("ch",
            //    Properties.Resources.ImperialUnitChain,
            //    20.1168, SIUnits.Metre);

            //ImperialUnits.Furlong = DerivedUnit.FromScale<Length>("fur",
            //    Properties.Resources.ImperialUnitFurlong,
            //    201.168, SIUnits.Metre);

            //ImperialUnits.Mile = DerivedUnit.FromScale<Length>("mi",
            //    Properties.Resources.ImperialUnitMile,
            //    1609.344, SIUnits.Metre);

            //ImperialUnits.League = DerivedUnit.FromScale<Length>("lea",
            //    Properties.Resources.ImperialUnitLeague,
            //    4828.032, SIUnits.Metre);
            //#endregion

            //#region Maritime Length Units
            //ImperialUnits.Fathom = DerivedUnit.FromScale<Length>("ftm",
            //    Properties.Resources.ImperialUnitFathom,
            //    1.853184, SIUnits.Metre);

            //ImperialUnits.Cable = DerivedUnit.FromScale<Length>("cb",
            //    Properties.Resources.ImperialUnitCable,
            //    185.3184, SIUnits.Metre);

            //ImperialUnits.NauticalMile = DerivedUnit.FromScale<Length>("nmi",
            //    Properties.Resources.ImperialUnitNauticalMile,
            //    1853.184, SIUnits.Metre);
            //#endregion

            //#region Gunter's Survey Units
            //ImperialUnits.Link = DerivedUnit.FromScale<Length>("li",
            //    Properties.Resources.ImperialUnitLink,
            //    1.0 / 100.0, ImperialUnits.Chain);

            //ImperialUnits.Rod = DerivedUnit.FromScale<Length>("rd",
            //    Properties.Resources.ImperialUnitRod,
            //    1.0 / 10.0, ImperialUnits.Furlong);
            //#endregion

            //#region Area Units
            //ImperialUnits.Perch = DerivedUnit.FromPower<Area>(
            //    "perch", Properties.Resources.ImperialUnitPerch,
            //    ImperialUnits.Rod, 2);

            //ImperialUnits.Rood = DerivedUnit.FromScale<Area>("rood",
            //    Properties.Resources.ImperialUnitRood,
            //    1011.7141056, SIUnits.SquareMetre);

            //ImperialUnits.Acre = DerivedUnit.FromScale<Area>("acre",
            //    Properties.Resources.ImperialUnitAcre,
            //    4046.8564224, SIUnits.SquareMetre);
            //#endregion

            //#region Volume Units
            //ImperialUnits.FluidOunce = DerivedUnit.FromScale<Volume>("fl oz",
            //    Properties.Resources.ImperialUnitFluidOunce,
            //    28.4130625 / 1000.0, MetricUnits.Litre);

            //ImperialUnits.Gill = DerivedUnit.FromScale<Volume>("gi",
            //    Properties.Resources.ImperialUnitGill,
            //    5.0, ImperialUnits.FluidOunce);

            //ImperialUnits.Pint = DerivedUnit.FromScale<Volume>("pt",
            //    Properties.Resources.ImperialUnitPint,
            //    20.0, ImperialUnits.FluidOunce);

            //ImperialUnits.Quart = DerivedUnit.FromScale<Volume>("qt",
            //    Properties.Resources.ImperialUnitQuart,
            //    40.0, ImperialUnits.FluidOunce);

            //ImperialUnits.Gallon = DerivedUnit.FromScale<Volume>("gal",
            //    Properties.Resources.ImperialUnitGallon,
            //    160.0, ImperialUnits.FluidOunce);
            //#endregion

            //#region British Apothecaries' Volume Units
            //ImperialUnits.Minim = DerivedUnit.FromScale<Volume>("♏",
            //    Properties.Resources.ImperialUnitMinim,
            //    59.193880208 + 1.0 / 3000000000.0,
            //    SIPrefix.Micro * MetricUnits.Litre);

            //// TODO: Possible problem with numerics!
            //ImperialUnits.FluidScruple = DerivedUnit.FromScale<Volume>("℈",
            //    Properties.Resources.ImperialUnitFluidScruple,
            //    20.0, ImperialUnits.Minim);

            //ImperialUnits.FluidDrachm = DerivedUnit.FromScale<Volume>("ʒ",
            //    Properties.Resources.ImperialUnitFluidDrachm,
            //    3.0, ImperialUnits.FluidScruple);
            //#endregion

            //#region Mass Units
            //ImperialUnits.Pound = DerivedUnit.FromScale<Mass>("lb",
            //    Properties.Resources.ImperialUnitPound,
            //    453.59237, SIUnits.Gramme);

            //ImperialUnits.Grain = DerivedUnit.FromScale<Mass>("gr",
            //    Properties.Resources.ImperialUnitGrain,
            //    1.0 / 7000.0, ImperialUnits.Pound);

            //ImperialUnits.Drachm = DerivedUnit.FromScale<Mass>("dr",
            //    Properties.Resources.ImperialUnitDrachm,
            //    1.0 / 256.0, ImperialUnits.Pound);

            //ImperialUnits.Ounce = DerivedUnit.FromScale<Mass>("oz",
            //    Properties.Resources.ImperialUnitOunce,
            //    1.0 / 16.0, ImperialUnits.Pound);

            //ImperialUnits.Stone = DerivedUnit.FromScale<Mass>("st",
            //    Properties.Resources.ImperialUnitStone,
            //    14.0, ImperialUnits.Pound);

            //ImperialUnits.Quarter = DerivedUnit.FromScale<Mass>("qtr",
            //    Properties.Resources.ImperialUnitQuarter,
            //    25.0, ImperialUnits.Pound);

            //ImperialUnits.Hundredweight = DerivedUnit.FromScale<Mass>("cwt",
            //    Properties.Resources.ImperialUnitHundredweight,
            //    112.0, ImperialUnits.Pound);

            //ImperialUnits.Ton = DerivedUnit.FromScale<Mass>("t (long)",
            //    Properties.Resources.ImperialUnitTon,
            //    2240.0, ImperialUnits.Pound);
            //#endregion
            #endregion
        }
        #endregion

        #region UCUM 4.4 §36 British Imperial lengths
        public static readonly Unit<Length> Inch;
        public static readonly Unit<Length> Foot;
        public static readonly Unit<Length> Rod;
        public static readonly Unit<Length> Chain;
        public static readonly Unit<Length> Link;
        public static readonly Unit<Length> Fathom;
        public static readonly Unit<Length> Pace;
        public static readonly Unit<Length> Yard;
        public static readonly Unit<Length> Mile;
        public static readonly Unit<Length> NauticalMile;
        public static readonly Unit<Velocity> Knot;
        public static readonly Unit<Area> Acre;
        #endregion

        #region UCUM 4.4 §38 British Imperial volumes
        public static readonly Unit<Volume> Gallon;
        public static readonly Unit<Volume> Peck;
        public static readonly Unit<Volume> Bushel;
        public static readonly Unit<Volume> Quart;
        public static readonly Unit<Volume> Pint;
        public static readonly Unit<Volume> Gill;
        public static readonly Unit<Volume> FluidOunce;
        public static readonly Unit<Volume> FluidDrachm;
        public static readonly Unit<Volume> Minim;
        #endregion


        //#region Maritime Length Units
        //public static readonly DerivedUnit<Length> Fathom;
        //public static readonly DerivedUnit<Length> Cable;
        //public static readonly DerivedUnit<Length> NauticalMile;
        //#endregion

        //#region Gunter's Survey Units
        //public static readonly DerivedUnit<Length> Link;
        //public static readonly DerivedUnit<Length> Rod;
        //#endregion

        //#region Area Units
        //public static readonly DerivedUnit<Area> Perch;
        //public static readonly DerivedUnit<Area> Rood;
        //public static readonly DerivedUnit<Area> Acre;
        //#endregion

        //#region Volume Units
        //public static readonly DerivedUnit<Volume> FluidOunce;
        //public static readonly DerivedUnit<Volume> Gill;
        //public static readonly DerivedUnit<Volume> Pint;
        //public static readonly DerivedUnit<Volume> Quart;
        //public static readonly DerivedUnit<Volume> Gallon;
        //#endregion

        //#region British Apothecaries' Volume Units
        //public static readonly DerivedUnit<Volume> Minim;
        //public static readonly DerivedUnit<Volume> FluidScruple;
        //public static readonly DerivedUnit<Volume> FluidDrachm;
        //#endregion

        //#region Mass Units
        //public static readonly DerivedUnit<Mass> Grain;
        //public static readonly DerivedUnit<Mass> Drachm;
        //public static readonly DerivedUnit<Mass> Ounce;
        //public static readonly DerivedUnit<Mass> Pound;
        //public static readonly DerivedUnit<Mass> Stone;
        //public static readonly DerivedUnit<Mass> Quarter;
        //public static readonly DerivedUnit<Mass> Hundredweight;
        //public static readonly DerivedUnit<Mass> Ton;
        //#endregion

        /// <summary>
        /// Disallow instances.
        /// </summary>
        private ImperialUnits() { }
    }
}
