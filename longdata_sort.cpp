#include <iostream>
#include <thread>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>

void LongFunction(int *mas, int start, int end)
{		
	for (int i = start; i < end; ++i)
	{
		for (int j = start; j < end - 1; ++j)
		{
			if (mas[j+1] < mas[j])
			{
				int temp = mas[j+1];
				mas[j+1] = mas[j];
				mas[j] = temp;
			}
		}
	}
}

int main()
{
	const int count = 100000;
	int cores = 4;
	int mas[count];
	srand(time(0));
	for (int i = 0; i < count; ++i)
	{
		mas[i] = rand() % count;
	}
	int start = 0;
	int end = count / cores;
	int step = end;
	
	std::vector<std::thread> threads;
	for (int i = 0; i < 4; ++i)
	{		
		std::cout << start << " " << end << std::endl;
		threads.push_back(std::thread(LongFunction, mas, start, end));
		start = end;
		end += step;		
	}
	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	
}