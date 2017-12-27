namespace ir_decode_app
{
    partial class Form1
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
            this.components = new System.ComponentModel.Container();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.cbxCom = new System.Windows.Forms.ComboBox();
            this.btCon = new System.Windows.Forms.Button();
            this.btDis = new System.Windows.Forms.Button();
            this.btLearn = new System.Windows.Forms.Button();
            this.tbCmd = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(33, 38);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(207, 76);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // cbxCom
            // 
            this.cbxCom.FormattingEnabled = true;
            this.cbxCom.Location = new System.Drawing.Point(57, 53);
            this.cbxCom.Name = "cbxCom";
            this.cbxCom.Size = new System.Drawing.Size(92, 21);
            this.cbxCom.TabIndex = 1;
            this.cbxCom.DropDown += new System.EventHandler(this.cbxCom_DropDown);
            // 
            // btCon
            // 
            this.btCon.Location = new System.Drawing.Point(57, 80);
            this.btCon.Name = "btCon";
            this.btCon.Size = new System.Drawing.Size(71, 23);
            this.btCon.TabIndex = 2;
            this.btCon.Text = "Connect";
            this.btCon.UseVisualStyleBackColor = true;
            this.btCon.Click += new System.EventHandler(this.btCon_Click);
            // 
            // btDis
            // 
            this.btDis.Location = new System.Drawing.Point(148, 80);
            this.btDis.Name = "btDis";
            this.btDis.Size = new System.Drawing.Size(73, 23);
            this.btDis.TabIndex = 3;
            this.btDis.Text = "Disconnect";
            this.btDis.UseVisualStyleBackColor = true;
            this.btDis.Click += new System.EventHandler(this.btDis_Click);
            // 
            // btLearn
            // 
            this.btLearn.Location = new System.Drawing.Point(70, 57);
            this.btLearn.Name = "btLearn";
            this.btLearn.Size = new System.Drawing.Size(75, 23);
            this.btLearn.TabIndex = 5;
            this.btLearn.Text = "Learning IR";
            this.btLearn.UseVisualStyleBackColor = true;
            this.btLearn.Click += new System.EventHandler(this.btLearn_Click);
            // 
            // tbCmd
            // 
            this.tbCmd.Location = new System.Drawing.Point(58, 19);
            this.tbCmd.Name = "tbCmd";
            this.tbCmd.Size = new System.Drawing.Size(100, 20);
            this.tbCmd.TabIndex = 6;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.tbCmd);
            this.groupBox1.Controls.Add(this.btLearn);
            this.groupBox1.Location = new System.Drawing.Point(33, 141);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(207, 100);
            this.groupBox1.TabIndex = 7;
            this.groupBox1.TabStop = false;
            // 
            // serialPort1
            // 
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(282, 298);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btDis);
            this.Controls.Add(this.btCon);
            this.Controls.Add(this.cbxCom);
            this.Controls.Add(this.pictureBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.ComboBox cbxCom;
        private System.Windows.Forms.Button btCon;
        private System.Windows.Forms.Button btDis;
        private System.Windows.Forms.Button btLearn;
        private System.Windows.Forms.TextBox tbCmd;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.IO.Ports.SerialPort serialPort1;
    }
}

