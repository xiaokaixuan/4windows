using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net.NetworkInformation;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Windows.Networking.Connectivity;
using Windows.Networking.NetworkOperators;
using static System.Net.WebRequestMethods;

namespace Wifi_Share
{
    public partial class FrmMain : Form
    {
        private List<string> _netGuids = new List<string>();
        private List<(string, string)> _wifiNames = new List<(string, string)>();
        private NetworkOperatorTetheringManager _tetheringManager;

        public FrmMain()
        {
            InitializeComponent();

            ReadWifiNames();
            comboBox2.SelectedIndex = 0;
        }

        private async void button1_Click(object sender, EventArgs e)
        {
            if (_tetheringManager != null)
            {
                await _stopAccessPoint();
                return;
            }
            string netGuid = _netGuids.Count > 0 ?_netGuids[comboBox1.SelectedIndex] : null;
            if (string.IsNullOrEmpty(netGuid))
            {
                MessageBox.Show("没有可共享的网络适配器 ！", "系统提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            var shareProfile = NetworkInformation.GetConnectionProfiles().FirstOrDefault(p =>
            {
                string guid = p.NetworkAdapter.NetworkAdapterId.ToString("B").ToLower();
                return guid == netGuid;
            });
            if (shareProfile == null)
            {
                MessageBox.Show("没有可共享的网络适配器 ！", "系统提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            string SSID = comboBox3.Text, password = textBox1.Text;
            if (string.IsNullOrEmpty(SSID) || password.Length < 8)
            {
                MessageBox.Show("Wi-Fi名称和密码配置错误 ！", "系统提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            SaveWifiName(SSID, password);
            MouseCursor.Wait();
            var tetheringManager = NetworkOperatorTetheringManager.CreateFromConnectionProfile(shareProfile);
            var tetherConfig = tetheringManager.GetCurrentAccessPointConfiguration();
            tetherConfig.Band = comboBox2.SelectedIndex == 0 ? TetheringWiFiBand.TwoPointFourGigahertz : TetheringWiFiBand.FiveGigahertz;
            tetherConfig.Ssid = SSID;
            tetherConfig.Passphrase = password;
            await tetheringManager.ConfigureAccessPointAsync(tetherConfig);
            await tetheringManager.StartTetheringAsync();
            _tetheringManager = tetheringManager;
            button1.Text = "关闭热点(&P)";
            MouseCursor.Default();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            SingleProcess.Instance.SaveMainWindow(Handle);
            foreach (var item in NetworkInterface.GetAllNetworkInterfaces())
            {
                if (item.NetworkInterfaceType == NetworkInterfaceType.Loopback) continue;
                if (item.Speed <= 0) continue;

                comboBox1.Items.Add(item.Name);
                _netGuids.Add(item.Id.ToLower());
            }
            if (comboBox1.Items.Count > 0) comboBox1.SelectedIndex = 0;
            foreach (var item in _wifiNames)
            {
                comboBox3.Items.Add(item.Item1);
            }
            if (comboBox3.Items.Count > 0) comboBox3.SelectedIndex = 0;
        }

        private void comboBox3_SelectedValueChanged(object sender, EventArgs e)
        {
            if (comboBox3.SelectedIndex < 0) return;
            textBox1.Text = _wifiNames[comboBox3.SelectedIndex].Item2;
        }

        private async Task _stopAccessPoint()
        {
            if (_tetheringManager == null) return;
            MouseCursor.Wait();
            await _tetheringManager.StopTetheringAsync();
            _tetheringManager = null;
            button1.Text = "强制开启热点(&O)";
            MouseCursor.Default();
        }

        private void ReadWifiNames()
        {
            try
            {
                foreach (string line in System.IO.File.ReadLines("Wifi_Names.txt"))
                {
                    string[] wifis = line.Split('|');
                    if (wifis.Length < 2) continue;
                    _wifiNames.Add((wifis[0], wifis[1]));
                }
            }
            catch (Exception)
            {
                _wifiNames.Add(("Huode_WiFi", "image510"));
                _wifiNames.Add(("HuodeSmartEye", "qwer1234"));
                WriteWifiNames();
            }
        }
        private void WriteWifiNames()
        {
            try
            {
                string[] lines = _wifiNames.Select(wifi => (wifi.Item1 + '|' + wifi.Item2)).ToArray();
                System.IO.File.WriteAllLines("Wifi_Names.txt", lines);
            }
            catch (Exception) { }
        }
        private void SaveWifiName(string WifiName, string password)
        {
            var index = _wifiNames.FindIndex(wifi => wifi.Item1 == WifiName);
            if (index == -1)
            {
                _wifiNames.Add((WifiName, password));
                comboBox3.Items.Add(WifiName);
            }
            else if (_wifiNames[index].Item2 == password) return;
            else
            {
                _wifiNames[index] = (WifiName, password);
            }
            WriteWifiNames();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("ncpa.cpl");
        }
    }
}

