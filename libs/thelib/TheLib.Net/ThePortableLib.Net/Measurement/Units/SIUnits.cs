/// <copyright file="SIUnits.cs" company="TheLib Team">
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

using The.Measurement.Dimensions;
using The.Measurement.Quantities;


namespace The.Measurement.Units {

    /// <summary>
    /// Provides the definition of SI base units and other commonly used units
    /// of the Système international d'unités.
    /// </summary>
    public sealed class SIUnits : UnitSystem<SIUnits> {

        /// <summary>
        /// Initialises the class.
        /// </summary>
        /// <remarks>
        /// The static constructor is guaranteed to be thread-safe according
        /// to http://msdn.microsoft.com/en-us/library/aa645612(VS.71).aspx.
        /// </remarks>
        static SIUnits() {
            SIUnits.One = new BaseUnit<Dimensionless>("1",
                Properties.Resources.SiUnitDimensionless,
                SIDimensions.None);

            #region SI Base Units
            SIUnits.Ampere = new BaseUnit<ElectricCurrent>("A",
                Properties.Resources.SiUnitAmpere, 
                SIDimensions.ElectricCurrent);

            SIUnits.Candela = new BaseUnit<LuminousIntensity>("cd",
                Properties.Resources.SiUnitCandela,
                SIDimensions.LuminousIntensity);

            SIUnits.Kelvin = new BaseUnit<Temperature>("K",
                Properties.Resources.SiUnitKelvin,
                SIDimensions.Temperature);

            SIUnits.Kilogramme = new BaseUnit<Mass>("kg",
                Properties.Resources.SiUnitKilogramme,
                SIDimensions.Mass);

            SIUnits.Metre = new BaseUnit<Length>("m",
                Properties.Resources.SiUnitMetre,
                SIDimensions.Length);

            SIUnits.Mole = new BaseUnit<AmountOfSubstance>("mol",
                Properties.Resources.SiUnitMole,
                SIDimensions.AmountOfSubstance);

            SIUnits.Second = new BaseUnit<Time>("s",
                Properties.Resources.SiUnitSecond,
                SIDimensions.Time);
            #endregion;

            #region Dervied SI Units
            #region Derived Units with Special Names
            // Initialise definitions according to 
            // http://en.wikipedia.org/wiki/International_System_of_Units.
            // Note that the order of the initialisations is important as some
            // derived units depend on derived units themselves.
            SIUnits.Hertz = ProductUnit.FromReciprocal<Frequency>(
                "Hz", Properties.Resources.SiUnitHertz,
                SIUnits.Second.Invert());

            SIUnits.Radian = ProductUnit.FromDivision<Angle>(
                "rad", Properties.Resources.SiUnitRadian,
                SIUnits.Metre, SIUnits.Metre);

            SIUnits.Steradian = ProductUnit.FromDivision<SolidAngle>(
                "sr", Properties.Resources.SiUnitSteradian,
                SIUnits.Metre.Pow(2), SIUnits.Metre.Pow(2));

            SIUnits.Newton = new AlternateUnit<Force>(
                "N", Properties.Resources.SiUnitNewton,
                SIUnits.Kilogramme * SIUnits.Metre / SIUnits.Second.Pow(2));

            SIUnits.Pascal = ProductUnit.FromDivision<Pressure>(
                "Pa", Properties.Resources.SiUnitPascal,
                SIUnits.Newton, SIUnits.Metre.Pow(2));

            SIUnits.Joule = ProductUnit.FromMultiplication<Energy>(
                "J", Properties.Resources.SiUnitJoule,
                SIUnits.Newton, SIUnits.Metre);

            SIUnits.Watt = ProductUnit.FromDivision<Power>(
                "W", Properties.Resources.SiUnitWatt,
                SIUnits.Joule, SIUnits.Second);

            SIUnits.Coulomb = ProductUnit.FromMultiplication<ElectricCharge>(
                "C", Properties.Resources.SiUnitCoulomb,
                SIUnits.Second, SIUnits.Ampere);

            SIUnits.Volt = ProductUnit.FromDivision<ElectricPotential>(
                "V", Properties.Resources.SiUnitVolt,
                SIUnits.Watt, SIUnits.Ampere);

            SIUnits.Farad = ProductUnit.FromDivision<ElectricCapacitance>(
                "F", Properties.Resources.SiUnitFarad,
                SIUnits.Coulomb, SIUnits.Volt);

            SIUnits.Ohm = ProductUnit.FromDivision<ElectricResistance>(
                "Ω", Properties.Resources.SiUnitOhm,
                SIUnits.Volt, SIUnits.Ampere);

            SIUnits.Siemens = ProductUnit.FromReciprocal<ElectricConductance>(
                "S", Properties.Resources.SiUnitSiemens,
                SIUnits.Ohm);

            SIUnits.Weber = ProductUnit.FromDivision<MagneticFlux>(
                "Wb", Properties.Resources.SiUnitWeber,
                SIUnits.Joule, SIUnits.Ampere);

            SIUnits.Tesla = new AlternateUnit<MagneticFluxDensity>(
                "T", Properties.Resources.SiUnitTesla,
                SIUnits.Volt * SIUnits.Second / SIUnits.Metre.Pow(2));

            SIUnits.Henry = new AlternateUnit<Inductance>(
                "H", Properties.Resources.SiUnitHenry,
                SIUnits.Volt * SIUnits.Second / SIUnits.Ampere);

            SIUnits.Lumen = ProductUnit.FromMultiplication<LuminousFlux>(
                "lm", Properties.Resources.SiUnitLumen,
                SIUnits.Candela, SIUnits.Steradian);

            SIUnits.Lux = ProductUnit.FromDivision<Illuminance>(
                "lx", Properties.Resources.SiUnitLux,
                SIUnits.Lumen, SIUnits.Metre.Pow(2));

            SIUnits.Becquerel = ProductUnit.FromReciprocal<Radioactivity>(
                "Bq", Properties.Resources.SiUnitBecquerel,
                SIUnits.Second);

            SIUnits.Gray = ProductUnit.FromDivision<AbsorbedDose>(
                "Gy", Properties.Resources.SiUnitGray,
                SIUnits.Joule, SIUnits.Kilogramme);

            SIUnits.Sievert = ProductUnit.FromDivision<EquivalentDose>(
                "Sv", Properties.Resources.SiUnitSievert,
                SIUnits.Joule, SIUnits.Kilogramme);

            SIUnits.Katal = ProductUnit.FromMultiplication<CatalyticActivity>(
                "kat", Properties.Resources.SiUnitKatal,
                SIUnits.Mole, SIUnits.Second);
            #endregion

            #region Derived Units without Special Name
            SIUnits.Gramme = new AlternateUnit<Mass>(
                "g", Properties.Resources.SiUnitGramme,
                (1.0 / SIPrefix.Kilo.Factor) * SIUnits.Kilogramme);

            SIUnits.SquareMetre = ProductUnit.FromPower<Area>(
                "m²", Properties.Resources.SiUnitSquareMetre,
                SIUnits.Metre, 2);

            SIUnits.CubicMetre = ProductUnit.FromPower<Volume>(
                "m³", Properties.Resources.SiUnitCubicMetre,
                SIUnits.Metre, 3);

            SIUnits.MetrePerSecond = new AlternateUnit<Velocity>(
                SIUnits.Metre / SIUnits.Second);

            SIUnits.CubicMetrePerSecond = new AlternateUnit<VolumetricFlow>(
                SIUnits.Metre.Pow(3) / SIUnits.Second);

            SIUnits.MetrePerSecondSquared = new AlternateUnit<Acceleration>(
                SIUnits.Metre / SIUnits.Second.Pow(2));

            SIUnits.MetrePerSecondCubed = new AlternateUnit<Jerk>(
                SIUnits.Metre / SIUnits.Second.Pow(3));

            SIUnits.MetrePerQuarticSecond = new AlternateUnit<Snap>(
                SIUnits.Metre / SIUnits.Second.Pow(4));

            SIUnits.MetrePerSecondSquared = new AlternateUnit<Acceleration>(
                SIUnits.Metre / SIUnits.Second.Pow(2));

            SIUnits.RadianPerSecond = new AlternateUnit<AngularVelocity>(
                SIUnits.Radian / SIUnits.Second);

            SIUnits.NewtonSecond = new AlternateUnit<Momentum>(
                SIUnits.Newton * SIUnits.Second);

            SIUnits.NewtonMetreSecond = new AlternateUnit<AngularMomentum>(
                SIUnits.Newton * SIUnits.Metre * SIUnits.Second);

            SIUnits.NewtonMetre = new AlternateUnit<Torque>(
                SIUnits.Newton * SIUnits.Metre);

            SIUnits.NewtonPerSecond = new AlternateUnit<Yank>(
                SIUnits.Newton / SIUnits.Second);

            SIUnits.ReciprocalMetre = new AlternateUnit<Wavenumber>(
                SIUnits.Metre.Invert());

            SIUnits.KilogrammePerSquareMetre = new AlternateUnit<AreaDensity>(
                SIUnits.Kilogramme / SIUnits.Metre.Pow(2));

            SIUnits.KilogrammePerCubicMetre = new AlternateUnit<Density>(
                SIUnits.Kilogramme / SIUnits.Metre.Pow(3));

            SIUnits.CubicMetrePerKilogramme = new AlternateUnit<SpecificVolume>(
                SIUnits.Metre.Pow(3) / SIUnits.Kilogramme);

            SIUnits.MolePerCubicMetre
                = new AlternateUnit<AmountOfSubstanceConcentration>(
                SIUnits.Mole / SIUnits.Metre.Pow(3));

            SIUnits.CubicMetrePerMole = new AlternateUnit<MolarVolume>(
                SIUnits.Metre.Pow(3) / SIUnits.Metre);

            SIUnits.JouleSecond = new AlternateUnit<Action>(
                SIUnits.Joule * SIUnits.Second);

            SIUnits.JoulePerKelvin = new AlternateUnit<HeatCapacity>(
                SIUnits.Joule / SIUnits.Kelvin);

            SIUnits.JoulePerKelvinMole = new AlternateUnit<MolarHeatCapacity>(
                SIUnits.Joule / (SIUnits.Kelvin * SIUnits.Mole));

            SIUnits.JoulePerKilogrammeKelvin 
                = new AlternateUnit<SpecificHeatCapacity>(
                SIUnits.Joule / (SIUnits.Kilogramme * SIUnits.Kelvin));

            SIUnits.JoulePerMole = new AlternateUnit<MolarEnergy>(
                SIUnits.Joule / SIUnits.Mole);

            SIUnits.JoulePerKilogramme = new AlternateUnit<SpecificEnergy>(
                SIUnits.Joule / SIUnits.Kilogramme);

            SIUnits.JoulePerCubicMetre = new AlternateUnit<EnergyDensity>(
                SIUnits.Joule / SIUnits.Metre.Pow(3));

            SIUnits.NewtonPerMetre = new AlternateUnit<SurfaceTension>(
                SIUnits.Newton / SIUnits.Metre);

            SIUnits.WattPerSquareMetre = new AlternateUnit<HeatFluxDensity>(
                SIUnits.Watt / SIUnits.Metre.Pow(2));

            SIUnits.WattPerMetreKelvin = new AlternateUnit<ThermalConductivity>(
                SIUnits.Watt / (SIUnits.Metre * SIUnits.Kelvin));

            SIUnits.SquareMetrePerSecond 
                = new AlternateUnit<KinematicViscosity>(
                SIUnits.Metre.Pow(2) / SIUnits.Second);

            SIUnits.PascalSecond = new AlternateUnit<DynamicViscosity>(
                SIUnits.Pascal / SIUnits.Second);

            SIUnits.CoulombPerSquareMetre
                = new AlternateUnit<ElectricDisplacementField>(
                SIUnits.Coulomb / SIUnits.Metre.Pow(2));

            SIUnits.CoulombPerCubicMetre
                = new AlternateUnit<ElectricChargeDensity>(
                SIUnits.Coulomb / SIUnits.Metre.Pow(3));

            SIUnits.AmperePerSquareMetre
                = new AlternateUnit<ElectricCurrentDensity>(
                SIUnits.Ampere / SIUnits.Metre.Pow(2));

            SIUnits.SiemensPerMetre = new AlternateUnit<Conductivity>(
                SIUnits.Siemens / SIUnits.Metre);

            SIUnits.SiemensSquareMetrePerMole 
                = new AlternateUnit<MolarConductivity>(
                (SIUnits.Siemens * SIUnits.Metre.Pow(2)) / SIUnits.Mole);

            SIUnits.FaradPerMetre = new AlternateUnit<Permittivity>(
                SIUnits.Farad / SIUnits.Metre);

            SIUnits.HenryPerMetre = new AlternateUnit<Permeability>(
                SIUnits.Henry / SIUnits.Metre);

            SIUnits.VoltPerMetre = new AlternateUnit<ElectricFieldStrength>(
                SIUnits.Volt / SIUnits.Metre);

            SIUnits.AmperePerMetre = new AlternateUnit<MagneticFieldStrength>(
                SIUnits.Ampere / SIUnits.Metre);

            SIUnits.CandelaPerSquareMetre = new AlternateUnit<Luminance>(
                SIUnits.Candela / SIUnits.Metre.Pow(2));

            SIUnits.LumenSecond = new AlternateUnit<LuminousEnergy>(
                SIUnits.Lumen * SIUnits.Second);

            SIUnits.LuxSecond = new AlternateUnit<LuminousExposure>(
                SIUnits.Lux * SIUnits.Second);

            SIUnits.CoulombPerKilogramme = new AlternateUnit<Exposure>(
                SIUnits.Coulomb / SIUnits.Kilogramme);

            SIUnits.GrayPerSecond = new AlternateUnit<AbsorbedDoseRate>(
                SIUnits.Gray / SIUnits.Second);

            SIUnits.OhmMetre = new AlternateUnit<Resistivity>(
                SIUnits.Ohm * SIUnits.Metre);
            #endregion
            #endregion
        }

