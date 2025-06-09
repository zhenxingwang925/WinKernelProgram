
#include "serive.h"


/*
    OpenSCManager
    CreateService
    StartService
    ControlService
    DeleteService
*/


HANDLE g_hService = NULL;

#define GC_SERVICE_NAME L"ctx"
#define GC_SERVICE_PATH L"C:\\Windows\\System32\\drivers\\ctx.sys"

int gcInstallService()
{
    int retval = -1;
    HANDLE hManager = NULL;
    HANDLE hService = NULL;
    DWORD lastError = 0;

    hManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hManager) 
    {
        printf("[-] OpenSCManagerA failed: %lu\n", GetLastError());
        goto __end__;
    }

    hService = CreateService(
        hManager,
        GC_SERVICE_NAME, // 服务名称
        GC_SERVICE_NAME, // 显示的服务名称
        SERVICE_ALL_ACCESS,
        SERVICE_KERNEL_DRIVER,
        SERVICE_DEMAND_START,
        SERVICE_ERROR_NORMAL,
        GC_SERVICE_PATH,
        NULL, NULL, NULL, NULL, NULL
    );

    if (!hService) 
    {
        lastError = GetLastError();

        if (lastError == ERROR_SERVICE_EXISTS)
        {
            printf("[*] Service already exists.\n");
            retval = 0;
        }
        else 
        {
            printf("[-] CreateService failed: 0x%x\n", lastError);
        }
        goto __end__;
    }
    
    retval = 0;
    printf("[+] Driver service create.\n");
    // g_hService = hService;
    // hService = NULL;

__end__:

    if (hManager)
    {
        CloseServiceHandle(hManager);
    }

    if (hService)
    {
        CloseServiceHandle(hService);
    }

    return retval;
}

/*
    获取服务handle
*/
HANDLE getServerHandle()
{
    SC_HANDLE hManager = NULL;
    SC_HANDLE hService = NULL;

    if (g_hService)
    {
        hService = g_hService;
        goto __end__;
    }

    hManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hManager)
    {
        printf("[-] OpenSCManager failed: 0x%x\n", GetLastError());
        goto __end__;
    }

    hService = OpenService(hManager, GC_SERVICE_NAME, SC_MANAGER_ALL_ACCESS);
    if (!hService) 
    {
        printf("[-] OpenService failed: 0x%x\n", GetLastError());
    }

    // g_hService = hService;
    // hService = NULL;

__end__:

    if (hManager)
    {
        CloseServiceHandle(hManager);
    }

    //if (hService)
    //{
    //    CloseServiceHandle(hService);
    //}

    return hService;
}


int gcStopService()
{
    int              retval = -1;
    SC_HANDLE        hService = NULL;
    SERVICE_STATUS   status = { 0 };
    BOOL             result = FALSE;

    hService = getServerHandle();
    if (!hService)
    {
        goto __end__;
    }

    result = ControlService(hService, SERVICE_CONTROL_STOP, &status);
    {
        printf("[-] ControlService failed: %lu\n", GetLastError());
    }

    retval = 0;
    printf("[+] Driver service stop.\n");

__end__:

    if (hService)
    {
        CloseServiceHandle(hService);
    }

    return result;
}


int gcStartService()
{
    int retval = -1;
    BOOL result = FALSE;
    HANDLE hService = NULL;
    DWORD lasterError = 0;

    hService = getServerHandle();
    if (!hService)
    {
        goto __end__;
    }

    result = StartService(hService, 0, NULL);
    if (result)
    {
        retval = 0; // 成功
        printf("[+] Driver service start.\n");
        goto __end__;
    }

    lasterError = GetLastError();
    if (lasterError != ERROR_SERVICE_ALREADY_RUNNING)
    {
        printf("[-] StartServiceA failed: 0x%x\n", lasterError);
        goto __end__;
    }

    // 成功
    printf("[*] Driver already running.\n");
    retval = 0;

__end__:

    if (hService)
    {
        CloseServiceHandle(hService);
    }

    return retval;
}



int gcDeleteService()
{
    int       retval = -1;
    SC_HANDLE hService = NULL;
    BOOL      result = FALSE;

    hService = getServerHandle();
    if (!hService)
    {
        goto __end__;
    }
    
    result = DeleteService(hService);
    if (!result)
    {
        printf("[-] DeleteService failed: %lu\n", GetLastError());
    }

    retval = 0;
    printf("[+] Driver service deleted.\n");

__end__:

    if (hService)
    {
        CloseServiceHandle(hService);
    }

    return retval;
}



int main()
{

    gcInstallService();

    gcStartService();

    gcStopService();

    gcDeleteService();

    return 0;

}






