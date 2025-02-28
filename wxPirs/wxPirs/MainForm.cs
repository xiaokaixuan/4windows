using System;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Reflection;
using System.Windows.Forms;
using wxPirs.Properties;
using WxTools;

namespace wxPirs
{
	// Token: 0x02000002 RID: 2
	public partial class MainForm : Form
	{
		// Token: 0x06000003 RID: 3 RVA: 0x000031F1 File Offset: 0x000021F1
		public MainForm(string[] args)
		{
			this.args = args;
			this.InitializeComponent();
		}

		// Token: 0x06000004 RID: 4 RVA: 0x00003211 File Offset: 0x00002211
		public void log(string message)
		{
			this.textBoxLog.AppendText(message);
		}

		// Token: 0x06000005 RID: 5 RVA: 0x0000321F File Offset: 0x0000221F
		private void openFile(object sender, EventArgs e)
		{
			if (this.openFileDialog.ShowDialog() == DialogResult.OK)
			{
				this.openFile(this.openFileDialog.FileName);
			}
		}

		// Token: 0x06000006 RID: 6 RVA: 0x00003240 File Offset: 0x00002240
		private void openFile(string filename)
		{
			this.textBoxLog.Clear();
			this.folderBrowserDialog.SelectedPath = this.wr.extractFolderName(this.openFileDialog.FileName);
			if (this.br != null)
			{
				this.br.Close();
			}
			if (this.fs != null)
			{
				this.fs.Dispose();
			}
			this.treeView.BeginUpdate();
			this.treeView.Nodes.Clear();
			this.treeView.EndUpdate();
			this.listView.BeginUpdate();
			this.listView.Items.Clear();
			this.listView.EndUpdate();
			try
			{
				this.fs = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.Read);
				this.br = new BinaryReader(this.fs);
				this.getDescription();
				this.br.BaseStream.Seek(0L, SeekOrigin.Begin);
				int num = this.wr.readInt32(this.br);
				if (num != MainForm.MAGIC_PIRS && num != MainForm.MAGIC_LIVE && num != MainForm.MAGIC_CON_)
				{
					this.log("Not a PIRS/LIVE file!\r\n");
					this.br.Close();
					this.fs.Close();
				}
				else
				{
					this.br.BaseStream.Seek(49200L, SeekOrigin.Begin);
					int num2 = this.wr.readInt32(this.br);
					if (num == MainForm.MAGIC_CON_)
					{
						this.pirs_offset = MainForm.PIRS_TYPE2;
						this.pirs_start = 49152L;
					}
					else if (num2 == 65535)
					{
						this.pirs_offset = MainForm.PIRS_TYPE1;
						this.pirs_start = MainForm.PIRS_BASE + this.pirs_offset;
					}
					else
					{
						this.pirs_offset = MainForm.PIRS_TYPE2;
						this.pirs_start = MainForm.PIRS_BASE + this.pirs_offset;
					}
					this.parse(filename);
				}
			}
			catch (IOException ex)
			{
				this.log(string.Format("{0}\r\n", ex.Message));
			}
		}

		// Token: 0x06000007 RID: 7 RVA: 0x00003444 File Offset: 0x00002444
		private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
		{
			this.Exit();
		}

		// Token: 0x06000008 RID: 8 RVA: 0x0000344C File Offset: 0x0000244C
		private void exitToolStripMenuItem_Click(object sender, EventArgs e)
		{
			this.Exit();
		}

		// Token: 0x06000009 RID: 9 RVA: 0x00003454 File Offset: 0x00002454
		private void Exit()
		{
			base.Dispose();
		}

