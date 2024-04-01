#include <Windows.h>
#include <stdio.h>
#include <newdev.h>

#pragma comment(lib, "Newdev.lib")

int main(int argc, char*argv[])
{
    const wchar_t* infFilePath = L"xxx.inf";

    if (strcmp(argv[1], "-i") == 0)
    {
        if (!DiInstallDriver(NULL, infFilePath, 0, NULL))
        {
            printf("Failed to install INF file. Error: %d\n", GetLastError());
            return 1;
        }

        printf("INF file installed successfully.\n");
    }
    else if (strcmp(argv[1], "-u") == 0)
    {
        if (!DiUninstallDriver(NULL, infFilePath, 0, NULL))
        {
            printf("Failed to uninstall INF file. Error: %d\n", GetLastError());
            return 1;
        }

        printf("INF file uninstalled successfully.\n");

    }

    return 0;
}
