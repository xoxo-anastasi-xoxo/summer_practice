#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

// Учебная практика 2017
// Казанцева Анастасия Романовна
// БПИ162
// VS

// Вспомогательные переменные для сортировок
int i = 0;
int j = 0;
// Для сортировки Пузырьком с условием Айверсона 1
bool flag = false;
// Для сортировки Пузырьком с условием Айверсона 1+2 и сортировки Вставками
int bound = 0;
int t = 0;
// Для сортировки Бинарными вставками
int l = 0;
int r = 0;
int c = 0;
int k = 0;
int p = 0;
// Для сортировки Подсчетом
int min = 0;
int max = 0;
int tempLegth = 0;
// Для Цифровой сортировки
int base = 256;
int exponent = 0;
int sum = 0;
union un
{
	unsigned a;
	unsigned char ch[4];
};

// Объявление функций сортировок
void doBubbleSort(int*, int);

void doBubbleSortWithIverson1(int*, int);

void doBubbleSortWithIverson2(int*, int);

void doInsertionSort(int*, int);

void doBinaryInsertionSort(int*, int);

void doCountingSort(int*, int);

void doRadixSort(int*, int);

// Объявление вспомогательных функций

void createCopyArray(int*, int*, int);

int main()
{


	// Создание массива функций
	void(*sort[7])(int*, int) =
	{ doBubbleSort,
	 doBubbleSortWithIverson1,
	 doBubbleSortWithIverson2,
	 doInsertionSort,
	 doBinaryInsertionSort,
	 doRadixSort,
	 doCountingSort };

	// Создание числовых массивов
	int** arrays = new int*[4];

	for (i = 0; i < 4; i++)
	{
		arrays[i] = new int[9000];
	}
	srand((unsigned)time(0));
	for (i = 0; i < 9000; i++)
	{
		arrays[0][i] = rand() % 8;
		arrays[1][i] = rand();
		arrays[2][i] = i;
		arrays[3][i] = 9000 - i;
	}
	for (j = 0; j < 5; j++)
	{
		int pos1 = rand() % 9000;
		int pos2 = rand() % 9000;
		swap(arrays[2][pos1], arrays[2][pos2]);
	}

	// Подготовка конечного файла
	fstream file;
	file.open("Results.csv", ios::out);
	file << ";BubbleSort;;;;;BubbleSortWithIverson1;;;;;BubbleSortWithIverson2;;;;;InsertionSort;;;;;BinaryInsertionSort;;;;;RadixSort;;;;;CountingSort;;;;;" << endl;
	for (i = 1; i <= 7; ++i)
	{
		file << ";Numbers between the range 0-7;Numbers between the range 0-max;Almost sorted array;Inverse array;";
	}
	file << endl;

	// Последовательный запуск сортировок
	unsigned long long StartTime4, EndTime4;
	for (int arrayLength = 1000; arrayLength <= 9000; arrayLength += 1000)
	{
		file << arrayLength << ";";
		for (int sortType = 0; sortType < 7; ++sortType)
		{
			for (int arrayType = 0; arrayType < 4; ++arrayType)
			{
				// Исключение из рассмотрения сортировки Подсчетом для массива второго типа
				if (!((sortType == 6) && (arrayType == 1)))
				{
					unsigned long long average = 0;
					int counter = 0;
					for (counter = 0; counter < 100; ++counter)
					{

						int* currentArray = new int[arrayLength];
						createCopyArray(arrays[arrayType], currentArray, arrayLength);

						// Помечаем, что здесь начинается ассемблерная вставка
						__asm
						{
							RDTSC // Снятие показаний счетчика тактов в регистры EDX:EAX(это 64-битное число)
							mov DWORD PTR StartTime4, eax // Передача данных из регистра EAX в младшие? 32 бита, выделенные от общей длинны числа StartTime4
							mov DWORD PTR StartTime4 + 4, edx // Передача данных из регистра EDX в старшие? 32 бита, выделенные от общей длинны числа StartTime4
						}

						sort[sortType](currentArray, arrayLength);
						// Помечаем, что здесь начинается ассемблерная вставка
						__asm
						{
							RDTSC // Снятие показаний счетчика тактов в регистры EDX:EAX(это 64-битное число)
							mov DWORD PTR EndTime4, eax // Передача данных из регистра EAX в младшие? 32 бита, выделенные от общей длинны числа EndTime4
							mov DWORD PTR EndTime4 + 4, edx // Передача данных из регистра EAX в старшие? 32 бита, выделенные от общей длинны числа EndTime4
						}
						average += (EndTime4 - StartTime4);
						delete[] currentArray;
					}
					average /= counter;
					file << average;
				}
				file << ";";
			}
			file << ";";
		}
		file << endl;
	}

	// Очистка памяти
	for (int i = 0; i < 4; i++)
	{
		delete[] arrays[i];
	}
	delete[] arrays;
	file.close();
	return 0;
}

