/// <copyright file="X509CertificateExtensions.cs" company="TheLib Team">
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
 * X509Certificate2Extensions.cs  06.02.2011
 * 
 * Copyright (C) 2011 Christoph Müller. Alle Rechte vorbehalten.
 */

using System;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;


namespace The.Cryptography {

    /// <summary>
    /// This class provides extension methods for 
    /// <see cref="X509Certificate2"/>s that facilitate the use of such 
    /// certificates.
    /// </summary>
    public static class X509CertificateExtensions {

        /// <summary>
        /// Gets the <see cref="RSACryptoServiceProvider"/> for the private
        /// key of the certificate.
        /// </summary>
        /// <param name="cert">The certificate to get the private key as 
        /// RSA provider for.</param>
        /// <returns>The private key of the certificate</returns>
        /// <exception cref="InvalidOperationException">If the certificate
        /// has no private key or does not support RSA.</exception>
        public static RSACryptoServiceProvider GetRSAPrivate(
                this X509Certificate2 cert) {
            RSACryptoServiceProvider retval = cert.PrivateKey
                as RSACryptoServiceProvider;
            if (retval != null) {
                return retval;
            } else {
                throw new InvalidOperationException();
            }
        }

        /// <summary>
        /// Gets the <see cref="RSACryptoServiceProvider"/> for the public
        /// key of the certificate.
        /// </summary>
        /// <param name="cert">The certificate to get the public key as 
        /// RSA provider for.</param>
        /// <returns>The public key of the certificate</returns>
        /// <exception cref="InvalidOperationException">If the public key
        /// does not support RSA.</exception>
        public static RSACryptoServiceProvider GetRSAPublic(
                this X509Certificate2 cert) {
            RSACryptoServiceProvider retval = cert.PublicKey.Key
                as RSACryptoServiceProvider;
            if (retval != null) {
                return retval;
            } else {
                throw new InvalidOperationException();
            }
        }

        /// <summary>
        /// Answer whether the given certificate <paramref name="cert"/>
        /// issued to <paramref name="user"/>.
        /// </summary>
        /// <param name="cert">The certificate to be checked.</param>
        /// <param name="userCN">The common name of the potential subject.
        /// </param>
        /// <returns><c>true</c> if the certificate is issued to the
        /// given user, <c>false</c> otherwise.</returns>
        public static bool IsIssuedTo(this X509Certificate2 cert,
                string userCN) {
            try {
                return (userCN == cert.SubjectName.Name);
            } catch {
                return false;
            }
        }

        /// <summary>
        /// Answer whether the given certificate <paramref name="cert"/>
        /// is already and still valid.
        /// </summary>
        /// <param name="cert">The certificate to be checked.</param>
        /// <returns><c>true</c> if the certificate is valid, <c>false</c>
        /// otherwise.</returns>
        public static bool IsValidNow(this X509Certificate2 cert) {
            if (DateTime.Now < cert.NotBefore) {
                return false;
            } else if (DateTime.Now > cert.NotAfter) {
                return false;
            } else {
                return true;
            }
        }
    }
}
