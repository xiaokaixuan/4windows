using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Wifi_Share
{
    public class MouseCursor
    {
        public static void Wait()
        {
            Application.UseWaitCursor = true;
            Cursor.Current = Cursors.WaitCursor;
        }

        public static void Default()
        {
            Application.UseWaitCursor = false;
            Cursor.Current = Cursors.Default;
        }
    }
}

