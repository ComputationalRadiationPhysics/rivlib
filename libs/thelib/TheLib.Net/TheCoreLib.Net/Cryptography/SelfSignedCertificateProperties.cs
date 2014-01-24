/// <copyright file="SelfSignedCertificateProperties.cs" company="TheLib Team">
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
 * SelfSignedCertificateProperties.cs
 * 
 * Copyright (C) 2010 Christoph Müller. Alle Rechte vorbehalten.
 */

using System;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Security.Principal;


namespace The.Cryptography {

    /// <summary>
    /// This class aggregates all input data that are required for creating a
    /// new self-signed RSA certificate.
    /// </summary>
    public class SelfSignedCertificateProperties {

        /// <summary>
        /// Gets the length of a key if no user-specified value is given.
        /// </summary>
        public static int DefaultKeyLength {
            get {
                return 4096;
            }
        }

        public SelfSignedCertificateProperties(
                X500DistinguishedName subjectName, DateTime notBefore, 
                DateTime notAfter, int keyLength, bool isPrivateKeyExportable) {
            this.IsPrivateKeyExportable = isPrivateKeyExportable;
            this.KeyLength = keyLength;
            this.NotAfter = notAfter;
            this.NotBefore = notBefore;
            this.SubjectName = subjectName;
        }

        public SelfSignedCertificateProperties(string subjectName,
                DateTime notBefore, DateTime notAfter, int keyLength,
                bool isPrivateKeyExportable) 
            : this(new X500DistinguishedName("CN=" + subjectName), notBefore, 
                notAfter, keyLength, isPrivateKeyExportable) { }

        public SelfSignedCertificateProperties(string subjectName,
                DateTime notBefore, DateTime notAfter, int keyLength)
            : this(subjectName, notBefore, notAfter, keyLength, false) { }

        public SelfSignedCertificateProperties(string subjectName,
                DateTime notBefore, DateTime notAfter, 
                bool isPrivateKeyExportable)
            : this(subjectName, notBefore, notAfter, DefaultKeyLength, 
                isPrivateKeyExportable) { }

        public SelfSignedCertificateProperties(string subjectName,
                DateTime notBefore, DateTime notAfter)
            : this(subjectName, notBefore, notAfter, DefaultKeyLength) { }

        public SelfSignedCertificateProperties(string subjectName)
            : this(subjectName, DateTime.Today.AddDays(-1), 
                DateTime.Today.AddYears(10)) { }

        public SelfSignedCertificateProperties() : this(string.Empty) {
            string curId = WindowsIdentity.GetCurrent().Name.ToString();
            curId = curId.Split(new char[] { '\\' }).Last();
            this.SubjectName = new X500DistinguishedName("CN=" + curId);
        }

        /// <summary>
        /// Gets or sets whether the private key will be exportable.
        /// </summary>
        public bool IsPrivateKeyExportable {
            get;
            set;
        }

        /// <summary>
        /// Gets or sets the key length.
        /// </summary>
        public int KeyLength {
            get;
            set;
        }

        /// <summary>
        /// Gets or sets the expriation date of the certificate.
        /// </summary>
        public DateTime NotAfter {
            get;
            set;
        }

        /// <summary>
        /// Gets or sets the point in time when the certificate becomes valid.
        /// </summary>
        public DateTime NotBefore {
            get; 
            set; 
        }

        /// <summary>
        /// Gets or sets the name of the subject the certificate is for.
        /// </summary>
        public X500DistinguishedName SubjectName {
            get; 
            set; 
        }
    }
}
