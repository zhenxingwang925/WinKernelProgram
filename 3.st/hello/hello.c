#include <ntddk.h>

// ����ж�غ���
VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);

    // ��ӡж����Ϣ
    KdPrint(("Hello World Driver Unloaded!\n"));
}

// ������ڵ�
NTSTATUS DriverEntry(
    PDRIVER_OBJECT DriverObject,
    PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    // ����ж�غ���
    DriverObject->DriverUnload = DriverUnload;

    // ��ӡ������Ϣ
    KdPrint(("Hello World Driver Loaded!\n"));

    return STATUS_SUCCESS;
}