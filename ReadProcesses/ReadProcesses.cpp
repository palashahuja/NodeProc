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


//TODO: To format the bytes into proper bytes
/**
Notes
=====
- Always take the input in the form of SIZE_T, implicit conversion of 
SIZE_T to int would make the value negative.
- lastindex would control what sort of information do we want to display
from the process.
if lastindex is 3, then it would return a string that ends in "GB"
if lastindex is 2, then it would return a string that ends in "MB"
**/
std::string format_in_proper_bytes(SIZE_T bytes, int lastindex) {
	int index = 0;
	int prev_bytes = 0;
	while (bytes > 0) {
		prev_bytes = bytes;
		bytes >>= 10;
		index++;
		if (lastindex + 1 == index) {
			break;
		}
	}
	return std::to_string(prev_bytes) + std::string(arrayOfByteConversions[--index]);
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
	std::cout << "Number of processes : " <<  ProcessPIDMap.size() << std::endl;
	for (const auto & procIter : ProcessPIDMap ) {
		std::wcout << procIter.first.c_str() << std::endl;
		std::cout << format_in_proper_bytes(procIter.second.totalMemory, 2) << std::endl;
	}
	return 0;
}
