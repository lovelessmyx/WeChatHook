using System.Diagnostics;
using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;
using System.Text;

namespace InjectApp
{
    public partial class Form1 : Form
    {

        [DllImport("kernel32.dll")]
        public static extern bool ReadProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, int dwSize, out int lpNumberOfBytesRead);

        public Form1()
        {
            InitializeComponent();
            //延迟加载textbox，防止焦点一直在这里
            this.btn_dll_inject.Enabled = false;
            this.btn_process.Enabled = false;
            textBox1.Enabled = false;
            this.text_process_id.Focus();
            this.Shown += (s, args) =>
            {
                textBox1.Enabled = true;
            };
            //textBox1.Enabled = true;
        }
        FmChoseProcess fp = null;
        InjectHelper inject = null;
        List<Process> processesList = new List<Process>();
        Dictionary<int, string> processIdDic = new Dictionary<int, string>();
        private void InitProcess()
        {
            processIdDic.Clear();
            processesList = Process.GetProcesses().ToList();
            foreach (var var in processesList)
            {
                processIdDic.Add(var.Id, var.ProcessName);
            }
        }


        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "DLL files (*.dll)|*.dll";
            openFileDialog.Title = "选择要注入的DLL文件";
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                this.textBox1.Text = openFileDialog.FileName;
                this.btn_process.Enabled = true;
            }
        }

        private void btn_dll_inject_Click(object sender, EventArgs e)
        {

            //开启命名通道接受信息
            Thread thread = new Thread(readMsg);
            thread.Start();

            //注入
            inject = new InjectHelper(int.Parse(this.text_process_id.Text), this.textBox1.Text);
            inject.DoInject(out long address);
            this.lab_address.Text = "0x" + address.ToString("X");
            this.btn_dll_unload.Enabled = true;
            this.btn_dll_inject.Enabled = false;

        }

        private void readMsg()
        {
            //首先打开命名通道
            NamedPipesHelper namedPipesHelper = new NamedPipesHelper();
            while (true)
            {
                string str = namedPipesHelper.Read();
                //Console.WriteLine(str);
                MessageBox.Show(str);
            }
        }

        private void btn_process_Click(object sender, EventArgs e)
        {
            InitProcess();
            //先便利所有进程
            fp = new FmChoseProcess(processIdDic.Values.ToList());
            fp.ParameterPassedBack += ParameterPassedBack;
            fp.Show();
        }
        private void ParameterPassedBack(string str)
        {
            foreach (var var in processesList)
            {
                if (var.ProcessName == str)
                {
                    this.text_process_id.Text = var.Id.ToString();
                    this.text_process_name.Text = var.ProcessName;
                    this.btn_dll_inject.Enabled = true;
                    return;
                }
            }
        }

        private void btn_dll_unload_Click(object sender, EventArgs e)
        {
            if (inject.UnloadDll() == EnumRetState.OK)
            {
                Console.WriteLine("  ");
                this.btn_dll_unload.Enabled = false;
                this.btn_dll_inject.Enabled = true;
            }
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            try
            {
                // 打开共享内存对象
                using (MemoryMappedFile mmf = MemoryMappedFile.OpenExisting("RAXSharedMemory"))
                {
                    // 创建视图来读取共享内存中的数据
                    using (MemoryMappedViewAccessor accessor = mmf.CreateViewAccessor(0,256))
                    {
                        // 读取共享内存中的数据
                        byte[] buffer = new byte[256];
                        accessor.ReadArray(0, buffer, 0, buffer.Length);

                        // 查找字符串的结束符位置 (NULL 字符)
                        int nullIndex = Array.IndexOf(buffer, (byte)0);
                        if (nullIndex >= 0)
                        {
                            // 将字节数组转换为字符串
                            string resultString = Encoding.UTF8.GetString(buffer, 0, nullIndex);
                            //Console.WriteLine($"Read string: {resultString}");
                            MessageBox.Show(resultString);
                        }
                        else
                        {
                            Console.WriteLine("Failed to find the end of the string in shared memory.");
                        }


                    }
                }
            }
            catch (Exception ex)
            {
                // 处理错误并输出错误信息
                Console.WriteLine("Error: " + ex.Message);
            }
        }
    }
}
