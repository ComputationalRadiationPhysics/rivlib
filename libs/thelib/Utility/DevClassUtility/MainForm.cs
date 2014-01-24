using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Xml;
using System.Text.RegularExpressions;

namespace DevClassUtility {

    public partial class MainForm : Form {

        /// <summary>
        /// Demo class string
        /// </summary>
        const string demoFileName = "the::demo_class";

        /// <summary>
        /// Ctor
        /// </summary>
        public MainForm() {
            InitializeComponent();
            this.dropPanel.AllowDrop = true;
        }

        /// <summary>
        /// Lazy initialization after form was created
        /// </summary>
        /// <param name="sender">not used</param>
        /// <param name="e">not used</param>
        private void Form1_Load(object sender, EventArgs e) {
            String path = Application.ExecutablePath;
            while (!string.IsNullOrEmpty(path)) {
                if (File.Exists(Path.Combine(path, "TheLib.sln"))) {
                    this.solutionPathTextBox.Text = path;
                    break;
                }
                path = Path.GetDirectoryName(path);
            }
            this.classNameTextBox.Text = demoFileName;
            this.classNameTextBox_TextChanged(this.classNameTextBox, null);

        }

        /// <summary>
        /// Test if input is valid
        /// </summary>
        /// <param name="sender">not used</param>
        /// <param name="e">not used</param>
        private void classNameTextBox_TextChanged(object sender, EventArgs e) {

            // first: parse input
            if (string.IsNullOrWhiteSpace(this.classNameTextBox.Text)) {
                this.statusLabel1.Text = "You must enter a class name, including the namespace";
                this.statusLabel2.Text = "";
                this.statusLabel3.Text = "";
                this.genInfoOK = false;
                return;
            }

            if (this.classNameTextBox.Text == demoFileName) {
                this.statusLabel1.Text = "You must enter a class name, including the namespace";
                this.statusLabel2.Text = "";
                this.statusLabel3.Text = "";
                this.genInfoOK = false;
                return;
            }

            string[] frag = this.classNameTextBox.Text.Trim().Split(new string[] { "::", "." }, StringSplitOptions.None);
            if ((frag == null) || (frag.Length < 2)) {
                this.statusLabel1.Text = "You must enter a class name, including the namespace";
                this.statusLabel2.Text = "";
                this.statusLabel3.Text = "";
                this.genInfoOK = false;
                return;
            }

            this.statusLabel1.Text = "Parsed: " + string.Join("::", frag);

            if (frag[0] != "the") {
                this.statusLabel2.Text = "Root namespace must be \"the\"";
                this.statusLabel3.Text = "";
                this.genInfoOK = false;
                return;
            }

            foreach (string f in frag) {
                if (string.IsNullOrEmpty(f)) {
                    this.statusLabel2.Text = "Empty class name or namespace is not allowed.";
                    this.statusLabel3.Text = "";
                    this.genInfoOK = false;
                    return;
                }
                if (!char.IsLetter(f[0]) && (f[0] != '_')) {
                    this.statusLabel2.Text = "Class name or namespace must start with a letter or underscore '_'.";
                    this.statusLabel3.Text = "";
                    this.genInfoOK = false;
                    return;
                }
                foreach (char c in f) {
                    if (!char.IsLetterOrDigit(c) && (c != '_')) {
                        this.statusLabel2.Text = "Class name or namespace may only use letters, digits, and underscores '_'.";
                        this.statusLabel3.Text = "";
                        this.genInfoOK = false;
                        return;
                    }
                }
            }

            string cn = frag[frag.Length - 1];
            string[] nsf = new string[frag.Length - 1];
            for (int i = 0; i < frag.Length - 1; i++) {
                nsf[i] = frag[i];
            }

            // second: test if namespace already exists:
            string headerDir = Path.Combine(this.solutionPathTextBox.Text, "include");
            string srcDir = Path.Combine(this.solutionPathTextBox.Text, "src");
            for (int i = 0; i < nsf.Length; i++) {
                headerDir = Path.Combine(headerDir, nsf[i]);
                if (i != 0) srcDir = Path.Combine(srcDir, nsf[i]);
            }

            if (Directory.Exists(headerDir) && Directory.Exists(srcDir)) {
                this.statusLabel2.Text = "Directories for namespaces already exist";
            } else {
                this.statusLabel2.Text = "Directories for namespaces will be created";
            }

            // thrid test if files already exist:
            if (File.Exists(Path.Combine(headerDir, cn + ".h"))) {
                this.statusLabel3.Text = "Header file already exists";
                this.genInfoOK = false;
            } else if (File.Exists(Path.Combine(srcDir, cn + ".cpp"))) {
                this.statusLabel3.Text = "Source file already exists";
                this.genInfoOK = false;
            } else {
                this.statusLabel3.Text = "Source Code files will be created";
                this.genInfoOK = true;
            }

        }