		// Token: 0x0600000A RID: 10 RVA: 0x0000345C File Offset: 0x0000245C
		private MainForm.PirsEntry getEntry()
		{
			MainForm.PirsEntry result = default(MainForm.PirsEntry);
			result.Filename = this.wr.readString(this.br, 38U);
			if (result.Filename.Trim() == "")
			{
				return result;
			}
			result.Unknow = this.wr.readInt32(this.br);
			result.BlockLen = this.wr.readInt32(this.br);
			result.Cluster = this.br.ReadInt32() >> 8;
			result.Parent = this.wr.readUInt16(this.br);
			result.Size = this.wr.readInt32(this.br);
			result.DateTime1 = this.dosDateTime(this.wr.readInt32(this.br));
			result.DateTime2 = this.dosDateTime(this.wr.readInt32(this.br));
			return result;
		}

		// Token: 0x0600000B RID: 11 RVA: 0x00003556 File Offset: 0x00002556
		private DateTime dosDateTime(int datetime)
		{
			return this.dosDateTime((short)(datetime >> 16), (short)(datetime - (datetime >> 16 << 16)));
		}

		// Token: 0x0600000C RID: 12 RVA: 0x00003570 File Offset: 0x00002570
		private DateTime dosDateTime(short date, short time)
		{
			if (date == 0 && time == 0)
			{
				return DateTime.Now;
			}
			int year = (((int)date & 65024) >> 9) + 1980;
			int month = (date & 480) >> 5;
			int day = (int)(date & 31);
			int hour = ((int)time & 63488) >> 11;
			int minute = (time & 2016) >> 5;
			int second = (int)((time & 31) * 2);
			DateTime result = new DateTime(year, month, day, hour, minute, second);
			return result;
		}

		// Token: 0x0600000D RID: 13 RVA: 0x000035D8 File Offset: 0x000025D8
		private void getFiles(TreeNode tn)
		{
			int num = 0;
			for (;;)
			{
				this.br.BaseStream.Seek(this.pirs_start + (long)(num * 64), SeekOrigin.Begin);
				MainForm.PirsEntry pirsEntry = default(MainForm.PirsEntry);
				pirsEntry = this.getEntry();
				if (pirsEntry.Filename.Trim() == "")
				{
					break;
				}
				if (pirsEntry.Cluster == 0 && pirsEntry.Size == 0 && pirsEntry.Parent == Convert.ToUInt16(tn.Tag))
				{
					ListViewItem listViewItem = new ListViewItem();
					listViewItem.Text = pirsEntry.Filename;
					listViewItem.SubItems.Add(pirsEntry.Size.ToString());
					listViewItem.SubItems.Add(pirsEntry.Cluster.ToString());
					listViewItem.SubItems.Add(pirsEntry.DateTime1.ToString());
					listViewItem.SubItems.Add("");
					listViewItem.ImageIndex = 0;
					listViewItem.Tag = num;
					this.listView.Items.Add(listViewItem);
				}
				num++;
			}
			num = 0;
			for (;;)
			{
				this.br.BaseStream.Seek(this.pirs_start + (long)(num * 64), SeekOrigin.Begin);
				MainForm.PirsEntry pirsEntry2 = default(MainForm.PirsEntry);
				pirsEntry2 = this.getEntry();
				if (pirsEntry2.Filename.Trim() == "")
				{
					break;
				}
				if (pirsEntry2.Cluster != 0 && pirsEntry2.Parent == Convert.ToUInt16(tn.Tag))
				{
					ListViewItem listViewItem2 = new ListViewItem();
					listViewItem2.Text = pirsEntry2.Filename;
					listViewItem2.SubItems.Add(pirsEntry2.Size.ToString());
					listViewItem2.SubItems.Add(pirsEntry2.Cluster.ToString());
					listViewItem2.SubItems.Add(pirsEntry2.DateTime1.ToString());
					listViewItem2.SubItems.Add("");
					listViewItem2.ImageIndex = 1;
					listViewItem2.ToolTipText = string.Format("Offset : 0x{0:X8}", this.getOffset((long)pirsEntry2.Cluster));
					this.listView.Items.Add(listViewItem2);
				}
				num++;
			}
		}

