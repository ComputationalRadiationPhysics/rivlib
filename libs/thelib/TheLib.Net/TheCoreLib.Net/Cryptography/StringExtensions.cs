/// <copyright file="StringExtensions.cs" company="TheLib Team">
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
 * StringExtensions.cs  30.01.2011 (mueller)
 * 
 * Copyright (C) 2011 Christoph Müller. Alle Rechte vorbehalten.
 */

using System;
using System.Runtime.InteropServices;
using System.Security;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;
using System.Text;


namespace The.Cryptography {

    /// <summary>
    /// This class provides extension methods for encrypting and decrypting
    /// <see cref="string"/>s and <see cref="StringBuilder"/>s.
    /// </summary>
    public static class StringExtensions {

        /// <summary>
        /// Assumes <paramref name="str"/> the base-64 encoding of an 
        /// RSA-ecrypted, UTF-8-ecoded string and decodes it.
        /// </summary>
        /// <remarks>
        /// <para>The certificate <paramref name="cert"/> must contain a 
        /// private key in order for this method to succeed.</para>
        /// <para>Use this method to decrypt data that have been encrypted
        /// using the <see cref="StringExtensions.EnrcyptRSA"/> methods.</para>
        /// </remarks>
        /// <param name="str">The string to be decrypted. This must be a base-64
        /// string representing RSA-encrypted string data, which have 
        /// orgininally been UTF-8-encoded.</param>
        /// <param name="cert">The certificate providing the private key for
        /// decrypting the string.</param>
        /// <param name="fOAEP">See 
        /// <see cref="RSACryptoServiceProvider.Decrypt"/> for the 
        /// implications of this flag.</param>
        /// <returns></returns>
        /// <exception cref="ArgumentNullException">If <paramref name="cert"/>
        /// is a <c>null</c> pointer.</exception>
        /// <exception cref="ArgumentNullException">If <paramref name="str"/>
        /// is a <c>null</c> pointer.</exception>
        /// <exception cref="ArgumentException">If <paramref name="cert"/>
        /// does not contain a private key.</exception>
        /// <exception cref="CryptographicException">If the cryptographic
        /// operation failed, e. g. because of invalid input data.</exception>
        public static string DecryptRSA(this string str, 
                X509Certificate2 cert, bool fOAEP = false) {
            if (cert == null) {
                throw new ArgumentNullException("cert");
            }
            if (str == null) {
                throw new ArgumentNullException("str");
            }
            if (cert.PrivateKey == null) {
                throw new ArgumentException("The certificate used for "
                    + "decrypting must contain a private key.", "cert");
            }

            RSACryptoServiceProvider rsa = cert.GetRSAPrivate();
            byte[] rawStr = Convert.FromBase64String(str);
            byte[] decrypted = rsa.Decrypt(rawStr, fOAEP);
            return Encoding.UTF8.GetString(decrypted);
        }

        /// <summary>
        /// Encrypt a given <see cref="string"/> using RSA and return a base-64
        /// encoding of the result of the encrypted value.
        /// </summary>
        /// <remarks>
        /// <para>A UTF-8 encoding of the input data is used for creating a 
        /// binary representation of it.</para>
        /// </remarks>
        /// <param name="str"></param>
        /// <param name="cert">The certificate containing the public key
        /// used for encryption.</param>
        /// <param name="fOAEP">See 
        /// <see cref="RSACryptoServiceProvider.Encrypt"/> for the 
        /// implications of this flag.</param>
        /// <returns>base-64-encoded, encrypted data.</returns>
        /// <exception cref="ArgumentNullException">If <paramref name="cert"/>
        /// is a <c>null</c> pointer.</exception>
        /// <exception cref="ArgumentNullException">If <paramref name="str"/>
        /// is a <c>null</c> pointer.</exception>
        public static string EncryptRSA(this string str, 
                X509Certificate2 cert, bool fOAEP = false) {
            return StringExtensions.encryptRSA(str.GetUTF8Bytes(), cert, fOAEP);
        }

