#include "pch.h"


HANDLE hMapFile = NULL;
LPVOID pBuf = NULL;

// 创建共享内存
void CreateSharedMemory()
{
    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // 使用系统分页文件
        NULL,                    // 默认安全属性
        PAGE_READWRITE,          // 可读写
        0,                       // 高位文件大小
        256,                     // 低位文件大小
        L"RAXSharedMemory");    // 共享内存名称

    if (hMapFile == NULL)
    {
        //MessageBox(NULL, L"Could not create file mapping object", L"Error", MB_OK);
        return;
    }

    pBuf = MapViewOfFile(hMapFile, // 共享内存句柄
        FILE_MAP_ALL_ACCESS, // 可读写权限
        0,
        0,
        256);

    if (pBuf == NULL)
    {
        //MessageBox(NULL, L"Could not map view of file", L"Error", MB_OK);
        CloseHandle(hMapFile);
    }
}
void OutputPointerValueToSharedMemory(DWORD64 raxPointer) 
{
    if (pBuf != NULL && raxPointer != 0) 
    {
        // 假设字符串的最大长度为 256 个字符
        char buffer[256];
        SIZE_T bytesRead;
        HANDLE hProcess = GetCurrentProcess();
        // 读取 RAX 指针指向的内存中的字符串
        if (ReadProcessMemory(hProcess, (LPCVOID)raxPointer, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) 
        {
            buffer[bytesRead] = '\0';  // 确保字符串以 NULL 结尾
            memcpy(pBuf, buffer, bytesRead + 1);  // 将字符串写入共享内存
        }
    }
}
