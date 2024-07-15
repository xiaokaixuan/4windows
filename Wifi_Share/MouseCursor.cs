using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Wifi_Share
{
    public static class MouseCursor
    {
        private static Cursor cursor = Cursors.Default;
        public static void Wait()
        {
            cursor = Cursor.Current;
            Application.UseWaitCursor = true;
            Cursor.Current = Cursors.WaitCursor;
        }

        public static void Restore()
        {
            Application.UseWaitCursor = false;
            Cursor.Current = cursor;
        }
    }
}

