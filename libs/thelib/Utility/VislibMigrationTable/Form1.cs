using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.Xml.Serialization;
using System.Xml;

namespace VisLibMigrationTable {

    public partial class Form1 : Form {

        /// <summary>
        /// The filename
        /// </summary>
        private string filename;

        /// <summary>
        /// Ctor
        /// </summary>
        public Form1() {
            InitializeComponent();
            this.Icon = Properties.Resources.vislib;
            this.filename = String.Empty;
        }

        /// <summary>
        /// Collects VISlib data from doxygen HTML documentation
        /// </summary>
        /// <param name="sender">not used</param>
        /// <param name="e">not used</param>
        private void collectFromDoxygenHTMLToolStripMenuItem_Click(object sender, EventArgs e) {
            List<Entry> content = new List<Entry>();
            DoxygenCollectorForm dcf = new DoxygenCollectorForm();
            if (dcf.Collect(content)) {
                this.setTable(content);
            }

        }

        /// <summary>
        /// Sets the content of the table
        /// </summary>
        /// <param name="content">The new content</param>
        private void setTable(List<Entry> content) {
            this.dataGridView1.Rows.Clear();
            this.dataGridView1.Rows.AddCopies(0, content.Count);
            int r = 0;
            foreach (Entry e in content) {
                this.dataGridView1.Rows[r].Cells[0].Value = e.vislibClass;
                this.dataGridView1.Rows[r].Cells[1].Value = e.vislibProject;
                this.dataGridView1.Rows[r].Cells[2].Value = e.vislibFile;
                this.dataGridView1.Rows[r].Cells[3].Value = e.theClass;
                this.dataGridView1.Rows[r].Cells[4].Value = e.theProject;
                this.dataGridView1.Rows[r].Cells[5].Value = e.theFile;
                this.dataGridView1.Rows[r].Cells[6].Value = e.comment;
                r++;
            }
        }

        /// <summary>
        /// Gets the content of the table
        /// </summary>
        /// <returns>The content</returns>
        private List<Entry> getTable() {
            List<Entry> l = new List<Entry>();
            foreach (DataGridViewRow r in this.dataGridView1.Rows) {
                Entry e = new Entry();
                e.vislibClass = makeString(r.Cells[0].Value);
                e.vislibProject = makeString(r.Cells[1].Value);
                e.vislibFile = makeString(r.Cells[2].Value);
                e.theClass = makeString(r.Cells[3].Value);
                e.theProject = makeString(r.Cells[4].Value);
                e.theFile = makeString(r.Cells[5].Value);
                e.comment = makeString(r.Cells[6].Value);
                if (!e.isEmpty) l.Add(e);
            }
            return l;
        }

        /// <summary>
        /// What a pain
        /// </summary>
        /// <param name="p"></param>
        /// <returns></returns>
        private string makeString(object p) {
            return (p == null) ? string.Empty : p.ToString();
        }

        /// <summary>
        /// Saves the table content to a file
        /// </summary>
        /// <param name="sender">not used</param>
        /// <param name="e">not used</param>
        private void saveButton_Click(object sender, EventArgs e) {
            this.saveFileDialog1.FileName = this.filename;
            try {
                this.saveFileDialog1.InitialDirectory = System.IO.Path.GetDirectoryName(this.saveFileDialog1.FileName);
            } catch {
            }
            if (this.saveFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK) {
                this.filename = this.saveFileDialog1.FileName;

                List<Entry> data = this.getTable();
                Entry.Cleanup(data);

                XmlWriterSettings setngs = new XmlWriterSettings();
                setngs.Encoding = Encoding.UTF8;
                setngs.Indent = true;
                using (XmlWriter wrtr = XmlWriter.Create(this.filename, setngs)) {
                    XmlSerializer ser = new XmlSerializer(typeof(List<Entry>));
                    ser.Serialize(wrtr, data);
                }

            }
        }

        /// <summary>
        /// Clear the data grid view
        /// </summary>
        /// <param name="sender">not used</param>
        /// <param name="e">not used</param>
        private void clearButton_Click(object sender, EventArgs e) {
            this.dataGridView1.Rows.Clear();
            this.filename = string.Empty;
        }

        /// <summary>
        /// Loads table data from a file
        /// </summary>
        /// <param name="sender">not used</param>
        /// <param name="e">not used</param>
        private void loadToolStripMenuItem_Click(object sender, EventArgs e) {
            this.openFileDialog1.FileName = this.filename;
            try {
                this.openFileDialog1.InitialDirectory = System.IO.Path.GetDirectoryName(this.openFileDialog1.FileName);
            } catch {
            }
            if (this.openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK) {
                LoadXmlFile(this.openFileDialog1.FileName);
            }
        }