        /// <summary>
        /// The dimensionless unit.
        /// </summary>
        public static readonly Unit<Dimensionless> One;

        #region SI Base Units
        /// <summary>
        /// The base SI unit (A) for electric current (I).
        /// </summary>
        public static readonly Unit<ElectricCurrent> Ampere;

        /// <summary>
        /// The base SI unit (cd) for luminous intensity (J).
        /// </summary>
        public static readonly Unit<LuminousIntensity> Candela;

        /// <summary>
        /// The base SI unit (K) for thermodynamic temperature (Θ).
        /// </summary>
        public static readonly Unit<Temperature> Kelvin;

        /// <summary>
        /// The base SI unit (kg) for mass (M).
        /// </summary>
        public static readonly Unit<Mass> Kilogramme;

        /// <summary>
        /// The base SI unit (m) for length (L).
        /// </summary>
        public static readonly Unit<Length> Metre;

        /// <summary>
        /// The base SI unit (mol) for an amount of substance (N).
        /// </summary>
        public static readonly Unit<AmountOfSubstance> Mole;

        /// <summary>
        /// The base SI unit (s) for time (T).
        /// </summary>
        public static readonly Unit<Time> Second;
        #endregion

        #region Dervied SI Units
        #region Derived Units with Special Names
        /// <summary>
        /// The SI unit for radioactivity (decays per unit time).
        /// </summary>
        public static readonly Unit<Radioactivity> Becquerel;

