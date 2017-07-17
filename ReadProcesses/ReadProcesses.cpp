
#include "ReadProcesses.h"


SIZE_T GetMemoryInfo(DWORD processID)
{
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	// Print the process identifier.


	// Print information about the memory usage of the process.

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
		return NULL;

	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
	{
		CloseHandle(hProcess);
		return pmc.WorkingSetSize;
	}

	CloseHandle(hProcess);
	return NULL;
}

// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1


void showProcessInformation() {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot) {
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnapshot, &pe32)) {
			do {
				DWORD procId = pe32.th32ProcessID;
				processName procName(pe32.szExeFile);
				SIZE_T memInfo = GetMemoryInfo(procId);
				if (memInfo == NULL || procName.empty()) {
					continue;
				}
				ProcInfo pInfo = ProcInfo(procId, memInfo);
				if(ProcessPIDMap.find(procName) == ProcessPIDMap.end()){
					ProcessPIDMap.emplace(procName, TotalProcInfo(pInfo));
				}
				else
				{
					(ProcessPIDMap[procName]).add(pInfo);
				}

			} while (Process32Next(hSnapshot, &pe32));
		}
		CloseHandle(hSnapshot);
	}
}



int main(void)
{
	// Get the list of process identifiers.


	showProcessInformation();
	std::cout << ProcessPIDMap.size() << std::endl;
	for (const auto & procIter : ProcessPIDMap ) {
		std::wcout << procIter.first.c_str() << std::endl;
		std::cout << procIter.second.totalMemory << std::endl;
	}
	return 0;
}
