/// <copyright file="CryptContext.cs" company="TheLib Team">
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
 * CryptContext.cs
 * 
 * Copyright (C) 2010 Christoph Müller. Alle Rechte vorbehalten.
 */

using System;
using System.Runtime.InteropServices;
using System.Security.Cryptography.X509Certificates;
using The.System.Interop;


namespace The.Cryptography {

    /// <summary>
    /// This class wraps a Crypto API context.
    /// </summary>
    public class CryptContext : IDisposable {

        public CryptContext(string containerName, ProviderType providerType, 
                CryptContextAcquireFlags flags) {
            this.ContainerName = containerName;
            this.ProviderType = providerType;
            this.Flags = flags;

            if (!cryptAcquireContext(out this.handle, 
                    this.ContainerName,
                    this.ProviderName, 
                    (int) this.ProviderType, 
                    (int) this.Flags)) {
                Win32ErrorCheck.CheckLastError();
            }
        }

        public CryptContext(ProviderType providerType, 
                CryptContextAcquireFlags flags)
            : this(Guid.NewGuid().ToString(), providerType, flags) { }

        public CryptContext(CryptContextAcquireFlags flags)
            : this(Guid.NewGuid().ToString(), ProviderType.RsaFull, flags) { }

        public CryptContext() : this(Guid.NewGuid().ToString(), 
            ProviderType.RsaFull, CryptContextAcquireFlags.NewKeySet) { }

        /// <inheritdoc />
        ~CryptContext() {
            this.Dispose(false);
        }

        #region Properties
        public string ContainerName { 
            get; 
            private set; 
        }

        public IntPtr Handle {
            get {
                return this.handle;
            }
        }

        public string ProviderName { 
            get; 
            set; 
        }

        public ProviderType ProviderType { 
            get; 
            private set; 
        }
        
        public CryptContextAcquireFlags Flags { 
            get; 
            private set; 
        }
        #endregion

        /// <summary>
        /// Create a self-signed X509 certificate with the specified properties.
        /// </summary>
        /// <param name="certProps">The properties of the certificate to be
        /// created.</param>
        /// <returns>The newly created certificate.</returns>
        public X509Certificate2 CreateSelfSignedCertificate(
                SelfSignedCertificateProperties certProps) {
            this.checkDisposed();

            this.GenerateExchangeKey(certProps.KeyLength,
                certProps.IsPrivateKeyExportable);

            byte[] issuer = certProps.SubjectName.RawData;
            GCHandle hIssuer = GCHandle.Alloc(issuer, GCHandleType.Pinned);
            
            CryptKeyProviderInformation kpi = new CryptKeyProviderInformation {
                ContainerName = this.ContainerName,
                KeySpec = (int) KeyType.Exchange,
                ProviderType = (int) ProviderType.RsaFull
            };

            CryptoApiBlob cab = new CryptoApiBlob(issuer.Length, 
                hIssuer.AddrOfPinnedObject());

            IntPtr hCert = certCreateSelfSignCertificate(this.handle,
                cab, 
                0, 
                kpi, 
                IntPtr.Zero,
                SystemTime.FromDateTime(certProps.NotBefore),
                SystemTime.FromDateTime(certProps.NotAfter),
                IntPtr.Zero);

            hIssuer.Free();

            if (hCert == IntPtr.Zero) {
                Win32ErrorCheck.CheckLastError();
            }

            X509Certificate2 retval = new X509Certificate2(hCert);

            // Release our copy of 'hCert'. 'retval' has its own.
            if (!certFreeCertificateContext(hCert)) {
                Win32ErrorCheck.CheckLastError();
            }

            return retval;
        }

        public Key GenerateExchangeKey(int keyLength, bool isExportable) {
            return this.generateKey(KeyType.Exchange, keyLength, isExportable);
        }

        public Key GenerateSignatureKey(int keyLength, bool isExportable) {
            return this.generateKey(KeyType.Signature, keyLength, isExportable);
        }



