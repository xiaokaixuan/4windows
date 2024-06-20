using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Wifi_Share
{
    internal static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            // 管理员运行
            if (!SingleProcess.Instance.IsAdmin)
            {
                SingleProcess.Instance.RunSelfAsAdmin();
                return;
            }
            // 恢复主窗口
            if (!SingleProcess.Instance.IsNew)
            {
                SingleProcess.Instance.ShowMainWindow();
                return;
            }
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new FrmMain());
        }
    }
}