        /// <summary>
        /// Loads an xml file
        /// </summary>
        /// <param name="filename">The filename</param>
        public void LoadXmlFile(string filename) {
            try {

                using (XmlReader rdr = XmlReader.Create(filename)) {
                    XmlSerializer ser = new XmlSerializer(typeof(List<Entry>));
                    List<Entry> data = (List<Entry>)ser.Deserialize(rdr);
                    Entry.Cleanup(data);

                    this.dataGridView1.Rows.Clear();
                    this.setTable(data);
                }

                this.filename = filename;
            } catch (Exception ex) {
                MessageBox.Show("Failed to load: " + ex.ToString(),
                    Application.ProductName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// Copy table data as Mediawiki table to the clipboard
        /// </summary>
        /// <param name="sender">not used</param>
        /// <param name="e">not used</param>
        private void copyTableButton_Click(object sender, EventArgs e) {
            List<Entry> data = this.getTable();
            Entry.Cleanup(data);

            StringBuilder sb = new StringBuilder();
            sb.Append("{| class=\"wikitable sortable\"\n! VIS''lib'' Class !! VIS''lib'' Project !! VIS''lib'' File !! THE Class !! THE File !! Comment\n");
            foreach (Entry d in data) {
                sb.Append("|-");
                if (this.isIncompleted(d)) {
                    sb.Append(" style=\"background:#f99\"");
                }
                sb.Append("\n");
                sb.Append(string.Format("| {0} || {1} || {2} || {3} || {5} || {6}\n",
                    new object[] { d.vislibClass, d.vislibProject, d.vislibFile, d.theClass, d.theProject, d.theFile, d.comment }));
            }
            sb.Append("|}\n\nCoverage: " + this.completenessLabel.Text);

            Clipboard.SetData(DataFormats.Text, sb.ToString());
        }

        /// <summary>
        /// Calculates the Completeness
        /// </summary>
        /// <param name="sender">not used</param>
        /// <param name="e">not used</param>
        private void toolStripLabel1_Click(object sender, EventArgs e) {
            int c = 0;
            int a = 0;
            List<Entry> data = this.getTable();
            Entry.Cleanup(data);
            foreach (Entry d in data) {
                c++;
                if (!this.isIncompleted(d)) {
                    a++;
                }
            }

            double pers = 100.0 * ((double)a) / ((double)c);

            this.completenessLabel.Text = String.Format(System.Globalization.CultureInfo.InvariantCulture, "{2:F1}%, {0}/{1}", a, c, pers);

        }

        /// <summary>
        /// Answer if d is incomplete
        /// </summary>
        /// <param name="d">The entry d</param>
        /// <returns>True if d is include</returns>
        private bool isIncompleted(Entry d) {
            return string.IsNullOrWhiteSpace(d.theClass)
                && string.IsNullOrWhiteSpace(d.theFile)
                //&& string.IsNullOrWhiteSpace(d.theProject)
                && (string.IsNullOrWhiteSpace(d.comment)
                    || (d.comment.StartsWith("TODO:", StringComparison.InvariantCultureIgnoreCase)));
        }

        /// <summary>
        /// Copy completeness to clipboard
        /// </summary>
        /// <param name="sender">not used</param>
        /// <param name="e">not used</param>
        private void completenessLabel_DoubleClick(object sender, EventArgs e) {
            Clipboard.SetText(this.completenessLabel.Text);
        }

        /// <summary>
        /// Performs the styleguide Crowbar28 fix
        /// </summary>
        /// <param name="sender">not used</param>
        /// <param name="e">not used</param>
        private void styleguideCrowbar28FixToolStripMenuItem_Click(object sender, EventArgs e) {
            foreach (DataGridViewRow r in this.dataGridView1.Rows) {
                bool changed = false;
                string theClass = makeString(r.Cells[3].Value);
                string theFile = makeString(r.Cells[5].Value);

                if (!string.IsNullOrEmpty(theClass)) {
                    if (theClass.StartsWith("The::")) {
                        this.fixCrowbar28String(ref theClass);
                        changed = true;
                    }
                }
                if (!string.IsNullOrEmpty(theFile)) {
                    if (theFile.StartsWith("The\\")) {
                        this.fixCrowbar28String(ref theFile);
                        changed = true;
                    }
                }

                if (changed) {
                    r.Cells[3].Value = theClass;
                    r.Cells[5].Value = theFile;
                }
            }

        }

        /// <summary>
        /// fixes a string to be conform to style guide crowbar28
        /// </summary>
        /// <param name="str">The string to be fixed</param>
        private void fixCrowbar28String(ref string str) {
            str = Regex.Replace(str, "([a-z]|[0-9])([A-Z])", @"$1_$2").ToLower();
        }

    }

}
