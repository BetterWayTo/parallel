#include <exception>
#include <thread> 
#include <memory>
#include <mutex>
#include <stack> 
#include <iostream>   

#include <time.h>
#include <stdlib.h>
 
void sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

struct empty_stack: std::exception
{
	const char *what() const throw()
	{
		return "asd";
	}
}; 

template<typename T>
class threadsafe_stack
{
private:
	std::stack<T> data;
	mutable std::mutex m;
public:
	threadsafe_stack() {}
	threadsafe_stack(const threadsafe_stack& other)
	{
		std::lock_guard<std::mutex> lock(other.m);
		data = other.data;
	}
	threadsafe_stack& operator =(const threadsafe_stack&) = delete;
	
	void push(T new_value)
	{
		std::lock_guard<std::mutex> lock(m);
		data.push(new_value);
	}
	
	std::shared_ptr<T> pop()
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.empty()) throw empty_stack();
		std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
		data.pop();
		return res;
	}
	
	void pop(T& value)
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.empty()) throw empty_stack();
		value = data.top();
		data.pop();
	}
	
	bool empty() const
	{
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}
};



template<typename T>
class Prov
{
private:
	static std::mutex m;
	static std::mutex cout;
	static int num;
public:
	static void Func(threadsafe_stack<T>& stack)
	{
		int count = 10;
		while(count-- > 0)
		{
			int rnd = rand() % 2;
			if (rnd == 0)
			{
				std::lock_guard<std::mutex> lock(m);
				stack.push(num++);
				{
					std::lock_guard<std::mutex> lock(cout);
					std::cout << "add " << num-1 << std::endl;
				}
			}
			else if (rnd == 1)
			{
				if (!stack.empty())
				{
					int res;
					stack.pop(res);
					{
						std::lock_guard<std::mutex> lock(cout);
						std::cout << "del " << res << std::endl;			
					}
				}
			}
		}
	}
	
};
template<typename T> std::mutex Prov<T>::m;
template<typename T> std::mutex Prov<T>::cout;
template<typename T> int Prov<T>::num = 0;

int main()
{
    threadsafe_stack<int> stack = threadsafe_stack<int>();
	
	std::thread first (Prov<int>::Func, std::ref(stack)); 
	std::thread second (Prov<int>::Func, std::ref(stack)); 
	std::thread third (Prov<int>::Func, std::ref(stack)); 
	first.join();                
	second.join(); 
	third.join(); 
	std::cout << std::endl;
	while (!stack.empty())
	{
		int res;
		stack.pop(res);
		std::cout << res << " ";
	}
		
	return 0;
}