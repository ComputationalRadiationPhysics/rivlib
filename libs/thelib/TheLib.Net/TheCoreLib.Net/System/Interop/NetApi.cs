/// <copyright file="NetApi.cs" company="TheLib Team">
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
/*
 * NetApi.cs
 * 
 * Copyright (C) 2012 Christoph Müller. Alle Rechte vorbehalten.
 */

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;


namespace The.System.Interop {

    /// <summary>
    /// Exposes methods for managing network shares using netapi32.
    /// </summary>
    public static class NetApi {

        #region ShareInfo Structures
        /// <summary>
        /// Contains the name of the shared resource.
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        [Serializable]
        [ShareInfoLevel(0)]
        public struct ShareInfo0 {

            /// <summary>
            /// Pointer to a Unicode string specifying the share name of a 
            /// resource.
            /// </summary>
            [MarshalAs(UnmanagedType.LPWStr)]
            public string NetName;
        }

        /// <summary>
        /// Contains information about the shared resource, including the name 
        /// and type of the resource, and a comment associated with the 
        /// resource.
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        [Serializable]
        [ShareInfoLevel(1)]
        public struct ShareInfo1 {

            /// <summary>
            /// Pointer to a Unicode string specifying the share name of a 
            /// resource. Calls to the NetShareSetInfo function ignore this 
            /// member.
            /// </summary>
            [MarshalAs(UnmanagedType.LPWStr)]
            public string NetName;

            /// <summary>
            /// A combination of values that specify the type of the shared 
            /// resource. Calls to the NetShareSetInfo function ignore this 
            /// member.
            /// </summary>
            public ShareType Type;

            /// <summary>
            /// Pointer to a Unicode string specifying an optional comment 
            /// about the shared resource.
            /// </summary>
            [MarshalAs(UnmanagedType.LPWStr)]
            public string Remark;
        }

        /// <summary>
        /// Contains information about the shared resource, including name of
        /// the resource, type and permissions, and the number of current 
        /// connections.
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        [Serializable]
        [ShareInfoLevel(2)]
        public struct ShareInfo2 {

            /// <summary>
            /// Pointer to a Unicode string specifying the share name of a 
            /// resource. Calls to the NetShareSetInfo function ignore this 
            /// member.
            /// </summary>
            [MarshalAs(UnmanagedType.LPWStr)]
            public string NetName;

            /// <summary>
            /// A combination of values that specify the type of the shared 
            /// resource. Calls to the NetShareSetInfo function ignore this 
            /// member.
            /// </summary>
            public ShareType Type;

            /// <summary>
            /// Pointer to a Unicode string specifying an optional comment 
            /// about the shared resource.
            /// </summary>
            [MarshalAs(UnmanagedType.LPWStr)]
            public string Remark;

            /// <summary>
            /// Specifies a DWORD value that indicates the shared resource's
            /// permissions for servers running with share-level security. 
            /// A server running user-level security ignores this member. 
            /// This member can be one or more of the following values. 
            /// Calls to the NetShareSetInfo function ignore this member.
            /// </summary>
            /// <remarks>
            /// Note that Windows does not support share-level security.
            /// <remarks>
            /// </summary>
            public uint Permissions;

            /// <summary>
            /// Specifies a DWORD value that indicates the maximum number of 
            /// concurrent connections that the shared resource can 
            /// accommodate. The number of connections is unlimited if the 
            /// value specified in this member is –1.
            /// </summary>
            public uint MaxUses;

            /// <summary>
            /// Specifies a DWORD value that indicates the number of current 
            /// connections to the resource. Calls to the NetShareSetInfo 
            /// function ignore this member.
            /// </summary>
            public uint CurrentUses;

            /// <summary>
            /// Pointer to a Unicode string specifying the local path for the 
            /// shared resource. For disks, shi2_path is the path being shared. 
            /// For print queues, shi2_path is the name of the print queue 
            /// being shared. Calls to the NetShareSetInfo function ignore this 
            /// member.
            /// </summary>
            [MarshalAs(UnmanagedType.LPWStr)]
            public string Path;

