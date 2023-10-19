#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <thread>
#include <chrono>
#include <io.h>
#include <fcntl.h>

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);

    int updateIntervalMs;

    std::wcout << L"Введіть інтервал оновлення (у мілісекундах): ";
    std::wcin >> updateIntervalMs;

    if (updateIntervalMs <= 0)
    {
        std::wcerr << L"Некоректний інтервал оновлення\n";
        return 1;
    }

    while (true)
    {
        system("chcp 65001>nul");
        std::wcout << L"Список усіх процесів!\n";
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot != INVALID_HANDLE_VALUE)
        {
            PROCESSENTRY32 processEntry;
            processEntry.dwSize = sizeof(PROCESSENTRY32);
            if (Process32First(snapshot, &processEntry))
            {
                do
                {
                    std::wcout << L"Ідентифікатор: " << processEntry.th32ParentProcessID;
                    std::wcout << L" назва: " << processEntry.szExeFile << std::endl;
                } while (Process32Next(snapshot, &processEntry));
            }
            CloseHandle(snapshot);
        }
        else
        {
            std::wcerr << L"Не вдалося отримати список процесів\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(updateIntervalMs));
    }

    return 0;
}