        /// <summary>
        /// The SI unit for electric charge.
        /// </summary>
        public static readonly Unit<ElectricCharge> Coulomb;

        /// <summary>
        /// The SI unit for electric capacitance.
        /// </summary>
        public static readonly Unit<ElectricCapacitance> Farad;

        /// <summary>
        /// The SI unit gramme (g).
        /// </summary>
        public static readonly Unit<Mass> Gramme;

        /// <summary>
        /// The SI unit for the absorbed dose (of ionising radiation).
        /// </summary>
        public static readonly Unit<AbsorbedDose> Gray;

        /// <summary>
        /// The SI unit for inductance.
        /// </summary>
        public static readonly Unit<Inductance> Henry;

        /// <summary>
        /// The SI unit (Hz) for frequency.
        /// </summary>
        public static readonly Unit<Frequency> Hertz;

        /// <summary>
        /// The SI unit for energy, work, and heat.
        /// </summary>
        public static readonly Unit<Energy> Joule;

        /// <summary>
        /// The SI unit for catalyctic activity.
        /// </summary>
        public static readonly Unit<CatalyticActivity> Katal;

        /// <summary>
        /// The SI unit for the luminous flux.
        /// </summary>
        public static readonly Unit<LuminousFlux> Lumen;

        /// <summary>
        /// The SI unit for illuminance.
        /// </summary>
        public static readonly Unit<Illuminance> Lux;