        /// <summary>
        /// Property flag if the information for class generation is ok
        /// </summary>
        public bool genInfoOK {
            get { return this.generateButton.Enabled; }
            set { this.generateButton.Enabled = value; }
        }

        /// <summary>
        /// Generates the files and optionally manipluates the VS project files
        /// </summary>
        /// <param name="sender">not used</param>
        /// <param name="e">not used</param>
        private void generateButton_Click(object sender, EventArgs e) {
            string[] frag = this.classNameTextBox.Text.Trim().Split(new string[] { "::", "." }, StringSplitOptions.None);
            string className = frag[frag.Length - 1];
            string[] namespaces = new string[frag.Length - 1];
            for (int i = 0; i < frag.Length - 1; i++) {
                namespaces[i] = frag[i];
            }

            AbstractCodeGenerator codeGen = null;
            if (this.radioButton1.Checked) codeGen = new NormalClassCodeGenerator();
            if (this.radioButton2.Checked) codeGen = new StaticClassCodeGenerator();

            generateFiles(className, namespaces, codeGen);

        }

        /// <summary>
        /// Generates the files
        /// </summary>
        /// <param name="className">The name of the class</param>
        /// <param name="namespaces">The namespaces</param>
        /// <param name="codeGen">The code generator</param>
        private void generateFiles(string className, string[] namespaces,
                AbstractCodeGenerator codeGen) {
            string headerDir = Path.Combine(this.solutionPathTextBox.Text, "include");
            string srcDir = Path.Combine(this.solutionPathTextBox.Text, "src");
            for (int i = 0; i < namespaces.Length; i++) {
                headerDir = Path.Combine(headerDir, namespaces[i]);
                if (i != 0) srcDir = Path.Combine(srcDir, namespaces[i]);
            }
            string headerFile = Path.Combine(headerDir, className + ".h");
            string srcFile = Path.Combine(srcDir, className + ".cpp");

            try {

                if (codeGen == null) {
                    throw new Exception("No code generator set");
                }

                this.ensureDirectory(Path.GetDirectoryName(headerFile));
                this.ensureDirectory(Path.GetDirectoryName(srcFile));

                codeGen.Namespaces = namespaces;
                codeGen.ClassName = className;

                //// DEBUG: to not fuck up the real code base
                //headerFile = Path.Combine(@"C:\tmp\", Path.GetFileName(headerFile));
                //srcFile = Path.Combine(@"C:\tmp\", Path.GetFileName(srcFile));

                File.WriteAllText(headerFile, codeGen.MakeHeaderFile());
                File.WriteAllText(srcFile, codeGen.MakeSourceFile());

                if (this.maniProjFilesCheckBox.Checked) {
                    headerDir = "include";
                    srcDir = "src";
                    for (int i = 0; i < namespaces.Length; i++) {
                        headerDir = Path.Combine(headerDir, namespaces[i]);
                        if (i != 0) srcDir = Path.Combine(srcDir, namespaces[i]);
                    }
                    headerFile = Path.Combine(headerDir, className + ".h");
                    srcFile = Path.Combine(srcDir, className + ".cpp");

                    VcxCrowbar vcxC = new VcxCrowbar();
                    vcxC.ManipulateProjectFiles(
                        this.solutionPathTextBox.Text,
                        headerFile, srcFile, namespaces, className);
                }

            } catch (Exception ex) {
                MessageBox.Show("Creating class files failed: " + ex.ToString());
            }

            this.classNameTextBox_TextChanged(this.classNameTextBox, null);
        }

        /// <summary>
        /// Makes sure the specified directory exists
        /// </summary>
        /// <param name="p">The directory</param>
        private void ensureDirectory(string p) {
            if (Directory.Exists(p)) return;
            Directory.CreateDirectory(p);
        }

        /// <summary>
        /// Accept incoming files
        /// </summary>
        /// <param name="sender">not used</param>
        /// <param name="e">The drag event holding the file names</param>
        private void dropPanel_DragOver(object sender, DragEventArgs e) {
            bool accept = dropPanel_acceptFiles(e);
            if (accept) {
                e.Effect = DragDropEffects.Copy;
            } else {
                e.Effect = DragDropEffects.None;
            }
        }

        /// <summary>
        /// Extracts the files from a drop event
        /// </summary>
        /// <param name="e">The drag event holding the file names</param>
        /// <returns>The file names or null</returns>
        private string[] dropPanel_files(DragEventArgs e) {
            if (e.Data.GetDataPresent(DataFormats.FileDrop)) {
                List<string> files = new List<string>();
                Array a = (Array)e.Data.GetData(DataFormats.FileDrop);
                for (int i = 0; i < a.Length; i++) {
                    files.Add(a.GetValue(i).ToString());
                }
                if (files.Count > 0) return files.ToArray();
            }
            return null;
        }

