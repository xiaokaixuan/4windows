using System;
using System.CodeDom.Compiler;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Globalization;
using System.Resources;
using System.Runtime.CompilerServices;

namespace wxPirs.Properties
{
	// Token: 0x02000006 RID: 6
	[GeneratedCode("System.Resources.Tools.StronglyTypedResourceBuilder", "2.0.0.0")]
	[CompilerGenerated]
	[DebuggerNonUserCode]
	internal class Resources
	{
		// Token: 0x0600002C RID: 44 RVA: 0x00004E2C File Offset: 0x00003E2C
		internal Resources()
		{
		}

		// Token: 0x17000007 RID: 7
		// (get) Token: 0x0600002D RID: 45 RVA: 0x00004E34 File Offset: 0x00003E34
		[EditorBrowsable(EditorBrowsableState.Advanced)]
		internal static ResourceManager ResourceManager
		{
			get
			{
				if (object.ReferenceEquals(Resources.resourceMan, null))
				{
					ResourceManager resourceManager = new ResourceManager("wxPirs.Properties.Resources", typeof(Resources).Assembly);
					Resources.resourceMan = resourceManager;
				}
				return Resources.resourceMan;
			}
		}

		// Token: 0x17000008 RID: 8
		// (get) Token: 0x0600002E RID: 46 RVA: 0x00004E73 File Offset: 0x00003E73
		// (set) Token: 0x0600002F RID: 47 RVA: 0x00004E7A File Offset: 0x00003E7A
		[EditorBrowsable(EditorBrowsableState.Advanced)]
		internal static CultureInfo Culture
		{
			get
			{
				return Resources.resourceCulture;
			}
			set
			{
				Resources.resourceCulture = value;
			}
		}

		// Token: 0x17000009 RID: 9
		// (get) Token: 0x06000030 RID: 48 RVA: 0x00004E84 File Offset: 0x00003E84
		internal static Bitmap openHS
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("openHS", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		// Token: 0x1700000A RID: 10
		// (get) Token: 0x06000031 RID: 49 RVA: 0x00004EAC File Offset: 0x00003EAC
		internal static Bitmap SaveAllHS
		{
			get
			{
				object @object = Resources.ResourceManager.GetObject("SaveAllHS", Resources.resourceCulture);
				return (Bitmap)@object;
			}
		}

		// Token: 0x04000044 RID: 68
		private static ResourceManager resourceMan;

		// Token: 0x04000045 RID: 69
		private static CultureInfo resourceCulture;
	}
}
