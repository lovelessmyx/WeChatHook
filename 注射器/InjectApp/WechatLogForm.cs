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
    public partial class WechatLogForm : Form
    {
        public WechatLogForm()
        {
            InitializeComponent();
        }


        // 提供一个线程安全的方法更新 TextBox
        public void UpdateTextBoxSafe(string text)
        {


            textBox1.Invoke(new Action(() =>
            {
                textBox1.Text += text;
                textBox1.SelectionStart = textBox1.Text.Length; // 将光标移动到文本的末尾
                textBox1.ScrollToCaret();  // 确保滚动到光标所在的位置
            } ));
            
            //if (textBox1.InvokeRequired)
            //{
            //    // 如果在其他线程调用，则使用 Invoke 跨线程执行
            //    textBox1.Invoke(new Action(() => textBox1.Text += text));
            //}
            //else
            //{
            //    // 如果在 UI 线程调用，直接更新
            //    textBox1.Text += text;
            //}
        }
    }
}
