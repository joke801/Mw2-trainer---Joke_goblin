#pragma once
#include "Header.hpp"

class mem {
private:

	HANDLE hGameHandle;
	DWORD ulProcId;


	bool SetProcessID(const char* processName);

	bool CreateHandle();


public:


	template<class T>
	void WriteProcess(DWORD addy, T value);

	template<class T>
	void WriteProcess(DWORD addy, T value, BYTE Offset);


	mem(const char* sProcessName);
	~mem();


};

template<class T>
inline void mem::WriteProcess(DWORD addy, T value) {
	WriteProcessMemory(this->hGameHandle, (LPVOID)addy, (LPCVOID)&value, sizeof(value), NULL);
}

template<class T>
inline void mem::WriteProcess(DWORD addy, T value, BYTE Offset) {
	DWORD newAddy = 0;
	ReadProcessMemory(this->hGameHandle, (LPCVOID)addy, (LPVOID)&newAddy, sizeof(newAddy), NULL);
	if (newAddy)
		WriteProcessMemory(this->hGameHandle, (LPVOID)(newAddy + Offset), (LPCVOID)&value, sizeof(value), NULL);
}