            /// <summary>
            /// Pointer to a Unicode string that specifies the share's password 
            /// when the server is running with share-level security. If the 
            /// server is running with user-level security, this member is 
            /// ignored. The shi2_passwd member can be no longer than 
            /// SHPWLEN+1 bytes (including a terminating null character). Calls 
            /// to the NetShareSetInfo function ignore this member.
            /// </summary>
            /// <remarks>
            /// Note that Windows does not support share-level security.
            /// </remarks>
            [MarshalAs(UnmanagedType.LPWStr)]
            public string Password;
        }

        /// <summary>
        /// Contains information about the shared resource including the name 
        /// and type of the resource, and a comment associated with the 
        /// resource.
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        [Serializable]
        [ShareInfoLevel(501)]
        public struct ShareInfo501 {

            /// <summary>
            /// Pointer to a Unicode string specifying the share name of a 
            /// resource. Calls to the NetShareSetInfo function ignore this 
            /// member.
            /// </summary>
            [MarshalAs(UnmanagedType.LPWStr)]
            public string NetName;

            /// <summary>
            /// A combination of values that specify the type of the shared 
            /// resource. Calls to the NetShareSetInfo function ignore this 
            /// member.
            /// </summary>
            public ShareType Type;

            /// <summary>
            /// Pointer to a Unicode string specifying an optional comment 
            /// about the shared resource.
            /// </summary>
            [MarshalAs(UnmanagedType.LPWStr)]
            public string Remark;

            /// <summary>
            /// Reserved; must be zero.
            /// </summary>
            public uint Flags;
        }

        // ShareInfo 502 not implemented because of security descriptors.
        // ShareInfo 503 not implemented because of security descriptors.

        /// <summary>
        /// Contains information about the shared resource.
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        [Serializable]
        [ShareInfoLevel(1005)]
        public struct ShareInfo1005 {

            /// <summary>
            /// bitmask of flags that specify information about the shared 
            /// resource.
            /// </summary>
            public ShareInfo1005Flags Flags;
        }
        #endregion

        #region Enumerations
        /// <summary>
        /// A combination of values that specify the type of the shared 
        /// resource. 
        /// </summary>
        /// <remarks>
        /// These flags are from "lmshare.h"
        /// </remarks>
        [Flags]
        public enum ShareType : uint {

            /// <summary>
            /// Disk drive.
            /// </summary>
            DiskTree = 0,

            /// <summary>
            /// Print queue.
            /// </summary>
            PrintQueue,

            /// <summary>
            /// Communication device.
            /// </summary>
            CommunicationDevice = 2,

            /// <summary>
            /// Interprocess communication (IPC).
            /// </summary>
            Ipc = 3,

            /// <summary>
            /// A temporary share.
            /// </summary>
            Temporary = 0x40000000,

            /// <summary>
            /// Special share reserved for interprocess communication (IPC$) 
            /// or remote administration of the server (ADMIN$). Can also 
            /// refer to administrative shares such as C$, D$, E$, and so 
            /// forth.
            /// </summary>
            Special = 0x80000000
        }

        /// <summary>
        /// A bitmask of flags that specify information about the shared 
        /// resource.
        /// </summary>
        /// <remarks>
        /// These flags are from "lmshare.h"
        /// </remarks>
        public enum ShareInfo1005Flags : uint {

            /// <summary>
            /// The specified share is present in a Dfs tree structure. 
            /// </summary>
            /// <remarks>
            /// This flag cannot be set with NetShareSetInfo.
            /// </remarks>
            Dfs = 0x0001,

            /// <summary>
            /// The specified share is the root volume in a Dfs tree structure.
            /// </summary>
            /// <remarks>
            /// This flag cannot be set with NetShareSetInfo.
            /// </remarks>
            DfsRoot = 0x0002,

            /// <summary>
            /// The specified share disallows exclusive file opens, where reads 
            /// to an open file are disallowed.
            /// </summary>
            RestrictExclusiveOpens = 0x0100,

            /// <summary>
            /// Shared files in the specified share can be forcibly deleted.
            /// </summary>
            ForceSharedDelete = 0x0200,

            /// <summary>
            /// Clients are allowed to cache the namespace of the specified 
            /// share.
            /// </summary>
            AllowNamespaceCaching = 0x0400,

