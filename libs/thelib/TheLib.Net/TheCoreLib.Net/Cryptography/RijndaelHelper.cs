/// <copyright file="RijndaelHelper.cs" company="TheLib Team">
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
 * RijndaelHelper.cs  11.09.2011 (mueller)
 * 
 * Copyright (C) 2011 Christoph Müller. Alle Rechte vorbehalten.
 */

using System;
using System.Diagnostics;
using System.IO;
using System.Security.Cryptography;
using System.Text;


namespace The.Cryptography {

    /// <summary>
    /// This helper class facilitates the use of <see cref="RijndaelManaged"/>.
    /// </summary>
    public static class RijndaelHelper {

        /// <summary>
        /// The default value used as initialisation vector.
        /// </summary>
        public const string DEFAULT_INIT_VECTOR = "(PCdYj2l+aF6(1xy";

        /// <summary>
        /// Possible value for the hash algorithm to be used. This constant 
        /// instructs the encryption/decryption methods to use MD5.
        /// </summary>
        public const string MD5_HASH_NAME = "MD5";

        /// <summary>
        /// Possible value for the hash algorithm to be used. This constant 
        /// instructs the encryption/decryption methods to use SHA1.
        /// </summary>
        public const string SHA1_HASH_NAME = "SHA1";

        /// <summary>
        /// Decrypts a base64-encoded Rijndael (AES) encrypted text.
        /// </summary>
        /// <param name="cipherText">The base64-encoded cipher text.</param>
        /// <param name="password">The password from which a pseudo-random 
        /// encryption key will be derived.</param>
        /// <param name="salt">A salt value used to generate the a pseudo-random
        /// encryption key from <paramref name="password"/>.</param>
        /// <param name="saltEncoding">The encoding of <paramref name="salt"/>.
        /// </param>
        /// <param name="outputEncoding">The encoding of the decrypted text. 
        /// This must be the same as the input for encryption had.</param>
        /// <param name="initVector">An initialisation vector (IV) to encrypt
        /// the first block of plain text data. This vector must comprise of
        /// exactly 16 ASCII characters.</param>
        /// <param name="hashName">The hash algorithm used to generate the key.
        /// This can be <see cref="MD5_HASH_NAME"/> or 
        /// <see cref="SHA1_HASH_NAME"/>.</param>
        /// <param name="keySize">The size of the encryption key being 
        /// generated. This can be 128, 192 or 256.</param>
        /// <param name="iterations">The number of iterations used to generate 
        /// the pseudo-random key. Two iterations can be assumed to be 
        /// sufficient.</param>
        /// <param name="cipherMode">The encryption mode being used. Using
        /// Cipher Block Chaining (CBC) is reasonable.</param>
        /// <returns>The decrypted text.</returns>
        public static string Decrypt(string cipherText, string password,
                string salt, Encoding saltEncoding, Encoding outputEncoding,
                string initVector = DEFAULT_INIT_VECTOR,
                string hashName = SHA1_HASH_NAME, int keySize = 256, 
                int iterations = 2, CipherMode cipherMode = CipherMode.CBC) {

            /* Sanity checks. */
            if (cipherText == null) {
                throw new ArgumentNullException("cipherText");
            }
            if (initVector == null) {
                throw new ArgumentNullException("initVector");
            }

            // Convert input into byte arrays. 'initVector' must be ASCII, so 
            // this conversion is not configurable
            byte[] initVectorBytes = Encoding.ASCII.GetBytes(initVector);
            byte[] cipherBytes = Convert.FromBase64String(cipherText);
            byte[] keyBytes = RijndaelHelper.calcKey(password, salt,
                saltEncoding, hashName, keySize, iterations);
            Debug.Assert(initVectorBytes.Length == 16);

            using (RijndaelManaged symmetricKey = new RijndaelManaged()) {
                symmetricKey.Mode = cipherMode;

                using (ICryptoTransform decryptor
                        = symmetricKey.CreateDecryptor(keyBytes,
                        initVectorBytes)) {
                    using (MemoryStream ms = new MemoryStream(cipherBytes)) {
                        using (CryptoStream cs = new CryptoStream(ms, decryptor,
                                CryptoStreamMode.Read)) {
                            // Since at this point we don't know what the size 
                            // of decrypted data will be, allocate the buffer 
                            // long enough to hold cipher text; plain text is 
                            // never longer than ciphertext.
                            byte[] plainBytes = new byte[cipherBytes.Length];
                            int cntRead = cs.Read(plainBytes, 0,
                                plainBytes.Length);

                            return outputEncoding.GetString(plainBytes, 0,
                                cntRead);
                        }
                    } /* end using (MemoryStream ms = ... */
                } /* end using (ICryptoTransform decryptor ... */
            } /* end using (RijndaelManaged symmetricKey ... */
        }

