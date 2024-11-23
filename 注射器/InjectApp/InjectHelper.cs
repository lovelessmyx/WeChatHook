using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace InjectApp
{
    /// <summary>
    /// 注射器工具类
    /// </summary>
    public class InjectHelper
    {
        #region windows外部函数
        [DllImport("kernel32.dll", SetLastError = true)]
        static extern IntPtr OpenProcess(int processAccess, bool bInheritHandle, int processId);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern IntPtr VirtualAllocEx(IntPtr hProcess, IntPtr lpAddress, uint dwSize, uint flAllocationType, uint flProtect);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, uint nSize, out IntPtr lpNumberOfBytesWritten);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern IntPtr CreateRemoteThread(IntPtr hProcess, IntPtr lpThreadAttributes, uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags, out IntPtr lpThreadId);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern IntPtr GetModuleHandle(string lpModuleName);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern IntPtr GetProcAddress(IntPtr hModule, string lpProcName);

        [DllImport("kernel32.dll")]
        static extern bool ReadProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, uint dwSize, out int lpNumberOfBytesRead);


        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool VirtualFreeEx(IntPtr hProcess, IntPtr lpAddress, uint dwSize, uint dwFreeType);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool CloseHandle(IntPtr hObject);
        #endregion





        private int processId;
        private string dllPath;


        private IntPtr injectedProcessHandle = IntPtr.Zero;//句柄
        private IntPtr allocMemAddress = IntPtr.Zero;//申请的内存控件


        public InjectHelper(int _processId,string _dllPath)
        {
            this.processId = _processId;
            this.dllPath = _dllPath;
        }

        public EnumRetState DoInject(out long memoryAddress)
        {
            memoryAddress = -1;
            injectedProcessHandle = OpenProcess(0x001F0FFF, false, processId);
            if (injectedProcessHandle == IntPtr.Zero)
            {
                MessageBox.Show("无法打开进程");
                return EnumRetState.CAN_NOT_OPEN_PROCESS;
            }

            //分配内存
            allocMemAddress = VirtualAllocEx(injectedProcessHandle, IntPtr.Zero, (uint)dllPath.Length, 0x3000, 0x40);
            if (allocMemAddress == IntPtr.Zero)
            {
                MessageBox.Show("无法在目标进程中分配内存");
                return EnumRetState.VIRTUAL_ALLOC_ERROR;
            }
            memoryAddress = allocMemAddress;
            byte[] bytes = Encoding.ASCII.GetBytes(dllPath);
            WriteProcessMemory(injectedProcessHandle, allocMemAddress, bytes, (uint)bytes.Length, out _);
            

            //启动远程线程
            IntPtr loadLibraryAddr = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
            CreateRemoteThread(injectedProcessHandle, IntPtr.Zero, 0, loadLibraryAddr, allocMemAddress, 0, out _);
            MessageBox.Show("DLL注入成功");
            return EnumRetState.OK;
        }


        public EnumRetState UnloadDll()
        {
            if (injectedProcessHandle != IntPtr.Zero && allocMemAddress != IntPtr.Zero)
            {
                // 释放在目标进程中分配的内存
                if (VirtualFreeEx(injectedProcessHandle, allocMemAddress, 0, 0x8000))
                {
                    MessageBox.Show("DLL内存释放成功");
                }
                else
                {
                    MessageBox.Show("DLL内存释放失败");
                    return EnumRetState.MEMORY_FREE_ERRPR;
                }

                // 关闭进程句柄
                if (CloseHandle(injectedProcessHandle))
                {
                    MessageBox.Show("进程句柄关闭成功");
                }
                else
                {
                    MessageBox.Show("进程句柄关闭失败");
                    return EnumRetState.HANDLE_CLOSE_ERROR;
                }

                injectedProcessHandle = IntPtr.Zero;
                allocMemAddress = IntPtr.Zero;
                return EnumRetState.OK;
            }
            else
            {
                MessageBox.Show("没有已注入的DLL可以卸载");
                return EnumRetState.NONE_DLL_UNLOAD;
            }
        }


        //首先打开进程，并且初始化进程句柄
        //IntPtr hProcess = OpenProcess(0x001F0FFF, false, processId);

        //申请内存
        //IntPtr allocatedMemory = VirtualAllocEx(hProcess, IntPtr.Zero, (uint)dllPath.Length, 0x1000 | 0x2000, 0x40);

        //创建远程线程

        //关闭句柄

    }
}