            /// <summary>
            /// The server will filter directory entries based on the access 
            /// permissions that the user on the client computer has for the 
            /// server on which the files reside. Only files for which the user 
            /// has read access and directories for which the user has 
            /// FILE_LIST_DIRECTORY access will be returned. If the user has 
            /// SeBackupPrivilege, all available information will be returned.
            /// </summary>
            /// <remarks>
            /// This flag is supported only on servers running Windows Server 
            /// 2003 with SP1 or later.
            /// </remarks>
            AccessBasedDirectoryEnum = 0x0800,

            /// <summary>
            /// Prevents exclusive caching modes that can cause delays for 
            /// highly shared read-only data. 
            /// </summary>
            /// <remarks>
            /// This flag is supported only on servers running Windows Server 
            /// 2008 R2 or later.
            /// </remarks>
            ForceLevelIIOplock = 0x1000,

            /// <summary>
            /// Enables server-side functionality needed for peer caching 
            /// support. Clients on high-latency or low-bandwidth connections 
            /// can use alternate methods to retrieve data from peers if 
            /// available, instead of sending requests to the server. This is 
            /// only supported on shares configured for manual caching 
            /// (CSC_CACHE_MANUAL_REINT).
            /// </summary>
            /// <remarks>
            /// This flag is supported only on servers running Windows Server 
            /// 2008 R2 or later.
            /// </remarks>
            EnableHash = 0x2000,

            /// <summary>
            /// Enables Continuous Availability on a cluster share. Handles 
            /// that are opened against a continuously available share can 
            /// survive network failures as well as cluster node failures.
            /// </summary>
            /// <remarks>
            /// This flag can only be set on a scoped share on a server that 
            /// meets the following conditions:
            /// <list type="bullet">
            /// <item>
            ///     <term>It is running Windows Server 2012 or later.</term>
            /// </item>
            /// <item>
            ///     <term>It is in a cluster configuration.</term>
            /// </item>
            /// <item>
            ///     <term>It has the "Services for Continuously Available 
            ///     shares" role service installed.</term>
            /// </item>
            /// </list>
            /// Windows 7, Windows Server 2008 R2, Windows Vista, Windows 
            /// Server 2008, and Windows Server 2003:  This flag is not 
            /// supported.
            /// </remarks>
            EnableCa = 0x4000,
        }
        #endregion

        /// <summary>
        /// Answer all shares on the local machine that export the given
        /// local path.
        /// </summary>
        /// <remarks>
        /// Please be aware that the list returned will contain all kinds
        /// of shares, including most importantly shares of
        /// <see cref="The.System.NetApi.ShareType.Special"/>, i.e. the
        /// administrative shares.
        /// </remarks>
        /// <param name="path">The local path which for the exports should
        /// be searched.</param>
        /// <returns>A list of all shares that export the given local
        /// path.</returns>
        /// <exception cref="System.ArgumentNullException">In case the
        /// parameter <paramref name="path"/> is <c>null</c> or empty.
        /// </exception>
        /// <exception cref="System.Win32Exception">In case the native API call 
        /// fails.</exception>
        public static List<ShareInfo2> GetSharesContaining(string path) {
            if (string.IsNullOrWhiteSpace(path)) {
                throw new ArgumentNullException("path");
            }

            List<ShareInfo2> shares = NetApi.NetShareEnum<ShareInfo2>(null);
            path = Path.GetFullPath(path);
            List<ShareInfo2> retval = (from s in shares
                                       where (!string.IsNullOrWhiteSpace(s.Path) && path.StartsWith(s.Path))
                                       select s).ToList();
            return retval;
        }

        /// <summary>
        /// Shares a server resource.
        /// </summary>
        /// <param name="serverName">The DNS or NetBIOS name of the of the 
        /// remote server on which the function is to execute. If this 
        /// parameter is <c>null</c>, the local computer is used.</param>
        /// <param name="shareInfo">The share configuration.</param>
        /// <exception cref="System.Win32Exception">In case of the native
        /// API call failing.</exception>
        public static void NetShareAdd(string serverName, 
                ShareInfo2 shareInfo) {
            uint error = 0;
            uint level = NetApi.getShareInfoLevel(shareInfo.GetType());
            int status = 0;
            
            status = NetApi.netShareAdd(serverName, level, ref shareInfo,
                out error);
            if (status == Win32ErrorCodes.ERROR_INVALID_PARAMETER) {
                string msg = string.Format(
                    Properties.Resources.ErrorShareParamInvalid, error);
                throw new Win32Exception(status, msg);
            } else if (status != Win32ErrorCodes.ERROR_SUCCESS) {
                throw new Win32Exception(status);
            }
        }

