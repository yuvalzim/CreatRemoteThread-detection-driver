#pragma once
#include <ntifs.h>
#include <ntddk.h>

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath);

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject);

void ThreadNotifyRoutine(HANDLE ProcessId, HANDLE ThreadId, BOOLEAN Create);

extern PCREATE_THREAD_NOTIFY_ROUTINE CreateThreadNotifyRoutine;