        /// <summary>
        /// The SI unit for force and weight.
        /// </summary>
        public static readonly Unit<Force> Newton;

        /// <summary>
        /// The SI unit for electrict resistance, impedance, reactance.
        /// </summary>
        public static readonly Unit<ElectricResistance> Ohm;

        /// <summary>
        /// The SI unit for pressure and stress.
        /// </summary>
        public static readonly Unit<Pressure> Pascal;

        /// <summary>
        /// The SI unit for angles.
        /// </summary>
        public static readonly Unit<Angle> Radian;

        /// <summary>
        /// The SI unit for electrical conductance.
        /// </summary>
        public static readonly Unit<ElectricConductance> Siemens;

        /// <summary>
        /// The SI unit for the equivalent dose (of ionising radiation).
        /// </summary>
        public static readonly Unit<EquivalentDose> Sievert;

        /// <summary>
        /// The SI unit for solid angles.
        /// </summary>
        public static readonly Unit<SolidAngle> Steradian;

        /// <summary>
        /// The SI unit for the magnetic field strength.
        /// </summary>
        public static readonly Unit<MagneticFluxDensity> Tesla;

        /// <summary>
        /// The SI unit for voltage, electrical potential difference, or
        /// electromotive force.
        /// </summary>
        public static readonly Unit<ElectricPotential> Volt;

