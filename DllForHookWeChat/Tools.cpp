#include "pch.h"

HANDLE hMapFile = NULL;
LPVOID pBuf = NULL;

HANDLE hPipe;

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

HANDLE ConnectToPipe()
{
    // 定义管道名称
    const TCHAR* pipeName = _T("\\\\.\\pipe\\HookServerPipe");
    // 连接到命名管道
    while (true)
    {
        hPipe = CreateFile(
            pipeName,             // 管道名称
            GENERIC_WRITE,        // 写入访问
            0,                    // 无共享
            NULL,                 // 默认安全属性
            OPEN_EXISTING,        // 只打开现有管道
            0,                    // 默认属性
            NULL);                // 无模板文件

        if (hPipe != INVALID_HANDLE_VALUE)
        {
            MessageBox(NULL, L"named pipe connect success", L"INFO", MB_OK | MB_ICONERROR);
            break;
        }
            

        DWORD errorCode = GetLastError();
        TCHAR errorMsg[256];
        _stprintf_s(errorMsg, _T("Could not open pipe. Error Code: %lu"), errorCode);
        MessageBox(NULL, errorMsg, _T("Error"), MB_OK | MB_ICONERROR);
    }

    return hPipe;
}

void WriteAddressToNamePipe(const char* message)
{
    // 写入消息到命名管道
    DWORD bytesWritten;

    BOOL success = WriteFile
    (
        hPipe,                  // 管道句柄
        message,                // 写入的数据
        strlen(message),        // 数据长度
        &bytesWritten,          // 实际写入的字节数
        NULL
    );                          // 不使用重叠 I/O

    if (!success)
    {
        MessageBox(NULL, L"Send Msg Not Success", L"INFO", MB_OK | MB_ICONERROR);
    }
    // 显式刷新缓冲区
    FlushFileBuffers(hPipe);
    
}
