using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using CS_Test_App.Protection;

namespace CS_Test_App
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			HookProxy.Init();
		
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new Form1());
			
			
		}
	}
}
