using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;

namespace DevClassUtility {

    /// <summary>
    /// Utility class manipulating VS vcx project files
    /// </summary>
    internal class VcxCrowbar {

        /// <summary>
        /// Manipulates the visual studio project files
        /// </summary>
        /// <param name="solutionPath">Full path to the solution files</param>
        /// <param name="headerFile">Full path of the header file</param>
        /// <param name="srcFile">Full path of the source file</param>
        /// <param name="namespaces">The namespaces</param>
        /// <param name="className">The class name</param>
        public void ManipulateProjectFiles(string solutionPath, string headerFile, string srcFile, string[] namespaces, string className) {
            string vcxprojFile = Path.Combine(solutionPath, "TheLib.vcxproj");
            string vcxprojFiltersFile = Path.Combine(solutionPath, "TheLib.vcxproj.filters");

            if (!File.Exists(vcxprojFile)) throw new Exception("\"TheLib.vcxproj\" not found");
            if (!File.Exists(vcxprojFiltersFile)) throw new Exception("\"TheLib.vcxproj.filters\" not found");

            XmlDocument pDoc = new XmlDocument();
            pDoc.Load(vcxprojFile);
            XmlDocument pfDoc = new XmlDocument();
            pfDoc.Load(vcxprojFiltersFile);

            this.vcxAddFilters(pfDoc, namespaces);
            this.vcxAddHeader(pDoc, headerFile, false);
            this.vcxAddHeader(pfDoc, headerFile, true);
            this.vcxAddSource(pDoc, srcFile, false);
            this.vcxAddSource(pfDoc, srcFile, true);

            pDoc.Save(vcxprojFile);
            pfDoc.Save(vcxprojFiltersFile);
        }

        /// <summary>
        /// Adds a header file to the project file
        /// </summary>
        /// <param name="doc">The project file document</param>
        /// <param name="headerFile">The header file path</param>
        /// <param name="addFilter">True if also add filter</param>
        private void vcxAddHeader(XmlDocument doc, string headerFile, bool addFilter) {
            XmlNode iGrp = this.vcxFindItemGroup(doc, "ClInclude");
            foreach (XmlNode node in iGrp.ChildNodes) {
                if (node.NodeType != XmlNodeType.Element) continue;
                if (node.Name != "ClInclude") continue;
                if (node.Attributes["Include"].Value == headerFile) return; // already exists!
            }

            XmlElement e = (XmlElement)iGrp.AppendChild(iGrp.OwnerDocument.CreateElement("ClInclude", iGrp.NamespaceURI));
            e.SetAttribute("Include", headerFile);

            if (addFilter) {
                XmlElement f = (XmlElement)e.AppendChild(iGrp.OwnerDocument.CreateElement("Filter", iGrp.NamespaceURI));
                string s = Path.GetDirectoryName(headerFile);
                if (s.StartsWith("include\\the\\")) s = s.Substring(12);
                if (string.IsNullOrEmpty(s)) s = ""; else s += "\\";
                f.InnerText = s + "Header Files";
            }

        }

        /// <summary>
        /// Adds a source file to the project file
        /// </summary>
        /// <param name="doc">The project file document</param>
        /// <param name="srcFile">The source file path</param>
        /// <param name="addFilter">True if also add filter</param>
        private void vcxAddSource(XmlDocument doc, string srcFile, bool addFilter) {
            XmlNode iGrp = this.vcxFindItemGroup(doc, "ClCompile");
            foreach (XmlNode node in iGrp.ChildNodes) {
                if (node.NodeType != XmlNodeType.Element) continue;
                if (node.Name != "ClCompile") continue;
                if (node.Attributes["Include"].Value == srcFile) return; // already exists!
            }

            XmlElement e = (XmlElement)iGrp.AppendChild(iGrp.OwnerDocument.CreateElement("ClCompile", iGrp.NamespaceURI));
            e.SetAttribute("Include", srcFile);

            if (addFilter) {
                XmlElement f = (XmlElement)e.AppendChild(iGrp.OwnerDocument.CreateElement("Filter", iGrp.NamespaceURI));
                string s = Path.GetDirectoryName(srcFile);
                if (s.StartsWith("src\\")) s = s.Substring(4);
                if (string.IsNullOrEmpty(s)) s = ""; else s += "\\";
                f.InnerText = s + "Source Files";
            }

        }

        /// <summary>
        /// Adds the required filteres
        /// </summary>
        /// <param name="pfDoc">The document</param>
        /// <param name="namespaces">The namespaces</param>
        private void vcxAddFilters(XmlDocument pfDoc, string[] namespaces) {
            XmlNode iGrp = this.vcxFindItemGroup(pfDoc, "Filter");

            string path = "";
            for (int i = 1; i < namespaces.Length; i++) {
                if (i > 1) path += "\\";
                path += namespaces[i];
                this.vcxAddFilters(iGrp, path);
            }
            this.vcxAddFilters(iGrp, path + "\\Header Files");
            this.vcxAddFilters(iGrp, path + "\\Source Files");

        }

        /// <summary>
        /// Adds a filter if required
        /// </summary>
        /// <param name="iGrp">The item group containing all filteres</param>
        /// <param name="path">The path for the new filter</param>
        private void vcxAddFilters(XmlNode iGrp, string path) {
            foreach (XmlNode node in iGrp.ChildNodes) {
                if (node.NodeType != XmlNodeType.Element) continue;
                if (node.Name != "Filter") continue;
                if (node.Attributes["Include"].Value == path) return; // already exists!
            }

            XmlElement n = (XmlElement)iGrp.AppendChild(
                iGrp.OwnerDocument.CreateElement("Filter", iGrp.NamespaceURI));
            n.SetAttribute("Include", path);
            XmlElement ui = (XmlElement)n.AppendChild(iGrp.OwnerDocument.CreateElement("UniqueIdentifier", iGrp.NamespaceURI));
            ui.InnerText = "{" + Guid.NewGuid().ToString() + "}";
        }

        /// <summary>
        /// Finds the item group which contains children of the specified type
        /// </summary>
        /// <param name="pfDoc">The document</param>
        /// <param name="name">The child type</param>
        /// <returns>The ItemGroup XmlNode</returns>
        private XmlNode vcxFindItemGroup(XmlDocument pfDoc, string name) {
            XmlNodeList groups = pfDoc.GetElementsByTagName("ItemGroup");
            foreach (XmlNode group in groups) {
                if (group.ChildNodes == null) continue;
                foreach (XmlNode child in group) {
                    if (child.NodeType != XmlNodeType.Element) continue;
                    if (child.Name == name) return group;
                }
            }
            throw new Exception("ItemGroup not found");
        }

    }

}
