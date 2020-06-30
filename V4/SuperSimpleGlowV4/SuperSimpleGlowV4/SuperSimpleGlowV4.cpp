// SuperSimpleGlowV4 : This file contains the 'main' function. Program execution begins and ends there.
//


/*
 * This file is part of voidzero-development repository (https://github.com/voidzero-development).
 * Copyright (c) 2020 voidzero-development
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */



#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <string>
#include "TlHelp32.h"
#include "math.h"
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

#include "csgo.hpp"
using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

#include "Hitmarkers.h"
#include <Windows.h>
#pragma comment(lib, "Winmm.lib")
#include <Mmsystem.h>  
#include <ShlObj.h>


int GLent;
int GLcent;
int GLcpg;
DWORD GLPOINTR;
DWORD PlayerBase;
int hp;
int m_flags;
int teamnum;
int GLteamnum;
bool GLdormant;

float R = 0.1f;
float G = 0.8f;
float B = 0.9f;
float R2 = 0.8f;
float G2 = 0.4f;
float B2 = 0.f;
float ALFA = 1.f;
float TALFA = 0.6f;
float HG = 0.4f;
float HR = 0.8f;

bool t = true;
bool f = false;

HANDLE handlProcess;
DWORD ProcessID;
DWORD dwClient;
DWORD dwEngine;
int iClientState;

DWORD FetchModBase(LPSTR lpModuleName, DWORD dwProcessId)
{
	MODULEENTRY32 lpModEntryPoint = { 0 };
	HANDLE handleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
	if (!handleSnap)	return NULL;
	lpModEntryPoint.dwSize = sizeof(lpModEntryPoint);
	BOOL bModule = Module32First(handleSnap, &lpModEntryPoint);
	while (bModule)
	{
		if (!strcmp(lpModEntryPoint.szModule, lpModuleName))
		{
			CloseHandle(handleSnap);
			return (DWORD)lpModEntryPoint.modBaseAddr;
		}
		bModule = Module32Next(handleSnap, &lpModEntryPoint);
	}
	CloseHandle(handleSnap);
	return NULL;
}

int main()
{
	PlaySoundA(Hitmarkers::zack, NULL, SND_ASYNC | SND_MEMORY);
	std::cout << "Super Simple external glow V4\n";
	std::cout << "Credits: VoidZero1337 @ https://github.com/voidzero-development\n";
	std::cout << "Please start CSGO!\n";

	HWND windowHandle = NULL;
	int retry = 0;

	do {
		windowHandle = FindWindowA(0, "Counter-Strike: Global Offensive");

		retry++;
		Sleep(10);
	} while (windowHandle == NULL);


	GetWindowThreadProcessId(windowHandle, &ProcessID);
	handlProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, ProcessID);

	Sleep(4000); //wait a few seconds until csgo loads client.dll

	dwClient = FetchModBase((LPSTR)"client.dll", ProcessID); //used to be client_panorama.dll

	std::cout << "Found Client! \n";

	PlaySoundA(Hitmarkers::injected, NULL, SND_ASYNC | SND_MEMORY);

	while (true) {
		for (int ent = 1; ent <= 64; ent++) {
			GLent = ent;
			ReadProcessMemory(handlProcess, (LPVOID)(dwClient + dwGlowObjectManager), &GLPOINTR, sizeof(GLPOINTR), NULL);
			ReadProcessMemory(handlProcess, (LPVOID)(dwClient + dwLocalPlayer), &PlayerBase, sizeof(PlayerBase), NULL);
			ReadProcessMemory(handlProcess, (LPVOID)(PlayerBase + m_iTeamNum), &teamnum, sizeof(teamnum), NULL);
			ReadProcessMemory(handlProcess, (LPVOID)(dwClient + dwEntityList + (GLent - 1) * 0x10), &GLcent, sizeof(GLcent), NULL);
			ReadProcessMemory(handlProcess, (LPVOID)(GLcent + m_iGlowIndex), &GLcpg, sizeof(GLcpg), NULL);
			ReadProcessMemory(handlProcess, (LPVOID)(GLcent + 0xED), &GLdormant, sizeof(GLdormant), NULL);
			ReadProcessMemory(handlProcess, (LPVOID)(GLcent + 0xF4), &GLteamnum, sizeof(GLteamnum), NULL);
			ReadProcessMemory(handlProcess, (LPVOID)(GLcent + 0x100), &hp, sizeof(hp), NULL);

			// Health based calculations
			HR = (255 - 2.55 * hp) / 255.f;
			HG = 2.55 * hp / 255.f;

			if (GLteamnum != teamnum) {
				WriteProcessMemory(handlProcess, (LPVOID)(GLPOINTR + ((GLcpg * 0x38) + 0x4)), &HR, sizeof(HR), NULL);
				WriteProcessMemory(handlProcess, (LPVOID)(GLPOINTR + ((GLcpg * 0x38) + 0x8)), &HG, sizeof(HG), NULL);
				WriteProcessMemory(handlProcess, (LPVOID)(GLPOINTR + ((GLcpg * 0x38) + 0xC)), &B2, sizeof(B2), NULL);
				WriteProcessMemory(handlProcess, (LPVOID)(GLPOINTR + ((GLcpg * 0x38) + 0x10)), &ALFA, sizeof(ALFA), NULL);
				WriteProcessMemory(handlProcess, (LPVOID)(GLPOINTR + ((GLcpg * 0x38) + 0x24)), &t, sizeof(t), NULL);
				WriteProcessMemory(handlProcess, (LPVOID)(GLPOINTR + ((GLcpg * 0x38) + 0x25)), &f, sizeof(f), NULL);
				WriteProcessMemory(handlProcess, (LPVOID)(GLPOINTR + ((GLcpg * 0x38) + 0x2C)), &f, sizeof(f), NULL);
			}
		}
		Sleep(1);
	}
	return 0;
}

