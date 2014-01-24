using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace DevClassUtility {

    /// <summary>
    /// Code generator for vislib migration
    /// </summary>
    internal class VislibMigrationCodeGenerator : AbstractCodeGenerator {

        /// <summary>
        /// The source sourc file
        /// </summary>
        private string origSrcFile;

        /// <summary>
        /// The sourc header file
        /// </summary>
        private string origHdrFile;

        /// <summary>
        /// The string builder for the new header file
        /// </summary>
        private StringBuilder header = null;

        /// <summary>
        /// The string builder for the new source file
        /// </summary>
        private StringBuilder source = null;

        /// <summary>
        /// Gets or sets the original source file
        /// </summary>
        public string OriginalSourceFile {
            get { return this.origSrcFile; }
            set { this.origSrcFile = value; }
        }

        /// <summary>
        /// Gets or sets the original header file
        /// </summary>
        public string OriginalHeaderFile {
            get { return this.origHdrFile; }
            set { this.origHdrFile = value; }
        }

        /// <summary>
        /// Gets the generated header file content
        /// </summary>
        /// <returns>The generated header file content</returns>
        public override string MakeHeaderFile() {
            this.generate();
            return (this.header == null) ? string.Empty : this.header.ToString();
        }

        /// <summary>
        /// Gets the generated source file content
        /// </summary>
        /// <returns>The generated source file content</returns>
        public override string MakeSourceFile() {
            this.generate();
            return (this.source == null) ? string.Empty : this.source.ToString();
        }

        /// <summary>
        /// Translates from Camel Case to Lower Case
        /// </summary>
        /// <param name="p">A name in camel case</param>
        /// <returns>The result in lower case</returns>
        public static string NameTranslation(string p) {

            // some pre heuristics
            p = p.Replace("OpenGL", "Opengl_");
            p = p.Replace("GLSL", "Glsl_");
            p = p.Replace("ARB", "Arb_");
            p = p.Replace("VIS", "Vis_");
            p = p.Replace("D3D9", "D3d9_");
            p = p.Replace("D3D", "D3d_");
            p = p.Replace("3D", "3d_");
            p = p.Replace("2D", "2d_");
            p = p.Replace("RGBA", "Rgba_");
            p = p.Replace("IP", "Ip_");
            p = p.Replace("ASCII", "Ascii_");
            p = p.Replace("COM", "Com_");
            p = p.Replace("IO", "Io_");
            p = p.Replace("IPC", "Ipc_");
            if (p.Last() == '_') p = p.Remove(p.Length - 1);

            // insert '_' before each word
            StringBuilder sb = new StringBuilder();
            char lc = 'X';
            foreach (char c in p) {
                if (char.IsUpper(c) && char.IsLower(lc)) {
                    sb.Append('_');
                }
                sb.Append(char.ToLower(c));
                lc = c;
            }

            return sb.ToString();
        }

        /// <summary>
        /// Translates from vislib projects to TheLib namespaces
        /// </summary>
        /// <param name="p">The vislib project</param>
        /// <returns>The TheLib namespace</returns>
        public static string[] VislibNamespacesFromProject(string p) {
            if (p.Equals("base", StringComparison.InvariantCultureIgnoreCase)) {
                return new string[] { "the" };
            }
            if (p.Equals("sys", StringComparison.InvariantCultureIgnoreCase)) {
                return new string[] { "the", "system" };
            }
            if (p.Equals("gl", StringComparison.InvariantCultureIgnoreCase)) {
                return new string[] { "the", "graphics", "gl" };
            }
            if (p.Equals("d3d", StringComparison.InvariantCultureIgnoreCase)) {
                return new string[] { "the", "graphics", "d3d" };
            }
            if (p.Equals("cluster", StringComparison.InvariantCultureIgnoreCase)) {
                return new string[] { "the", "net", "cluster" };
            }
            if (p.Equals("clustergl", StringComparison.InvariantCultureIgnoreCase)) {
                return new string[] { "the", "net", "cluster" };
            }
            if (p.Equals("clusterd3d", StringComparison.InvariantCultureIgnoreCase)) {
                return new string[] { "the", "net", "cluster" };
            }
            if (p.Equals("ibcomm", StringComparison.InvariantCultureIgnoreCase)) {
                return new string[] { "the", "net", "ib" };
            }
            return new string[] { "the", p.ToLower() };
        }

        /// <summary>
        /// Processes everything and produces the output
        /// </summary>
        private void generate() {
            if ((this.source != null) && (this.header != null)) return; // already generated

            this.source = new StringBuilder();
            this.header = new StringBuilder();

            string oldH = string.Empty;
            try {
                oldH = System.IO.File.ReadAllText(this.origHdrFile);
            } catch {
            }
            string oldC = string.Empty;
            try {
                oldC = System.IO.File.ReadAllText(this.origSrcFile);
            } catch {
            }

            HashSet<string> names = new HashSet<string>();
            string oldnamespaces = string.Empty;

            if (!string.IsNullOrEmpty(oldH)) {
                // process header to this.header
                this.header.Append(this.copyrightComment(string.Format("{0}/{1}.h", this.JoinedNamespace("/"), this.ClassName)));

                // vislib header comment
                MatchCollection matches = Regex.Matches(oldH, @"/\*.*?\*/", RegexOptions.Singleline);
                if ((matches.Count > 0) && (matches[0].Success == true)) {
                    if (matches[0].Index < 10) { // is really at the beginning of the file
                        this.header.Append(matches[0].Value);
                        this.header.Append("\n");
                    }
                }
                this.header.Append(this.includeGuardStart);

                // now remove include guards
                int idx;
                for (int i = 0; i < 2; i++) {
                    idx = oldH.IndexOf("#endif");
                    oldH = oldH.Substring(idx + 1);
                }
                idx = oldH.IndexOf('\n');
                oldH = oldH.Substring(idx + 1);
                idx = oldH.LastIndexOf("#if");
                oldH = oldH.Substring(0, idx);

                // Find block before namespaces
                matches = Regex.Matches(oldH, @"^\s*namespace\s*(\w*)\s*{\s*$", RegexOptions.Multiline);
                if (matches.Count > 0) {
                    foreach (Match m in matches) {
                        oldnamespaces = m.Groups[1].Value + "::" + oldnamespaces;
                    }
                    string prens = oldH.Substring(0, matches[0].Index);
                    prens.Replace("\r\n", "\n");
                    int len = prens.Length + 1;
                    while (len != prens.Length) {
                        len = prens.Length;
                        prens.Replace("\n\n", "\n");
                    }

                    prens = prens.Replace("#include \"vislib", "#include \"the");

                    this.header.Append(prens);
                    idx = oldH.IndexOf('\n');

                    oldH = oldH.Substring(matches[matches.Count - 1].Index + matches[matches.Count - 1].Length);
                    for (int i = 0; i < matches.Count; i++) {
                        idx = oldH.LastIndexOf("}");
                        oldH = oldH.Substring(0, idx);
                    }
                }

                this.header.Append("\n");
                this.header.Append("\n");
                this.addNamespacesOpen(this.header);

                // now find all symbol names to be changed
                //  - classes names
                matches = Regex.Matches(oldH, @"class\s+(\w+)\s*(?::|{)");
                foreach (Match m in matches) {
                    names.Add(m.Groups[1].Value);
                }
                //  - method names
                matches = Regex.Matches(oldH, @"(\w+)\s*\([^\)]*\)[^;{\.]*(?:;|{)", RegexOptions.Singleline);
                foreach (Match m in matches) {
                    names.Add(m.Groups[1].Value);
                }
                //  - heuristic fix because this is Regex and no DOM ... :-(
                names.Remove("return");
                List<string> sortedNames = new List<string>();
                sortedNames.AddRange(names);
                sortedNames.Sort(delegate(string a, string b) { return -a.Length.CompareTo(b.Length); });

                foreach (string s in sortedNames) {
                    oldH = oldH.Replace(s, NameTranslation(s));
                }

                this.header.Append(oldH);

                this.addNamespacesClose(this.header);
                this.header.Append(this.includeGuardEnd);
            }
            if (!string.IsNullOrEmpty(oldC)) {
                // process source to this.source
                this.source.Append(this.copyrightComment(string.Format("{0}/{1}.cpp", this.JoinedNamespace("/"), this.ClassName)));

                // vislib header comment
                MatchCollection matches = Regex.Matches(oldC, @"/\*.*?\*/", RegexOptions.Singleline);
                if ((matches.Count > 0) && (matches[0].Success == true)) {
                    if (matches[0].Index < 10) { // is really at the beginning of the file
                        this.source.Append(matches[0].Value);
                        this.source.Append("\n");
                    }
                }
                this.source.Append(string.Format("\n#include \"{0}/{1}.h\"\n", this.JoinedNamespace("/"), this.ClassName));

                List<string> sortedNames = new List<string>();
                sortedNames.AddRange(names);
                sortedNames.Sort(delegate(string a, string b) { return -a.Length.CompareTo(b.Length); });

                matches = Regex.Matches(oldC, @"^#include.*$", RegexOptions.Multiline);
                if (matches.Count > 0) {
                    string precode = oldC.Substring(matches[0].Index, matches[matches.Count - 1].Index + matches[matches.Count - 1].Length - matches[0].Index);
                    precode = precode.Replace("#include \"vislib", "#include \"the");
                    foreach (string s in sortedNames) {
                        precode = precode.Replace(s, NameTranslation(s));
                    }
                    precode = precode.Replace(string.Format("#include \"{0}/{1}.h\"", this.JoinedNamespace("/"), this.ClassName), string.Empty);
                    this.source.Append(precode);
                    oldC = oldC.Substring(matches[matches.Count - 1].Index + matches[matches.Count - 1].Length);
                }

                foreach (string s in sortedNames) {
                    oldC = oldC.Replace(s, NameTranslation(s));
                }
                oldC = oldC.Replace(oldnamespaces, this.JoinedNamespace("::") + "::");

                this.source.Append(oldC);

            }

            if (this.header.Length == 0) {
                // empty header
                this.header.Append(this.copyrightComment(string.Format("{0}/{1}.h", this.JoinedNamespace("/"), this.ClassName)));
                this.header.Append(this.includeGuardStart);
                this.header.Append("\n");
                this.addNamespacesOpen(this.header);
                this.header.Append(@"
/*
 * TODO: Empty?
 */

");
                this.addNamespacesClose(this.header);
                this.header.Append(this.includeGuardEnd);
            }
            this.unifyNewLines(ref this.header);
            if (this.source.Length == 0) {
                // empty source
                this.source.Append(this.copyrightComment(string.Format("{0}/{1}.cpp", this.JoinedNamespace("/"), this.ClassName)));
                this.source.Append(string.Format("\n#include \"{0}/{1}.h\"\n", this.JoinedNamespace("/"), this.ClassName));
                this.source.Append(@"

/*
 * TODO: Empty?
 */
");
            }
            this.unifyNewLines(ref this.source);
        }

    }

}
