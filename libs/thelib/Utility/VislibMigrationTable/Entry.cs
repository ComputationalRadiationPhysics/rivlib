using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;
using System.ComponentModel;

namespace VisLibMigrationTable {

    /// <summary>
    /// Entry class (Row)
    /// </summary>
    public class Entry {
        private string s1 = null;
        private string s2 = null;
        private string s3 = null;
        private string s4 = null;
        private string s5 = null;
        private string s6 = null;
        private string s7 = null;

        [XmlAttribute(), DefaultValue(null)]
        public string vislibClass { get { return this.s1; } set { if (string.IsNullOrEmpty(value)) { this.s1 = null; } else { this.s1 = value; } } }

        [XmlAttribute(), DefaultValue(null)]
        public string vislibProject { get { return this.s2; } set { if (string.IsNullOrEmpty(value)) { this.s2 = null; } else { this.s2 = value; } } }

        [XmlAttribute(), DefaultValue(null)]
        public string vislibFile { get { return this.s3; } set { if (string.IsNullOrEmpty(value)) { this.s3 = null; } else { this.s3 = value; } } }

        [XmlAttribute(), DefaultValue(null)]
        public string theClass { get { return this.s4; } set { if (string.IsNullOrEmpty(value)) { this.s4 = null; } else { this.s4 = value; } } }

        [XmlIgnore()]
        public string theProject { get { return this.s5; } set { if (string.IsNullOrEmpty(value)) { this.s5 = null; } else { this.s5 = value; } } }

        [XmlAttribute(), DefaultValue(null)]
        public string theFile { get { return this.s6; } set { if (string.IsNullOrEmpty(value)) { this.s6 = null; } else { this.s6 = value; } } }

        [XmlAttribute(), DefaultValue(null)]
        public string comment { get { return this.s7; } set { if (string.IsNullOrEmpty(value)) { this.s7 = null; } else { this.s7 = value; } } }

        /// <summary>
        /// Answer if all fields are empty
        /// </summary>
        [XmlIgnore()]
        public bool isEmpty {
            get {
                return (this.s1 == null)
                    && (this.s2 == null)
                    && (this.s3 == null)
                    && (this.s4 == null)
                    && (this.s5 == null)
                    && (this.s6 == null)
                    && (this.s7 == null);
            }
        }

        /// <summary>
        /// Cleans up (sorting, removing empty and duplicated values) a list of entries
        /// </summary>
        /// <param name="data">List of entries</param>
        public static void Cleanup(List<Entry> data) {
            data.Sort(SortEntries);
            int cnt = data.Count;
            for (int i = 0; i < cnt; ) {
                if (data[i].isEmpty) {
                    data.RemoveAt(i);
                } else if ((i + 1 < cnt) && (Entry.SortEntries(data[i], data[i + 1]) == 0)) {
                    data.RemoveAt(i + 1);
                    cnt--;
                    i--;
                } else {
                    i++;
                }
            }
        }

        /// <summary>
        /// Sorts entries
        /// </summary>
        /// <param name="l">The left entry</param>
        /// <param name="r">The right entry</param>
        /// <returns>The sort order</returns>
        public static int SortEntries(Entry l, Entry r) {
            int cmp = 0;

            if (cmp == 0) {
                if (l.s1 == null) {
                    if (r.s1 != null) cmp = -r.s1.CompareTo(l.s1);
                } else cmp = l.s1.CompareTo(r.s1);
            }
            if (cmp == 0) {
                if (l.s2 == null) {
                    if (r.s2 != null) cmp = -r.s2.CompareTo(l.s2);
                } else cmp = l.s2.CompareTo(r.s2);
            }
            if (cmp == 0) {
                if (l.s3 == null) {
                    if (r.s3 != null) cmp = -r.s3.CompareTo(l.s3);
                } else cmp = l.s3.CompareTo(r.s3);
            }
            if (cmp == 0) {
                if (l.s4 == null) {
                    if (r.s4 != null) cmp = -r.s4.CompareTo(l.s4);
                } else cmp = l.s4.CompareTo(r.s4);
            }
            if (cmp == 0) {
                if (l.s5 == null) {
                    if (r.s5 != null) cmp = -r.s5.CompareTo(l.s5);
                } else cmp = l.s5.CompareTo(r.s5);
            }
            if (cmp == 0) {
                if (l.s6 == null) {
                    if (r.s6 != null) cmp = -r.s6.CompareTo(l.s6);
                } else cmp = l.s6.CompareTo(r.s6);
            }
            if (cmp == 0) {
                if (l.s7 == null) {
                    if (r.s7 != null) cmp = -r.s7.CompareTo(l.s7);
                } else cmp = l.s7.CompareTo(r.s7);
            }

            return cmp;
        }

    }

}
