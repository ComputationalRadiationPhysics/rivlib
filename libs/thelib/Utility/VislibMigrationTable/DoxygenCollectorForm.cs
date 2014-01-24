using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace VisLibMigrationTable {

    /// <summary>
    /// Form collecting from Doxygen
    /// </summary>
    public partial class DoxygenCollectorForm : Form {

        /// <summary>
        /// The data target
        /// </summary>
        private List<Entry> data = null;

        /// <summary>
        /// Ctor
        /// </summary>
        public DoxygenCollectorForm() {
            InitializeComponent();
        }

        /// <summary>
        /// Collects data
        /// </summary>
        /// <param name="data">Receives the data</param>
        /// <returns>True on success</returns>
        public bool Collect(List<Entry> data) {

            if (this.openIndexHtmlFileDialog.ShowDialog() != System.Windows.Forms.DialogResult.OK) return false;
            try {
                this.data = data;
                //this.loadDoxygen(this.openIndexHtmlFileDialog.FileName, ref content);
                return this.ShowDialog() == System.Windows.Forms.DialogResult.OK;

            } catch (Exception ex) {
                MessageBox.Show("Unable to collect from Doxygen: " + ex.ToString(),
                    Application.ProductName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            return false;
        }

        /// <summary>
        /// Start collecting
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void DoxygenCollectorForm_Shown(object sender, EventArgs e) {
            try {

                this.loadDoxygen(this.openIndexHtmlFileDialog.FileName);
                this.DialogResult = System.Windows.Forms.DialogResult.OK;
                return;

            } catch (Exception ex) {
                MessageBox.Show("Unable to collect from Doxygen: " + ex.ToString(),
                    Application.ProductName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            this.DialogResult = System.Windows.Forms.DialogResult.Cancel;
        }

        /// <summary>
        /// Loads a html document from a text file
        /// </summary>
        /// <param name="filename">The full file name</param>
        /// <returns>The html document</returns>
        private static HtmlDocument loadHtml(string filename) {
            WebBrowser webControl = new WebBrowser();
            webControl.DocumentText = System.IO.File.ReadAllText(filename);
            do {
                Application.DoEvents();
            } while ((webControl.ReadyState != WebBrowserReadyState.Complete)
                || String.IsNullOrEmpty(webControl.Document.Body.OuterHtml)
                || !webControl.Document.Body.OuterHtml.Contains("</body")
                );
            return webControl.Document;
        }

        /// <summary>
        /// Loads from doxygen HTML
        /// </summary>
        /// <param name="indexPath">The doxygen HTML index file (full path)</param>
        /// <param name="content">Receives the loaded data</param>
        private void loadDoxygen(string indexPath) {
            if (!System.IO.Path.IsPathRooted(indexPath)) {
                throw new Exception("index file path must be absolute");
            }
            if (this.data == null) {
                throw new Exception("data structure not initialized");
            }
            this.data.Clear();

            // Load "index" and find next files
            HtmlDocument doc = loadHtml(indexPath);
            string baseDir = System.IO.Path.GetDirectoryName(indexPath);
            string classesPath = string.Empty;
            string filesPath = string.Empty;
            foreach (HtmlElement l in doc.Links) {
                if (string.IsNullOrWhiteSpace(l.InnerText)) continue;
                if (l.InnerText.ToLower().Contains("classes")) {
                    classesPath = System.IO.Path.Combine(baseDir, getCleanHref(l));
                }
                if (l.InnerText.ToLower().Contains("files")) {
                    filesPath = System.IO.Path.Combine(baseDir, getCleanHref(l));
                }
            }
            if (string.IsNullOrEmpty(classesPath)) throw new Exception("Unable to find \"Classes\" link in index file");
            if (string.IsNullOrEmpty(filesPath)) throw new Exception("Unable to find \"Files\" link in index file");

            // Load "files" and list all includable files
            List<string> includeFiles = new List<string>();
            doc = loadHtml(filesPath);
            baseDir = System.IO.Path.GetDirectoryName(filesPath);
            HtmlElementCollection tabCells = doc.Body.GetElementsByTagName("td");
            foreach (HtmlElement tabCell in tabCells) {
                string attrClass = tabCell.GetAttribute("classname");
                if (string.IsNullOrEmpty(attrClass)) continue;
                if (!attrClass.Equals("indexkey")) continue;

                string filename = tabCell.InnerText.Replace('/', '\\');
                string dir = System.IO.Path.GetDirectoryName(filename);
                //shortnVislibDir(ref dir); not yet, because we need to search for the full path to recover the project
                string ext = System.IO.Path.GetExtension(filename);
                int pos = ext.IndexOf(' ');
                if (pos >= 0) ext = ext.Substring(0, pos);
                if (dir.ToLower().Contains("include")) {
                    filename = dir + System.IO.Path.DirectorySeparatorChar + System.IO.Path.GetFileNameWithoutExtension(filename) + ext;
                    includeFiles.Add(filename);
                }
            }
            includeFiles.Sort();

            // Load all classes
            Dictionary<string, string> classes = new Dictionary<string, string>();
            doc = loadHtml(classesPath);
            baseDir = System.IO.Path.GetDirectoryName(classesPath);
            tabCells = doc.Body.GetElementsByTagName("td");
            foreach (HtmlElement tabCell in tabCells) {
                string attrClass = tabCell.GetAttribute("classname");
                if (string.IsNullOrEmpty(attrClass)) continue;
                if (!attrClass.Equals("indexkey")) continue;

                string className = tabCell.InnerText;
                string namespaces = string.Empty;
                int pos = className.LastIndexOf("::");
                if (pos >= 0) {
                    pos += 2;
                    namespaces = className.Substring(0, pos);
                    className = className.Substring(pos);
                }
                if ((className.First() == '_') || (className.Last() == '_')) continue; // private class

                HtmlElementCollection links = tabCell.GetElementsByTagName("a");
                if (links.Count != 1) continue;

                classes[namespaces + className] = System.IO.Path.Combine(baseDir, getCleanHref(links[0]));
            }
            string[] classNames = classes.Keys.ToArray();

            this.progressBar.Maximum = classNames.Length;
            this.progressBar.Value = 0;
            this.progressBar.Style = ProgressBarStyle.Continuous;
            this.progressBar.Step = 1;

            foreach (string className in classNames) {
                doc = loadHtml(classes[className]);
                this.progressBar.PerformStep();

                HtmlElementCollection uls = doc.GetElementsByTagName("ul");
                if (uls.Count <= 0) {
                    classes[className] = string.Empty;
                    continue; // no source information :-/
                }
                HtmlElement lul = uls[uls.Count - 1];
                uls = lul.GetElementsByTagName("li");
                List<string> localFiles = new List<string>();
                bool hadFiles = false;
                foreach (HtmlElement li in uls) {
                    hadFiles = true;
                    string filename = li.InnerText.Replace('/', '\\');
                    //shortnVislibDir(ref filename); not yet, because we need to recover the project name
                    if (filename.ToLower().Contains("\\include\\")) {
                        localFiles.Add(filename);
                    }
                }

                if (localFiles.Count == 0) {
                    if (hadFiles) {
                        // it is an internal class!
                        classes.Remove(className);
                        continue;
                    }
                    classes[className] = string.Empty;
                    continue; // no source information :-/
                }
                if (localFiles.Count == 1) {
                    classes[className] = localFiles[0];
                    continue; // no source information :-/
                }

                // Good enough a fallback for now
                classes[className] = localFiles[0];
            }

            // finally construct data
            classNames = classes.Keys.ToArray(); // because some keys might got removed
            foreach (string className in classNames) {
                Entry e = new Entry();

                e.vislibClass = removeTemplateName(className);

                string fullFn = classes[className];
                string shortFn = System.IO.Path.GetDirectoryName(fullFn);
                shortFn += "\\" + System.IO.Path.GetFileName(fullFn);
                shortnVislibDir(ref shortFn);
                e.vislibFile = shortFn;
                e.vislibProject = recoverVislibProjectName(fullFn);

                includeFiles.Remove(fullFn);

                e.theClass = string.Empty;
                e.theFile = string.Empty;
                e.theProject = string.Empty;

                this.data.Add(e);
            }

            // add the files without classes
            foreach (string file in includeFiles) {
                Entry e = new Entry();

                e.vislibClass = string.Empty;
                string shortFn = file;
                shortnVislibDir(ref shortFn);
                if (shortFn.LastIndexOf('\\') != 6) continue; // it's a non-relevant subfile
                e.vislibFile = shortFn;
                e.vislibProject = recoverVislibProjectName(file);

                e.theClass = string.Empty;
                e.theFile = string.Empty;
                e.theProject = string.Empty;

                this.data.Add(e);
            }

            // Sort and remove duplicates
            Entry.Cleanup(this.data);

        }

        /// <summary>
        /// Removes template list from a class name
        /// </summary>
        /// <param name="className">A class name</param>
        /// <returns>A class name without template list</returns>
        private string removeTemplateName(string className) {
            int pos = className.LastIndexOf('<');
            if (pos >= 0) return className.Substring(0, pos);
            return className;
        }

        /// <summary>
        /// Extracts the vislib project name from a full-path filename
        /// </summary>
        /// <param name="fullFn">full-path filename</param>
        /// <returns>vislib project name</returns>
        private static string recoverVislibProjectName(string fullFn) {
            int pos = fullFn.LastIndexOf("\\include");
            string project;
            if (pos >= 0) {
                project = fullFn.Substring(0, pos);
                pos = project.LastIndexOf('\\');
                if (pos >= 0) {
                    project = project.Substring(pos + 1);
                } else project = string.Empty;
            } else project = string.Empty;
            return project;
        }

        /// <summary>
        /// shortens a vislib directory
        /// </summary>
        /// <param name="dir">The vislib directory</param>
        private static void shortnVislibDir(ref string dir) {
            int pos = dir.LastIndexOf("vislib\\", StringComparison.CurrentCultureIgnoreCase);
            if (pos >= 0) dir = dir.Substring(pos);
        }

        /// <summary>
        /// Answer a cleaned up href value of the tag
        /// </summary>
        /// <param name="tag">The a tag</param>
        /// <returns>The cleaned href value</returns>
        private static string getCleanHref(HtmlElement tag) {
            string href = tag.GetAttribute("HREF");
            if (href.StartsWith("about:", StringComparison.CurrentCultureIgnoreCase)) href = href.Substring(6);
            href = href.Replace('/', '\\');
            return href;
        }

    }

}
