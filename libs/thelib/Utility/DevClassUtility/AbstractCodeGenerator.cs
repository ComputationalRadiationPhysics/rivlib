using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DevClassUtility {

    /// <summary>
    /// Abstract base class for code generators
    /// </summary>
    internal abstract class AbstractCodeGenerator {

        #region private fields

        /// <summary>
        /// The namespaces
        /// </summary>
        private string[] namespaces;

        /// <summary>
        /// The class name
        /// </summary>
        private string className;

        #endregion

        #region public properties

        /// <summary>
        /// Gets or sets the namespaces
        /// </summary>
        public string[] Namespaces {
            get { return this.namespaces; }
            set { this.namespaces = value; }
        }

        /// <summary>
        /// Gets or sets the class name
        /// </summary>
        public string ClassName {
            get { return this.className; }
            set { this.className = value; }
        }

        #endregion

        #region public methods

        /// <summary>
        /// Generates the header file content
        /// </summary>
        /// <param name="namespaces">The namespaces</param>
        /// <param name="className">The class name</param>
        /// <returns>the header file content</returns>
        public string MakeHeaderFile(string[] namespaces, string className) {
            this.namespaces = namespaces;
            this.className = className;
            return this.MakeHeaderFile();
        }

        /// <summary>
        /// Generates the source file content
        /// </summary>
        /// <param name="namespaces">The namespaces</param>
        /// <param name="className">The class name</param>
        /// <returns>the source file content</returns>
        public string MakeSourceFile(string[] namespaces, string className) {
            this.namespaces = namespaces;
            this.className = className;
            return this.MakeSourceFile();
        }

        #endregion

        #region public abstract methods

        /// <summary>
        /// Generates the header file content
        /// </summary>
        public abstract string MakeHeaderFile();

        /// <summary>
        /// Generates the source file content
        /// </summary>
        public abstract string MakeSourceFile();

        #endregion

        #region protected utility methods

        protected string copyrightComment(string filename) {
            return this.copyrightComment(filename, DateTime.Now.Year);
        }

        protected string copyrightComment(string filename, int year) {
            return string.Format(@"/*
 * {0}
 *
 * Copyright (c) {1}, TheLib Team (http://www.thelib.org/license)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of TheLib, TheLib Team, nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THELIB TEAM AS IS AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THELIB TEAM BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
", filename, year);
        }

        protected string JoinedNamespace(string glue) {
            return string.Join(glue, this.namespaces);
        }

        protected void unifyNewLines(ref StringBuilder sb) {
            sb.Replace("\r\n", "\n");
            sb.Replace("\n", Environment.NewLine);
        }

        protected string includeGuardSymbol {
            get {
                return string.Format("{0}_{1}_H_INCLUDED",
                    string.Join("_", this.namespaces).ToUpper(),
                    this.className.ToUpper());
            }
        }

        protected string includeGuardStart {
            get {
                return string.Format(@"
#ifndef {0}
#define {0}
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */
", this.includeGuardSymbol);
            }
        }

        protected string includeGuardEnd {
            get {
                return string.Format(@"
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* {0} */
", this.includeGuardSymbol);
            }
        }

        protected string mandatoryHeaderIncludes {
            get {
                return @"#include ""the/config.h""";
            }
        }

        protected void addNamespacesOpen(StringBuilder sb) {
            for (int i = 0; i < this.Namespaces.Length; i++) {
                sb.Append(string.Format("namespace {0} {{\n", this.Namespaces[i]));
            }
        }

        protected void addNamespacesClose(StringBuilder sb) {
            for (int i = this.Namespaces.Length - 1; i >= 0; i--) {
                sb.Append(string.Format("}} /* end namespace {0} */\n", this.Namespaces[i]));
            }
        }

        #endregion

    }

}
