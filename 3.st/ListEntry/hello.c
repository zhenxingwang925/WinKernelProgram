#include <ntddk.h>

// 链表节点结构体（只包含两个int和LIST_ENTRY）
typedef struct _SIMPLE_NODE {
    int Data1;
    int Data2;
    LIST_ENTRY ListEntry;  // 链表连接件
} SIMPLE_NODE, * PSIMPLE_NODE;

// 全局链表头
LIST_ENTRY g_SimpleListHead;

// 初始化链表
void InitSimpleList()
{
    InitializeListHead(&g_SimpleListHead);
}

// 添加节点到链表尾部
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

// 打印所有节点
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

// 删除所有节点
void CleanupList()
{
    while (!IsListEmpty(&g_SimpleListHead)) {
        PLIST_ENTRY pEntry = RemoveHeadList(&g_SimpleListHead);
        PSIMPLE_NODE pNode = CONTAINING_RECORD(pEntry, SIMPLE_NODE, ListEntry);
        ExFreePool(pNode);
    }
}

// 驱动入口
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    InitSimpleList();

    // 添加测试数据
    AddNode(1, 100);
    AddNode(2, 200);
    AddNode(3, 300);

    DbgBreakPoint();

    PrintNodes();
    CleanupList();

    DriverObject->DriverUnload = NULL; // 最简单的驱动不需要卸载函数
    return STATUS_SUCCESS;
}