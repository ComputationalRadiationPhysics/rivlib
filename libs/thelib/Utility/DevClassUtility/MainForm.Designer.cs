namespace DevClassUtility
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.solutionPathTextBox = new System.Windows.Forms.TextBox();
            this.classNameTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.statusLabel1 = new System.Windows.Forms.Label();
            this.statusLabel2 = new System.Windows.Forms.Label();
            this.statusLabel3 = new System.Windows.Forms.Label();
            this.generateButton = new System.Windows.Forms.Button();
            this.maniProjFilesCheckBox = new System.Windows.Forms.CheckBox();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.radioButton3 = new System.Windows.Forms.RadioButton();
            this.dropPanel = new System.Windows.Forms.Panel();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(68, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "TheLib Path:";
            // 
            // solutionPathTextBox
            // 
            this.solutionPathTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.solutionPathTextBox.Location = new System.Drawing.Point(83, 12);
            this.solutionPathTextBox.Name = "solutionPathTextBox";
            this.solutionPathTextBox.Size = new System.Drawing.Size(715, 20);
            this.solutionPathTextBox.TabIndex = 1;
            // 
            // classNameTextBox
            // 
            this.classNameTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.classNameTextBox.Location = new System.Drawing.Point(196, 38);
            this.classNameTextBox.Name = "classNameTextBox";
            this.classNameTextBox.Size = new System.Drawing.Size(602, 20);
            this.classNameTextBox.TabIndex = 2;
            this.classNameTextBox.Text = "the::demo_class";
            this.classNameTextBox.TextChanged += new System.EventHandler(this.classNameTextBox_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(9, 41);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(181, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "New Class Name (with Namespaces)";
            // 
            // statusLabel1
            // 
            this.statusLabel1.AutoSize = true;
            this.statusLabel1.Location = new System.Drawing.Point(9, 61);
            this.statusLabel1.Name = "statusLabel1";
            this.statusLabel1.Size = new System.Drawing.Size(67, 13);
            this.statusLabel1.TabIndex = 4;
            this.statusLabel1.Text = "Parse Status";
            // 
            // statusLabel2
            // 
            this.statusLabel2.AutoSize = true;
            this.statusLabel2.Location = new System.Drawing.Point(9, 74);
            this.statusLabel2.Name = "statusLabel2";
            this.statusLabel2.Size = new System.Drawing.Size(97, 13);
            this.statusLabel2.TabIndex = 5;
            this.statusLabel2.Text = "Namespace Status";
            // 
            // statusLabel3
            // 
            this.statusLabel3.AutoSize = true;
            this.statusLabel3.Location = new System.Drawing.Point(9, 87);
            this.statusLabel3.Name = "statusLabel3";
            this.statusLabel3.Size = new System.Drawing.Size(86, 13);
            this.statusLabel3.TabIndex = 6;
            this.statusLabel3.Text = "File/Class Status";
            // 
            // generateButton
            // 
            this.generateButton.Location = new System.Drawing.Point(12, 103);
            this.generateButton.Name = "generateButton";
            this.generateButton.Size = new System.Drawing.Size(75, 23);
            this.generateButton.TabIndex = 7;
            this.generateButton.Text = "Generate";
            this.generateButton.UseVisualStyleBackColor = true;
            this.generateButton.Click += new System.EventHandler(this.generateButton_Click);
            // 
            // maniProjFilesCheckBox
            // 
            this.maniProjFilesCheckBox.AutoSize = true;
            this.maniProjFilesCheckBox.Checked = true;
            this.maniProjFilesCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.maniProjFilesCheckBox.Location = new System.Drawing.Point(93, 107);
            this.maniProjFilesCheckBox.Name = "maniProjFilesCheckBox";
            this.maniProjFilesCheckBox.Size = new System.Drawing.Size(151, 17);
            this.maniProjFilesCheckBox.TabIndex = 8;
            this.maniProjFilesCheckBox.Text = "manipulate VS Project files";
            this.maniProjFilesCheckBox.UseVisualStyleBackColor = true;
            // 
            // radioButton1
            // 
            this.radioButton1.AutoSize = true;
            this.radioButton1.Checked = true;
            this.radioButton1.Location = new System.Drawing.Point(12, 132);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(94, 17);
            this.radioButton1.TabIndex = 9;
            this.radioButton1.TabStop = true;
            this.radioButton1.Text = "\"normal\" Class";
            this.radioButton1.UseVisualStyleBackColor = true;
            // 
            // radioButton2
            // 
            this.radioButton2.AutoSize = true;
            this.radioButton2.Location = new System.Drawing.Point(12, 155);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(80, 17);
            this.radioButton2.TabIndex = 10;
            this.radioButton2.Text = "Static Class";
            this.radioButton2.UseVisualStyleBackColor = true;
            // 
            // radioButton3
            // 
            this.radioButton3.AutoSize = true;
            this.radioButton3.Location = new System.Drawing.Point(12, 178);
            this.radioButton3.Name = "radioButton3";
            this.radioButton3.Size = new System.Drawing.Size(141, 17);
            this.radioButton3.TabIndex = 11;
            this.radioButton3.TabStop = true;
            this.radioButton3.Text = "Drop-Migrate Vislib Class";
            this.radioButton3.UseVisualStyleBackColor = true;
            // 
            // dropPanel
            // 
            this.dropPanel.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.dropPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.dropPanel.Location = new System.Drawing.Point(598, 87);
            this.dropPanel.Name = "dropPanel";
            this.dropPanel.Size = new System.Drawing.Size(200, 100);
            this.dropPanel.TabIndex = 12;
            this.dropPanel.DragDrop += new System.Windows.Forms.DragEventHandler(this.dropPanel_DragDrop);
            this.dropPanel.DragEnter += new System.Windows.Forms.DragEventHandler(this.dropPanel_DragOver);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(810, 359);
            this.Controls.Add(this.dropPanel);
            this.Controls.Add(this.radioButton3);
            this.Controls.Add(this.radioButton2);
            this.Controls.Add(this.radioButton1);
            this.Controls.Add(this.maniProjFilesCheckBox);
            this.Controls.Add(this.generateButton);
            this.Controls.Add(this.statusLabel3);
            this.Controls.Add(this.statusLabel2);
            this.Controls.Add(this.statusLabel1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.classNameTextBox);
            this.Controls.Add(this.solutionPathTextBox);
            this.Controls.Add(this.label1);
            this.Name = "MainForm";
            this.Text = "TheLib Class Generation/Import Tool";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.Shown += new System.EventHandler(this.MainForm_Shown);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox solutionPathTextBox;
        private System.Windows.Forms.TextBox classNameTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label statusLabel1;
        private System.Windows.Forms.Label statusLabel2;
        private System.Windows.Forms.Label statusLabel3;
        private System.Windows.Forms.Button generateButton;
        private System.Windows.Forms.CheckBox maniProjFilesCheckBox;
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.RadioButton radioButton2;
        private System.Windows.Forms.RadioButton radioButton3;
        private System.Windows.Forms.Panel dropPanel;
    }
}

