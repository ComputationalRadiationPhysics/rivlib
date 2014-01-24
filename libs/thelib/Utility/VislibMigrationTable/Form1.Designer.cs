namespace VisLibMigrationTable {
    partial class Form1 {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.VLClassColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.vislibProjectColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.VLFileColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.TheClassColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.theProjectColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.TheFileColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.CommentColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.clearButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.loadDropDownButton = new System.Windows.Forms.ToolStripDropDownButton();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadAndMergeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.generateDropDownButton = new System.Windows.Forms.ToolStripDropDownButton();
            this.collectFromDoxygenHTMLToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.collectFromDoxygenHTMLAndMergeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.copyTableButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripDropDownButton1 = new System.Windows.Forms.ToolStripDropDownButton();
            this.styleguideCrowbar28FixToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripButton();
            this.completenessLabel = new System.Windows.Forms.ToolStripLabel();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.VLClassColumn,
            this.vislibProjectColumn,
            this.VLFileColumn,
            this.TheClassColumn,
            this.theProjectColumn,
            this.TheFileColumn,
            this.CommentColumn});
            this.dataGridView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridView1.Location = new System.Drawing.Point(0, 39);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(1264, 643);
            this.dataGridView1.TabIndex = 0;
            // 
            // VLClassColumn
            // 
            this.VLClassColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.VLClassColumn.HeaderText = "VISlib Class";
            this.VLClassColumn.Name = "VLClassColumn";
            this.VLClassColumn.Width = 87;
            // 
            // vislibProjectColumn
            // 
            this.vislibProjectColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.vislibProjectColumn.HeaderText = "VISlib Project";
            this.vislibProjectColumn.Name = "vislibProjectColumn";
            this.vislibProjectColumn.Width = 95;
            // 
            // VLFileColumn
            // 
            this.VLFileColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.VLFileColumn.HeaderText = "VISlib File";
            this.VLFileColumn.Name = "VLFileColumn";
            this.VLFileColumn.Width = 78;
            // 
            // TheClassColumn
            // 
            this.TheClassColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.TheClassColumn.HeaderText = "THE Class";
            this.TheClassColumn.Name = "TheClassColumn";
            this.TheClassColumn.Width = 82;
            // 
            // theProjectColumn
            // 
            this.theProjectColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.theProjectColumn.HeaderText = "THE Project";
            this.theProjectColumn.Name = "theProjectColumn";
            this.theProjectColumn.Visible = false;
            // 
            // TheFileColumn
            // 
            this.TheFileColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.TheFileColumn.HeaderText = "THE File";
            this.TheFileColumn.Name = "TheFileColumn";
            this.TheFileColumn.Width = 73;
            // 
            // CommentColumn
            // 
            this.CommentColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.CommentColumn.HeaderText = "Comment";
            this.CommentColumn.Name = "CommentColumn";
            this.CommentColumn.Width = 76;
            // 
            // toolStrip1
            // 
            this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStrip1.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.clearButton,
            this.toolStripSeparator1,
            this.loadDropDownButton,
            this.saveButton,
            this.toolStripSeparator2,
            this.generateDropDownButton,
            this.toolStripSeparator3,
            this.copyTableButton,
            this.toolStripDropDownButton1,
            this.toolStripSeparator4,
            this.toolStripLabel1,
            this.completenessLabel});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(1264, 39);
            this.toolStrip1.TabIndex = 1;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // clearButton
            // 
            this.clearButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.clearButton.Image = global::VisLibMigrationTable.Properties.Resources.NewFile;
            this.clearButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.clearButton.Name = "clearButton";
            this.clearButton.Size = new System.Drawing.Size(36, 36);
            this.clearButton.Text = "Clear";
            this.clearButton.Click += new System.EventHandler(this.clearButton_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 39);
            // 
            // loadDropDownButton
            // 
            this.loadDropDownButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.loadDropDownButton.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem,
            this.loadAndMergeToolStripMenuItem});
            this.loadDropDownButton.Image = global::VisLibMigrationTable.Properties.Resources.OpenFromFolder;
            this.loadDropDownButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.loadDropDownButton.Name = "loadDropDownButton";
            this.loadDropDownButton.Size = new System.Drawing.Size(45, 36);
            this.loadDropDownButton.Text = "Load";
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
            this.loadToolStripMenuItem.Text = "Load ...";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.loadToolStripMenuItem_Click);
            // 
            // loadAndMergeToolStripMenuItem
            // 
            this.loadAndMergeToolStripMenuItem.Enabled = false;
            this.loadAndMergeToolStripMenuItem.Name = "loadAndMergeToolStripMenuItem";
            this.loadAndMergeToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
            this.loadAndMergeToolStripMenuItem.Text = "Load and Merge ...";
            // 
            // saveButton
            // 
            this.saveButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.saveButton.Image = global::VisLibMigrationTable.Properties.Resources.SaveToDisk;
            this.saveButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.saveButton.Name = "saveButton";
            this.saveButton.Size = new System.Drawing.Size(36, 36);
            this.saveButton.Text = "Save";
            this.saveButton.Click += new System.EventHandler(this.saveButton_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 39);
            // 
            // generateDropDownButton
            // 
            this.generateDropDownButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.generateDropDownButton.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.collectFromDoxygenHTMLToolStripMenuItem,
            this.collectFromDoxygenHTMLAndMergeToolStripMenuItem});
            this.generateDropDownButton.Image = global::VisLibMigrationTable.Properties.Resources.doxygen;
            this.generateDropDownButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.generateDropDownButton.Name = "generateDropDownButton";
            this.generateDropDownButton.Size = new System.Drawing.Size(45, 36);
            this.generateDropDownButton.Text = "Collect from Doxygen (HTML)";
            // 
            // collectFromDoxygenHTMLToolStripMenuItem
            // 
            this.collectFromDoxygenHTMLToolStripMenuItem.Name = "collectFromDoxygenHTMLToolStripMenuItem";
            this.collectFromDoxygenHTMLToolStripMenuItem.Size = new System.Drawing.Size(305, 22);
            this.collectFromDoxygenHTMLToolStripMenuItem.Text = "Collect from Doxygen (HTML) ...";
            this.collectFromDoxygenHTMLToolStripMenuItem.Click += new System.EventHandler(this.collectFromDoxygenHTMLToolStripMenuItem_Click);
            // 
            // collectFromDoxygenHTMLAndMergeToolStripMenuItem
            // 
            this.collectFromDoxygenHTMLAndMergeToolStripMenuItem.Enabled = false;
            this.collectFromDoxygenHTMLAndMergeToolStripMenuItem.Name = "collectFromDoxygenHTMLAndMergeToolStripMenuItem";
            this.collectFromDoxygenHTMLAndMergeToolStripMenuItem.Size = new System.Drawing.Size(305, 22);
            this.collectFromDoxygenHTMLAndMergeToolStripMenuItem.Text = "Collect from Doxygen (HTML) and Merge ...";
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 39);
            // 
            // copyTableButton
            // 
            this.copyTableButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.copyTableButton.Image = global::VisLibMigrationTable.Properties.Resources.mediawiki;
            this.copyTableButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.copyTableButton.Name = "copyTableButton";
            this.copyTableButton.Size = new System.Drawing.Size(36, 36);
            this.copyTableButton.Text = "Copy as Wiki Table";
            this.copyTableButton.Click += new System.EventHandler(this.copyTableButton_Click);
            // 
            // toolStripDropDownButton1
            // 
            this.toolStripDropDownButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripDropDownButton1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.styleguideCrowbar28FixToolStripMenuItem});
            this.toolStripDropDownButton1.Image = global::VisLibMigrationTable.Properties.Resources.hammer_icon;
            this.toolStripDropDownButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripDropDownButton1.Name = "toolStripDropDownButton1";
            this.toolStripDropDownButton1.Size = new System.Drawing.Size(45, 36);
            this.toolStripDropDownButton1.Text = "toolStripDropDownButton1";
            this.toolStripDropDownButton1.ToolTipText = "Tools";
            // 
            // styleguideCrowbar28FixToolStripMenuItem
            // 
            this.styleguideCrowbar28FixToolStripMenuItem.Name = "styleguideCrowbar28FixToolStripMenuItem";
            this.styleguideCrowbar28FixToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
            this.styleguideCrowbar28FixToolStripMenuItem.Text = "Styleguide Crowbar28 fix";
            this.styleguideCrowbar28FixToolStripMenuItem.Click += new System.EventHandler(this.styleguideCrowbar28FixToolStripMenuItem_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 39);
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripLabel1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripLabel1.Image")));
            this.toolStripLabel1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(111, 36);
            this.toolStripLabel1.Text = "THECompleteness:";
            this.toolStripLabel1.Click += new System.EventHandler(this.toolStripLabel1_Click);
            // 
            // completenessLabel
            // 
            this.completenessLabel.Name = "completenessLabel";
            this.completenessLabel.Size = new System.Drawing.Size(12, 36);
            this.completenessLabel.Text = "?";
            this.completenessLabel.DoubleClick += new System.EventHandler(this.completenessLabel_DoubleClick);
            // 
            // saveFileDialog1
            // 
            this.saveFileDialog1.DefaultExt = "xml";
            this.saveFileDialog1.Filter = "Xml Files|*.xml|All Files|*.*";
            this.saveFileDialog1.Title = "Save Table Data ...";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.DefaultExt = "xml";
            this.openFileDialog1.FileName = "openFileDialog1";
            this.openFileDialog1.Filter = "Xml Files|*.xml|All Files|*.*";
            this.openFileDialog1.Title = "Load Table Data ...";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1264, 682);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.toolStrip1);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.WindowsDefaultBounds;
            this.Text = "VISlib Migration Table";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton clearButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripDropDownButton loadDropDownButton;
        private System.Windows.Forms.ToolStripButton saveButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton copyTableButton;
        private System.Windows.Forms.ToolStripDropDownButton generateDropDownButton;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadAndMergeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem collectFromDoxygenHTMLToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem collectFromDoxygenHTMLAndMergeToolStripMenuItem;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripLabel completenessLabel;
        private System.Windows.Forms.ToolStripButton toolStripLabel1;
        private System.Windows.Forms.DataGridViewTextBoxColumn VLClassColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn vislibProjectColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn VLFileColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn TheClassColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn theProjectColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn TheFileColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn CommentColumn;
        private System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButton1;
        private System.Windows.Forms.ToolStripMenuItem styleguideCrowbar28FixToolStripMenuItem;
    }
}

