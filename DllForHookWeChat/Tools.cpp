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
