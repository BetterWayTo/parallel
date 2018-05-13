#include <iostream>
#include <thread>
#include <chrono>

void hello()
{
	for (int i = 0; i < 10; ++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << "1";
	}
}

void hello2()
{
	for (int i = 0; i < 10; ++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		std::cout << "2";
	}
}

int main()
{
	
	std::thread t(hello);
	std::thread t2(hello2);
	t.join();
	t2.join();
}