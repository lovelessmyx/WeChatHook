#include "pch.h"

HANDLE hMapFile = NULL;
LPVOID pBuf = NULL;

// ���������ڴ�
void CreateSharedMemory()
{
    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // ʹ��ϵͳ��ҳ�ļ�
        NULL,                    // Ĭ�ϰ�ȫ����
        PAGE_READWRITE,          // �ɶ�д
        0,                       // ��λ�ļ���С
        256,                     // ��λ�ļ���С
        L"RAXSharedMemory");    // �����ڴ�����

    if (hMapFile == NULL)
    {
        //MessageBox(NULL, L"Could not create file mapping object", L"Error", MB_OK);
        return;
    }

    pBuf = MapViewOfFile(hMapFile, // �����ڴ���
        FILE_MAP_ALL_ACCESS, // �ɶ�дȨ��
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
        // �����ַ�������󳤶�Ϊ 256 ���ַ�
        char buffer[256];
        SIZE_T bytesRead;
        HANDLE hProcess = GetCurrentProcess();
        // ��ȡ RAX ָ��ָ����ڴ��е��ַ���
        if (ReadProcessMemory(hProcess, (LPCVOID)raxPointer, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) 
        {
            buffer[bytesRead] = '\0';  // ȷ���ַ����� NULL ��β
            memcpy(pBuf, buffer, bytesRead + 1);  // ���ַ���д�빲���ڴ�
        }
    }
}

//���������ܵ�
#define PIPE_NAME "\\\\.\\pipe\\HookServerPipe"
HANDLE hPipe;
HANDLE hThread;
bool stopThread = false;

// ��Ϣ����
#define MAX_QUEUE_SIZE 100
char messageQueue[MAX_QUEUE_SIZE][1024];
int queueStart = 0;
int queueEnd = 0;
CRITICAL_SECTION queueLock;
HANDLE queueEvent;

// �����̺߳���
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

            // ������Ϣ���ܵ�
            DWORD bytesWritten;
            BOOL success = WriteFile(hPipe, message, strlen(message), &bytesWritten, NULL);
            if (!success)
            {
                
            }

            FlushFileBuffers(hPipe); // ȷ������ˢ��
            EnterCriticalSection(&queueLock);
        }
        ResetEvent(queueEvent);
        LeaveCriticalSection(&queueLock);
    }
    return 0;
}

// ��ʼ�������߳�
void StartWorkerThread() 
{
    // �����ܵ�����
    hPipe = CreateFileA(
        PIPE_NAME,              // �ܵ�����
        GENERIC_WRITE,          // д����
        0,                      // �޹���
        NULL,                   // Ĭ�ϰ�ȫ����
        OPEN_EXISTING,          // �������Ѵ��ڵĹܵ�
        0,                      // Ĭ������
        NULL);                  // ��ģ���ļ�

    if (hPipe == INVALID_HANDLE_VALUE) {
        return;
    }

    // ��ʼ���ؼ������¼�
    InitializeCriticalSection(&queueLock);
    queueEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    // ���������߳�
    hThread = CreateThread(NULL, 0, PipeWorkerThread, NULL, 0, NULL);
}


void WriteAddressToNamePipe(DWORD64 raxPointer)
{
    const char* data = "12345";
    EnterCriticalSection(&queueLock);
    if ((queueEnd + 1) % MAX_QUEUE_SIZE != queueStart) { // �������Ƿ�����
        strncpy_s(messageQueue[queueEnd], data, sizeof(messageQueue[queueEnd]) - 1);
        messageQueue[queueEnd][sizeof(messageQueue[queueEnd]) - 1] = '\0';
        queueEnd = (queueEnd + 1) % MAX_QUEUE_SIZE;
        SetEvent(queueEvent); // ֪ͨ�����̴߳�����Ϣ
    }
    else {
        //printf("Message queue is full. Dropping message.\n");
    }
    LeaveCriticalSection(&queueLock);
    
}