        /// <summary>
        /// The SI unit for power.
        /// </summary>
        public static readonly Unit<Power> Watt;

        /// <summary>
        /// The SI unit for the magnetic flux.
        /// </summary>
        public static readonly Unit<MagneticFlux> Weber;
        #endregion

        #region Derived Units without Special Name
        /// <summary>
        /// The SI unit for areas.
        /// </summary>
        public static readonly Unit<Area> SquareMetre;

        /// <summary>
        /// The SI unit for volume.
        /// </summary>
        public static readonly Unit<Volume> CubicMetre;

        /// <summary>
        /// The SI unit for speed.
        /// </summary>
        public static readonly Unit<Velocity> MetrePerSecond;

        /// <summary>
        /// The SI unit for volumetric flow.
        /// </summary>
        public static readonly Unit<VolumetricFlow> CubicMetrePerSecond;

        /// <summary>
        /// The SI unit for acceleration.
        /// </summary>
        public static readonly Unit<Acceleration> MetrePerSecondSquared;

        /// <summary>
        /// The SI unit for jerk.
        /// </summary>
        public static readonly Unit<Jerk> MetrePerSecondCubed;

        /// <summary>
        /// The SI unit for snap.
        /// </summary>
        public static readonly Unit<Snap> MetrePerQuarticSecond;

        /// <summary>
        /// The SI unit for angular velocity.
        /// </summary>
        public static readonly Unit<AngularVelocity> RadianPerSecond;

        /// <summary>
        /// The SI unit for momentum or impulse.
        /// </summary>
        public static readonly Unit<Momentum> NewtonSecond;

        /// <summary>
        /// The SI unit for angular momentum.
        /// </summary>
        public static readonly Unit<AngularMomentum> NewtonMetreSecond;

        /// <summary>
        /// The SI unit for torque.
        /// </summary>
        public static readonly Unit<Torque> NewtonMetre;

        /// <summary>
        /// The SI unit for yank.
        /// </summary>
        public static readonly Unit<Yank> NewtonPerSecond;

        /// <summary>
        /// The SI unit for the spatial frequency of a wave.
        /// </summary>
        public static readonly Unit<Wavenumber> ReciprocalMetre;

        /// <summary>
        /// The SI unit for area density.
        /// </summary>
        public static readonly Unit<AreaDensity>
            KilogrammePerSquareMetre;

        /// <summary>
        /// The SI unit for density.
        /// </summary>
        public static readonly Unit<Density> KilogrammePerCubicMetre;

        /// <summary>
        /// The SI unit for the specific volume.
        /// </summary>
        public static readonly Unit<SpecificVolume>
            CubicMetrePerKilogramme;

        /// <summary>
        /// The SI unit for the concentration.
        /// </summary>
        public static readonly Unit<AmountOfSubstanceConcentration>
            MolePerCubicMetre;

        /// <summary>
        /// The SI unit for the molar volume.
        /// </summary>
        public static readonly Unit<MolarVolume> CubicMetrePerMole;

        /// <summary>
        /// The SI unit for action.
        /// </summary>
        public static readonly Unit<Action> JouleSecond;

