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

//创建命名管道
#define PIPE_NAME "\\\\.\\pipe\\HookServerPipe"
HANDLE hPipe;
HANDLE hThread;
bool stopThread = false;

// 消息队列
#define MAX_QUEUE_SIZE 100
char messageQueue[MAX_QUEUE_SIZE][1024];
int queueStart = 0;
int queueEnd = 0;
CRITICAL_SECTION queueLock;
HANDLE queueEvent;

// 工作线程函数
DWORD WINAPI PipeWorkerThread(LPVOID lpParam) 
{
    while (true) 
    {
        WaitForSingleObject(queueEvent, INFINITE);
        if (stopThread) 
        {
            break;
        }

        EnterCriticalSection(&queueLock);
        while (queueStart != queueEnd) 
        {
            char* message = messageQueue[queueStart];
            queueStart = (queueStart + 1) % MAX_QUEUE_SIZE;
            LeaveCriticalSection(&queueLock);

            // 发送消息到管道
            DWORD bytesWritten;
            BOOL success = WriteFile(hPipe, message, strlen(message), &bytesWritten, NULL);
            if (!success)
            {
                
            }

            FlushFileBuffers(hPipe); // 确保立即刷新
            EnterCriticalSection(&queueLock);
        }
        ResetEvent(queueEvent);
        LeaveCriticalSection(&queueLock);
    }
    return 0;
}

// 初始化工作线程
void StartWorkerThread() 
{
    // 创建管道连接
    hPipe = CreateFileA(
        PIPE_NAME,              // 管道名称
        GENERIC_WRITE,          // 写访问
        0,                      // 无共享
        NULL,                   // 默认安全属性
        OPEN_EXISTING,          // 必须是已存在的管道
        0,                      // 默认属性
        NULL);                  // 无模板文件

    if (hPipe == INVALID_HANDLE_VALUE) {
        return;
    }

    // 初始化关键区和事件
    InitializeCriticalSection(&queueLock);
    queueEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    // 创建工作线程
    hThread = CreateThread(NULL, 0, PipeWorkerThread, NULL, 0, NULL);
}


void WriteAddressToNamePipe(DWORD64 raxPointer)
{
    const char* data = "12345";
    EnterCriticalSection(&queueLock);
    if ((queueEnd + 1) % MAX_QUEUE_SIZE != queueStart) { // 检查队列是否已满
        strncpy_s(messageQueue[queueEnd], data, sizeof(messageQueue[queueEnd]) - 1);
        messageQueue[queueEnd][sizeof(messageQueue[queueEnd]) - 1] = '\0';
        queueEnd = (queueEnd + 1) % MAX_QUEUE_SIZE;
        SetEvent(queueEvent); // 通知工作线程处理消息
    }
    else {
        //printf("Message queue is full. Dropping message.\n");
    }
    LeaveCriticalSection(&queueLock);
    
}
