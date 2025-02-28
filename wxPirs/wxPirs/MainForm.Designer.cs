namespace wxPirs
{
	// Token: 0x02000002 RID: 2
	public partial class MainForm : global::System.Windows.Forms.Form
	{
		// Token: 0x06000001 RID: 1 RVA: 0x00002050 File Offset: 0x00001050
		protected override void Dispose(bool disposing)
		{
			if (disposing && this.components != null)
			{
				this.components.Dispose();
			}
			base.Dispose(disposing);
		}

		// Token: 0x06000002 RID: 2 RVA: 0x00002070 File Offset: 0x00001070
		private void InitializeComponent()
		{
			this.components = new global::System.ComponentModel.Container();
			global::System.ComponentModel.ComponentResourceManager componentResourceManager = new global::System.ComponentModel.ComponentResourceManager(typeof(global::wxPirs.MainForm));
			this.menuStrip = new global::System.Windows.Forms.MenuStrip();
			this.fileToolStripMenuItem = new global::System.Windows.Forms.ToolStripMenuItem();
			this.openFileToolStripMenuItem = new global::System.Windows.Forms.ToolStripMenuItem();
			this.extractAllToolStripMenuItem = new global::System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem2 = new global::System.Windows.Forms.ToolStripSeparator();
			this.exitToolStripMenuItem = new global::System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem1 = new global::System.Windows.Forms.ToolStripMenuItem();
			this.aboutPirsToolStripMenuItem = new global::System.Windows.Forms.ToolStripMenuItem();
			this.statusStrip = new global::System.Windows.Forms.StatusStrip();
			this.toolStripStatusLabelVersion = new global::System.Windows.Forms.ToolStripStatusLabel();
			this.toolStripStatusLabelSeparator = new global::System.Windows.Forms.ToolStripStatusLabel();
			this.toolStripStatusLabelGael360 = new global::System.Windows.Forms.ToolStripStatusLabel();
			this.toolStripContainer1 = new global::System.Windows.Forms.ToolStripContainer();
			this.splitContainerH = new global::System.Windows.Forms.SplitContainer();
			this.splitContainerV = new global::System.Windows.Forms.SplitContainer();
			this.treeView = new global::System.Windows.Forms.TreeView();
			this.imageListTreeView = new global::System.Windows.Forms.ImageList(this.components);
			this.listView = new global::System.Windows.Forms.ListView();
			this.columnHeaderName = new global::System.Windows.Forms.ColumnHeader();
			this.columnHeaderSize = new global::System.Windows.Forms.ColumnHeader();
			this.columnHeaderCluster = new global::System.Windows.Forms.ColumnHeader();
			this.columnHeaderDateModified = new global::System.Windows.Forms.ColumnHeader();
			this.columnHeaderStatus = new global::System.Windows.Forms.ColumnHeader();
			this.imageList = new global::System.Windows.Forms.ImageList(this.components);
			this.textBoxLog = new global::System.Windows.Forms.TextBox();
			this.fileToolStrip = new global::System.Windows.Forms.ToolStrip();
			this.openFileToolStripButton = new global::System.Windows.Forms.ToolStripButton();
			this.extractAllToolStripButton = new global::System.Windows.Forms.ToolStripButton();
			this.contextMenuStripFolder = new global::System.Windows.Forms.ContextMenuStrip(this.components);
			this.extractFolderToolStripMenuItem = new global::System.Windows.Forms.ToolStripMenuItem();
			this.openFileDialog = new global::System.Windows.Forms.OpenFileDialog();
			this.contextMenuStrip = new global::System.Windows.Forms.ContextMenuStrip(this.components);
			this.extractFileToolStripMenuItem = new global::System.Windows.Forms.ToolStripMenuItem();
			this.contextMenuStripMulti = new global::System.Windows.Forms.ContextMenuStrip(this.components);
			this.extractFilesToolStripMenuItem = new global::System.Windows.Forms.ToolStripMenuItem();
			this.saveFileDialog = new global::System.Windows.Forms.SaveFileDialog();
			this.folderBrowserDialog = new global::System.Windows.Forms.FolderBrowserDialog();
			this.menuStrip.SuspendLayout();
			this.statusStrip.SuspendLayout();
			this.toolStripContainer1.ContentPanel.SuspendLayout();
			this.toolStripContainer1.TopToolStripPanel.SuspendLayout();
			this.toolStripContainer1.SuspendLayout();
			this.splitContainerH.Panel1.SuspendLayout();
			this.splitContainerH.Panel2.SuspendLayout();
			this.splitContainerH.SuspendLayout();
			this.splitContainerV.Panel1.SuspendLayout();
			this.splitContainerV.Panel2.SuspendLayout();
			this.splitContainerV.SuspendLayout();
			this.fileToolStrip.SuspendLayout();
			this.contextMenuStripFolder.SuspendLayout();
			this.contextMenuStrip.SuspendLayout();
			this.contextMenuStripMulti.SuspendLayout();
			base.SuspendLayout();
			this.menuStrip.Items.AddRange(new global::System.Windows.Forms.ToolStripItem[]
			{
				this.fileToolStripMenuItem,
				this.toolStripMenuItem1
			});
			this.menuStrip.Location = new global::System.Drawing.Point(0, 0);
			this.menuStrip.Name = "menuStrip";
			this.menuStrip.Size = new global::System.Drawing.Size(729, 24);
			this.menuStrip.TabIndex = 0;
			this.menuStrip.Text = "menuStrip1";
			this.fileToolStripMenuItem.DropDownItems.AddRange(new global::System.Windows.Forms.ToolStripItem[]
			{
				this.openFileToolStripMenuItem,
				this.extractAllToolStripMenuItem,
				this.toolStripMenuItem2,
				this.exitToolStripMenuItem
			});
			this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
			this.fileToolStripMenuItem.Size = new global::System.Drawing.Size(35, 20);
			this.fileToolStripMenuItem.Text = "File";
			this.openFileToolStripMenuItem.Image = global::wxPirs.Properties.Resources.openHS;
			this.openFileToolStripMenuItem.Name = "openFileToolStripMenuItem";
			this.openFileToolStripMenuItem.Size = new global::System.Drawing.Size(134, 22);
			this.openFileToolStripMenuItem.Text = "Open...";
			this.openFileToolStripMenuItem.Click += new global::System.EventHandler(this.openFile);
			this.extractAllToolStripMenuItem.Image = global::wxPirs.Properties.Resources.SaveAllHS;
			this.extractAllToolStripMenuItem.Name = "extractAllToolStripMenuItem";
			this.extractAllToolStripMenuItem.Size = new global::System.Drawing.Size(134, 22);
			this.extractAllToolStripMenuItem.Text = "Extract all...";
			this.extractAllToolStripMenuItem.Click += new global::System.EventHandler(this.extractAll);
			this.toolStripMenuItem2.Name = "toolStripMenuItem2";
			this.toolStripMenuItem2.Size = new global::System.Drawing.Size(131, 6);
			this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
			this.exitToolStripMenuItem.Size = new global::System.Drawing.Size(134, 22);
			this.exitToolStripMenuItem.Text = "Exit";
			this.exitToolStripMenuItem.Click += new global::System.EventHandler(this.exitToolStripMenuItem_Click);
			this.toolStripMenuItem1.DropDownItems.AddRange(new global::System.Windows.Forms.ToolStripItem[]
			{
				this.aboutPirsToolStripMenuItem
			});
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new global::System.Drawing.Size(24, 20);
			this.toolStripMenuItem1.Text = "?";
			this.aboutPirsToolStripMenuItem.Name = "aboutPirsToolStripMenuItem";
			this.aboutPirsToolStripMenuItem.Size = new global::System.Drawing.Size(123, 22);
			this.aboutPirsToolStripMenuItem.Text = "About Pirs";
			this.aboutPirsToolStripMenuItem.Click += new global::System.EventHandler(this.aboutPirsToolStripMenuItem_Click);
			this.statusStrip.Items.AddRange(new global::System.Windows.Forms.ToolStripItem[]
			{
				this.toolStripStatusLabelVersion,
				this.toolStripStatusLabelSeparator,
				this.toolStripStatusLabelGael360
			});
			this.statusStrip.Location = new global::System.Drawing.Point(0, 373);
			this.statusStrip.Name = "statusStrip";
			this.statusStrip.Size = new global::System.Drawing.Size(729, 22);
			this.statusStrip.TabIndex = 1;
			this.statusStrip.Text = "statusStrip1";
			this.toolStripStatusLabelVersion.Name = "toolStripStatusLabelVersion";
			this.toolStripStatusLabelVersion.Size = new global::System.Drawing.Size(664, 17);
			this.toolStripStatusLabelVersion.Spring = true;
			this.toolStripStatusLabelVersion.Text = "wxPirs";
			this.toolStripStatusLabelVersion.TextAlign = global::System.Drawing.ContentAlignment.MiddleLeft;
			this.toolStripStatusLabelSeparator.BorderSides = global::System.Windows.Forms.ToolStripStatusLabelBorderSides.All;
			this.toolStripStatusLabelSeparator.BorderStyle = global::System.Windows.Forms.Border3DStyle.Sunken;
			this.toolStripStatusLabelSeparator.Name = "toolStripStatusLabelSeparator";
			this.toolStripStatusLabelSeparator.Size = new global::System.Drawing.Size(4, 17);
			this.toolStripStatusLabelGael360.Name = "toolStripStatusLabelGael360";
			this.toolStripStatusLabelGael360.Size = new global::System.Drawing.Size(46, 17);
			this.toolStripStatusLabelGael360.Text = "Gael360";
			this.toolStripContainer1.ContentPanel.Controls.Add(this.splitContainerH);
			this.toolStripContainer1.ContentPanel.Size = new global::System.Drawing.Size(729, 324);
			this.toolStripContainer1.Dock = global::System.Windows.Forms.DockStyle.Fill;
			this.toolStripContainer1.Location = new global::System.Drawing.Point(0, 24);
			this.toolStripContainer1.Name = "toolStripContainer1";
			this.toolStripContainer1.Size = new global::System.Drawing.Size(729, 349);
			this.toolStripContainer1.TabIndex = 2;
			this.toolStripContainer1.Text = "toolStripContainer1";
			this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.fileToolStrip);
			this.splitContainerH.Dock = global::System.Windows.Forms.DockStyle.Fill;
			this.splitContainerH.FixedPanel = global::System.Windows.Forms.FixedPanel.Panel2;
			this.splitContainerH.Location = new global::System.Drawing.Point(0, 0);
			this.splitContainerH.Name = "splitContainerH";
			this.splitContainerH.Orientation = global::System.Windows.Forms.Orientation.Horizontal;
			this.splitContainerH.Panel1.Controls.Add(this.splitContainerV);
			this.splitContainerH.Panel2.Controls.Add(this.textBoxLog);
			this.splitContainerH.Size = new global::System.Drawing.Size(729, 324);
			this.splitContainerH.SplitterDistance = 243;
			this.splitContainerH.TabIndex = 0;
			this.splitContainerV.Dock = global::System.Windows.Forms.DockStyle.Fill;
			this.splitContainerV.FixedPanel = global::System.Windows.Forms.FixedPanel.Panel1;
			this.splitContainerV.Location = new global::System.Drawing.Point(0, 0);
			this.splitContainerV.Name = "splitContainerV";
			this.splitContainerV.Panel1.Controls.Add(this.treeView);
			this.splitContainerV.Panel2.Controls.Add(this.listView);
			this.splitContainerV.Size = new global::System.Drawing.Size(729, 243);
			this.splitContainerV.SplitterDistance = 180;
			this.splitContainerV.TabIndex = 0;
			this.treeView.Dock = global::System.Windows.Forms.DockStyle.Fill;
			this.treeView.ImageIndex = 1;
			this.treeView.ImageList = this.imageListTreeView;
			this.treeView.Location = new global::System.Drawing.Point(0, 0);
			this.treeView.Name = "treeView";
			this.treeView.SelectedImageIndex = 2;
			this.treeView.ShowRootLines = false;
			this.treeView.Size = new global::System.Drawing.Size(180, 243);
			this.treeView.TabIndex = 0;
			this.treeView.AfterSelect += new global::System.Windows.Forms.TreeViewEventHandler(this.treeView_AfterSelect);
			this.treeView.NodeMouseClick += new global::System.Windows.Forms.TreeNodeMouseClickEventHandler(this.treeView_NodeMouseClick);
			this.imageListTreeView.ImageStream = (global::System.Windows.Forms.ImageListStreamer)componentResourceManager.GetObject("imageListTreeView.ImageStream");
			this.imageListTreeView.TransparentColor = global::System.Drawing.Color.Fuchsia;
			this.imageListTreeView.Images.SetKeyName(0, "Control_FolderBrowserDialog.bmp");
			this.imageListTreeView.Images.SetKeyName(1, "VSFolder_closed.bmp");
			this.imageListTreeView.Images.SetKeyName(2, "VSFolder_open.bmp");
			this.listView.Columns.AddRange(new global::System.Windows.Forms.ColumnHeader[]
			{
				this.columnHeaderName,
				this.columnHeaderSize,
				this.columnHeaderCluster,
				this.columnHeaderDateModified,
				this.columnHeaderStatus
			});
			this.listView.Dock = global::System.Windows.Forms.DockStyle.Fill;
			this.listView.FullRowSelect = true;
			this.listView.Location = new global::System.Drawing.Point(0, 0);
			this.listView.Name = "listView";
			this.listView.ShowItemToolTips = true;
			this.listView.Size = new global::System.Drawing.Size(545, 243);
			this.listView.SmallImageList = this.imageList;
			this.listView.TabIndex = 0;
			this.listView.UseCompatibleStateImageBehavior = false;
			this.listView.View = global::System.Windows.Forms.View.Details;
			this.listView.MouseClick += new global::System.Windows.Forms.MouseEventHandler(this.listView_MouseClick);
			this.columnHeaderName.Text = "Name";
			this.columnHeaderName.Width = 200;
			this.columnHeaderSize.Text = "Size";
			this.columnHeaderSize.TextAlign = global::System.Windows.Forms.HorizontalAlignment.Right;
			this.columnHeaderCluster.Text = "Cluster";
			this.columnHeaderCluster.TextAlign = global::System.Windows.Forms.HorizontalAlignment.Right;
			this.columnHeaderDateModified.Text = "Date modified";
			this.columnHeaderDateModified.Width = 120;
			this.columnHeaderStatus.Text = "Status";
			this.columnHeaderStatus.Width = 80;
			this.imageList.ImageStream = (global::System.Windows.Forms.ImageListStreamer)componentResourceManager.GetObject("imageList.ImageStream");
			this.imageList.TransparentColor = global::System.Drawing.Color.Fuchsia;
			this.imageList.Images.SetKeyName(0, "VSFolder_closed.bmp");
			this.imageList.Images.SetKeyName(1, "DocumentHS.png");
			this.textBoxLog.Dock = global::System.Windows.Forms.DockStyle.Fill;
			this.textBoxLog.Location = new global::System.Drawing.Point(0, 0);
			this.textBoxLog.Multiline = true;
			this.textBoxLog.Name = "textBoxLog";
			this.textBoxLog.ScrollBars = global::System.Windows.Forms.ScrollBars.Both;
			this.textBoxLog.Size = new global::System.Drawing.Size(729, 77);
			this.textBoxLog.TabIndex = 0;
			this.fileToolStrip.Dock = global::System.Windows.Forms.DockStyle.None;
			this.fileToolStrip.Items.AddRange(new global::System.Windows.Forms.ToolStripItem[]
			{
				this.openFileToolStripButton,
				this.extractAllToolStripButton
			});
			this.fileToolStrip.Location = new global::System.Drawing.Point(3, 0);
			this.fileToolStrip.Name = "fileToolStrip";
			this.fileToolStrip.Size = new global::System.Drawing.Size(58, 25);
			this.fileToolStrip.TabIndex = 0;
			this.fileToolStrip.Text = "File";
			this.openFileToolStripButton.DisplayStyle = global::System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.openFileToolStripButton.Image = global::wxPirs.Properties.Resources.openHS;
			this.openFileToolStripButton.ImageTransparentColor = global::System.Drawing.Color.Magenta;
			this.openFileToolStripButton.Name = "openFileToolStripButton";
			this.openFileToolStripButton.Size = new global::System.Drawing.Size(23, 22);
			this.openFileToolStripButton.Text = "Open file";
			this.openFileToolStripButton.Click += new global::System.EventHandler(this.openFile);
			this.extractAllToolStripButton.DisplayStyle = global::System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.extractAllToolStripButton.Image = global::wxPirs.Properties.Resources.SaveAllHS;
			this.extractAllToolStripButton.ImageTransparentColor = global::System.Drawing.Color.Magenta;
			this.extractAllToolStripButton.Name = "extractAllToolStripButton";
			this.extractAllToolStripButton.Size = new global::System.Drawing.Size(23, 22);
			this.extractAllToolStripButton.Text = "Extract all";
			this.extractAllToolStripButton.Click += new global::System.EventHandler(this.extractAll);
			this.contextMenuStripFolder.Items.AddRange(new global::System.Windows.Forms.ToolStripItem[]
			{
				this.extractFolderToolStripMenuItem
			});
			this.contextMenuStripFolder.Name = "contextMenuStripFolder";
			this.contextMenuStripFolder.Size = new global::System.Drawing.Size(141, 26);
			this.extractFolderToolStripMenuItem.Name = "extractFolderToolStripMenuItem";
			this.extractFolderToolStripMenuItem.Size = new global::System.Drawing.Size(140, 22);
			this.extractFolderToolStripMenuItem.Text = "Extract folder";
			this.extractFolderToolStripMenuItem.Click += new global::System.EventHandler(this.extractFolder);
			this.openFileDialog.Filter = "All files (*.*)|*.*";
			this.contextMenuStrip.Items.AddRange(new global::System.Windows.Forms.ToolStripItem[]
			{
				this.extractFileToolStripMenuItem
			});
			this.contextMenuStrip.Name = "contextMenuStrip";
			this.contextMenuStrip.Size = new global::System.Drawing.Size(127, 26);
			this.extractFileToolStripMenuItem.Name = "extractFileToolStripMenuItem";
			this.extractFileToolStripMenuItem.Size = new global::System.Drawing.Size(126, 22);
			this.extractFileToolStripMenuItem.Text = "Extract file";
			this.extractFileToolStripMenuItem.Click += new global::System.EventHandler(this.extractFileToolStripMenuItem_Click);
			this.contextMenuStripMulti.Items.AddRange(new global::System.Windows.Forms.ToolStripItem[]
			{
				this.extractFilesToolStripMenuItem
			});
			this.contextMenuStripMulti.Name = "contextMenuStripMulti";
			this.contextMenuStripMulti.Size = new global::System.Drawing.Size(132, 26);
			this.extractFilesToolStripMenuItem.Name = "extractFilesToolStripMenuItem";
			this.extractFilesToolStripMenuItem.Size = new global::System.Drawing.Size(131, 22);
			this.extractFilesToolStripMenuItem.Text = "Extract files";
			this.extractFilesToolStripMenuItem.Click += new global::System.EventHandler(this.extractFilesToolStripMenuItem_Click);
			this.saveFileDialog.Filter = "All files (*.*)|*.*";
			base.AutoScaleDimensions = new global::System.Drawing.SizeF(6f, 13f);
			base.AutoScaleMode = global::System.Windows.Forms.AutoScaleMode.Font;
			base.ClientSize = new global::System.Drawing.Size(729, 395);
			base.Controls.Add(this.toolStripContainer1);
			base.Controls.Add(this.statusStrip);
			base.Controls.Add(this.menuStrip);
			base.Icon = (global::System.Drawing.Icon)componentResourceManager.GetObject("$this.Icon");
			base.MainMenuStrip = this.menuStrip;
			base.Name = "MainForm";
			this.Text = "wxPirs";
			this.Size = new global::System.Drawing.Size(1024, 720);
			base.FormClosing += new global::System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
			base.Load += new global::System.EventHandler(this.MainForm_Load);
			this.menuStrip.ResumeLayout(false);
			this.menuStrip.PerformLayout();
			this.statusStrip.ResumeLayout(false);
			this.statusStrip.PerformLayout();
			this.toolStripContainer1.ContentPanel.ResumeLayout(false);
			this.toolStripContainer1.TopToolStripPanel.ResumeLayout(false);
			this.toolStripContainer1.TopToolStripPanel.PerformLayout();
			this.toolStripContainer1.ResumeLayout(false);
			this.toolStripContainer1.PerformLayout();
			this.splitContainerH.Panel1.ResumeLayout(false);
			this.splitContainerH.Panel2.ResumeLayout(false);
			this.splitContainerH.Panel2.PerformLayout();
			this.splitContainerH.ResumeLayout(false);
			this.splitContainerV.Panel1.ResumeLayout(false);
			this.splitContainerV.Panel2.ResumeLayout(false);
			this.splitContainerV.ResumeLayout(false);
			this.fileToolStrip.ResumeLayout(false);
			this.fileToolStrip.PerformLayout();
			this.contextMenuStripFolder.ResumeLayout(false);
			this.contextMenuStrip.ResumeLayout(false);
			this.contextMenuStripMulti.ResumeLayout(false);
			base.ResumeLayout(false);
			base.PerformLayout();
		}

		// Token: 0x04000001 RID: 1
		private global::System.ComponentModel.IContainer components;

		// Token: 0x04000002 RID: 2
		private global::System.Windows.Forms.MenuStrip menuStrip;

		// Token: 0x04000003 RID: 3
		private global::System.Windows.Forms.StatusStrip statusStrip;

		// Token: 0x04000004 RID: 4
		private global::System.Windows.Forms.ToolStripContainer toolStripContainer1;

		// Token: 0x04000005 RID: 5
		private global::System.Windows.Forms.SplitContainer splitContainerH;

		// Token: 0x04000006 RID: 6
		private global::System.Windows.Forms.SplitContainer splitContainerV;

		// Token: 0x04000007 RID: 7
		private global::System.Windows.Forms.TreeView treeView;

		// Token: 0x04000008 RID: 8
		private global::System.Windows.Forms.ListView listView;

		// Token: 0x04000009 RID: 9
		private global::System.Windows.Forms.ColumnHeader columnHeaderName;

		// Token: 0x0400000A RID: 10
		private global::System.Windows.Forms.TextBox textBoxLog;

		// Token: 0x0400000B RID: 11
		private global::System.Windows.Forms.ImageList imageListTreeView;

		// Token: 0x0400000C RID: 12
		private global::System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;

		// Token: 0x0400000D RID: 13
		private global::System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;

		// Token: 0x0400000E RID: 14
		private global::System.Windows.Forms.ToolStrip fileToolStrip;

		// Token: 0x0400000F RID: 15
		private global::System.Windows.Forms.ToolStripButton openFileToolStripButton;

		// Token: 0x04000010 RID: 16
		private global::System.Windows.Forms.OpenFileDialog openFileDialog;

		// Token: 0x04000011 RID: 17
		private global::System.Windows.Forms.ColumnHeader columnHeaderSize;

		// Token: 0x04000012 RID: 18
		private global::System.Windows.Forms.ColumnHeader columnHeaderDateModified;

		// Token: 0x04000013 RID: 19
		private global::System.Windows.Forms.ImageList imageList;

		// Token: 0x04000014 RID: 20
		private global::System.Windows.Forms.ColumnHeader columnHeaderCluster;

		// Token: 0x04000015 RID: 21
		private global::System.Windows.Forms.ContextMenuStrip contextMenuStrip;

		// Token: 0x04000016 RID: 22
		private global::System.Windows.Forms.ToolStripMenuItem extractFileToolStripMenuItem;

		// Token: 0x04000017 RID: 23
		private global::System.Windows.Forms.ContextMenuStrip contextMenuStripMulti;

		// Token: 0x04000018 RID: 24
		private global::System.Windows.Forms.ToolStripMenuItem extractFilesToolStripMenuItem;

		// Token: 0x04000019 RID: 25
		private global::System.Windows.Forms.SaveFileDialog saveFileDialog;

		// Token: 0x0400001A RID: 26
		private global::System.Windows.Forms.ColumnHeader columnHeaderStatus;

		// Token: 0x0400001B RID: 27
		private global::System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;

		// Token: 0x0400001C RID: 28
		private global::System.Windows.Forms.ToolStripMenuItem aboutPirsToolStripMenuItem;

		// Token: 0x0400001D RID: 29
		private global::System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelVersion;

		// Token: 0x0400001E RID: 30
		private global::System.Windows.Forms.ToolStripMenuItem openFileToolStripMenuItem;

		// Token: 0x0400001F RID: 31
		private global::System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;

		// Token: 0x04000020 RID: 32
		private global::System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelSeparator;

		// Token: 0x04000021 RID: 33
		private global::System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelGael360;

		// Token: 0x04000022 RID: 34
		private global::System.Windows.Forms.FolderBrowserDialog folderBrowserDialog;

		// Token: 0x04000023 RID: 35
		private global::System.Windows.Forms.ContextMenuStrip contextMenuStripFolder;

		// Token: 0x04000024 RID: 36
		private global::System.Windows.Forms.ToolStripMenuItem extractFolderToolStripMenuItem;

		// Token: 0x04000025 RID: 37
		private global::System.Windows.Forms.ToolStripButton extractAllToolStripButton;

		// Token: 0x04000026 RID: 38
		private global::System.Windows.Forms.ToolStripMenuItem extractAllToolStripMenuItem;
	}
}
