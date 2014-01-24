/// <copyright file="Math.cs" company="TheLib Team">
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


namespace The.Mathematics {

    /// <summary>
    /// Addtional math functionality which is not available in
    /// <see cref="System.Math"/>.
    /// </summary>
    public static class MathExtensions {

        #region Clamp
        /// <summary>
        /// Clamps <paramref name="value"/> to the given bounds.
        /// </summary>
        /// <typeparam name="T">The type of variable to be clamped.</typeparam>
        /// <param name="value">The value to be clamped.</param>
        /// <param name="min">The minimum value allowed.</param>
        /// <param name="max">The maximum value allowed.</param>
        /// <returns><paramref name="value"/> if it lies within 
        /// [<paramref name="min"/>, <paramref name="max"/>], or
        /// <paramref name="min"/> if it is smaller, or
        /// <paramref name="max"/> if it is larger than the respective boundary
        /// value.
        /// </returns>
        /// <exception cref="System.ArgumentException">If
        /// <paramref name="min"/> is larger than or equal to
        /// <paramref name="max"/>.</exception>
        public static T Clamp<T>(this T value, T min, T max)
                where T : struct, IComparable<T> {
            if (min.CompareTo(max) >= 0) {
                throw new ArgumentException(
                    Properties.Resources.ErrorMinNotLessThanMax);
            }

            if (value.CompareTo(min) < 0) {
                return min;
            } else if (value.CompareTo(max) > 0) {
                return max;
            } else {
                return value;
            }
        }
        #endregion

        #region GreatestCommonDivisor
        /// <summary>
        /// Answer the greatest common divisor of <paramref name="m"/> and
        /// <paramref name="n"/>.
        /// </summary>
        /// <remarks>
        /// This implementation uses the Euclidean algorithm.
        /// </remarks>
        /// <param name="m">A number.</param>
        /// <param name="n">A number.</param>
        /// <returns>The greatest common divisor.</returns>
        public static int GreatestCommonDivisor(int m, int n) {
            return (int) MathExtensions.GreatestCommonDivisor((long) m,
                (long) n);
        }

        /// <summary>
        /// Answer the greatest common divisor of <paramref name="m"/> and
        /// <paramref name="n"/>.
        /// </summary>
        /// <remarks>
        /// This implementation uses the Euclidean algorithm.
        /// </remarks>
        /// <param name="m">A number.</param>
        /// <param name="n">A number.</param>
        /// <returns>The greatest common divisor.</returns>
        public static long GreatestCommonDivisor(long m, long n) {
            while (n != 0) {
                long remainder = m % n;
                m = n;
                n = remainder;
            }
            return m;
        }
        #endregion

        #region IsNearlyEqual
        /// <summary>
        /// Answer whether <paramref name="lhs"/> and <paramref name="rhs"/>
        /// are nearly equal up to a maximum distance of
        /// <paramref name="epsilon"/>.
        /// </summary>
        /// <param name="lhs">The left hand side operand.</param>
        /// <param name="rhs">The right hand side operand.</param>
        /// <param name="epsilon">The maximum difference allowed until the
        /// values are not considered equal any more.</param>
        /// <returns><c>true</c> if <paramref name="lhs"/> and
        /// <paramref name="rhs"/> are equal up to a maximum distance of
        /// <paramref name="epsilon"/>, <c>false</c> otherwise.</returns>
        public static bool IsNearlyEqual(this decimal lhs, decimal rhs,
                decimal epsilon) {
            decimal d = lhs - rhs;
            return (Math.Abs(d) <= epsilon);
        }

        /// <summary>
        /// Answer whether <paramref name="lhs"/> and <paramref name="rhs"/>
        /// are nearly equal up to a maximum distance of
        /// <paramref name="epsilon"/>.
        /// </summary>
        /// <param name="lhs">The left hand side operand.</param>
        /// <param name="rhs">The right hand side operand.</param>
        /// <param name="epsilon">The maximum difference allowed until the
        /// values are not considered equal any more. The parameter defaults
        /// to <see cref="System.Double.Epsilon"/>.</param>
        /// <returns><c>true</c> if <paramref name="lhs"/> and
        /// <paramref name="rhs"/> are equal up to a maximum distance of
        /// <paramref name="epsilon"/>, <c>false</c> otherwise.</returns>
        public static bool IsNearlyEqual(this double lhs, double rhs,
                double epsilon = double.Epsilon) {
            double d = lhs - rhs;
            return (Math.Abs(d) <= epsilon);
        }

