#pragma once
/*
returns the memory info
*/
#include <vector>
#include <iostream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include <map>
#include <TlHelp32.h>

#ifndef READ_PROCESS_INCLUDE_H
#define READ_PROCESS_INCLUDE_H

// this is currently a macro but it needs to
// be a custom value that could be set.
#define NUMBER_OF_PROCESSES 4096


typedef std::wstring processName; 
typedef WCHAR processString[MAX_PATH];

typedef struct ProcInfo{
	DWORD pid;
	SIZE_T memory;
	ProcInfo(DWORD i_pid, DWORD i_memory) {
		pid = i_pid;
		memory = i_memory;
	}
} ProcInfo;

typedef struct TotalProcInfo{
	std::vector<ProcInfo> ProcessInfoList;
	SIZE_T totalMemory;
	TotalProcInfo() {
		totalMemory = 0;
	}
	TotalProcInfo(ProcInfo& p) {
			add(p);
	}
	void add(ProcInfo& p) {
		ProcessInfoList.push_back(p);
		totalMemory += p.memory;
	}

} TotalProcInfo;

SIZE_T GetMemoryInfo(DWORD processID);

void showProcessInformation();
typedef std::map<processName, TotalProcInfo > ProcessMapType;
ProcessMapType ProcessPIDMap;
typedef ProcessMapType::iterator ProcessMapIterator;
const char * arrayOfByteConversions[4] = { "B", "KB", "MB", "GB" };

// returns the handle for a given process
HANDLE GetHandleFromPID(DWORD procId) {
	return OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, procId);
}
#endif // !READ_PROCESS_INCLUDE_H