        // TODO: Add NetShareAdd for level 502 and 503

        /// <summary>
        /// Shares a resource of the local machine.
        /// </summary>
        /// <param name="path">The local path to be shared.</param>
        /// <param name="netName">The name of the share.</param>
        /// <param name="remark">Optional comments for the share.</param>
        /// <param name="type">The type of the share.</param>
        /// <param name="maxUses">The maximum number of simultaneous connection
        /// or <see cref="System.uint.MaxValue"/> for unlimited connections.
        /// </param>
        /// <exception cref="System.Win32Exception">In case of the native
        /// API call failing.</exception
        public static void NetShareAdd(string path, string netName,
                string remark = null, ShareType type = ShareType.DiskTree, 
                uint maxUses = uint.MaxValue) {
            NetApi.ShareInfo2 shareInfo = new NetApi.ShareInfo2() {
                NetName = netName,
                Type = type,
                Remark = remark,
                Path = path,
                MaxUses = maxUses
            };
            NetApi.NetShareAdd(null, shareInfo);
        }

        /// <summary>
        /// Deletes a share name from a server's list of shared resources,
        /// disconnecting all connections to the shared resource.
        /// </summary>
        /// <param name="serverName">The DNS or NetBIOS name of the of the 
        /// remote server on which the function is to execute. If this 
        /// parameter is <c>null</c>, the local computer is used.</param>
        /// <param name="netName">The name of the share to be deleted.</param>
        /// <exception cref="System.Win32Exception">In case of the native
        /// API call failing.</exception
        public static void NetShareDel(string serverName, string netName) {
            NetApi.netShareDel(serverName, netName, 0);
        }

        /// <summary>
        /// Enumerate all shares of the given machine.
        /// </summary>
        /// <typeparam name="T">The type of information that should be 
        /// retrieved, which must be one of the ShareInfo structures defined
        /// in this class, e.g. <see cref="The.System.NetApi.ShareInfo2"/>
        /// </typeparam>
        /// <param name="serverName">The name of the server which of the shares
        /// should be enumerated. If <c>null</c>, the local machine is 
        /// assumed.</param>
        /// <returns>A list of shares exported from
        /// <paramref name="serverName"/>.</returns>
        /// <exception cref="System.ArgumentException">In case the requested
        /// structure type (generic parameter) is not supported.</exception>
        /// <exception cref="System.Win32Exception">In case the native API call 
        /// fails.</exception>
        public static List<T> NetShareEnum<T>(string serverName = null) {
            IntPtr buffer = IntPtr.Zero;            // Receives the data.
            IntPtr curShare = IntPtr.Zero;          // Cursor for native data.
            uint entriesread = 0;                   // # of entries in 'buffer'.
            List<T> retval = new List<T>();         // Managed result.
            Type type = typeof(T);                  // Requested type.
            int typeSize = Marshal.SizeOf(type);    // Size of requested type.
            uint level = NetApi.getShareInfoLevel(type);
            uint resume_handle = 0;                 // Resume handle for query.
            int status = 0;                         // Result of API call.
            uint totalentries = 0;                  // Total # of shares.
        
            do {
                try {
                    status = NetApi.netShareEnum(serverName, level, ref buffer,
                        (uint) (5 * typeSize), ref entriesread, 
                        ref totalentries, ref resume_handle);
                    if ((status != Win32ErrorCodes.ERROR_SUCCESS)
                            && (status != Win32ErrorCodes.ERROR_MORE_DATA)) {
                        throw new Win32Exception(status);
                    }

                    curShare = buffer;
                    for (int i = 1; i <= entriesread; i++) {
                        T shareInfo = (T) Marshal.PtrToStructure(curShare, 
                            type);
                        retval.Add(shareInfo);
                        curShare = new IntPtr(curShare.ToInt32() + typeSize);
                    }

                } finally {
                    NetApi.netApiBufferFree(buffer);
                }
            } while (status == Win32ErrorCodes.ERROR_MORE_DATA);

            return retval;
        }