        /// <summary>
        /// Encrypt a given <see cref="StringBuilder"/> using RSA and return a 
        /// base-64 encoding of the result of the encrypted value.
        /// </summary>
        /// <remarks>
        /// <para>A UTF-8 encoding of the input data is used for creating a 
        /// binary representation of it.</para>
        /// </remarks>
        /// <param name="sb"></param>
        /// <param name="cert">The certificate containing the public key
        /// used for encryption.</param>
        /// <param name="fOAEP">See 
        /// <see cref="RSACryptoServiceProvider.Encrypt"/> for the 
        /// implications of this flag.</param>
        /// <returns>base-64-encoded, encrypted data.</returns>
        /// <exception cref="ArgumentNullException">If <paramref name="cert"/>
        /// is a <c>null</c> pointer.</exception>
        /// <exception cref="ArgumentNullException">If <paramref name="sb"/>
        /// is a <c>null</c> pointer.</exception>
        public static string EncryptRSA(this StringBuilder sb,
                X509Certificate2 cert, bool fOAEP = false) {
            if (sb == null) {
                throw new ArgumentNullException("sb");
            }
            return sb.ToString().EncryptRSA(cert, fOAEP);
        }

        ///// <summary>
        ///// Encrypt a given <see cref="SecureString"/> using RSA and return a 
        ///// base-64 encoding of the result of the encrypted value.
        ///// </summary>
        ///// <remarks>
        ///// <para>The native representation of the string is directly 
        ///// encrypted as it is.</para>
        ///// <para>A managed copy of the input data is required to perform
        ///// the encryption.</para>
        ///// </remarks>
        ///// <param name="str"></param>
        ///// <param name="cert">The certificate containing the public key
        ///// used for encryption.</param>
        ///// <param name="fOAEP">See 
        ///// <see cref="RSACryptoServiceProvider.Encrypt"/> for the 
        ///// implications of this flag.</param>
        ///// <returns>base-64-encoded, encrypted data.</returns>
        ///// <exception cref="ArgumentNullException">If <paramref name="cert"/>
        ///// is a <c>null</c> pointer.</exception>
        //public static string EncryptRSA(this SecureString str,
        //        X509Certificate2 cert, bool fOAEP = false) {
        //    IntPtr bstr = Marshal.SecureStringToBSTR(str);
        //    try {
        //        byte[] chars = new byte[str.Length];
        //        Marshal.Copy(bstr, chars, 0, str.Length);
        //        return StringExtensions.encryptRSA(chars, cert, fOAEP);
        //    } finally {
        //        Marshal.ZeroFreeBSTR(bstr);
        //    }
        //}

        /// <summary>
        /// Converts the content of a <see cref="String"/> into a UTF-8
        /// binary representation.
        /// </summary>
        /// <param name="str">The string to get the UTF-8 encoding for.</param>
        /// <returns>A <see cref="byte"/> array containing the UTF-8-encoded
        /// bytes of the input.</returns>
        /// <exception cref="ArgumentNullException">If <paramref name="str"/>
        /// is a <c>null</c> pointer.</exception>
        public static byte[] GetUTF8Bytes(this string str) {
            return Encoding.UTF8.GetBytes(str);
        }

        /// <summary>
        /// Converts the content of a <see cref="StringBuilder"/> into a UTF-8
        /// binary representation.
        /// </summary>
        /// <param name="sb">The string to get the UTF-8 encoding for.</param>
        /// <returns>A <see cref="byte"/> array containing the UTF-8-encoded
        /// bytes of the input.</returns>
        /// <exception cref="ArgumentNullException">If <paramref name="sb"/>
        /// is a <c>null</c> pointer.</exception>
        public static byte[] GetUTF8Bytes(this StringBuilder sb) {
            if (sb == null) {
                throw new ArgumentNullException("sb");
            }
            return sb.ToString().GetUTF8Bytes();
        }

