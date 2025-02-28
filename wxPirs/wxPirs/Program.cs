using System;
using System.Windows.Forms;

namespace wxPirs
{
	// Token: 0x02000005 RID: 5
	internal static class Program
	{
		// Token: 0x0600002B RID: 43 RVA: 0x00004E14 File Offset: 0x00003E14
		[STAThread]
		private static void Main(string[] args)
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new MainForm(args));
		}
	}
}
