#include "ReadProcesses.h"

/*
returns the resident size of the process in windows.
This should actually return the memory that is allocated in windows.
*/
SIZE_T GetMemoryInfo(DWORD processID)
{
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

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
	return std::to_string(prev_bytes) + ' ' + std::string(arrayOfByteConversions[--index]);
}


// Currently lists out all the processes in a global PID Map.
// TODO: Create a local Process PID Map, in such a way that
// it would be able to cache it as well.
void showProcessInformation() {
	DWORD listOfProcesses[NUMBER_OF_PROCESSES]; // will store all the pid's
	DWORD allPIDList;                           // this will accumulate all the pid's and hence be needed
	if (!EnumProcesses(listOfProcesses, sizeof(listOfProcesses), &allPIDList)) {
		std::cout << "There is some problem in enumerating all the processes" << std::endl;
		return;
	}
	DWORD numberOfProcesses;
	numberOfProcesses = allPIDList / sizeof(DWORD);
	for (DWORD index = 0; index < numberOfProcesses; index++) {
		if (listOfProcesses[index] != 0) {

		}
	}
}

ProcInfo getProcessNameAndPID(DWORD procId) {
	TCHAR processName[MAX_PATH] = TEXT("<unknown>");
	HANDLE ProcessHandle = GetHandleFromPID(procId);
	// Get Process Name as well as memory information
	if (ProcessHandle != NULL) {
		HMODULE handleModule;
		DWORD  bytesNeeded;
		// EnumProcessModules will list out the information required
		if (EnumProcessModules(ProcessHandle, &handleModule, sizeof(handleModule), &bytesNeeded)) {
			GetModuleBaseName(ProcessHandle, handleModule, processName, sizeof(processName) / sizeof(TCHAR));

		}
	}

}



int main(void)
{
	// Get the list of process identifiers.
	//showProcessInformation();
	//std::cout << "Number of processes : " <<  ProcessPIDMap.size() << std::endl;
	//for (const auto & procIter : ProcessPIDMap ) {
	//	std::wcout << procIter.first.c_str() << std::endl;
	//	std::cout << format_in_proper_bytes(procIter.second.totalMemory, 2) << std::endl;
	//}
	SIZE_T x = 1;
	int  a = x;
	printf("%d\n", a);

	return 0;
}