        /// <summary>
        /// Convert a <see cref="string"/> into a <see cref="SecureString"/>
        /// by adding all characters of the former to the latter.
        /// </summary>
        /// <remarks>
        /// This method can be used for doing inherently unsafe stuff, because
        /// the input <paramref name="str"/> is unsafe and cannot be reliably
        /// destroyed once the <see cref="SecureString"/> has been created.
        /// </remarks>
        /// <param name="str">The <see cref="string"/> to be converted.</param>
        /// <returns>A <see cref="SecureString"/> with the same content as
        /// <see cref="str"/>.</returns>
        public static SecureString MakeSecure(this string str) {
            SecureString retval = new SecureString();
            foreach (char c in str) {
                retval.AppendChar(c);
            }
            return retval;
        }

        /// <summary>
        /// Convert a <see cref="SecureString"/> to an unsecure managed
        /// <see cref="string"/>.
        /// </summary>
        /// <remarks>
        /// This method is inherently unsafe, because it provides the 
        /// possibility to circumvent all security measures of the .NET 
        /// framework regarding <see cref="SecureString"/>s.
        /// </remarks>
        /// <param name="str">The <see cref="SecureString"/> to get the 
        /// decrypted data for.</param>
        /// <returns></returns>
        /// <exception cref="ArgumentNullException">If <paramref name="str"/>
        /// is a <c>null</c> pointer.</exception>
        public static string MakeUnsecure(this SecureString str) {
            IntPtr bstr = Marshal.SecureStringToBSTR(str);
            try {
                return Marshal.PtrToStringUni(bstr);
            } finally {
                Marshal.ZeroFreeBSTR(bstr);
            }
        }

        /// <summary>
        /// Converts the content of a <see cref="string"/> into the 
        /// base-64 representation of its UTF-8 encoding.
        /// </summary>
        /// <param name="str">The string to be converted to base-64.</param>
        /// <returns>A base-64 encoded version of the UTF-8 binary 
        /// representation of <paramref name="str"/>.</returns>
        /// <exception cref="ArgumentNullException">If <paramref name="str"/>
        /// is a <c>null</c> pointer.</exception>
        public static string ToUTF8Base64String(this string str) {
            return Convert.ToBase64String(str.GetUTF8Bytes());
        }

        /// <summary>
        /// Converts the content of a <see cref="StringBuilder"/> into the 
        /// base-64 representation of its UTF-8 encoding.
        /// </summary>
        /// <param name="sb">The string to be converted to base-64.</param>
        /// <returns>A base-64 encoded version of the UTF-8 binary 
        /// representation of <paramref name="str"/>.</returns>
        /// <exception cref="ArgumentNullException">If <paramref name="sb"/>
        /// is a <c>null</c> pointer.</exception>
        public static string ToUTF8Base64String(this StringBuilder sb) {
            if (sb == null) {
                throw new ArgumentNullException("sb");
            }
            return sb.ToString().ToUTF8Base64String();
        }

        /// <summary>
        /// Helper method that encrypts <paramref name="data"/> using RSA
        /// and returns the base-64 encoded result.
        /// </summary>
        /// <param name="data">The raw data to be encrypted.</param>
        /// <param name="cert">The certificate containing the public key
        /// used for encryption.</param>
        /// <param name="fOAEP">See 
        /// <see cref="RSACryptoServiceProvider.Encrypt"/> for the 
        /// implications of this flag.</param>
        /// <returns>base-64-encoded, encrypted data.</returns>
        /// <exception cref="ArgumentNullException">If <paramref name="cert"/>
        /// is a <c>null</c> pointer.</exception>
        private static string encryptRSA(byte[] data, X509Certificate2 cert,
                bool fOAEP = false) {
            if (cert == null) {
                throw new ArgumentNullException("cert");
            }
            RSACryptoServiceProvider rsa = cert.GetRSAPublic();
            byte[] encrypted = rsa.Encrypt(data, fOAEP);
            return Convert.ToBase64String(encrypted);
        }
    }
}
