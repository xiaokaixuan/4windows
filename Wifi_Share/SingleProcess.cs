using System;
using System.Diagnostics;
using System.IO;
using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;
using System.Security.Principal;
using System.Windows.Forms;

namespace Wifi_Share
{
    public class SingleProcess : TSingleton<SingleProcess>
    {
        private static readonly string _uniqueName = $"__Mutex_{nameof(Wifi_Share)}__";
        private readonly bool _createNew = false;
        private readonly bool _isAdmin = false;
        private readonly MemoryMappedFile _mapFile;

        public bool IsNew => _createNew;
        public bool IsAdmin => _isAdmin;

		[DllImport("User32", CharSet = CharSet.Unicode, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		private static extern bool IsIconic(IntPtr hWnd);

		[DllImport("User32", CharSet = CharSet.Unicode)]
		[return: MarshalAs(UnmanagedType.Bool)]
		private static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

		[DllImport("User32", CharSet = CharSet.Unicode, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		private static extern bool SetForegroundWindow(IntPtr hWnd);

        private SingleProcess()
        {
            WindowsIdentity identity = WindowsIdentity.GetCurrent();
            WindowsPrincipal principal = new WindowsPrincipal(identity);
            _isAdmin = principal.IsInRole(WindowsBuiltInRole.Administrator);
            if (!_isAdmin) return;
            try
            {
                _mapFile = MemoryMappedFile.CreateNew(_uniqueName, 16);
                _createNew = true;
            }
            catch (IOException)
            {
                _mapFile = MemoryMappedFile.OpenExisting(_uniqueName);
            }
        }
        ~SingleProcess()
        {
            _mapFile?.Dispose();
        }
        public void ShowMainWindow()
        {
            if (null == _mapFile) return;
            using (MemoryMappedViewStream stream = _mapFile.CreateViewStream())
            {
                BinaryReader reader = new BinaryReader(stream);
                IntPtr hWindow = new IntPtr(reader.ReadInt64());
                if (hWindow == IntPtr.Zero) return;
                if (IsIconic(hWindow))
                {
                    ShowWindow(hWindow, 9); // SW_RESTORE
                }
                SetForegroundWindow(hWindow);
            }
        }
        public void SaveMainWindow(IntPtr hWindow)
        {
            if (null == _mapFile) return;
            using (MemoryMappedViewStream stream = _mapFile.CreateViewStream())
            {
                BinaryWriter writer = new BinaryWriter(stream);
                writer.Write(hWindow.ToInt64());
            }
        }
        public void RunSelfAsAdmin()
        {
            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.FileName = Application.ExecutablePath;
            startInfo.UseShellExecute = true;
            startInfo.Verb = "runas";
            Process.Start(startInfo);
        }
    }
}

