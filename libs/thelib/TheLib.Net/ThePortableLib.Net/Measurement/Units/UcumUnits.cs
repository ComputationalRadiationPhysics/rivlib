/// <copyright file="UcumUnits.cs" company="TheLib Team">
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
    /// Provides all units as defined in the The Unified Code for Units of 
    /// Measure Specification (http://unitsofmeasure.org/).
    /// </summary>
    /// <remarks>
    /// This class contains units and symbols according to version 1.8.2 of the
    /// specification.
    /// </remarks>
    public sealed class UcumUnits : UnitSystem<UcumUnits> {

        // TODO: Documentation
        static UcumUnits() {

            // TODO: Missing units

            #region 4.3 §31 other units from ISO 1000, ISO 2955 and ANSI X3.50
            UcumUnits.Minute = new AlternateUnit<Time>(
                "m", "TODO",
                60 * SIUnits.Second);

            UcumUnits.Hour = new AlternateUnit<Time>(
                "h", "TODO",
                60 * UcumUnits.Minute);

            UcumUnits.Day = new AlternateUnit<Time>(
                "d", "TODO",
                24 * UcumUnits.Hour);

            UcumUnits.TropicalYear = new AlternateUnit<Time>(
                "a_t", "TODO",
                365.24219 * UcumUnits.Day);

            UcumUnits.MeanJulianYear = new AlternateUnit<Time>(
                "a_j", "TODO",
                365.25 * UcumUnits.Day);

            UcumUnits.MeanGregorianYear = new AlternateUnit<Time>(
                "a_g", "TODO",
                365.2425 * UcumUnits.Day);

            //year  time  a  a   ANN   no 1  a_j   
            UcumUnits.Year = new BaseUnit<Time>(
                "y", "TODO", SIDimensions.Time);

            UcumUnits.Week = new AlternateUnit<Time>(
                "wk", "TODO",
                7 * UcumUnits.Day);

            UcumUnits.SynodalMonth = new AlternateUnit<Time>(
                "mo_s", "TODO",
                29.53059 * UcumUnits.Day);

            UcumUnits.MeanJulianMonth = new AlternateUnit<Time>(
                "mo_j", "TODO", 
                UcumUnits.MeanJulianYear / 12);

            UcumUnits.MeanGregorianMonth = new AlternateUnit<Time>(
                "mo_g", "TODO",
                UcumUnits.MeanGregorianYear / 12);

            //month  time  mo  mo   MO   no 1  mo_j   
            UcumUnits.Month = new BaseUnit<Time>(
                "mo", "TODO", SIDimensions.Time);
            #endregion

        }

        #region §27 prefixes
        /// <summary>
        /// The SI prefix "Yotta" which represents 1E24.
        /// </summary>
        [UcumSymbol("Y", "Y", "YA")]
        public static SIPrefix Yotta {
            get {
                return SIPrefix.Yotta;
            }
        }

        /// <summary>
        /// The SI prefix "Zetta" which represents 1E21.
        /// </summary>
        [UcumSymbol("Z", "Z", "ZA")]
        public static SIPrefix Zetta {
            get {
                return SIPrefix.Zetta;
            }
        }

        /// <summary>
        /// The SI prefix "Exa" which represents 1E18.
        /// </summary>
        [UcumSymbol("E", "E", "EX")]
        public static SIPrefix Exa {
            get {
                return SIPrefix.Exa;
            }
        }

        /// <summary>
        /// The SI prefix "Peta" which represents 1E15.
        /// </summary>
        [UcumSymbol("P", "P", "PT")]
        public static SIPrefix Peta {
            get {
                return SIPrefix.Peta;
            }
        }

        /// <summary>
        /// The SI prefix "Tera" which represents 1E12.
        /// </summary>
        [UcumSymbol("T", "T", "TR")]
        public static SIPrefix Tera {
            get {
                return SIPrefix.Tera;
            }
        }

        /// <summary>
        /// The SI prefix "Giga" which represents 1E9.
        /// </summary>
        [UcumSymbol("G", "G", "GA")]
        public static SIPrefix Giga {
            get {
                return SIPrefix.Giga;
            }
        }

        /// <summary>
        /// The SI prefix "Mega" which represents 1E6.
        /// </summary>
        [UcumSymbol("M", "M", "MA")]
        public static SIPrefix Mega {
            get {
                return SIPrefix.Mega;
            }
        }

        /// <summary>
        /// The SI prefix "Kilo" which represents 1E3.
        /// </summary>
        [UcumSymbol("k", "k", "K")]
        public static SIPrefix Kilo {
            get {
                return SIPrefix.Kilo;
            }
        }

        /// <summary>
        /// The SI prefix "Hecto" which represents 1E2.
        /// </summary>
        [UcumSymbol("h", "h", "H")]
        public static SIPrefix Hecto {
            get {
                return SIPrefix.Hecto;
            }
        }

        /// <summary>
        /// The SI prefix "Deca" which represents 1E1.
        /// </summary>
        [UcumSymbol("da", "da", "DA")]
        public static SIPrefix Deca {
            get {
                return SIPrefix.Deca;
            }
        }

        /// <summary>
        /// The SI prefix "Deci" which represents 1E-1.
        /// </summary>
        [UcumSymbol("d", "d", "D")]
        public static SIPrefix Deci {
            get {
                return SIPrefix.Deci;
            }
        }

        /// <summary>
        /// The SI prefix "Centi" which represents 1E-2.
        /// </summary>
        [UcumSymbol("c", "c", "C")]
        public static SIPrefix Centi {
            get {
                return SIPrefix.Centi;
            }
        }

        /// <summary>
        /// The SI prefix "Milli" which represents 1E-3.
        /// </summary>
        [UcumSymbol("m", "m", "M")]
        public static SIPrefix Milli {
            get {
                return SIPrefix.Milli;
            }
        }

        /// <summary>
        /// The SI prefix "Micro" which represents 1E-6.
        /// </summary>
        [UcumSymbol("μ", "u", "U")]
        public static SIPrefix Micro {
            get {
                return SIPrefix.Micro;
            }
        }

        /// <summary>
        /// The SI prefix "Nano" which represents 1E9.
        /// </summary>
        [UcumSymbol("n", "n", "N")]
        public static SIPrefix Nano {
            get {
                return SIPrefix.Nano;
            }
        }

        /// <summary>
        /// The SI prefix "Pico" which represents 1E-12.
        /// </summary>
        [UcumSymbol("p", "p", "P")]
        public static SIPrefix Pico {
            get {
                return SIPrefix.Pico;
            }
        }

        /// <summary>
        /// The SI prefix "Femto" which represents 1E-15.
        /// </summary>
        [UcumSymbol("f", "f", "F")]
        public static SIPrefix Femto {
            get {
                return SIPrefix.Femto;
            }
        }

        /// <summary>
        /// The SI prefix "Atto" which represents 1E-18.
        /// </summary>
        [UcumSymbol("a", "a", "A")]
        public static SIPrefix Atto {
            get {
                return SIPrefix.Atto;
            }
        }

        /// <summary>
        /// The SI prefix "Zepto" which represents  1E-21.
        /// </summary>
        [UcumSymbol("z", "z", "ZO")]
        public static SIPrefix Zepto {
            get {
                return SIPrefix.Zepto;
            }
        }

        /// <summary>
        /// The SI prefix "Yocto" which represents 1E-24.
        /// </summary>
        [UcumSymbol("y", "y", "YO")]
        public static SIPrefix Yocto {
            get {
                return SIPrefix.Yocto;
            }
        }
        #endregion

        #region §28 base units
        [UcumSymbol("m", "m", "M")]
        public static Unit<Length> Metre {
            get {
                return SIUnits.Metre;
            }
        }

        [UcumSymbol("s", "s", "S")]
        public static Unit<Time> Second {
            get {
                return SIUnits.Second;
            }
        }

        [UcumSymbol("g", "g", "G")]
        public static Unit<Mass> Gramme {
            get {
                return SIUnits.Gramme;
            }
        }

        [UcumSymbol("rad", "rad", "RAD")]
        public static Unit<Angle> Radian {
            get {
                return SIUnits.Radian;
            }
        }

        [UcumSymbol("K", "K", "K")]
        public static Unit<Temperature> Kelvin {
            get {
                return SIUnits.Kelvin;
            }
        }

        [UcumSymbol("C", "C", "C")]
        public static Unit<ElectricCharge> Coulomb {
            get {
                return SIUnits.Coulomb;
            }
        }

        [UcumSymbol("cd", "cd", "CD")]
        public static Unit<LuminousIntensity> Candela {
            get {
                return SIUnits.Candela;
            }
        }
        #endregion

        #region §29 dimensionless units
        //the number ten for arbitrary powers  10n   10*   10*   no 10  1   
        //the number ten for arbitrary powers  10n   10^   10^   no 10  1   
        //the number pi  π  [pi]   [PI]   no π  1   
        //percent  %  %   %   no 1  10*-2   
        //parts per thousand  ppth  [ppth]   [PPTH]   no 1  10*-3   
        //parts per million  ppm  [ppm]   [PPM]   no 1  10*-6   
        //parts per billion  ppb  [ppb]   [PPB]   no 1  10*-9   
        //parts per trillion  pptr  [pptr]   [PPTR]   no 1  10*-12   
        #endregion

        #region §30 SI units
        [UcumSymbol("mol", "mol", "MOL")]
        public static Unit<AmountOfSubstance> Mole {
            get {
                return SIUnits.Mole;
            }
        }

        [UcumSymbol("sr", "sr", "SR")]
        public static Unit<SolidAngle> Steradian {
            get {
                return SIUnits.Steradian;
            }
        }

        [UcumSymbol("Hz", "Hz", "HZ")]
        public static Unit<Frequency> Hertz {
            get {
                return SIUnits.Hertz;
            }
        }

        [UcumSymbol("N", "N", "N")]
        public static Unit<Force> Newton {
            get {
                return SIUnits.Newton;
            }
        }

        [UcumSymbol("Pa", "Pa", "PAL")]
        public static Unit<Pressure> Pascal {
            get {
                return SIUnits.Pascal;
            }
        }

        [UcumSymbol("J", "J", "J")]
        public static Unit<Energy> Joule {
            get {
                return SIUnits.Joule;
            }
        }

        [UcumSymbol("W", "W", "W")]
        public static Unit<Power> Watt {
            get {
                return SIUnits.Watt;
            }
        }

        [UcumSymbol("A", "A", "A")]
        public static Unit<ElectricCurrent> Ampere {
            get {
                return SIUnits.Ampere;
            }
        }

        [UcumSymbol("V", "V", "V")]
        public static Unit<ElectricPotential> Volt {
            get {
                return SIUnits.Volt;
            }
        }

        [UcumSymbol("F", "F", "F")]
        public static Unit<ElectricCapacitance> Farad {
            get {
                return SIUnits.Farad;
            }
        }

        [UcumSymbol("Ω", "Ohm", "OHM")]
        public static Unit<ElectricResistance> Ohm {
            get {
                return SIUnits.Ohm;
            }
        }

        [UcumSymbol("S", "S", "SIE")]
        public static Unit<ElectricConductance> Siemens {
            get {
                return SIUnits.Siemens;
            }
        }

        [UcumSymbol("Wb", "Wb", "WBE")]
        public static Unit<MagneticFlux> Weber {
            get {
                return SIUnits.Weber;
            }
        }

        [UcumSymbol("°C", "Cel", "CEL")]
        public static Unit<Temperature> DegreeCelsius {
            get {
                return MetricUnits.Celsius;
            }
        }

        [UcumSymbol("T", "T", "T")]
        public static Unit<MagneticFluxDensity> Tesla {
            get {
                return SIUnits.Tesla;
            }
        }

        [UcumSymbol("H", "H", "H")]
        public static Unit<Inductance> Henry {
            get {
                return SIUnits.Henry;
            }
        }

        [UcumSymbol("lm", "lm", "LM")]
        public static Unit<LuminousFlux> Lumen {
            get {
                return SIUnits.Lumen;
            }
        }

        [UcumSymbol("lx", "lx", "LX")]
        public static Unit<Illuminance> Lux {
            get {
                return SIUnits.Lux;
            }
        }

        [UcumSymbol("Bq", "Bq", "BQ")]
        public static Unit<Radioactivity> Becquerel {
            get {
                return SIUnits.Becquerel;
            }
        }

        [UcumSymbol("Gy", "Gy", "GY")]
        public static Unit<AbsorbedDose> Gray {
            get {
                return SIUnits.Gray;
            }
        }

        [UcumSymbol("Sv", "Sv", "SV")]
        public static Unit<EquivalentDose> Sievert {
            get {
                return SIUnits.Sievert;
            }
        }
        #endregion

        #region §31 other units from ISO 1000, ISO 2955 and ANSI X3.50
        //gon, grade  plane angle  □g   gon   GON   no 0.9  deg   
        //degree  plane angle  °  deg   DEG   no 2  [pi].rad/360   
        //minute  plane angle  '  '   '   no 1  deg/60   
        //second  plane angle  ''  ''   ''   no 1  '/60   

        [UcumSymbol("l", "l", "L")]
        public static Unit<Volume> Litre {
            get {
                return MetricUnits.Litre;
            }
        }

        [UcumSymbol("a", "ar", "AR")]
        public static Unit<Area> Are {
            get {
                return MetricUnits.Are;
            }
        }

        [UcumSymbol("min", "min", "MIN")]
        public static readonly Unit<Time> Minute;

        [UcumSymbol("h", "h", "HR")]
        public static readonly Unit<Time> Hour;

        [UcumSymbol("d", "d", "D")]
        public static readonly Unit<Time> Day;

        [UcumSymbol("a_t", "ANN_T")]
        public static readonly Unit<Time> TropicalYear;

        [UcumSymbol("a_j", "ANN_J")]
        public static readonly Unit<Time> MeanJulianYear;

        [UcumSymbol("a_g", "ANN_G")]
        public static readonly Unit<Time> MeanGregorianYear;

        [UcumSymbol("a", "a", "ANN")]
        public static readonly Unit<Time> Year;

        [UcumSymbol("wk", "wk", "WK")]
        public static readonly Unit<Time> Week;

        [UcumSymbol("mo_s", "MO_S")]
        public static readonly Unit<Time> SynodalMonth;

        [UcumSymbol("mo_j", "MO_J")]
        public static readonly Unit<Time> MeanJulianMonth;

        [UcumSymbol("mo_g", "MO_G")]
        public static readonly Unit<Time> MeanGregorianMonth;

        [UcumSymbol("mo", "MO")]
        public static readonly Unit<Time> Month;

        [UcumSymbol("t", "t", "TNE")]
        public static Unit<Mass> Tonne {
            get {
                return MetricUnits.Tonne;
            }
        }

        [UcumSymbol("bar", "bar", "BAR")]
        public static Unit<Pressure> Bar {
            get {
                return MetricUnits.Bar;
            }
        }

        //unified atomic mass unit  mass  u  u   AMU   yes 1.6605402 × 10-24   g   
        //electronvolt  energy  eV  eV   EV   yes 1  [e].V   
        //astronomic unit  length  AU  AU   ASU   no 149597.870  Mm   
        //parsec  length  pc  pc   PRS   yes 3.085678 × 1016   m   
        #endregion

        #region §32 natural units
        //velocity of light  velocity  c   [c]   [C]   yes 299792458  m/s   
        //Planck constant  action  h   [h]   [H]   yes 6.6260755 × 10-24   J.s   
        //Boltzmann constant  (unclassified)  k   [k]   [K]   yes 1.380658 × 10-23   J/K   
        //permittivity of vacuum  electric permittivity  ε0   [eps_0]   [eps_0]   yes 8.854187817 × 10-12   F/m   
        //permeability of vacuum  magnetic permeability  μ0   [mu_0]   [mu_0]   yes 1  4.[pi].10*-7.N/A2   
        //elementary charge  electric charge  e   [e]   [E]   yes 1.60217733 × 10-19   C   
        //electron mass  mass  me   [m_e]   [M_E]   yes 9.1093897 × 10-28   g   
        //proton mass  mass  mp   [m_p]   [M_P]   yes 1.6726231 × 10-24   g   
        //Newtonian constant of gravitation  (unclassified)  G   [G]   [GC]   yes 6.67259 × 10-11   m3.kg-1.s-2   
        //standard acceleration of free fall  acceleration  gn   [g]   [G]   yes 9.80665  m/s2   
        //standard atmosphere  pressure  atm  atm   ATM   no 101325  Pa   
        //light-year  length  l.y.  [ly]   [LY]   yes 1  [c].a_j   
        //gram-force  force  gf  gf   GF   yes 1  g.[g]   
        //pound force  force  lbf  [lbf_av]   [LBF_AV]   no 1  [lb_av].[g]   
        #endregion

        #region §33 CGS units
        //Kayser  lineic number  K  Ky   KY   yes 1  cm-1   
        //Gal  acceleration  Gal  Gal   GL   yes 1  cm/s2   
        //dyne  force  dyn  dyn   DYN   yes 1  g.cm/s2   
        //erg  energy  erg  erg   ERG   yes 1  dyn.cm   
        //Poise  dynamic viscosity  P  P   P   yes 1  dyn.s/cm2   
        //Biot  electric current  Bi  Bi   BI   yes 10  A   
        //Stokes  kinematic viscosity  St  St   ST   yes 1  cm2/s   
        //Maxwell  flux of magnetic induction  Mx  Mx   MX   yes 1 × 10-8   Wb   
        //Gauss  magnetic flux density  Gs, G  G   GS   yes 1 × 10-4   T   
        //Oersted  magnetic field intensity  Oe  Oe   OE   yes 250  /[pi].A/m   
        //Gilbert  magnetic tension  Gb  Gb   GB   yes 1  Oe.cm   
        //stilb  lum. intensity density  sb  sb   SB   yes 1  cd/cm2   
        //Lambert  brightness  L  Lmb   LMB   yes 1  cd/cm2/[pi]   
        //phot  illuminance  ph  ph   PHT   yes 1 × 10-4   lx   
        //Curie  radioactivity  Ci  Ci   CI   yes 3.7 × 1010   Bq   
        //Roentgen  ion dose  R  R   ROE   yes 2.58 × 10-4   C/kg   
        //radiation absorbed dose  energy dose  RAD  RAD   [RAD]   yes 100  erg/g   
        //radiation equivalent man  dose equivalent  REM  REM   [REM]   yes 1  RAD   
        #endregion

        #region §34 international customary units
        //inch  length  [in_i]   [IN_I]   no 2.54  cm   
        //foot  length  [ft_i]   [FT_I]   no 12  [in_i]   
        //yard  length  [yd_i]   [YD_I]   no 3  [ft_i]   
        //statute mile  length  [mi_i]   [MI_I]   no 5280  [ft_i]   
        //fathom  depth of water  [fth_i]   [FTH_I]   no 6  [ft_i]   
        //nautical mile  length  [nmi_i]   [NMI_I]   no 1852  m   
        //knot  velocity  [kn_i]   [KN_I]   no 1  [nmi_i]/h   
        //square inch  area  [sin_i]   [SIN_I]   no 1  [in_i]2   
        //square foot  area  [sft_i]   [SFT_I]   no 1  [ft_i]2   
        //square yard  area  [syd_i]   [SYD_I]   no 1  [yd_i]2   
        //cubic inch  volume  [cin_i]   [CIN_I]   no 1  [in_i]3   
        //cubic foot  volume  [cft_i]   [CFT_I]   no 1  [ft_i]3   
        //cubic yard  volume  [cyd_i]   [CYD_I]   no 1  [yd_i]3   
        //board foot  volume  [bf_i]   [BF_I]   no 144  [in_i]3   
        //cord  volume  [cr_i]   [CR_I]   no 128  [ft_i]3   
        //mil  length  [mil_i]   [MIL_I]   no 1 × 10-3   [in_i]   
        //circular mil  area  [cml_i]   [CML_I]   no 1  [pi]/4.[mil_i]2   
        //hand  height of horses  [hd_i]   [HD_I]   no 4  [in_i]   
        #endregion

        #region §35 U.S. survey lengths
        //foot  length  [ft_us]   [FT_US]   no 1200  m/3937   
        //yard  length  [yd_us]   [YD_US]   no 3  [ft_us]   
        //inch  length  [in_us]   [IN_US]   no 1  [ft_us]/12   
        //rod  length  [rd_us]   [RD_US]   no 16.5  [ft_us]   
        //Gunter's chain, Surveyor's chain  length  [ch_us]   [CH_US]   no 4  [rd_us]   
        //link for Gunter's chain  length  [lk_us]   [LK_US]   no 1  [ch_us]/100   
        //Ramden's chain, Engineer's chain  length  [rch_us]   [RCH_US]   no 100  [ft_us]   
        //link for Ramden's chain  length  [rlk_us]   [RLK_US]   no 1  [rch_us]/100   
        //fathom  length  [fth_us]   [FTH_US]   no 6  [ft_us]   
        //furlong  length  [fur_us]   [FUR_US]   no 40  [rd_us]   
        //mile  length  [mi_us]   [MI_US]   no 8  [fur_us]   
        //acre  area  [acr_us]   [ACR_US]   no 160  [rd_us]2   
        //square rod  area  [srd_us]   [SRD_US]   no 1  [rd_us]2   
        //square mile  area  [smi_us]   [SMI_US]   no 1  [mi_us]2   
        //section  area  [sct]   [SCT]   no 1  [mi_us]2   
        //township  area  [twp]   [TWP]   no 36  [sct]   
        //mil  length  [mil_us]   [MIL_US]   no 1 × 10-3   [in_us]   
        #endregion

        #region §36 British Imperial lengths
        [UcumSymbol("[in_br]", "[IN_BR]")]
        public static Unit<Length> BritishImperialInch {
            get {
                return ImperialUnits.Inch;
            }
        }

        [UcumSymbol("[ft_br]", "[FT_BR]")]
        public static Unit<Length> BritishImperialFoot {
            get {
                return ImperialUnits.Foot;
            }
        }

        [UcumSymbol("rd_br]", "[RD_BR]")]
        public static Unit<Length> BritishImperialRod {
            get {
                return ImperialUnits.Rod;
            }
        }

        [UcumSymbol("[ch_br]", "[CH_BR]")]
        public static Unit<Length> BritishImperialChain {
            get {
                return ImperialUnits.Chain;
            }
        }

        [UcumSymbol("[lk_br]", "[LK_BR]")]
        public static Unit<Length> BritishImperialLink {
            get {
                return ImperialUnits.Link;
            }
        }

        [UcumSymbol("[fth_br]", "[FTH_BR]")]
        public static Unit<Length> BritishImperialFathom {
            get {
                return ImperialUnits.Fathom;
            }
        }

        [UcumSymbol("[pc_br]", "[PC_BR]")]
        public static Unit<Length> BritishImperialPace {
            get {
                return ImperialUnits.Pace;
            }
        }

        [UcumSymbol("[yd_br]", "[YD_BR]")]
        public static Unit<Length> BritishImperialYard {
            get {
                return ImperialUnits.Yard;
            }
        }

        [UcumSymbol("[mi_br]", "[MI_BR]")]
        public static Unit<Length> BritishImperialMile {
            get {
                return ImperialUnits.Mile;
            }
        }

        [UcumSymbol("[nmi_br]", "[NMI_BR]")]
        public static Unit<Length> BritishImperialNauticalMile {
            get {
                return ImperialUnits.NauticalMile;
            }
        }

        [UcumSymbol("[kn_br]", "[KN_BR]")]
        public static Unit<Velocity> BritishImperialKnot {
            get {
                return ImperialUnits.Knot;
            }
        }

        [UcumSymbol("[acr_br]", "[ACR_BR]")]
        public static Unit<Area> BritishImperialAcre {
            get {
                return ImperialUnits.Acre;
            }
        }
        #endregion

        #region §37 U.S. volumes
        //Queen Anne's wine gallon  fluid volume  [gal_us]   [GAL_US]   no 231  [in_i]3   
        //barrel  fluid volume  [bbl_us]   [BBL_US]   no 42  [gal_us]   
        //quart  fluid volume  [qt_us]   [QT_US]   no 1  [gal_us]/4   
        //pint  fluid volume  [pt_us]   [PT_US]   no 1  [qt_us]/2   
        //gill  fluid volume  [gil_us]   [GIL_US]   no 1  [pt_us]/4   
        //fluid ounce  fluid volume  [foz_us]   [FOZ_US]   no 1  [gil_us]/4   
        //fluid dram  fluid volume  [fdr_us]   [FDR_US]   no 1  [foz_us]/8   
        //minim  fluid volume  [min_us]   [MIN_US]   no 1  [fdr_us]/60   
        //cord  fluid volume  [crd_us]   [CRD_US]   no 128  [ft_i]3   
        //bushel  dry volume  [bu_us]   [BU_US]   no 2150.42  [in_i]3   
        //historical winchester gallon  dry volume  [gal_wi]   [GAL_WI]   no 1  [bu_us]/8   
        //peck  dry volume  [pk_us]   [PK_US]   no 1  [bu_us]/4   
        //dry quart  dry volume  [dqt_us]   [DQT_US]   no 1  [pk_us]/8   
        //dry pint  dry volume  [dpt_us]   [DPT_US]   no 1  [dqt_us]/2   
        //tablespoon  volume  [tbs_us]   [TBS_US]   no 1  [foz_us]/2   
        //teaspoon  volume  [tsp_us]   [TSP_US]   no 1  [tbs_us]/3   
        //cup  volume  [cup_us]   [CUP_US]   no 16  [tbs_us]   
        #endregion

        #region §38 British Imperial volumes
        public Unit<Volume> BritishImperialGallon {
            get {
                return ImperialUnits.Gallon;
            }
        }

        public Unit<Volume> BritishImperialPeck {
            get {
                return ImperialUnits.Peck;
            }
        }

        public Unit<Volume> BritishImperialBushel {
            get {
                return ImperialUnits.Bushel;
            }
        }

        public Unit<Volume> BritishImperialQuart {
            get {
                return ImperialUnits.Quart;
            }
        }

        public Unit<Volume> BritishImperialPint {
            get {
                return ImperialUnits.Pint;
            }
        }

        public Unit<Volume> BritishImperialGill {
            get {
                return ImperialUnits.Gill;
            }
        }

        public Unit<Volume> BritishImperialFluidOunce {
            get {
                return ImperialUnits.FluidOunce;
            }
        }

        public Unit<Volume> BritishImperialFluidDrachm {
            get {
                return ImperialUnits.FluidDrachm;
            }
        }

        public Unit<Volume> BritishImperialMinim {
            get {
                return ImperialUnits.Minim;
            }
        }
        #endregion

        #region §39 avoirdupois weights
        //grain  mass  [gr]   [GR]   no 64.79891  mg   
        //pound  mass  [lb_av]   [LB_AV]   no 7000  [gr]   
        //ounce  mass  [oz_av]   [OZ_AV]   no 1  [lb_av]/16   
        //dram  mass  [dr_av]   [DR_AV]   no 1  [oz_av]/16   
        //short hundredweight, U.S. hundredweight  mass  [scwt_av]   [SCWT_AV]   no 100  [lb_av]   
        //long hunderdweight, British hundredweight  mass  [lcwt_av]   [LCWT_AV]   no 112  [lb_av]   
        //short ton, U.S. ton  mass  [ston_av]   [STON_AV]   no 20  [scwt_av]   
        //long ton, British ton  mass  [lton_av]   [LTON_AV]   no 20  [lcwt_av]   
        //stone, British stone  mass  [stone_av]   [STONE_AV]   no 14  [lb_av]   
        #endregion

        #region §40 troy weights
        //pennyweight  mass  [pwt_tr]   [PWT_TR]   no 24  [gr]   
        //ounce  mass  [oz_tr]   [OZ_TR]   no 20  [pwt_tr]   
        //pound  mass  [lb_tr]   [LB_TR]   no 12  [oz_tr]   
        #endregion

        #region §41 apothecaries' weights
        //scruple  mass  [sc_ap]   [SC_AP]   no 20  [gr]   
        //dram, drachm  mass  [dr_ap]   [DR_AP]   no 3  [sc_ap]   
        //ounce  mass  [oz_ap]   [OZ_AP]   no 8  [dr_ap]   
        //pound  mass  [lb_ap]   [LB_AP]   no 12  [oz_ap]   
        #endregion

        #region §42 typesetter's lengths
        //line  length  [lne]   [LNE]   no 1  [in_i]/12   
        //point  length  [pnt]   [PNT]   no 1  [lne]/6   
        //pica  length  [pca]   [PCA]   no 12  [pnt]   
        //Printer's point  length  [pnt_pr]   [PNT_PR]   no 0.013837  [in_i]   
        //Printer's pica  length  [pca_pr]   [PCA_PR]   no 12  [pnt_pr]   
        //pied, French foot  length  [pied]   [PIED]   no 32.48  cm   
        //pouce, French inch  length  [pouce]   [POUCE]   no 1  [pied]/12   
        //ligne, French line  length  [ligne]   [LIGNE]   no 1  [pouce]/12   
        //didot, Didot's point  length  [didot]   [DIDOT]   no 1  [ligne]/6   
        //cicero, Didot's pica  length  [cicero]   [CICERO]   no 12  [didot]   
        #endregion

        #region §43 legacy units for heat and temperature
        //degree Fahrenheit  temperature  °F  [degF]   [DEGF]   no •  degf(5 K/9)   
        //calorie at 15 °C  energy  cal15°C   cal_[15]   CAL_[15]   yes 4.18580  J   
        //calorie at 20 °C  energy  cal20°C   cal_[20]   CAL_[20]   yes 4.18190  J   
        //mean calorie  energy  calm   cal_m   CAL_M   yes 4.19002  J   
        //international table calorie  energy  calIT   cal_IT   CAL_IT   yes 4.1868  J   
        //thermochemical calorie  energy  calth   cal_th   CAL_TH   yes 4.184  J   
        //calorie  energy  cal  cal   CAL   yes 1  cal_th   
        //nutrition label Calories  energy  Cal  [Cal]   [CAL]   no 1  kcal_th   
        //British thermal unit at 39 °F  energy  Btu39°F   [Btu_39]   [BTU_39]   no 1.05967  kJ   
        //British thermal unit at 59 °F  energy  Btu59°F   [Btu_59]   [BTU_59]   no 1.05480  kJ   
        //British thermal unit at 60 °F  energy  Btu60°F   [Btu_60]   [BTU_60]   no 1.05468  kJ   
        //mean British thermal unit  energy  Btum   [Btu_m]   [BTU_M]   no 1.05587  kJ   
        //international table British thermal unit  energy  BtuIT   [Btu_IT]   [BTU_IT]   no 1.05505585262  kJ   
        //thermochemical British thermal unit  energy  Btuth   [Btu_th]   [BTU_TH]   no 1.054350  kJ   
        //British thermal unit  energy  btu  [Btu]   [BTU]   no 1  [Btu_th]   
        //horsepower  power    [HP]   [HP]   no 550  [ft_i].[lbf_av]/s   
        #endregion

        #region §44 units used predominantly in clinical medicine
        //meter of water column  pressure  m H2O   m[H2O]   M[H2O]   yes 9.80665  kPa   
        //meter of mercury column  pressure  m Hg  m[Hg]   M[HG]   yes 133.3220  kPa   
        //inch of water column  pressure  in H2O   [in_i'H2O]   [IN_I'H2O]   no 1  m[H2O].[in_i]/m   
        //inch of mercury column  pressure  in Hg  [in_i'Hg]   [IN_i'HG]   no 1  m[Hg].[in_i]/m   
        //peripheral vascular resistance unit  fluid resistance  P.R.U.  [PRU]   [PRU]   no 1  mm[Hg].s/ml   
        //Wood unit  fluid resistance  Wood U.  [wood'U]   [WOOD'U]   no 1  mm[Hg].min/L   
        //diopter  refraction of a lens  dpt  [diop]   [DIOP]   no 1  /m   
        //prism diopter  refraction of a prism  PD  [p'diop]   [P'DIOP]   no •  100tan(1 rad)   
        //percent of slope  slope  %  %[slope]   %[SLOPE]   no •  100tan(1 rad)   
        //mesh  lineic number    [mesh_i]   [MESH_I]   no 1  /[in_i]   
        //Charrière, french  gauge of catheters  Ch  [Ch]   [CH]   no 1  mm/[pi]   
        //drop  volume  drp  [drp]   [DRP]   no 1  ml/12   
        //Hounsfield unit  x-ray attenuation  HF  [hnsf'U]   [HNSF'U]   no 1  1   
        //metabolic equivalent  metabolic cost of physical activity  MET  [MET]   [MET]   no 3.5  mL/min/kg   
        //homeopathic potency of decimal series (retired)  homeopathic potency (retired)  X  [hp'_X]   [HP'_X]   no •  hpX(1 1)   
        //homeopathic potency of centesimal series (retired)  homeopathic potency (retired)  C  [hp'_C]   [HP'_C]   no •  hpC(1 1)   
        //homeopathic potency of millesimal series (retired)  homeopathic potency (retired)  M  [hp'_M]   [HP'_M]   no •  hpM(1 1)   
        //homeopathic potency of quintamillesimal series (retired)  homeopathic potency (retired)  Q  [hp'_Q]   [HP'_Q]   no •  hpQ(1 1)   
        //homeopathic potency of decimal hahnemannian series  homeopathic potency (Hahnemann)  X  [hp_X]   [HP_X]   no •  •  
        //homeopathic potency of centesimal hahnemannian series  homeopathic potency (Hahnemann)  C  [hp_C]   [HP_C]   no •  •  
        //homeopathic potency of millesimal hahnemannian series  homeopathic potency (Hahnemann)  M  [hp_M]   [HP_M]   no •  •  
        //homeopathic potency of quintamillesimal hahnemannian series  homeopathic potency (Hahnemann)  Q  [hp_Q]   [HP_Q]   no •  •  
        //homeopathic potency of decimal korsakovian series  homeopathic potency (Korsakov)  X  [kp_X]   [KP_X]   no •  •  
        //homeopathic potency of centesimal korsakovian series  homeopathic potency (Korsakov)  C  [kp_C]   [KP_C]   no •  •  
        //homeopathic potency of millesimal korsakovian series  homeopathic potency (Korsakov)  M  [kp_M]   [KP_M]   no •  •  
        //homeopathic potency of quintamillesimal korsakovian series  homeopathic potency (Korsakov)  Q  [kp_Q]   [KP_Q]   no •  •  
        #endregion

        #region §45 chemical and biochemical units
        //equivalents  amount of substance  eq  eq   EQ   yes 1  mol   
        //osmole  amount of substance (dissolved particles)  osm  osm   OSM   yes 1  mol   
        //pH  acidity  pH  [pH]   [PH]   no •  pH(1 mol/l)   
        //gram percent  mass concentration  g%  g%   G%   yes 1  g/dl   
        //Svedberg unit  sedimentation coefficient  S  [S]   [S]   no 1  10*-13.s   
        //high power field  view area in microscope  HPF  [HPF]   [HPF]   no 1  1   
        //low power field  view area in microscope  LPF  [LPF]   [LPF]   no 100  1   
        //katal  catalytic activity  kat  kat   KAT   yes 1  mol/s   
        //Unit  catalytic activity  U  U   U   yes 1  umol/min   
        //international unit  arbitrary  IU  [iU]   [IU]   yes •  •  
        //international unit  arbitrary  i.U.  [IU]   [IU]   yes •  •  
        //arbitary unit  arbitrary  arb. U  [arb'U]   [ARB'U]   no •  •  
        //United States Pharmacopeia unit  arbitrary  U.S.P.  [USP'U]   [USP'U]   no •  •  
        //GPL unit  biologic activity of anticardiolipin IgG    [GPL'U]   [GPL'U]   no •  •  
        //MPL unit  biologic activity of anticardiolipin IgM    [MPL'U]   [MPL'U]   no •  •  
        //APL unit  biologic activity of anticardiolipin IgA    [APL'U]   [APL'U]   no •  •  
        //Bethesda unit  biologic activity of factor VIII inhibitor    [beth'U]   [BETH'U]   no •  •  
        //Todd unit  biologic activity antistreptolysin O    [todd'U]   [TODD'U]   no •  •  
        //Dye unit  biologic activity of amylase    [dye'U]   [DYE'U]   no •  •  
        //Somogyi unit  biologic activity of amylase    [smgy'U]   [SMGY'U]   no •  •  
        //Bodansky unit  biologic activity of phosphatase    [bdsk'U]   [BDSK'U]   no •  •  
        //King-Armstrong unit  biologic activity of phosphatase    [ka'U]   [KA'U]   no •  •  
        //Kunkel unit  arbitrary biologic activity    [knk'U]   [KNK'U]   no •  •  
        //Mac Lagan unit  arbitrary biologic activity    [mclg'U]   [MCLG'U]   no •  •  
        //tuberculin unit  biologic activity of tuberculin    [tb'U]   [TB'U]   no •  •  
        //50% cell culture infectious dose  biologic activity (infectivity) of an infectious agent preparation  CCID50   [CCID_50]   [CCID_50]   no •  •  
        //50% tissue culture infectious dose  biologic activity (infectivity) of an infectious agent preparation  TCID50   [TCID_50]   [TCID_50]   no •  •  
        //plaque forming units  amount of an infectious agent  PFU  [PFU]   [PFU]   no •  •  
        //focus forming units  amount of an infectious agent  FFU  [FFU]   [FFU]   no •  •  
        //colony forming units  amount of a proliferating organism  CFU  [CFU]   [CFU]   no •  •  
        //bioequivalent allergen unit  amount of an allergen callibrated through in-vivo testing based on the ID50EAL method of (intradermal dilution for 50mm sum of erythema diameters   BAU  [BAU]   [BAU]   no •  •  
        //allergen unit  procedure defined amount of an allergen using some reference standard  AU  [AU]   [AU]   no •  •  
        //allergen unit for Ambrosia artemisiifolia  procedure defined amount of the major allergen of ragweed.  Amb a 1 U  [Amb'a'1'U]   [AMB'A'1'U]   no •  •  
        //protein nitrogen unit  procedure defined amount of a protein substance  PNU  [PNU]   [PNU]   no •  •  
        //Limit of flocculation  procedure defined amount of an antigen substance  Lf  [Lf]   [LF]   no •  •  
        //D-antigen unit  procedure defined amount of an antigen substance    [D'ag'U]   [D'AG'U]   no •  •  
        #endregion

        #region §46 levels
        //neper  level  Np  Np   NEP   yes •  ln(1 1)   
        //bel  level  B  B   B   yes •  lg(1 1)   
        //bel sound pressure  pressure level  B(SPL)  B[SPL]   B[SPL]   yes •  2lg(2 10*-5.Pa)   
        //bel volt  electric potential level  B(V)  B[V]   B[V]   yes •  2lg(1 V)   
        //bel millivolt  electric potential level  B(mV)  B[mV]   B[MV]   yes •  2lg(1 mV)   
        //bel microvolt  electric potential level  B(μV)  B[uV]   B[UV]   yes •  2lg(1 uV)   
        //bel 10 nanovolt  electric potential level  B(10 nV)  B[10.nV]   B[10.NV]   yes •  2lg(10 nV)   
        //bel watt  power level  B(W)  B[W]   B[W]   yes •  lg(1 W)   
        //bel kilowatt  power level  B(kW)  B[kW]   B[KW]   yes •  lg(1 kW)   
        #endregion

        #region §47 miscellaneous units
        //stere  volume  st  st   STR   yes 1  m3   
        //Ångström  length  Å  Ao   AO   no 0.1  nm   
        //barn  action area  b  b   BRN   no 100  fm2   
        //technical atmosphere  pressure  at  att   ATT   no 1  kgf/cm2   
        //mho  electric conductance  mho  mho   MHO   yes 1  S   
        //pound per sqare inch  pressure  psi  [psi]   [PSI]   no 1  [lbf_av]/[in_i]2   
        //circle  plane angle  circ  circ   CIRC   no 2  [pi].rad   
        //spere  solid angle  sph  sph   SPH   no 4  [pi].sr   
        //metric carat  mass  ctm   [car_m]   [CAR_M]   no 0.2  g   
        //carat of gold alloys  mass fraction  ctAu   [car_Au]   [CAR_AU]   no 1  /24   
        //Smoot  length    [smoot]   [SMOOT]   no 67  [in_i]   
        #endregion

        #region §48 units used in information technology
        //bit  amount of information  bits   bit_s   BIT_S   no •  ld(1 1)   
        //bit  amount of information  bit  bit   BIT   yes 1  1   
        //byte  amount of information  B  By   BY   yes 8  bit   
        //baud  signal transmission rate  Bd  Bd   Bd   yes 1  /s   
        #endregion

//§49 prefixes       ■1 The prefix symbols based on powers of two for use in information technology as proposed by the IEEE are defined in Table 23.   ■2 The meaning of the columns is declared in §49.2.   ■3 Only the columns titled “c/s,” “c/i,” and “value,” are normative. Full name and print symbol are out of scope of The Unified Code for Units of Measure. 

// Table 23: The special prefix symbols for powers of 2 

//kibi  Ki  Ki KIB 1024  
//mebi  Mi  Mi MIB 1048576  
//gibi  Gi  Gi GIB 1073741824  
//tebi  Ti  Ti TIB 1099511627776  

        #region §50 Non-units
        //particles total count  number  tot.  {tot}   {TOT}   no   1   
        //tablets  number  tbl.  {tbl}   {TBL}   no   1   
        //red blood cell count  number  R.B.C.  {rbc}   {RBC}   no   1   
        //gram meter per heartbeat  proportional to ventricular stroke work  g· m/H.B.  g.m/{H.B.}   G.M/{H.B.}   no   g.m   
        //gram-force meter per heartbeat  ventricular stroke work  gf· m/H.B.  gf.m/{H.B.}   GF.M/{H.B.}   no   gf.m   
        //kilogram of wet tissue  mass  kg(wet tissue)  kg{wet'tis}   KG{wet'tis}   no   kg   
        //milligram of creatinine  mass  mg(creat.)  mg{creat}   MG{creat}   no   mg   
        #endregion

        /// <summary>
        /// Disallow instances.
        /// </summary>
        private UcumUnits() { }

    }
}
