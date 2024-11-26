#include "pch.h"

HANDLE hMapFile = NULL;
LPVOID pBuf = NULL;

HANDLE hPipe;

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

HANDLE ConnectToPipe()
{
    // ����ܵ�����
    const TCHAR* pipeName = _T("\\\\.\\pipe\\HookServerPipe");
    // ���ӵ������ܵ�
    while (true)
    {
        hPipe = CreateFile(
            pipeName,             // �ܵ�����
            GENERIC_WRITE,        // д�����
            0,                    // �޹���
            NULL,                 // Ĭ�ϰ�ȫ����
            OPEN_EXISTING,        // ֻ�����йܵ�
            0,                    // Ĭ������
            NULL);                // ��ģ���ļ�

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
    // д����Ϣ�������ܵ�
    DWORD bytesWritten;

    BOOL success = WriteFile
    (
        hPipe,                  // �ܵ����
        message,                // д�������
        strlen(message),        // ���ݳ���
        &bytesWritten,          // ʵ��д����ֽ���
        NULL
    );                          // ��ʹ���ص� I/O

    if (!success)
    {
        MessageBox(NULL, L"Send Msg Not Success", L"INFO", MB_OK | MB_ICONERROR);
    }
    // ��ʽˢ�»�����
    FlushFileBuffers(hPipe);
    
}