        /// <summary>
        /// Regular expression for valid vislib cpp files
        /// </summary>
        private static Regex vislibCppMatch = new Regex(@"^(.*)\\([^\\]+)\\src\\([^\\]+)\.cpp$");

        /// <summary>
        /// Regular expression for valid vislib h files
        /// </summary>
        private static Regex vislibHMatch = new Regex(@"^(.*)\\([^\\]+)\\include\\vislib\\([^\\]+)\.h$");

        /// <summary>
        /// Accept incoming cpp/h files
        /// </summary>
        /// <param name="e">The drag event holding the file names</param>
        /// <returns>True if we can accept them</returns>
        private bool dropPanel_acceptFiles(DragEventArgs e) {
            if (this.radioButton3.Checked) {
                string[] files = this.dropPanel_files(e);
                bool hasCppH = false;
                bool hasOther = false;

                foreach(string s in files) {
                    string ext = System.IO.Path.GetExtension(s).ToLower();
                    if (ext == ".cpp") {
                        Match m = vislibCppMatch.Match(s);
                        if (m.Success) hasCppH = true;
                        else hasOther = true;
                    } else 
                    if (ext == ".h") {
                        Match m = vislibHMatch.Match(s);
                        if (m.Success) hasCppH = true;
                        else hasOther = true;
                    } else {
                        hasOther = true;
                    }
                }

                return hasCppH && !hasOther;
            }

            return false;
        }

        /// <summary>
        /// Accept dropped files
        /// </summary>
        /// <param name="sender">not used</param>
        /// <param name="e">The drag event holding the file names</param>
        private void dropPanel_DragDrop(object sender, DragEventArgs e) {
            if (!dropPanel_acceptFiles(e)) return;
            string[] files = dropPanel_files(e);

            // Header, Source, Project, Name
            HashSet<Tuple<string, string, string, string>> filePairs = new HashSet<Tuple<string, string, string, string>>();
            foreach (string f in files) {
                string ext = System.IO.Path.GetExtension(f).ToLower();
                Tuple<string, string, string, string> filePair = null;

                if (ext == ".cpp") {
                    Match m = vislibCppMatch.Match(f);
                    if (!m.Success) throw new Exception("Does not work");
                    string of = string.Format("{0}\\{1}\\include\\vislib\\{2}.h",
                        m.Groups[1].Value, m.Groups[2].Value, m.Groups[3].Value);
                    if (!System.IO.File.Exists(of)) of = null;
                    filePair = new Tuple<string, string, string, string>(of, f, m.Groups[2].Value, m.Groups[3].Value);

                } else if (ext == ".h") {
                    Match m = vislibHMatch.Match(f);
                    if (!m.Success) throw new Exception("Does not work");
                    string of = string.Format("{0}\\{1}\\src\\{2}.cpp",
                        m.Groups[1].Value, m.Groups[2].Value, m.Groups[3].Value);
                    if (!System.IO.File.Exists(of)) of = null;
                    filePair = new Tuple<string, string, string, string>(f, of, m.Groups[2].Value, m.Groups[3].Value);

                } else {
                    throw new Exception("Arglh");
                }
                filePairs.Add(filePair);
            }

            int cnt = 0;
            foreach (Tuple<string, string, string, string> entry in filePairs) {
                VislibMigrationCodeGenerator codeGen = new VislibMigrationCodeGenerator();
                codeGen.OriginalHeaderFile = entry.Item1;
                codeGen.OriginalSourceFile = entry.Item2;

                this.generateFiles(VislibMigrationCodeGenerator.NameTranslation(entry.Item4),
                    VislibMigrationCodeGenerator.VislibNamespacesFromProject(entry.Item3),
                    codeGen);
                cnt++;
            }

            MessageBox.Show(string.Format("{0} File Pairs migrated", cnt), Application.ProductName);

        }

        private void MainForm_Shown(object sender, EventArgs e) {
            //// for debugging
            //this.maniProjFilesCheckBox.Checked = false;
            //this.radioButton3.Checked = true;
            //DataObject data = new DataObject();
            //System.Collections.Specialized.StringCollection files = new System.Collections.Specialized.StringCollection();
            //files.Add(@"C:\dev\vis\vislib\math\include\vislib\AbstractPointImpl.h");
            //data.SetFileDropList(files);
            //DragEventArgs dea = new DragEventArgs(data, 0, this.dropPanel.Left + 10, this.dropPanel.Top + 10, DragDropEffects.All, DragDropEffects.Copy);
            //this.dropPanel_DragDrop(this, dea);
        }

    }

}
