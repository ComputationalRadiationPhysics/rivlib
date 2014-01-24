/// <copyright file="SystemTime.cs" company="TheLib Team">
/// Copyright (C) 2013 TheLib Team (http://thelib.org). All rights reserved.
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
/*
 * SystemTime.cs
 * 
 * Copyright (C) 2010 Christoph Müller. Alle Rechte vorbehalten.
 */

using System;
using System.Runtime.InteropServices;


namespace The.System.Interop {

    /// <summary>
    /// Managed version of Win32 SYSTEMTIME.
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public class SystemTime {
        public short Year;
        public short Month;
        public short DayOfWeek;
        public short Day;
        public short Hour;
        public short Minute;
        public short Second;
        public short Milliseconds;

        /// <summary>
        /// Creates a new <see cref="SystemTime"/> from the given file time.
        /// </summary>
        /// <param name="fileTime"></param>
        /// <returns></returns>
        public static SystemTime FromFileTime(long fileTime) {
            SystemTime retval = new SystemTime();
            if (!SystemTime.fileTimeToSystemTime(ref fileTime, retval)) {
                Win32ErrorCheck.CheckLastError();
            }
            return retval;
        }

        /// <summary>
        /// Creates a new <see cref="SystemTime"/> from a given
        /// <see cref="DateTime"/>.
        /// </summary>
        /// <param name="dateTime"></param>
        /// <returns></returns>
        public static SystemTime FromDateTime(DateTime dateTime) {
            return SystemTime.FromFileTime(dateTime.ToFileTime());
        }

        #region P/Invoke declarations
        [DllImport("kernel32.dll",
            EntryPoint = "FileTimeToSystemTime",
            SetLastError = true)]
        private static extern bool fileTimeToSystemTime(
            [In] ref long fileTime,
            [Out] SystemTime systemTime);
        #endregion
    }
}
