using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CS_Test_App.BLL;

namespace CS_Test_App
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}

		private void button1_Click(object sender, 
			EventArgs e)
		{
			var testClass = new ClassWithHiddenCode();
			testClass.OpenCode();
		}

		private void button2_Click(object sender, 
			EventArgs e)
		{
			var testClass = new ClassWithHiddenCode();
			testClass.HidenCode();
		}			
	}
}
