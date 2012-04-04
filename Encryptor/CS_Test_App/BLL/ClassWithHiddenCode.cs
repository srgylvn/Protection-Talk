using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CS_Test_App.Protection;

namespace CS_Test_App.BLL
{
	public class ClassWithHiddenCode
	{
		public void OpenCode()
		{
			System.Windows.Forms.MessageBox.Show(
				"Open code!"
			);		
		}
		
		[Hide]
		public void HidenCode()
		{
			System.Windows.Forms.MessageBox.Show(
				"Hidden code!"
			);				
		}
	}
}
