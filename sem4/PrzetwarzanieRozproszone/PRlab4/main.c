#include <windows.h>
#include <stdio.h>


void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# pragma argsused
struct dane_dla_watku // tablica zawiera dane , ktore otrzymaja watki
{
	char nazwa[50];
	int parametr;
} dane[5] = { { "[1]" , 2 } , { "[2]" , 3 } , { "[3]" , 4 } , { "[4]" , 5 } , { "[5]" , 6} };
// priorytety watkow
int priorytety[5] = { THREAD_PRIORITY_BELOW_NORMAL ,
THREAD_PRIORITY_NORMAL , THREAD_PRIORITY_ABOVE_NORMAL,
THREAD_PRIORITY_NORMAL , THREAD_PRIORITY_ABOVE_NORMAL
};
HANDLE Semaphore;
HANDLE watki[3]; // dojscia ( uchwyty ) watkow
// deklaracja funkcji watku
DWORD WINAPI funkcja_watku(void* argumenty);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int main(int argc, char** argv)
{
	int i;
	DWORD id; // identyfikator watku
	system("cls");
	printf(" Uruchomienie programu \ n ");
	Semaphore = CreateSemaphore(NULL, 1, 1, NULL);
	// tworzenie watkow
	for (i = 0; i < 5; i++)
	{
		watki[i] = CreateThread(
			NULL, // atrybuty bezpieczenstwa
			0, // inicjalna wielkosc stosu
			funkcja_watku, // funkcja watku
			(void*)&dane[i],// dane dla funkcji watku
			0, // flagi utworzenia
			&id);
		if (watki[i] != INVALID_HANDLE_VALUE)
		{
			//gotoxy(0,dane[i].parametr);
			//printf("%s %x ", dane[i].nazwa, id);
			
			// ustawienie priorytetu
			SetThreadPriority(watki[i], priorytety[i]);
		}
	}
	SetThreadPriority(watki[0], THREAD_PRIORITY_ABOVE_NORMAL);
	Sleep(1000);
	TerminateThread(watki[2], 0);
	Sleep(20000); // uspienie watku glownego na 20 s
	CloseHandle(Semaphore);
	return 0;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// trzy takie funkcje pracuja wspolbieznie w programie
DWORD WINAPI funkcja_watku(void* argumenty)
{
	unsigned int licznik = 0;
	// rzutowanie struktury na wlasny wskaznik
	struct dane_dla_watku* moje_dane = (struct dane_dla_watku*)argumenty;
	// wyswietlenie informacji o uruchomieniu
	//gotoxy(0, moje_dane->parametr);
	//printf("%s\n", moje_dane->nazwa);
	
	// praca , watki sa terminowane przez zakonczenie programu
// - funkcji main
	//while (1)
	//{
	//	gotoxy(licznik++ / 5000 + 10, moje_dane->parametr);
	//	printf(".");
	//}
	BOOL bcontinue = TRUE;
	DWORD wait_result;
	while (bcontinue) {
		wait_result = WaitForSingleObject(Semaphore, 0L);
		switch(wait_result) {
		case WAIT_OBJECT_0:
			gotoxy(10, moje_dane->parametr);
			printf("%s - start", moje_dane->nazwa);
			bcontinue = FALSE;
			Sleep(100);
			ReleaseSemaphore(Semaphore, 1, NULL);
			break;
		}
		
	}
	Sleep(5000);
	bcontinue = TRUE;
	while (bcontinue) {
		wait_result = WaitForSingleObject(Semaphore, 0L);
		switch (wait_result) {
		case WAIT_OBJECT_0:
			gotoxy(30, moje_dane->parametr);
			printf("%s - stop", moje_dane->nazwa);
			bcontinue = FALSE;
			ReleaseSemaphore(Semaphore, 1, NULL);
			break;
		}

	}
	
	return 0;
}
