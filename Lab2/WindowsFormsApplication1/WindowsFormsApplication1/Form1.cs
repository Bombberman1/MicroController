using System;
using System.Data;
using System.Linq;
using System.Windows.Forms;
using System.IO.Ports;
using System.Drawing;
using System.Threading;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {

        public Form1()
        {
            InitializeComponent();
        }
        
        private void comboBox1_Click(object sender, EventArgs e)
        {
            int num;
            comboBox1.Items.Clear();
            string[] ports = SerialPort.GetPortNames().OrderBy(a => a.Length > 3 && int.TryParse(a.Substring(3), out num) ? num : 0).ToArray();
            comboBox1.Items.AddRange(ports);
        }

        private void buttonOpenPort_Click(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
                try
                {
                    serialPort1.PortName = comboBox1.Text;
                    serialPort1.Open();
                    buttonOpenPort.Text = "Close";
                    comboBox1.Enabled = false;
                    button1.Visible = true;
                    button2.Visible = true;
                }
                catch
                {
                    MessageBox.Show("Port " + comboBox1.Text + " is invalid!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            else
            {
                serialPort1.Close();
                buttonOpenPort.Text = "Open";
                comboBox1.Enabled = true;
                button1.Visible = false;
                button2.Visible = false;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            byte[] b1 = new byte[1];
            b1[0] = 0xA1;
            serialPort1.Write(b1, 0, 1);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            byte[] b1 = new byte[1];
            b1[0] = 0xB1;
            serialPort1.Write(b1, 0, 1);
        }

        private void algo_3()
        {
            for (int i = 1; i <= 4; i++)
            {
                Control panelBegin = Controls.Find("panel" + i, true).FirstOrDefault();
                panelBegin.BackColor = Color.Red;
                Control panelEnd = Controls.Find("panel" + (9 - i), true).FirstOrDefault();
                panelEnd.BackColor = Color.Red;
                Thread.Sleep(900);
                panelBegin.BackColor = Color.SkyBlue;
                panelEnd.BackColor = Color.SkyBlue;
            }
        }

        private void algo_6()
        {
            for (int i = 8; i >= 1; i--)
            {
                Control panel = Controls.Find("panel" + i, true).FirstOrDefault();
                panel.BackColor = Color.Red;
                Thread.Sleep(900);
                panel.BackColor = Color.SkyBlue;
            }
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            byte commandFromArduino = (byte)serialPort1.ReadByte();
            if (commandFromArduino == 0xA1)
            {
                algo_3();
            } else if (commandFromArduino == 0xB1)
            {
                algo_6();
            }
        }
    }
}
