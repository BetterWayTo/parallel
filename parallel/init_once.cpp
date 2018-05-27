/*Init only once*/

#include <iostream>
#include <thread>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <mutex>
#include <string>

class Connection
{
public:
	Connection(std::string details) : m_Details(details)
	{}

	void SendData(const std::string& data)
	{
		std::call_once(m_ConnectionInitFlag,
			&Connection::OpenConnection, this);
		std::cout << data.c_str() << std::endl;
	}

	void RecieveData()
	{
		const std::string data = "321";
		std::call_once(m_ConnectionInitFlag,
			&Connection::OpenConnection, this);
		std::cout << data.c_str() << std::endl;
	}
private:
	void OpenConnection()
	{
		std::cout << "Connection openned" << std::endl;
	}

	std::string m_Details;
	std::once_flag m_ConnectionInitFlag;
};


int main()
{
	const int count = 100;
	int cores = 4;

	srand(time(0));

	Connection connection("1");
	std::vector<std::thread> threads;
	for (int i = 0; i < count; ++i)
	{
		int random = rand() % 2;
		if (random == 0)
		{
			threads.push_back(std::thread(&Connection::SendData, &connection, "123"));
		}
		else
		{
			threads.push_back(std::thread(&Connection::RecieveData, &connection));
		}

	}
	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

}