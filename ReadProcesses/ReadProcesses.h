#pragma once
/*
returns the memory info
*/
#include <vector>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include <map>
#include <TlHelp32.h>

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
