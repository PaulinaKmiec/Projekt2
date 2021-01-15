#include <iostream>
#include <fstream>
#include <time.h>
#include <chrono>
using namespace std;

int  N = 2000;		      //liczby bêd¹ generowane z zakresu 0 - N-1
int  ROZMIAR = 100000;    //rozmiar sortowanej tablicy

int tabN[6] = { 1000 , 10000, 50000, 100000, 500000, 5000000 };

void wygenerujLiczbyDoPliku()
{
	int liczba;
	ofstream plik("liczbyLosowe.txt");

	//w pêtli generujê liczby losowe
	for (int i = 0; i < ROZMIAR; i++)
	{
		liczba = rand() % N;
		//zapisujê do pliku liczby oddzielone spacj¹
		plik << liczba << " ";
	}

	//zamykam plik
	plik.close();
}

void zapiszTabliceDoPliku(int* tablica)
{
	ofstream plik("posortowane.txt");

	for (int i = 0; i < ROZMIAR; i++)
	{
		plik << tablica[i] << " ";
	}

	plik.close();
}

void wczytajTablice(int* tablica)
{
	ifstream plik("liczbyLosowe.txt");

	//w pêtli zczytujê kolejne elementy tablicy
	for (int i = 0; i < ROZMIAR; i++)
	{
		plik >> tablica[i];
	}

	//zamykam plik
	plik.close();
}

void sortowaniePrzezWstawianie(int* tablica)
{
	for (int i = 1; i < ROZMIAR; i++)
	{
		int klucz = tablica[i];
		int j = i - 1;

		while (j >= 0 && tablica[j] > klucz)
		{
			tablica[j + 1] = tablica[j];
			j--;
		}
		tablica[j + 1] = klucz;
	}
}

void sortowaniePrzezZliczanie(int* tablica)
{
	//alokuje pamiêæ do tablicy pomocniczej
	int* posortowana = new int[ROZMIAR];

	//alokuje miejsce na tablice przechowujaca zliczenia poszczególnych liczb
	int* liczniki = new int[N+1];
	for (int i = 0; i < N; i++)
		liczniki[i] = 0;

	//zliczam wyst¹pienia liczb w tablicy 'liczniki'
	for (int i = 0; i < ROZMIAR; i++)
		liczniki[tablica[i]]++;

	//obliczamy pozycje koñcowe elementów
	for (int i = 1; i <= N; ++i)
		liczniki[i] += liczniki[i - 1];

	//przepisujê elementy do tablicy wynikowej
	for (int i = 0; tablica[i]; i++)
	{
		int index = liczniki[tablica[i]] - 1;
		posortowana[index] = tablica[i];
		liczniki[tablica[i]]--;
	}

	for (int i = 0; i < ROZMIAR; i++)
		tablica[i] = posortowana[i];

	//zwalniam dynamicznie zaalokowan¹ pamiêæ
	delete[] posortowana;
	delete[] liczniki;
}

//funkcja pomocnicza do wypisywania zawartoœci tablicy
void wypiszTablice(int* tablica)
{
	for (int i = 0; i < ROZMIAR; i++)
	{
		cout << tablica[i] << endl;
	}
}

void testDaneLosowe(int *tablica)
{
	cout << "DANE LOSOWE\n";

	for (int i = 0; i < 6; i++)
	{
		N = tabN[i];

		//zliczam czas sortowanie przez zliczanie
		wygenerujLiczbyDoPliku();
		wczytajTablice(tablica);
		auto start = chrono::high_resolution_clock::now();
		sortowaniePrzezZliczanie(tablica);
		auto stop = chrono::high_resolution_clock::now();
		auto czasSortowaniePrzezZliczanie = chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

		//zliczam czas sortowania przez wstawianie
		wygenerujLiczbyDoPliku();
		wczytajTablice(tablica);
		start = chrono::high_resolution_clock::now();
		sortowaniePrzezWstawianie(tablica);
		stop = chrono::high_resolution_clock::now();
		auto czasSortowaniePrzezWstawianie = chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

		cout << "N = " << N << "S. przez zliczanie " << czasSortowaniePrzezZliczanie << "ms S. przez wstawianie " << czasSortowaniePrzezWstawianie << "ms\n";
	}
}

void testDanePosortowane(int* tablica)
{
	cout << "\n\nDANE POSORTOWANE\n";

	for (int i = 0; i < 6; i++)
	{
		N = tabN[i];

		wygenerujLiczbyDoPliku();
		wczytajTablice(tablica);
		sortowaniePrzezWstawianie(tablica);

		//zliczam czas sortowanie przez zliczanie
		auto start = chrono::high_resolution_clock::now();
		sortowaniePrzezZliczanie(tablica);
		auto stop = chrono::high_resolution_clock::now();
		auto czasSortowaniePrzezZliczanie = chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

		//zliczam czas sortowania przez wstawianie
		start = chrono::high_resolution_clock::now();
		sortowaniePrzezWstawianie(tablica);
		stop = chrono::high_resolution_clock::now();
		auto czasSortowaniePrzezWstawianie = chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

		cout << "N = " << N << "S. przez zliczanie " << czasSortowaniePrzezZliczanie << "ms S. przez wstawianie " << czasSortowaniePrzezWstawianie << "ms\n";
	}
}

void sortujOdwrotnie(int* tablica)
{
	for (int  i = 0; i < ROZMIAR; i++)
		for (int j = 0; j < ROZMIAR; j++)
		{
			if (tablica[i] > tablica[j])
				swap(tablica[i], tablica[j]);
		}
}

void testDanePosortowaneOdwrotnie(int* tablica)
{
	cout << "\n\nDANE POSORTOWANE ODWROTNIE\n";

	for (int i = 0; i < 6; i++)
	{
		N = tabN[i];

		wygenerujLiczbyDoPliku();
		wczytajTablice(tablica);

		sortujOdwrotnie(tablica);
		//zliczam czas sortowanie przez zliczanie
		auto start = chrono::high_resolution_clock::now();
		sortowaniePrzezZliczanie(tablica);
		auto stop = chrono::high_resolution_clock::now();
		auto czasSortowaniePrzezZliczanie = chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

		sortujOdwrotnie(tablica);
		//zliczam czas sortowania przez wstawianie
		start = chrono::high_resolution_clock::now();
		sortowaniePrzezWstawianie(tablica);
		stop = chrono::high_resolution_clock::now();
		auto czasSortowaniePrzezWstawianie = chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

		cout << "N = " << N << "S. przez zliczanie " << czasSortowaniePrzezZliczanie << "ms S. przez wstawianie " << czasSortowaniePrzezWstawianie << "ms\n";
	}
}
int main()
{


	//alokuje dynamicznie pamiêæ na tablicê
	int* tablica = new int[ROZMIAR];

	wczytajTablice(tablica);

	//wypiszTablice(tablica);

	//zapiszTabliceDoPliku(tablica);

	testDaneLosowe(tablica);
	//testDanePosortowane(tablica);
	//testDanePosortowaneOdwrotnie(tablica);
	return 0;
}