        /// <summary>
        /// Decrypts a base64-encoded Rijndael (AES) encrypted text. The plain
        /// text is assumed to be UTF8-encoded.
        /// </summary>
        /// <param name="cipherText">The base64-encoded cipher text.</param>
        /// <param name="password">The password from which a pseudo-random 
        /// encryption key will be derived.</param>
        /// <param name="salt">A salt value used to generate the a pseudo-random
        /// encryption key from <paramref name="password"/>. The salt is assumed
        /// to be UTF-8-encoded</param>
        /// <param name="initVector">An initialisation vector (IV) to encrypt
        /// the first block of plain text data. This vector must comprise of
        /// exactly 16 ASCII characters.</param>
        /// <param name="hashName">The hash algorithm used to generate the key.
        /// This can be <see cref="MD5_HASH_NAME"/> or 
        /// <see cref="SHA1_HASH_NAME"/>.</param>
        /// <param name="keySize">The size of the encryption key being 
        /// generated. This can be 128, 192 or 256.</param>
        /// <param name="iterations">The number of iterations used to generate 
        /// the pseudo-random key. Two iterations can be assumed to be 
        /// sufficient.</param>
        /// <param name="cipherMode">The encryption mode being used. Using
        /// Cipher Block Chaining (CBC) is reasonable.</param>
        /// <returns>The decrypted text.</returns>
        public static string Decrypt(string cipherText, string password,
                string salt, string initVector = DEFAULT_INIT_VECTOR,
                string hashName = SHA1_HASH_NAME, int keySize = 256,
                int iterations = 2, CipherMode cipherMode = CipherMode.CBC) {
            return RijndaelHelper.Decrypt(cipherText, password, salt, 
                Encoding.UTF8, Encoding.UTF8, initVector, hashName, keySize, 
                iterations, cipherMode);
        }

        /// <summary>
        /// Encrypts specified <paramref name="text"/> using the Rijndael (AES)
        /// symmetric encryption algorithm and returns a base64-encoded result.
        /// </summary>
        /// <param name="text">The plain text to be encrypted.</param>
        /// <param name="textEncoding">The encoding of <paramref name="text"/>.
        /// </param>
        /// <param name="password">The password from which a pseudo-random 
        /// encryption key will be derived.</param>
        /// <param name="salt">A salt value used to generate the a pseudo-random
        /// encryption key from <paramref name="password"/>.</param>
        /// <param name="saltEncoding">The encoding of <paramref name="salt"/>.
        /// </param>
        /// <param name="initVector">An initialisation vector (IV) to encrypt
        /// the first block of plain text data. This vector must comprise of
        /// exactly 16 ASCII characters.</param>
        /// <param name="hashName">The hash algorithm used to generate the key.
        /// This can be <see cref="MD5_HASH_NAME"/> or 
        /// <see cref="SHA1_HASH_NAME"/>.</param>
        /// <param name="keySize">The size of the encryption key being 
        /// generated. This can be 128, 192 or 256.</param>
        /// <param name="iterations">The number of iterations used to generate 
        /// the pseudo-random key. Two iterations can be assumed to be 
        /// sufficient.</param>
        /// <param name="cipherMode">The encryption mode being used. Using
        /// Cipher Block Chaining (CBC) is reasonable.</param>
        /// <returns>A base64-encoded string containing the encryption 
        /// result.</returns>
        public static string Encrypt(string text, Encoding textEncoding, 
                string password, string salt, Encoding saltEncoding,
                string initVector = DEFAULT_INIT_VECTOR,
                string hashName = SHA1_HASH_NAME, int keySize = 256, 
                int iterations = 2, CipherMode cipherMode = CipherMode.CBC) {

            /* Sanity checks. */
            if (text == null) {
                throw new ArgumentNullException("text");
            }
            if (textEncoding == null) {
                throw new ArgumentNullException("textEncoding");
            }
            if (initVector == null) {
                throw new ArgumentNullException("initVector");
            }

            // Convert input into byte arrays. 'initVector' must be ASCII, so 
            // this conversion is not configurable
            byte[] initVectorBytes = Encoding.ASCII.GetBytes(initVector);
            byte[] textBytes = textEncoding.GetBytes(text);
            byte[] keyBytes = RijndaelHelper.calcKey(password, salt, 
                saltEncoding, hashName, keySize, iterations);
            Debug.Assert(initVectorBytes.Length == 16);

            using (RijndaelManaged symmetricKey = new RijndaelManaged()) {
                symmetricKey.Mode = cipherMode;

                using (ICryptoTransform encryptor
                        = symmetricKey.CreateEncryptor(keyBytes, 
                        initVectorBytes)) {
                    using (MemoryStream ms = new MemoryStream()) { 
                        using (CryptoStream cs = new CryptoStream(ms, encryptor, 
                                CryptoStreamMode.Write)) {
                            cs.Write(textBytes, 0, textBytes.Length);
                            cs.FlushFinalBlock();
                            byte[] cipherBytes = ms.ToArray();
                            return Convert.ToBase64String(cipherBytes);
                        }
                    }
                } /* end using (ICryptoTransform encryptor ... */
            } /* end using (RijndaelManaged symmetricKey = ... */
        }