		// Token: 0x0600000E RID: 14 RVA: 0x00003828 File Offset: 0x00002828
		private void getDirectories(TreeNode tn)
		{
			int num = 0;
			for (;;)
			{
				this.br.BaseStream.Seek(this.pirs_start + (long)(num * 64), SeekOrigin.Begin);
				MainForm.PirsEntry pirsEntry = default(MainForm.PirsEntry);
				pirsEntry = this.getEntry();
				if (pirsEntry.Filename.Trim() == "")
				{
					break;
				}
				if (pirsEntry.Size == 0 && pirsEntry.Cluster == 0 && pirsEntry.Parent == Convert.ToUInt16(tn.Tag))
				{
					TreeNode treeNode = new TreeNode(pirsEntry.Filename);
					treeNode.Tag = num;
					treeNode.ToolTipText = string.Format("0x{0:X4}", treeNode.Tag);
					tn.Nodes.Add(treeNode);
					this.getDirectories(treeNode);
				}
				num++;
			}
		}

		// Token: 0x0600000F RID: 15 RVA: 0x000038F0 File Offset: 0x000028F0
		private void parse(string filename)
		{
			this.treeView.BeginUpdate();
			this.treeView.Nodes.Clear();
			this.listView.BeginUpdate();
			this.listView.Items.Clear();
			TreeNode treeNode = new TreeNode(this.wr.extractFileName(filename), 0, 0);
			treeNode.Tag = ushort.MaxValue;
			treeNode.ToolTipText = string.Format("0x{0:X4}", treeNode.Tag);
			this.treeView.Nodes.Add(treeNode);
			this.getDirectories(treeNode);
			this.getFiles(treeNode);
			treeNode.Expand();
			this.listView.EndUpdate();
			this.treeView.EndUpdate();
		}

		// Token: 0x06000010 RID: 16 RVA: 0x000039A9 File Offset: 0x000029A9
		private void treeView_AfterSelect(object sender, TreeViewEventArgs e)
		{
			this.listView.BeginUpdate();
			this.listView.Items.Clear();
			this.getFiles(e.Node);
			this.listView.EndUpdate();
		}

		// Token: 0x06000011 RID: 17 RVA: 0x000039E0 File Offset: 0x000029E0
		private long getOffset(long cluster)
		{
			long num = this.pirs_start + cluster * 4096L;
			long num2 = cluster / 170L;
			long num3 = num2 / 170L;
			if (num2 > 0L)
			{
				num += (num2 + 1L) * this.pirs_offset;
			}
			if (num3 > 0L)
			{
				num += (num3 + 1L) * this.pirs_offset;
			}
			return num;
		}

		// Token: 0x06000012 RID: 18 RVA: 0x00003A38 File Offset: 0x00002A38
		private void extractFile(ListViewItem listViewItem, string filename)
		{
			try
			{
				if (!Directory.Exists(this.wr.extractFolderName(filename)))
				{
					Directory.CreateDirectory(this.wr.extractFolderName(filename));
				}
			}
			catch (IOException)
			{
			}
			FileStream fileStream;
			BinaryWriter binaryWriter;
			try
			{
				fileStream = new FileStream(filename, FileMode.Create, FileAccess.Write, FileShare.None);
				binaryWriter = new BinaryWriter(fileStream);
			}
			catch (IOException arg)
			{
				this.log(string.Format("Error : {0}\r\n", arg));
				return;
			}
			long num = Convert.ToInt64(listViewItem.SubItems[this.columnHeaderCluster.Index].Text);
			long offset = this.getOffset(num);
			long num2 = Convert.ToInt64(listViewItem.SubItems[this.columnHeaderSize.Index].Text);
			long num3 = num2 >> 12;
			long num4 = num2 - (num3 << 12);
			for (long num5 = num; num5 < num + num3; num5 += 1L)
			{
				offset = this.getOffset(num5);
				this.br.BaseStream.Seek(offset, SeekOrigin.Begin);
				binaryWriter.Write(this.br.ReadBytes(4096));
				string text = string.Format("{0}%", 100L * (num5 - num) / num3);
				if (text != listViewItem.SubItems[this.columnHeaderStatus.Index].Text)
				{
					listViewItem.SubItems[this.columnHeaderStatus.Index].Text = text;
					Application.DoEvents();
				}
			}
			offset = this.getOffset(num + num3);
			this.br.BaseStream.Seek(offset, SeekOrigin.Begin);
			binaryWriter.Write(this.br.ReadBytes((int)num4));
			listViewItem.SubItems[this.columnHeaderStatus.Index].Text = "Done";
			Application.DoEvents();
			binaryWriter.Close();
			fileStream.Dispose();
		}