        public void DestroyKey(Key key) {
            this.checkDisposed();
            if (!cryptDestroyKey(key.Handle)) {
                Win32ErrorCheck.CheckLastError();
            }
        }

        /// <inheritdoc />
        public void Dispose() {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected void Dispose(bool isDisposing) {
            if (this.handle != IntPtr.Zero) {
                if (!cryptReleaseContext(this.handle, 0)) {
                    if (isDisposing) {
                        Win32ErrorCheck.CheckLastError();
                    }
                } else {
                    this.handle = IntPtr.Zero;
                }
            }
        }

        #region P/Invoke structures
        [StructLayout(LayoutKind.Sequential)]
        internal class CryptoApiBlob {
            public int DataLength;
            public IntPtr Data;

            public CryptoApiBlob(int dataLength, IntPtr data) {
                this.DataLength = dataLength;
                this.Data = data;
            }
        }

        [StructLayout(LayoutKind.Sequential, CharSet=CharSet.Unicode)]
        internal class CryptKeyProviderInformation {
            public string ContainerName;
            public string ProviderName;
            public int ProviderType;
            public int Flags;
            public int ProviderParameterCount;
            public IntPtr ProviderParameters;
            public int KeySpec;
        }
        #endregion

        #region P/Invoke declarations
        [DllImport("crypt32.dll", 
            EntryPoint="CertCreateSelfSignCertificate",
            SetLastError=true)]
        private static extern IntPtr certCreateSelfSignCertificate(
            IntPtr providerHandle,
            [In] CryptoApiBlob subjectIssuerBlob,
            int flags,
            [In] CryptKeyProviderInformation keyProviderInfo,
            IntPtr signatureAlgorithm,
            [In] SystemTime startTime,
            [In] SystemTime endTime,
            IntPtr extensions);

        [DllImport("crypt32.dll",
            EntryPoint = "CertFreeCertificateContext",
            SetLastError = true)]
        private static extern bool certFreeCertificateContext(
            IntPtr certContext);

        [DllImport("advapi32.dll",
            EntryPoint = "CryptAcquireContextW",
            ExactSpelling = true,
            CharSet = CharSet.Unicode,
            SetLastError = true)]
        private static extern bool cryptAcquireContext(
            out IntPtr providerContext,
            string containerName,
            string providerName,
            int providerType,
            int flags);

        [DllImport("advapi32.dll", 
            EntryPoint="CryptDestroyKey",
            SetLastError=true)]
        private static extern bool cryptDestroyKey(IntPtr cryptKeyHandle);

        [DllImport("advapi32.dll",
            EntryPoint = "CryptGenKey",
            SetLastError=true)]
        private static extern bool cryptGenKey(
            IntPtr providerContext,
            int algorithmId,
            uint flags,
            out IntPtr cryptKeyHandle);

        [DllImport("advapi32.dll", 
            EntryPoint="CryptReleaseContext", 
            SetLastError=true)]
        private static extern bool cryptReleaseContext(IntPtr ctx, int flags);
        #endregion

        private Key generateKey(KeyType type, int keyLength, bool isExportable) {
            this.checkDisposed();

            uint flags = (isExportable ? 1U : 0U) | (((uint) keyLength) << 16);
            IntPtr hKey;

            if (!cryptGenKey(this.handle, (int) type, flags, out hKey)) {
                Win32ErrorCheck.CheckLastError();
            }

            return new Key(this, hKey, type);
        }

        /// <summary>
        /// Test whether native resources have already been disposed. If so, the
        /// method throws an <see cref="ObjectDisposedException"/>.
        /// </summary>
        private void checkDisposed() {
            if (this.Handle == IntPtr.Zero) {
                throw new ObjectDisposedException(this.GetType().Name);
            }
        }

        IntPtr handle = IntPtr.Zero;
    }
}
