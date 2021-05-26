#include "Header.hpp"
#include "Mem.hpp"
#include <time.h>

struct Values {
	float fFov = 65; //Default is 65
	bool bRedBoxes = false;
	bool bNoRecoil = false;
	bool bNametags = false;
};
Values values;




int main() {

	//MAKES CURSOR INVISIBLE
	HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cInfo;
	GetConsoleCursorInfo(hConsoleHandle, &cInfo);
	cInfo.bVisible = NULL;
	SetConsoleCursorInfo(hConsoleHandle, &cInfo);


	//GETTING HANDLE TO THE GAME
	mem Mem("iw4mp.exe");

	//TIMER TO SEPERATE TOGGLES
	unsigned long ulOnePressTimer = clock();

//	Mem.WriteProcess<long>(0x004B751E, (long)2268696720); // PATCHING COLD BLOODED should be xd


	//HELPS FIND LOBBIES
	Mem.WriteProcess<int>(0x00BC3864, 500, 0xC);
	Mem.WriteProcess<int>(0x06644390, 300, 0xC);
	Mem.WriteProcess<int>(0x1B8C7A9 + 0x80, 0x0000);



	//LOOP THAT CHANGES VALUES INGAME
	while (!GetAsyncKeyState(VK_ADD)) {

		//FOV RESETS ON DEATH SO JUST KEEP WRITING IT
		Mem.WriteProcess<float>(0x00AAC278, values.fFov, 0xC); // 0x6392ec40 + 0xC Without the Offset

		//CHECKING FOR KEY PRESSES
		if (clock() - ulOnePressTimer > 250) {
			if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000) { // CHANGE THE KEYS TO WHAT YOU LIKE | https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
				values.bRedBoxes = !values.bRedBoxes;
				Mem.WriteProcess<unsigned long>(0x004885A5, values.bRedBoxes ? 3897987216 : 3897952628); //NOPPING
				ulOnePressTimer = clock();
			}
			else if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000) {
				values.bNoRecoil = !values.bNoRecoil;
				//Mem.WriteProcess<BYTE>(0x004B9FCB, values.bNoRecoil ? 117 : 116); // PATCHING //CRASHES WILL FIx iN THE FUTURE
				ulOnePressTimer = clock();
			}
			else if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000) {
				values.bNametags = !values.bNametags;
				Mem.WriteProcess<BYTE>(0x004879FC, values.bNametags ? (BYTE)0x84 : (BYTE)15);//PATCHING
				Mem.WriteProcess<unsigned long>(0x004877F0, values.bNametags ? 3296919984 : 3296968754); //NOPPING
				ulOnePressTimer = clock();
			}
			else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
				values.fFov -= 5;
				ulOnePressTimer = clock();
			}
			else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
				values.fFov += 5;
				ulOnePressTimer = clock();
			}
		}

		//PRINT MENU / UPDATE AFTER VALUE HAS CHANGED
		if (clock() - 10 <= ulOnePressTimer) {
			system("cls");
			printf("[ARROW] Field Of View: %.0f\n", values.fFov);
			printf("[NUM 1] Red Boxes: %s\n", values.bRedBoxes ? "Online" : "Offline");
			printf("[NUM 2] No Recoil: %s\n", values.bNoRecoil ? "Online" : "Offline");
			printf("[NUM 3] Nametags: %s\n", values.bNametags ? "Online" : "Offline");
		}
		Sleep(1);
	}


	//SETTING IT BACK TO ORIGINAL
	Mem.WriteProcess<unsigned long>(0x004885A5, 3897952628); //NOPPING
	Mem.WriteProcess<BYTE>(0x4B9FCB, 117); // PATCHING
	Mem.WriteProcess<BYTE>(0x004879FC, (BYTE)15);//PATCHING
	Mem.WriteProcess<unsigned long>(0x004877F0, 3296968754); //NOPPING
	Mem.WriteProcess<float>(0x63932AC, 65);


	return 0;
}