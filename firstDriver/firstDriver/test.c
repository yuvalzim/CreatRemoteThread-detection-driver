#include "test.h"
#include "messages.h"

PCREATE_THREAD_NOTIFY_ROUTINE CreateThreadNotifyRoutine = ThreadNotifyRoutine;


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	UNREFERENCED_PARAMETER(pRegistryPath);
	pDriverObject->DriverUnload = UnloadDriver;
	DebugMessage("Driver is loaded\n");
	PsSetCreateThreadNotifyRoutine(CreateThreadNotifyRoutine);
	return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject)
{
	UNREFERENCED_PARAMETER(pDriverObject);
	DebugMessage("Driver is unloaded\n");

	PsRemoveCreateThreadNotifyRoutine(CreateThreadNotifyRoutine);

	return STATUS_SUCCESS;
}



void ThreadNotifyRoutine(HANDLE ProcessId, HANDLE ThreadId, BOOLEAN Create)
{
	if (!Create)
		return;

	PETHREAD thread;
	PETHREAD caller_thread = PsGetCurrentThread();
	NTSTATUS status = PsLookupThreadByThreadId(ThreadId, &thread);
	if (!NT_SUCCESS(status))
		return;

	ULONG caller_proc_id = HandleToUlong(PsGetThreadProcessId(caller_thread)); //id of the process that calls CreateRemoteThread
	
	if (HandleToULong(ProcessId) != caller_proc_id)
		DebugMessage("Process %i created thread %i in process %i \n", caller_proc_id, HandleToULong(ThreadId), HandleToULong(ProcessId));

	ObDereferenceObject(thread);
}