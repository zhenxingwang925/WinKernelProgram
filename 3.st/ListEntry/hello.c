#include <ntddk.h>

// ����ڵ�ṹ�壨ֻ��������int��LIST_ENTRY��
typedef struct _SIMPLE_NODE {
    int Data1;
    int Data2;
    LIST_ENTRY ListEntry;  // �������Ӽ�
} SIMPLE_NODE, * PSIMPLE_NODE;

// ȫ������ͷ
LIST_ENTRY g_SimpleListHead;

// ��ʼ������
void InitSimpleList()
{
    InitializeListHead(&g_SimpleListHead);
}

// ��ӽڵ㵽����β��
void AddNode(int d1, int d2)
{
    PSIMPLE_NODE pNode = (PSIMPLE_NODE)ExAllocatePoolWithTag(
        NonPagedPool,
        sizeof(SIMPLE_NODE),
        'Node');

    if (pNode) {
        pNode->Data1 = d1;
        pNode->Data2 = d2;
        InsertTailList(&g_SimpleListHead, &pNode->ListEntry);
    }
}

// ��ӡ���нڵ�
void PrintNodes()
{
    PLIST_ENTRY pEntry;
    PSIMPLE_NODE pNode;

    KdPrint(("List Contents:\n"));

    pEntry = g_SimpleListHead.Flink;
    while (pEntry != &g_SimpleListHead) {
        pNode = CONTAINING_RECORD(pEntry, SIMPLE_NODE, ListEntry);
        KdPrint(("  Data1=%d, Data2=%d\n", pNode->Data1, pNode->Data2));
        pEntry = pEntry->Flink;
    }
}

// ɾ�����нڵ�
void CleanupList()
{
    while (!IsListEmpty(&g_SimpleListHead)) {
        PLIST_ENTRY pEntry = RemoveHeadList(&g_SimpleListHead);
        PSIMPLE_NODE pNode = CONTAINING_RECORD(pEntry, SIMPLE_NODE, ListEntry);
        ExFreePool(pNode);
    }
}

// �������
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    InitSimpleList();

    // ��Ӳ�������
    AddNode(1, 100);
    AddNode(2, 200);
    AddNode(3, 300);

    DbgBreakPoint();

    PrintNodes();
    CleanupList();

    DriverObject->DriverUnload = NULL; // ��򵥵���������Ҫж�غ���
    return STATUS_SUCCESS;
}