        /// <summary>
        /// Answer whether <paramref name="lhs"/> and <paramref name="rhs"/>
        /// are nearly equal up to a maximum distance of
        /// <paramref name="epsilon"/>.
        /// </summary>
        /// <param name="lhs">The left hand side operand.</param>
        /// <param name="rhs">The right hand side operand.</param>
        /// <param name="epsilon">The maximum difference allowed until the
        /// values are not considered equal any more. The parameter defaults
        /// to <see cref="System.Float.Epsilon"/>.</param>
        /// <returns><c>true</c> if <paramref name="lhs"/> and
        /// <paramref name="rhs"/> are equal up to a maximum distance of
        /// <paramref name="epsilon"/>, <c>false</c> otherwise.</returns>
        public static bool IsNearlyEqual(this float lhs, float rhs,
                float epsilon = float.Epsilon) {
            float d = lhs - rhs;
            return (Math.Abs(d) <= epsilon);
        }
        #endregion

        #region IsWithin
        /// <summary>
        /// Answer whether <paramref name="value"/> lies within
        /// <paramref name="min"/> and <paramref name="max"/>.
        /// </summary>
        /// <typeparam name="T">The type of the value to be compared.
        /// </typeparam>
        /// <param name="value">The value to be tested.</param>
        /// <param name="min">The minimum value of the range.</param>
        /// <param name="max">The maximum value of the range.</param>
        /// <param name="includeMin">If <paramref name="true"/>, consider
        /// a value of <paramref name="min"/> as within the range. This
        /// parameter defaults to <c>false</c>.</param>
        /// <param name="includeMax">If <paramref name="true"/>, consider
        /// a value of <paramref name="max"/> as within the range. This
        /// parameter defaults to <c>false</c>.</param>
        /// <returns><c>true</c> if <paramref name="value"/> is in the
        /// range, <c>false</c> otherwise.</returns>
        public static bool IsWithin<T>(this T value, T min, T max,
                bool includeMin = false, bool includeMax = false)
                where T : struct, IComparable<T> {
            if (min.CompareTo(max) >= 0) {
                throw new ArgumentException(
                    Properties.Resources.ErrorMinNotLessThanMax);
            }

            int di = value.CompareTo(min);
            int da = value.CompareTo(max);

            return (((di > 0) || (includeMin && (di == 0)))
                && ((da < 0) || (includeMax && (da == 0))));
        }
        #endregion

        #region LeastCommonMultiple
        /// <summary>
        /// Find the least common multiple of <paramref name="m"/> and
        /// <paramref name="n"/>
        /// </summary>
        /// <param name="m">A number.</param>
        /// <param name="n">A number.</param>
        /// <returns>The least common multiple.</returns>
        public static long LeastCommonMultiple(int m, int n) {
            return (int) MathExtensions.LeastCommonMultiple((long) m, (long) n);
        }

        /// <summary>
        /// Find the least common multiple of <paramref name="m"/> and
        /// <paramref name="n"/>
        /// </summary>
        /// <param name="m">A number.</param>
        /// <param name="n">A number.</param>
        /// <returns>The least common multiple.</returns>
        public static long LeastCommonMultiple(long m, long n) {
            return (m * n) / MathExtensions.GreatestCommonDivisor(m, n);
        }
        #endregion

        #region Pow
        /// <summary>
        /// Computes the <paramref name="power"/>th power of
        /// <paramref name="value"/>.
        /// </summary>
        /// <param name="value">The value to raise.</param>
        /// <param name="power">The exponent.</param>
        /// <returns>The <paramref name="power"/>th power of
        /// <paramref name="value"/>.</returns>
        public static double Pow(this double value, double power) {
            return Math.Pow(value, power);
        }

        /// <summary>
        /// Computes the <paramref name="power"/>th power of
        /// <paramref name="value"/>.
        /// </summary>
        /// <param name="value">The value to raise.</param>
        /// <param name="power">The exponent.</param>
        /// <returns>The <paramref name="power"/>th power of
        /// <paramref name="value"/>.</returns>
        public static double Pow(this double value, int power) {
            return Math.Pow(value, (double) power);
        }
        #endregion

        #region Root
        /// <summary>
        /// Computes the <paramref name="root"/>th root of
        /// <paramref name="value"/>.
        /// </summary>
        /// <param name="value">The value to compute the root of.</param>
        /// <param name="root">The root exponent.</param>
        /// <returns>The <paramref name="root"/>th root of
        /// <paramref name="value"/>.</returns>
        public static double Root(this double value, double root) {
            return Math.Pow(value, 1.0 / root);
        }

        /// <summary>
        /// Computes the <paramref name="root"/>th root of
        /// <paramref name="value"/>.
        /// </summary>
        /// <param name="value">The value to compute the root of.</param>
        /// <param name="root">The root exponent.</param>
        /// <returns>The <paramref name="root"/>th root of
        /// <paramref name="value"/>.</returns>
        public static double Root(this double value, int root) {
            return Math.Pow(value, 1.0 / (double) root);
        }
        #endregion
    }
}
