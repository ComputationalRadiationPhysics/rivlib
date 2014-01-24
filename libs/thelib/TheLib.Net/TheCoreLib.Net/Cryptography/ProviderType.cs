/// <copyright file="ProviderType.cs" company="TheLib Team">
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
 * ProviderType.cs
 * 
 * Copyright (C) 2010 Christoph Müller. Alle Rechte vorbehalten.
 */


namespace The.Cryptography {

    /// <summary>
    /// Enumeration of possible crypto providers for use with
    /// <seealso cref="CryptContext"/>.
    /// </summary>
    /// <remarks>
    /// The values are derived from wincrypt.h.
    /// </remarks>
    public enum ProviderType : int {
        RsaFull = 1,
        RsaSignature = 2,
        Dss = 3,
        Fortezza = 4,
        MicrosoftExchange = 5,
        Ssl = 6,
        RsaSchannel = 12,
        DssDiffieHellman = 13,
        RsaAes = 24
    }
}
