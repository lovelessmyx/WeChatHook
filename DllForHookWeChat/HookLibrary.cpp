#include "pch.h"
#include <stdio.h>
#include <windows.h>

DWORD64 baseAddress = (DWORD64)LoadLibrary(L"WeChatWin.dll");



#pragma region 获取日志HOOK
DWORD64 getLogHookOffset = 0x261BBC1;      // 需要hook的偏移地址
DWORD64 getLogHookOgifinalCallAdress = baseAddress + 0x436FBA0;   // 原始跳转地址
BYTE getInfoHookOriginalBytes[12];              // 保存原始字节，用于恢复hook


void GetInfoHookFuncForOutRaxValue(DWORD64 raxValue)
{
    OutputPointerValueToSharedMemory(raxValue);
    return;
}



void InstallShowLogHook()
{
    DWORD oldProtect;
    DWORD64 hookAddress = baseAddress + getLogHookOffset;

    // 获取当前进程句柄
    HANDLE hProcess = GetCurrentProcess();

    // 保存原始字节
    ReadProcessMemory(hProcess, (LPCVOID)hookAddress, getInfoHookOriginalBytes, sizeof(getInfoHookOriginalBytes), NULL);

    // 修改内存保护
    if (VirtualProtect((LPVOID)hookAddress, sizeof(getInfoHookOriginalBytes), PAGE_EXECUTE_READWRITE, &oldProtect) == false)
    {
        DWORD error = GetLastError();
        MessageBox(NULL, L"VirtualProtect Error", L"RAX Pointer Value", MB_OK | MB_ICONINFORMATION);
        // 格式化错误信息
        char message[256];
        sprintf_s(message, sizeof(message), "Error Code: %d\0", error);

        // 显示错误信息
        MessageBox(NULL, (LPCWSTR)message, L"Error", MB_OK | MB_ICONERROR);
    }
    else
    {
        MessageBox(NULL, L"VirtualProtect Success", L"RAX Pointer Value", MB_OK | MB_ICONINFORMATION);
    }

    // 分配可执行内存用于存放hook代码
    void* pShellcode = VirtualAlloc(NULL, 1024, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!pShellcode) {
        //sprintf("Failed to allocate memory for shellcode\n");
        MessageBox(NULL, L"Failed to allocate memory for shellcode", L"error", MB_OK | MB_ICONINFORMATION);
        return;
    }
    BYTE shellcode[] = {
        //将参数RAX放入RCX中
        0x51,                               //push rcx                      0
        0x48, 0x81, 0xEC, 0xE8, 0x00, 0x00, 0x00,
        0x48,0x89,0xc1,                     //mov rcx,rax 
        0x48, 0xBA,                         //mov rdx,TestFunc              11


        0x90, 0x90, 0x90, 0x90,                                           //13
        0x90, 0x90, 0x90, 0x90,             // 占位符，用于后续填充      
        0xFF, 0xD2,                         // call rdx

        0x48 ,0x81 ,0xC4 ,0xE8, 0x00, 0x00, 0x00,
        0x59,                               //pop rcx



        0x48, 0xBA,                         // mov rdx, <getLogHookOgifinalCallAdress 地址>
        0x90, 0x90, 0x90, 0x90,                                          //33
        0x90, 0x90, 0x90, 0x90,             // 占位符，用于后续填充      
        0xFF, 0xD2,                         // call rdx
        0x5A,                               // pop rdx
        0x48, 0x81, 0xC4, 0x18, 0x13, 0x00, 0x00,//add rsp,1318
        0x41,0x5f,//pop r15                                            
        0x41,0x5e,//pop r14
        0x41,0x5d,//pop r13
        0x41,0x5c,//pop r12
        0x5f,//pop rdi
        0x5e,//pop rsi
        0x5b,//pop rbx
        0x5d,//pop rbp

        //最后跳回之前的位置
        0x48, 0xB8,                         // mov rax, <返回的 地址>
        0x90, 0x90, 0x90, 0x90,                                         //65
        0x90, 0x90, 0x90, 0x90,             // 占位符，用于后续填充
        0xFF, 0xE0,
    };
    // 填充原始跳转地址
    *(DWORD64*)(shellcode + 13) = (DWORD64)GetInfoHookFuncForOutRaxValue;
    *(DWORD64*)(shellcode + 33) = getLogHookOgifinalCallAdress;
    *(DWORD64*)(shellcode + 65) = hookAddress + 0x18;



    // 将 hook 代码写入分配的内存
    memcpy(pShellcode, shellcode, sizeof(shellcode));

    wchar_t message[256];
    swprintf_s(message, sizeof(message) / sizeof(wchar_t), L"Shellcode is located at: %p", pShellcode);
    MessageBox(NULL, message, L"Shellcode Memory Address", MB_OK | MB_ICONINFORMATION);

    BYTE shellcodeAddress[sizeof(DWORD64)];
    memcpy(shellcodeAddress, &pShellcode, sizeof(DWORD64));


    // 写入跳转指令到 hook 位置
    DWORD64 relativeAddress = (DWORD64)pShellcode;
    BYTE jumpInstruction[24] =
    {
        0x52,                                                           // push rdx
        0x48, 0xBA,                                                     // mov rdx
        0x90, 0x90, 0x90, 0x90,0x90, 0x90, 0x90, 0x90,                  // 预留rdx的值，指向上面的代码段
        0xFF, 0xE2,                                                     // jmp rdx
        0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
    }; // 24字节 
    for (int i = 0; i < 8; i++)
    {
        jumpInstruction[i + 3] = shellcodeAddress[i];
    }



    if (WriteProcessMemory(hProcess, (LPVOID)hookAddress, jumpInstruction, sizeof(jumpInstruction), NULL) == false)
    {
        MessageBox(NULL, L"WriteProcessMemory Error", L"RAX Pointer Value", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBox(NULL, L"WriteProcessMemory SUCCESS", L"RAX Pointer Value", MB_OK | MB_ICONINFORMATION);
    }

    // 恢复内存保护
    VirtualProtect((LPVOID)hookAddress, sizeof(getInfoHookOriginalBytes), oldProtect, &oldProtect);
}

// 移除 Hook
void RemoveShowLogHook()
{
    DWORD oldProtect;
    DWORD64 hookAddress = baseAddress + getLogHookOffset;

    // 获取目标进程句柄
    HANDLE hProcess = GetCurrentProcess();

    // 修改内存保护
    VirtualProtect((LPVOID)hookAddress, sizeof(getInfoHookOriginalBytes), PAGE_EXECUTE_READWRITE, &oldProtect);

    // 恢复原始字节
    WriteProcessMemory(hProcess, (LPVOID)hookAddress, getInfoHookOriginalBytes, sizeof(getInfoHookOriginalBytes), NULL);

    // 恢复内存保护
    VirtualProtect((LPVOID)hookAddress, sizeof(getInfoHookOriginalBytes), oldProtect, &oldProtect);
}
#pragma endregion