		// Token: 0x06000013 RID: 19 RVA: 0x00003C24 File Offset: 0x00002C24
		private void extractFile(long cluster, long size, string filename)
		{
			try
			{
				if (!Directory.Exists(this.wr.extractFolderName(filename)))
				{
					Directory.CreateDirectory(this.wr.extractFolderName(filename));
				}
			}
			catch (IOException)
			{
			}
			FileStream fileStream;
			BinaryWriter binaryWriter;
			try
			{
				fileStream = new FileStream(filename, FileMode.Create, FileAccess.Write, FileShare.None);
				binaryWriter = new BinaryWriter(fileStream);
			}
			catch (IOException arg)
			{
				this.log(string.Format("Error : {0}\r\n", arg));
				return;
			}
			long num = size >> 12;
			long num2 = size - (num << 12);
			long offset;
			for (long num3 = cluster; num3 < cluster + num; num3 += 1L)
			{
				offset = this.getOffset(num3);
				this.br.BaseStream.Seek(offset, SeekOrigin.Begin);
				binaryWriter.Write(this.br.ReadBytes(4096));
				Application.DoEvents();
			}
			offset = this.getOffset(cluster + num);
			this.br.BaseStream.Seek(offset, SeekOrigin.Begin);
			binaryWriter.Write(this.br.ReadBytes((int)num2));
			Application.DoEvents();
			binaryWriter.Close();
			fileStream.Dispose();
		}

		// Token: 0x06000014 RID: 20 RVA: 0x00003D40 File Offset: 0x00002D40
		private void extractFolder(ListViewItem listViewItem, string pathname)
		{
			listViewItem.SubItems[this.columnHeaderStatus.Index].Text = "Working...";
			Application.DoEvents();
			this.extractFolder(Convert.ToUInt16(listViewItem.Tag), listViewItem.Text, pathname);
			listViewItem.SubItems[this.columnHeaderStatus.Index].Text = "Done";
			Application.DoEvents();
		}

		// Token: 0x06000015 RID: 21 RVA: 0x00003DB0 File Offset: 0x00002DB0
		private void extractFolder(ushort tag, string foldername, string pathname)
		{
			ushort num = 0;
			for (;;)
			{
				this.br.BaseStream.Seek(MainForm.PIRS_BASE + this.pirs_offset + (long)(num * 64), SeekOrigin.Begin);
				MainForm.PirsEntry pirsEntry = default(MainForm.PirsEntry);
				pirsEntry = this.getEntry();
				if (pirsEntry.Filename.Trim() == "")
				{
					break;
				}
				if (pirsEntry.Cluster == 0 && pirsEntry.Size == 0 && pirsEntry.Parent == tag)
				{
					this.extractFolder(num, pirsEntry.Filename, pathname + "\\" + foldername);
				}
				else if (pirsEntry.Cluster != 0 && pirsEntry.Parent == tag)
				{
					this.extractFile((long)pirsEntry.Cluster, (long)pirsEntry.Size, string.Concat(new string[]
					{
						pathname,
						"\\",
						foldername,
						"\\",
						pirsEntry.Filename
					}));
				}
				num += 1;
			}
		}

