#include <ntddk.h>

// 驱动卸载函数
VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);

    // 打印卸载信息
    KdPrint(("Hello World Driver Unloaded!\n"));
}

// 驱动入口点
NTSTATUS DriverEntry(
    PDRIVER_OBJECT DriverObject,
    PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    // 设置卸载函数
    DriverObject->DriverUnload = DriverUnload;

    // 打印加载信息
    KdPrint(("Hello World Driver Loaded!\n"));

    return STATUS_SUCCESS;
}