// inline_hook_vjppro.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <winternl.h>

int main()
{
    unsigned char shellcode[] = {
     0x53,
     0x6a, 0x00,
     0x68, 0x00, 0x00, 0x00, 0x00,
     0x68, 0x00, 0x00, 0x00, 0x00,
     0x6A, 0x00,
     0XE8, 0x00, 0x00, 0x00, 0x00,
     0xE8, 0x01, 0x00, 0x00, 0x00,
     0xCC
    };

    unsigned char Tranpoline[] = {
        0x00, 0x00, 0x00, 0x00, 0x00,
        0x5B,
        0xC3
    };

    const char* mess_cap = "hooking_mf";
    const char* mess_title = "warning!";
    DWORD DWreturn ;

    //get "GetMessageA" adress on memory
    HMODULE hUser32 = LoadLibrary(TEXT("user32.dll"));
    DWORD Hooked_API_Adress= (DWORD)GetProcAddress(hUser32, "MessageBoxA");
    DWORD a = (DWORD)&shellcode;
    if (Hooked_API_Adress == NULL)
    {
        printf("Get API adress faild");
        return 1;
    }

    //
    DWORD lpflOldProtect = 0;
    if (!VirtualProtect((LPVOID)Hooked_API_Adress, 1024, PAGE_READWRITE, &lpflOldProtect)) {
        std::cerr << "Failed to change protect. Error: " << GetLastError() << std::endl;
        return 1;
    }

    //read first 5 byte of "GetMessageA" and save it in Tranpoline
    memcpy_s(Tranpoline, 0x5, (LPVOID)Hooked_API_Adress, 0x5);
    //memcpy_s((LPVOID)Hooked_API_Adress, 0x5, &a, 0x5);
    //memcpy_s((LPVOID)Hooked_API_Adress, 0x5, Tranpoline, 0x5);
    VirtualProtect((LPVOID)Hooked_API_Adress, 6, lpflOldProtect, &lpflOldProtect);

    MessageBoxA(0, "hi", "hi", 0);

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