		// Token: 0x06000016 RID: 22 RVA: 0x00003EA4 File Offset: 0x00002EA4
		private void listView_MouseClick(object sender, MouseEventArgs e)
		{
			if (e.Button != MouseButtons.Right || this.listView.SelectedItems.Count != 1)
			{
				if (e.Button == MouseButtons.Right && this.listView.SelectedItems.Count > 1)
				{
					this.contextMenuStripMulti.Show(this.listView, e.X, e.Y);
				}
				return;
			}
			if (this.isFolder(this.listView.SelectedItems[0]))
			{
				this.contextMenuStripMulti.Show(this.listView, e.X, e.Y);
				return;
			}
			this.contextMenuStrip.Show(this.listView, e.X, e.Y);
		}

		// Token: 0x06000017 RID: 23 RVA: 0x00003F63 File Offset: 0x00002F63
		private bool isFolder(ListViewItem listViewItem)
		{
			return listViewItem.ImageIndex == 0;
		}

		// Token: 0x06000018 RID: 24 RVA: 0x00003F70 File Offset: 0x00002F70
		private void extractFilesToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (this.folderBrowserDialog.ShowDialog() == DialogResult.OK)
			{
				for (int i = 0; i < this.listView.SelectedItems.Count; i++)
				{
					if (this.isFolder(this.listView.SelectedItems[i]))
					{
						this.log(string.Format("Extract folder {0}\r\n", this.listView.SelectedItems[i].Text));
						this.extractFolder(this.listView.SelectedItems[i], this.folderBrowserDialog.SelectedPath);
					}
					else
					{
						this.extractFile(this.listView.SelectedItems[i], this.folderBrowserDialog.SelectedPath + "\\" + this.listView.SelectedItems[i].Text);
					}
				}
			}
		}

		// Token: 0x06000019 RID: 25 RVA: 0x00004054 File Offset: 0x00003054
		private void extractFileToolStripMenuItem_Click(object sender, EventArgs e)
		{
			this.saveFileDialog.FileName = this.listView.SelectedItems[0].Text;
			if (this.saveFileDialog.ShowDialog() == DialogResult.OK)
			{
				this.extractFile(this.listView.SelectedItems[0], this.saveFileDialog.FileName);
			}
		}

		// Token: 0x0600001A RID: 26 RVA: 0x000040B4 File Offset: 0x000030B4
		private void aboutPirsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			AboutBox aboutBox = new AboutBox();
			aboutBox.ShowDialog();
			aboutBox.Dispose();
		}

		// Token: 0x0600001B RID: 27 RVA: 0x000040D4 File Offset: 0x000030D4
		private void MainForm_Load(object sender, EventArgs e)
		{
			this.toolStripStatusLabelVersion.Text = Application.ProductName + " - " + Assembly.GetExecutingAssembly().GetName().Version.ToString();
			if (this.args.Length > 0)
			{
				this.openFileDialog.FileName = this.args[0];
				this.openFile(this.args[0]);
			}
		}

		// Token: 0x0600001C RID: 28 RVA: 0x0000413B File Offset: 0x0000313B
		private void treeView_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
		{
			if (e.Button == MouseButtons.Right)
			{
				this.treeView.SelectedNode = e.Node;
				this.contextMenuStripFolder.Show(this.treeView, e.X, e.Y);
			}
		}

		// Token: 0x0600001D RID: 29 RVA: 0x00004178 File Offset: 0x00003178
		private void extractFolder(object sender, EventArgs e)
		{
			if (this.treeView.SelectedNode != null && this.folderBrowserDialog.ShowDialog() == DialogResult.OK)
			{
				for (int i = 0; i < this.listView.Items.Count; i++)
				{
					if (this.isFolder(this.listView.Items[i]))
					{
						this.extractFolder(this.listView.Items[i], this.folderBrowserDialog.SelectedPath);
					}
					else
					{
						this.extractFile(this.listView.Items[i], this.folderBrowserDialog.SelectedPath + "\\" + this.listView.Items[i].Text);
					}
					Application.DoEvents();
				}
			}
		}

		// Token: 0x0600001E RID: 30 RVA: 0x0000424B File Offset: 0x0000324B
		private void extractAll(object sender, EventArgs e)
		{
			if (this.treeView.Nodes.Count <= 0) return;
			this.treeView.SelectedNode = this.treeView.Nodes[0];
			this.extractFolder(sender, e);
		}

		// Token: 0x0600001F RID: 31 RVA: 0x00004274 File Offset: 0x00003274
		private long getCultureOffset()
		{
			long result;
			if (Application.CurrentCulture.TwoLetterISOLanguageName.ToLower() == "de")
			{
				result = 512L;
			}
			else if (Application.CurrentCulture.TwoLetterISOLanguageName.ToLower() == "fr")
			{
				result = 768L;
			}
			else if (Application.CurrentCulture.TwoLetterISOLanguageName.ToLower() == "es")
			{
				result = 1024L;
			}
			else if (Application.CurrentCulture.TwoLetterISOLanguageName.ToLower() == "it")
			{
				result = 1280L;
			}
			else
			{
				result = 0L;
			}
			return result;
		}

		// Token: 0x06000020 RID: 32 RVA: 0x00004318 File Offset: 0x00003318
		private void getDescription()
		{
			long cultureOffset = this.getCultureOffset();
			this.br.BaseStream.Seek(1040L + cultureOffset, SeekOrigin.Begin);
			byte[] data = new byte[256];
			data = this.br.ReadBytes(256);
			this.log("Title : " + this.wr.unicodeToStr(data, 2) + "\r\n");
			this.br.BaseStream.Seek(3344L + cultureOffset, SeekOrigin.Begin);
			byte[] data2 = new byte[256];
			data2 = this.br.ReadBytes(256);
			this.log("Description : " + this.wr.unicodeToStr(data2, 2) + "\r\n");
			this.br.BaseStream.Seek(5648L, SeekOrigin.Begin);
			byte[] data3 = new byte[256];
			data3 = this.br.ReadBytes(256);
			this.log("Publisher : " + this.wr.unicodeToStr(data3, 2) + "\r\n");
		}

		// Token: 0x04000027 RID: 39
		public static int MAGIC_PIRS = 1346982483;

		// Token: 0x04000028 RID: 40
		public static int MAGIC_LIVE = 1279874629;

		// Token: 0x04000029 RID: 41
		public static int MAGIC_CON_ = 1129270816;

		// Token: 0x0400002A RID: 42
		public static long PIRS_TYPE1 = 4096L;

		// Token: 0x0400002B RID: 43
		public static long PIRS_TYPE2 = 8192L;

		// Token: 0x0400002C RID: 44
		public static long PIRS_BASE = 45056L;

		// Token: 0x0400002D RID: 45
		private string[] args;

		// Token: 0x0400002E RID: 46
		private WxReader wr = new WxReader();

		// Token: 0x0400002F RID: 47
		private FileStream fs;

		// Token: 0x04000030 RID: 48
		private BinaryReader br;

		// Token: 0x04000031 RID: 49
		private long pirs_offset;

		// Token: 0x04000032 RID: 50
		private long pirs_start;

		// Token: 0x02000003 RID: 3
		public struct PirsEntry
		{
			// Token: 0x04000033 RID: 51
			public string Filename;

			// Token: 0x04000034 RID: 52
			public int Unknow;

			// Token: 0x04000035 RID: 53
			public int BlockLen;

			// Token: 0x04000036 RID: 54
			public int Cluster;

			// Token: 0x04000037 RID: 55
			public ushort Parent;

			// Token: 0x04000038 RID: 56
			public int Size;

			// Token: 0x04000039 RID: 57
			public DateTime DateTime1;

			// Token: 0x0400003A RID: 58
			public DateTime DateTime2;
		}
	}
}
