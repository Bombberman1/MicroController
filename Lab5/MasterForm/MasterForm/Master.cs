using System;
using System.Data;
using System.Linq;
using System.Windows.Forms;
using System.IO.Ports;
using System.Drawing;
using System.Threading;

namespace MasterForm
{
    public partial class Master : Form
    {

        public Master()
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
                    serialPort1.DataReceived += serialPort1_DataReceived;
                    buttonOpenPort.Text = "Close";
                    comboBox1.Enabled = false;
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
            }
        }

        private void displayDataSlaveA(string data)
        {
            textBox1.AppendText(data);
            textBox1.AppendText(Environment.NewLine);
            messageA = "";
            modeA = false;
        }

        private void displayDataSlaveB(string data)
        {
            textBox2.AppendText(data);
            textBox2.AppendText(Environment.NewLine);
            messageB = "";
            modeB = false;
        }

        private string messageA = "";
        private string messageB = "";
        private bool modeA = false;
        private bool modeB = false;

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            byte[] buffer = new byte[1];
            serialPort1.Read(buffer, 0, 1);

            BeginInvoke(new Action(() =>
            {
                if (buffer[0] == 24 && !modeA && !modeB) 
                {
                    modeA = true;
                } else if (buffer[0] == 45 && !modeA && !modeB)
                {
                    modeB = true;
                } else
                {
                    if(modeA)
                    {
                        messageA += (char)buffer[0];
                        if(messageA.Length == 22)
                        {
                            displayDataSlaveA(messageA);
                        }
                    } else if(modeB)
                    {
                        messageB += (char)buffer[0];
                        if (messageB.Length == 10)
                        {
                            displayDataSlaveB(messageB);
                        }
                    }
                }
            }));
        }
    }
}