        /// <summary>
        /// Encrypts specified <paramref name="text"/> using the Rijndael (AES)
        /// symmetric encryption algorithm and returns a base64-encoded result.
        /// </summary>
        /// <param name="text">The plain text to be encrypted. The text is 
        /// assumed to be UTF-8-encoded.</param>
        /// <param name="password">The password from which a pseudo-random 
        /// encryption key will be derived.</param>
        /// <param name="salt">A salt value used to generate the a pseudo-random
        /// encryption key from <paramref name="password"/>. The salt is assumed
        /// to be UTF-8-encoded</param>
        /// <param name="initVector">An initialisation vector (IV) to encrypt
        /// the first block of plain text data. This vector must comprise of
        /// exactly 16 ASCII characters.</param>
        /// <param name="hashName">The hash algorithm used to generate the key.
        /// This can be <see cref="MD5_HASH_NAME"/> or 
        /// <see cref="SHA1_HASH_NAME"/>.</param>
        /// <param name="keySize">The size of the encryption key being 
        /// generated. This can be 128, 192 or 256.</param>
        /// <param name="iterations">The number of iterations used to generate 
        /// the pseudo-random key. Two iterations can be assumed to be 
        /// sufficient.</param>
        /// <param name="cipherMode">The encryption mode being used. Using
        /// Cipher Block Chaining (CBC) is reasonable.</param>
        /// <returns>A base64-encoded string containing the encryption 
        /// result.</returns>
        public static string Encrypt(string text, string password, string salt,
                string initVector = DEFAULT_INIT_VECTOR,
                string hashName = SHA1_HASH_NAME, int keySize = 256,
                int iterations = 2, CipherMode cipherMode = CipherMode.CBC) {
            return RijndaelHelper.Encrypt(text, Encoding.UTF8, password, salt,
                Encoding.UTF8, initVector, hashName, keySize, iterations,
                cipherMode);
        }

        /// <summary>
        /// Compute the encryption key from the password and the salt value.
        /// </summary>
        /// <param name="password"></param>
        /// <param name="salt"></param>
        /// <param name="saltEncoding"></param>
        /// <param name="hashName"></param>
        /// <param name="keySize"></param>
        /// <param name="iterations"></param>
        /// <returns></returns>
        private static byte[] calcKey(string password, string salt, 
                Encoding saltEncoding, string hashName, int keySize, 
                int iterations) {

            /* Sanity checks. */
            if (password == null) {
                throw new ArgumentNullException("password");
            }
            if (salt == null) {
                throw new ArgumentNullException("salt");
            }
            if (saltEncoding == null) {
                throw new ArgumentNullException("saltEncoding");
            }

            byte[] saltBytes = saltEncoding.GetBytes(salt);

            /* Derive key from password, salt and hash. */
            PasswordDeriveBytes pdb = new PasswordDeriveBytes(password,
                saltBytes, hashName, iterations);
            byte[] keyBytes = pdb.GetBytes(keySize / 8);

            return keyBytes;
        }

    }
}
