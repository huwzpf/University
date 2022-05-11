#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>



#pragma comment(lib,"ws2_32.lib")

int main() {
	SOCKET s;
	struct sockaddr_in sa;
	WSADATA wsas;
	WORD wersja;
	wersja = MAKEWORD(2, 0);
	WSAStartup(wersja, &wsas);
	s = socket(AF_INET, SOCK_STREAM, 0);
	memset((void*)(&sa), 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(3000);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");
	int result;
	result = connect(s, (struct sockaddr FAR*) & sa, sizeof(sa));
	if (result == SOCKET_ERROR)
	{
		printf("Blad polaczenia!");
		return 0;
	}
	int dlug;
	char buf[80];
	for (int i = 0;; i++)
	{
		fgets(buf, 80, stdin);
		dlug = strlen(buf); buf[dlug-1] = '\0';
		send(s, buf, dlug+1, 0);
		if (strcmp(buf, "KONIEC") == 0) break;
	}
	closesocket(s);
	WSACleanup();
	return 0;
}

#ifdef _WIN32
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	return main();
}
#endif
