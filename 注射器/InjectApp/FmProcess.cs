using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace InjectApp
{
    public partial class FmChoseProcess : Form
    {

        public event Action<string> ParameterPassedBack = null; // 定义一个事件，用于传递参数

        List<string> list = new List<string>();
        public FmChoseProcess()
        {
            InitializeComponent();
        }

        public FmChoseProcess(List<string> _list)
        {
            InitializeComponent();
            this.list = _list;
            foreach (var var in list)
            {
                this.listBox1.Items.Add(var);
            }

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            this.listBox1.Items.Clear();
            foreach (var var in list)
            {
                if (var.ToLower().Contains(this.textBox1.Text))
                {
                    this.listBox1.Items.Add(var);
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if(this.listBox1.SelectedItem == null)
            {
                MessageBox.Show("请选择正确的进程注入");
            }
            else
            {
                var str = this.listBox1.SelectedItem.ToString();
                ParameterPassedBack?.Invoke(str);
                this.Close();
            }

            
        }
    }
}