// Определение дополнительных функций
int getMin(int* arr, int n)
{
	int min = INT_MAX;

	for (int i = 0; i < n; i++)
	{
		if (arr[i] < min)
		{
			min = arr[i];
		}
	}

	return min;
}

int getMax(int* arr, int n)
{
	int max = INT_MIN;

	for (int i = 0; i < n; i++)
	{
		if (arr[i] > max)
		{
			max = arr[i];
		}
	}

	return max;
}

void createCopyArray(int* arr, int* copyArr, int n)
{
	for (int i = 0; i < n; ++i)
		copyArr[i] = arr[i];
}

// Определение функций сортировок
void doBubbleSort(int* arr, int n)
{
	for (i = 0; i < n - 1; ++i)
	{
		for (j = 0; j < n - i - 1; ++j)
		{
			if (arr[j] > arr[j + 1])
			{
				swap(arr[j], arr[j + 1]);
			}
		}
	}
}

void doBubbleSortWithIverson1(int* arr, int n)
{
	flag = false;
	for (i = 0; ((i < n - 1) && !(flag)); ++i)
	{
		flag = true;
		for (j = 0; j < n - i - 1; ++j)
		{
			if (arr[j] > arr[j + 1])
			{
				swap(arr[j], arr[j + 1]);
				flag = false;
			}
		}
	}
}

void doBubbleSortWithIverson2(int* arr, int n)
{
	bound = n - 1;
	for (i = 0; i < n - 1; ++i)
	{
		t = 0;
		for (int j = 0; j < bound; ++j)
		{

			if (arr[j] > arr[j + 1])
			{
				swap(arr[j], arr[j + 1]);
				t = j;
			}
		}
		if (t == 0) break;
		bound = t;
	}
}

void doInsertionSort(int* arr, int n)
{
	for (i = 1; i < n; ++i)
	{
		t = arr[i];
		j = i - 1;
		while ((t < arr[j]) && (j >= 0))
		{
			arr[j + 1] = arr[j];
			--j;
		}
		arr[j + 1] = t;
	}
}

void doBinaryInsertionSort(int* arr, int n)
{

	for (int i = 1; i < n; ++i)
	{
		l = 0;
		r = i - 1;
		c = (l + r) / 2;
		while (l != c)
		{
			if (arr[c] > arr[i])
				r = c - 1;
			else
				l = c;
			c = (l + r) / 2;
		}
		if (arr[l] < arr[i])
			if (arr[i] > arr[r])
				l = r + 1;
			else
				l = r;

		k = i;
		p = arr[i];

		while (k >= l)
		{
			arr[k] = arr[k - 1];
			--k;
		}
		arr[l] = p;
	}
}

void doCountingSort(int* arr, int n)
{
	min = getMin(arr, n);
	max = getMax(arr, n);
	tempLegth = max - min + 1;
	int *temp = new int[tempLegth];

	for (i = 0; i < tempLegth; ++i)
	{
		temp[i] = 0;
	}

	for (int i = 0; i < n; ++i)
	{
		++temp[arr[i] - min];
	}

	j = 0;

	for (int i = min; i <= max; ++i)
	{
		while (temp[i - min])
		{
			arr[j++] = i;
			--temp[i - min];
		}
	}
	delete[] temp;
}

void doRadixSort(int* arr, int n)
{
	exponent = 0;
	un* temp = new un[n];
	int* cnt;
	while (exponent < 4)
	{
		for (i = 0; i < n; ++i)
		{
			temp[i].a = (unsigned)arr[i];
		}
		cnt = new int[base];
		for (i = 0; i < base; ++i)
		{
			cnt[i] = 0;
		}

		for (i = 0; i < n; ++i)
		{
			++cnt[temp[i].ch[exponent]];
		}

		sum = 0;

		for (i = 0; i < base; ++i)
		{
			j = cnt[i];
			cnt[i] = sum;
			sum += j;
		}

		for (i = 0; i < n; ++i)
		{
			int tempIndex = temp[i].ch[exponent];
			arr[cnt[tempIndex]] = temp[i].a;
			cnt[tempIndex]++;
		}

		exponent++;

	}
	delete temp;
	delete[] cnt;
}