#include "Mem.hpp"
#include <TlHelp32.h>

bool mem::SetProcessID(const char* sProcessName) {

	HANDLE hProc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hProc == INVALID_HANDLE_VALUE)
		return false;

	PROCESSENTRY32  processEntry;
	processEntry.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hProc, &processEntry))
	{
		do
		{
			if (!strcmp(sProcessName, processEntry.szExeFile))
			{
				this->ulProcId = processEntry.th32ProcessID;
				CloseHandle(hProc);
				return true;
			}
		} while (Process32Next(hProc, &processEntry));
	}
	CloseHandle(hProc);

	return false;
}

bool mem::CreateHandle() {
	this->hGameHandle = INVALID_HANDLE_VALUE;

	this->hGameHandle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_READ |PROCESS_VM_OPERATION, NULL, this->ulProcId);

	if (this->hGameHandle == INVALID_HANDLE_VALUE)
		return false;

	return true;
}


mem::~mem() {
	CloseHandle(this->hGameHandle);
}

mem::mem(const char* sProcessName) {

	while (!this->SetProcessID(sProcessName)) {
		printf("\rSearching for %s  /", sProcessName);
		Sleep(800);
		printf("\rSearching for %s  -", sProcessName);
		Sleep(800);
		printf("\rSearching for %s  \\", sProcessName);
		Sleep(800);
		printf("\rSearching for %s  |", sProcessName);
		Sleep(800);
	}
	system("cls");

	if (!CreateHandle()) {
		std::cout << "Could not Create Handle";
		Sleep(3000);
		exit(-1);
	}
}