        /// <summary>
        /// The SI unit for heat capacity.
        /// </summary>
        public static readonly Unit<HeatCapacity> JoulePerKelvin;

        /// <summary>
        /// The SI unit for the molar heat capacity.
        /// </summary>
        public static readonly Unit<MolarHeatCapacity>
            JoulePerKelvinMole;

        /// <summary>
        /// The SI unit for the specific heat capacity.
        /// </summary>
        public static readonly Unit<SpecificHeatCapacity>
            JoulePerKilogrammeKelvin;

        /// <summary>
        /// The SI unit for the molar energy.
        /// </summary>
        public static readonly Unit<MolarEnergy> JoulePerMole;

        /// <summary>
        /// The SI unit for the specific energy.
        /// </summary>
        public static readonly Unit<SpecificEnergy> JoulePerKilogramme;

        /// <summary>
        /// The SI unit for energy density.
        /// </summary>
        public static readonly Unit<EnergyDensity> JoulePerCubicMetre;

        /// <summary>
        /// The SI unit for surface tension.
        /// </summary>
        public static readonly Unit<SurfaceTension> NewtonPerMetre;

        /// <summary>
        /// The SI unit for heat flux density.
        /// </summary>
        public static readonly Unit<HeatFluxDensity> WattPerSquareMetre;

        /// <summary>
        /// The SI unit for thermal conductivity.
        /// </summary>
        public static readonly Unit<ThermalConductivity>
            WattPerMetreKelvin;

        /// <summary>
        /// The SI unit for kinematic viscosity.
        /// </summary>
        public static readonly Unit<KinematicViscosity>
            SquareMetrePerSecond;

        /// <summary>
        /// The SI unit for dynamic viscosity.
        /// </summary>
        public static readonly Unit<DynamicViscosity> PascalSecond;

        /// <summary>
        /// The SI unit for the electric displacement field.
        /// </summary>
        public static readonly Unit<ElectricDisplacementField>
            CoulombPerSquareMetre;

        /// <summary>
        /// The SI unit for electric charge density.
        /// </summary>
        public static readonly Unit<ElectricChargeDensity>
            CoulombPerCubicMetre;

        /// <summary>
        /// The SI unit for electric current density.
        /// </summary>
        public static readonly Unit<ElectricCurrentDensity>
            AmperePerSquareMetre;

        /// <summary>
        /// The SI unit for conductivity.
        /// </summary>
        public static readonly Unit<Conductivity> SiemensPerMetre;

        /// <summary>
        /// The SI unit for molar conductivity.
        /// </summary>
        public static readonly Unit<MolarConductivity>
            SiemensSquareMetrePerMole;

        /// <summary>
        /// The SI unit for permittivity.
        /// </summary>
        public static readonly Unit<Permittivity> FaradPerMetre;

        /// <summary>
        /// The SI unit for permeability.
        /// </summary>
        public static readonly Unit<Permeability> HenryPerMetre;

        /// <summary>
        /// The SI unit for electric field strength.
        /// </summary>
        public static readonly Unit<ElectricFieldStrength> VoltPerMetre;

        /// <summary>
        /// The SI unit for magnetic field strength.
        /// </summary>
        public static readonly Unit<MagneticFieldStrength>
            AmperePerMetre;

        /// <summary>
        /// The SI unit for luminance.
        /// </summary>
        public static readonly Unit<Luminance> CandelaPerSquareMetre;

        /// <summary>
        /// The SI unit for luminous energy.
        /// </summary>
        public static readonly Unit<LuminousEnergy> LumenSecond;

        /// <summary>
        /// The SI unit for luminous exposure.
        /// </summary>
        public static readonly Unit<LuminousExposure> LuxSecond;

        /// <summary>
        /// The SI unit for exposure.
        /// </summary>
        public static readonly Unit<Exposure> CoulombPerKilogramme;

        /// <summary>
        /// The SI unit for the absorbed dose rate.
        /// </summary>
        public static readonly Unit<AbsorbedDoseRate> GrayPerSecond;

        /// <summary>
        /// The SI unit for resistivity.
        /// </summary>
        public static readonly Unit<Resistivity> OhmMetre;
        #endregion
        #endregion

        /// <summary>
        /// Disallow instances.
        /// </summary>
        private SIUnits() { }
    }
}
