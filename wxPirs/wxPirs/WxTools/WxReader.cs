using System;
using System.Globalization;
using System.IO;

namespace WxTools
{
	// Token: 0x02000008 RID: 8
	internal class WxReader
	{
		// Token: 0x06000035 RID: 53 RVA: 0x00004EFC File Offset: 0x00003EFC
		public string readString(BinaryReader br, uint nbchar)
		{
			string text = "";
			for (uint num = 0U; num < nbchar; num += 1U)
			{
				char c = br.ReadChar();
				if (c != '\0')
				{
					text += Convert.ToString(c);
				}
			}
			return text;
		}

		// Token: 0x06000036 RID: 54 RVA: 0x00004F34 File Offset: 0x00003F34
		public string readWideString(BinaryReader br, uint nbchar)
		{
			string text = "";
			for (uint num = 0U; num < nbchar; num += 1U)
			{
				br.ReadChar();
				text += Convert.ToString(br.ReadChar());
			}
			return text;
		}

		// Token: 0x06000037 RID: 55 RVA: 0x00004F70 File Offset: 0x00003F70
		public short readInt16(BinaryReader br)
		{
			short num = 0;
			for (int i = 0; i < 2; i++)
			{
				num = (short)(num << 8);
				num += Convert.ToInt16(br.ReadByte());
			}
			return num;
		}

		// Token: 0x06000038 RID: 56 RVA: 0x00004FA0 File Offset: 0x00003FA0
		public ushort readUInt16(BinaryReader br)
		{
			ushort num = 0;
			for (int i = 0; i < 2; i++)
			{
				num = Convert.ToUInt16((int)(num * 256));
				num += Convert.ToUInt16(br.ReadByte());
			}
			return num;
		}

		// Token: 0x06000039 RID: 57 RVA: 0x00004FD8 File Offset: 0x00003FD8
		public int readInt32(BinaryReader br)
		{
			int num = 0;
			for (int i = 0; i < 4; i++)
			{
				num *= 256;
				num += (int)br.ReadByte();
			}
			return num;
		}

		// Token: 0x0600003A RID: 58 RVA: 0x00005008 File Offset: 0x00004008
		public uint readUInt32(BinaryReader br)
		{
			uint num = 0U;
			for (int i = 0; i < 4; i++)
			{
				num *= 256U;
				num += (uint)br.ReadByte();
			}
			return num;
		}

		// Token: 0x0600003B RID: 59 RVA: 0x00005038 File Offset: 0x00004038
		public int HexToInt(string hex)
		{
			if (hex.Substring(0, 2).ToLower() == "0x")
			{
				hex = hex.Substring(2);
			}
			int result;
			try
			{
				result = int.Parse(hex, NumberStyles.HexNumber);
			}
			catch (FormatException)
			{
				result = 0;
			}
			return result;
		}

		// Token: 0x0600003C RID: 60 RVA: 0x0000508C File Offset: 0x0000408C
		public uint HexToUInt(string hex)
		{
			if (hex.Substring(0, 2).ToLower() == "0x")
			{
				hex = hex.Substring(2);
			}
			uint result;
			try
			{
				result = uint.Parse(hex, NumberStyles.HexNumber);
			}
			catch (FormatException)
			{
				result = 0U;
			}
			return result;
		}

		// Token: 0x0600003D RID: 61 RVA: 0x000050E0 File Offset: 0x000040E0
		public string extractFileName(string fullName)
		{
			string result;
			try
			{
				result = fullName.Substring(fullName.LastIndexOf("\\") + 1);
			}
			catch (ArgumentOutOfRangeException)
			{
				result = "";
			}
			return result;
		}

		// Token: 0x0600003E RID: 62 RVA: 0x00005120 File Offset: 0x00004120
		public string extractFolderName(string fullName)
		{
			string result;
			try
			{
				result = fullName.Substring(0, fullName.LastIndexOf("\\"));
			}
			catch (ArgumentOutOfRangeException)
			{
				result = "";
			}
			return result;
		}

		// Token: 0x0600003F RID: 63 RVA: 0x0000515C File Offset: 0x0000415C
		public string unicodeToStr(byte[] data)
		{
			return this.unicodeToStr(data, 0, data.Length);
		}

		// Token: 0x06000040 RID: 64 RVA: 0x00005169 File Offset: 0x00004169
		public string unicodeToStr(byte[] data, int start)
		{
			return this.unicodeToStr(data, start, data.Length);
		}

		// Token: 0x06000041 RID: 65 RVA: 0x00005178 File Offset: 0x00004178
		public string unicodeToStr(byte[] data, int start, int length)
		{
			string text = "";
			int num = start;
			while (num < data.Length && data[num] != 0)
			{
				text += Convert.ToChar(data[num]);
				num += 2;
			}
			return text;
		}
	}
}