        /// <summary>
        /// Retrieves information about a network share.
        /// </summary>
        /// <example>
        /// <code>
        /// NetApi.ShareInfo501 shareInfo = NetApi.NetShareGetInfo<
        ///     NetApi.ShareInfo501>("noh", "software");
        /// </code>
        /// </example>
        /// <typeparam name="T">The type of information that should be 
        /// retrieved, which must be one of the ShareInfo structures defined
        /// in this class, e.g. <see cref="Visus.Utilities.NetApi.ShareInfo2"/>
        /// </typeparam>
        /// <param name="serverName">The name of the server that exports the
        /// share. If <c>null</c>, the local machine is assumed.</param>
        /// <param name="netName">The name of the share.</param>
        /// <returns>The requested information about the share.</returns>
        /// <exception cref="System.ArgumentException">In case the requested
        /// structure type (generic parameter) is not supported.</exception>
        /// <exception cref="System.Win32Exception">In case the native API call 
        /// fails.</exception>
        public static T NetShareGetInfo<T>(string serverName, string netName) {
            IntPtr buffer = IntPtr.Zero;
            uint level = NetApi.getShareInfoLevel(typeof(T));
            T retval = default(T);
            int status = 0;

            try {
                status = NetApi.netShareGetInfo(serverName, netName, level, 
                    ref buffer);
                if (status != Win32ErrorCodes.ERROR_SUCCESS) {
                    throw new Win32Exception(status);
                }

                retval = (T) Marshal.PtrToStructure(buffer, typeof(T));

            } finally {
                NetApi.netApiBufferFree(buffer);
            }

            return retval;
        }

        /// <summary>
        /// This attribute is used to connect the info structures with
        /// their version number, which must be passed into the API.
        /// </summary>
        [AttributeUsage(AttributeTargets.Struct, AllowMultiple = false)]
        internal class ShareInfoLevelAttribute : Attribute {

            /// <summary>
            /// Initialises a new instance.
            /// </summary>
            /// <param name="level">The info level that the 
            /// structure represents.</param>
            public ShareInfoLevelAttribute(uint level) {
                this.Level = level;
            }

            /// <summary>
            /// Gets the info level.
            /// </summary>
            public uint Level {
                get;
                private set;
            }
        }

        /// <summary>
        /// Gets the level of the share info type <paramref name="t"/>.
        /// </summary>
        /// <param name="t"></param>
        /// <returns></returns>
        /// <exception cref="System.ArgumentException">If <paramref name="t"/>
        /// does not have a 
        /// <see cref="The.System.NetApi.ShareInfoLevelAttribute"/>
        /// attached.</exception>
        private static uint getShareInfoLevel(Type t) {
            object[] atts = t.GetCustomAttributes(
                typeof(ShareInfoLevelAttribute), false);
            if (atts.Length == 1) {
                return ((ShareInfoLevelAttribute) atts[0]).Level;
            } else {
                throw new ArgumentException(
                    Properties.Resources.ErrorNetShareInfoInvalid, "t");
            }
        }

        #region P/Invokes
        [DllImport("Netapi32", EntryPoint = "NetApiBufferFree", 
            CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.U4)]
        private static extern int netApiBufferFree(IntPtr Buffer);

        [DllImport("NetApi32", EntryPoint = "NetShareAdd",
            CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.U4)]
        private static extern int netShareAdd(string servername, uint level, 
            ref ShareInfo2 buf, out uint parm_err);

        [DllImport("NetApi32", EntryPoint = "NetShareDel",
            CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.U4)]
        private static extern int netShareDel(string servername, string netname,
            uint reserved);

        [DllImport("NetApi32", EntryPoint = "NetShareEnum",
            CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.U4)]
        private static extern int netShareEnum(string servername, uint level,
            ref IntPtr bufptr, uint prefmaxlen, ref uint entriesread, 
            ref uint totalentries, ref uint resume_handle);

        [DllImport("Netapi32", EntryPoint = "NetShareGetInfo", 
            CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.U4)]
        static extern int netShareGetInfo(string servername, string netname, 
            uint level, ref IntPtr bufptr);
        #endregion

        //const uint NERR_BufTooSmall = 2123; // The API return buffer is too small.
        //const uint NERR_NetNameNotFound = 2310; // This shared resource does not exist

    }
}
