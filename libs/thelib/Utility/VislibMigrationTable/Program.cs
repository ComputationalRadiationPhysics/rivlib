using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace VisLibMigrationTable {
    static class Program {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args) {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Form1 mainForm = new Form1();

            if ((args != null) && (args.Length > 0)) {
                mainForm.LoadXmlFile(args[0]);
            }

            Application.Run(mainForm);
        }
    }
}
