using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace CS_Test_App.Protection
{
	internal class HookProxy
	{
		private class Init32
		{
			[DllImport("Hook.dll", CallingConvention = CallingConvention.Cdecl, 
				EntryPoint = "Init")]
			public static extern void Init();
		};

		private class Init64
		{
			[DllImport("HookX64.dll", CallingConvention = CallingConvention.Cdecl, 
				EntryPoint = "Init")]
			public static extern void Init();
		};
		
		private static bool LoadW32()
		{
			try	{
				Init32.Init();
			}
			catch (BadImageFormatException)	{
				return false;
			}		
			return true;
		}				
		
		private static bool LoadX64()
		{
			try {
				Init64.Init();
			}
			catch (BadImageFormatException)	{
				return false;
			}			
			return true;
		}

		public static bool Init()
		{
			try
			{
				if (!LoadX64())
					return LoadW32();				
				return true;
			}
			catch (Exception) // swallow exception when no lib is found
			{
				return true; 
			}
		}
	}
}
