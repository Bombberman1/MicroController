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
        private int Alength = 0;
        private string messageB = "";
        private int Blength = 0;
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
                        if (messageA.Length == 0)
                        {
                            if (buffer[0] >= 191 && buffer[0] <= 255)
                            {
                                messageA += "-";
                                messageA += (256 - buffer[0]).ToString();
                                Alength = messageA.Length;
                            } else
                            {
                                messageA += buffer[0].ToString();
                                Alength = messageA.Length;
                            }
                        }
                        else
                        {
                            if (messageA.Length == Alength)
                            {
                                messageA += "     ";
                            }
                            messageA += buffer[0].ToString("X2");
                            if (messageA.Length == (9 + Alength))
                            {
                                displayDataSlaveA(messageA);
                            }
                        }
                    } else if(modeB)
                    {
                        if(messageB.Length == 0)
                        {
                            if (buffer[0] >= 191 && buffer[0] <= 255)
                            {
                                messageB += "-";
                                messageB += (256 - buffer[0]).ToString();
                                Blength = messageB.Length;
                            }
                            else
                            {
                                messageB += buffer[0].ToString();
                                Blength = messageB.Length;
                            }
                        } else
                        {
                            if (messageB.Length == Blength)
                            {
                                messageB += "     ";
                            }
                            messageB += buffer[0].ToString("X2");
                            if (messageB.Length == (9 + Blength))
                            {
                                displayDataSlaveB(messageB);
                            }
                        }
                    }
                }
            }));
        }

        private void Master_Load(object sender, EventArgs e)
        {

        }
    }
}
