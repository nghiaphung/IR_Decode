using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;

namespace ir_decode_app
{
    public partial class Form1 : Form
    {
        byte[] data = new byte[1];
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Control.CheckForIllegalCrossThreadCalls = false;
            if (cbxCom.Items.Count > 1)
            {
                cbxCom.SelectedIndex = 0;
            }
            btCon.Enabled = true;
            btDis.Enabled = false;
            
        }

        private void btCon_Click(object sender, EventArgs e)
        {
            serialPort1.PortName = cbxCom.Text;
            serialPort1.BaudRate = 115200;
            try
            {
                serialPort1.Open();
                btCon.Enabled = false;
                btDis.Enabled = true;
            }
            catch (Exception)
            {

                MessageBox.Show("Connection Error", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btDis_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort1.Close();
            }
            catch (Exception)
            {

                throw;
            }
            btCon.Enabled = true;
            btDis.Enabled = false;
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            serialPort1.Read(data, 0, 1);
            switch (data[0])
            {
                case 0x65:
                    MessageBox.Show("Start Learning", "Notice", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    break;
                case 0x66:
                    MessageBox.Show("Done Learning", "Notice", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    break;
                case 0x14:
                    tbCmd.Text = "Mute";
                    break;
                case 0x3A:
                    tbCmd.Text = "i+";
                    break;
                case 0x15:
                    tbCmd.Text = "Power";
                    break;
                case 0x3F:
                    tbCmd.Text = "Table";
                    break;
                case 0x25:
                    tbCmd.Text = "arrow";
                    break;
                case 0x38:
                    tbCmd.Text = "empty table";
                    break;
                case 0x00:
                    tbCmd.Text = "Digit 1";
                    break;
                case 0x01:
                    tbCmd.Text = "Digit 2";
                    break;
                case 0x02:
                    tbCmd.Text = "Digit 3";
                    break;
                case 0x03:
                    tbCmd.Text = "Digit 4";
                    break;
                case 0x04:
                    tbCmd.Text = "Digit 5";
                    break;
                case 0x05:
                    tbCmd.Text = "Digit 6";
                    break;
                case 0x06:
                    tbCmd.Text = "Digit 7";
                    break;
                case 0x07:
                    tbCmd.Text = "Digit 8";
                    break;
                case 0x08:
                    tbCmd.Text = "Digit 9";
                    break;
                case 0x09:
                    tbCmd.Text = "Digit 0";
                    break;
                case 0x3B:
                    tbCmd.Text = "Jump";
                    break;
                case 0x1D:
                    tbCmd.Text = "-/--";
                    break;
                case 0x28:
                    tbCmd.Text = "Sound Mode";
                    break;
                case 0x12:
                    tbCmd.Text = "Volume +";
                    break;
                case 0x13:
                    tbCmd.Text = "Volume -";
                    break;
                case 0x10:
                    tbCmd.Text = "Channel +";
                    break;
                case 0x11:
                    tbCmd.Text = "Channel -";
                    break;
                case 0x17:
                    tbCmd.Text = "A/B";
                    break;
                case 0x3C:
                    tbCmd.Text = "Timer On";
                    break;
                case 0x36:
                    tbCmd.Text = "Timer Off";
                    break;
                case 0x74:
                    tbCmd.Text = "Select Left";
                    break;
                case 0x7C:
                    tbCmd.Text = "Select Right";
                    break;
                case 0x75:
                    tbCmd.Text = "PIC MODE Left";
                    break;
                case 0x64:
                    tbCmd.Text = "PIC MODE Right";
                    break;
                case 0x29:
                    tbCmd.Text = "Space Sound";
                    break;
                default:
                    break;
            }
        }

        private void cbxCom_DropDown(object sender, EventArgs e)
        {
            cbxCom.DataSource = SerialPort.GetPortNames();
        }

        private void btLearn_Click(object sender, EventArgs e)
        {
            data[0] = 0x65;
            try
            {
                serialPort1.Write(data, 0, 1);
            }
            catch (Exception)
            {

                throw;
            }
        }
    }
}
