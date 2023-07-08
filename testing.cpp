#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <ratio>
#include <thread>
#include <vector>

#include "slow_sort.hpp"

template <typename container, typename ratio = std::milli, typename sorting_function>
void time_test(sorting_function sort, int amount = 10000)
{

	using lim = std::numeric_limits<int>;
	std::mt19937 engine;
	std::uniform_int_distribution rand(lim::min(), lim::max());

	using namespace std::chrono;
	duration<long long, ratio>
		time_taken_monotonus{ 0 },
		time_taken_growing{ 0 },
		total_time_monotonus{ 0 },
		total_time_growing{ 0 };
	time_point<high_resolution_clock> 
		monotonus_begin, monotonus_end,
		growing_begin, growing_end,
		start_cycle, end_cycle;

	monotonus_begin = high_resolution_clock::now();
	for (int i = 0; i < amount; ++i)
	{
		container c(100);
		for (auto& x : c) x = rand(engine);

		start_cycle = high_resolution_clock::now();
		sort(c.begin(), c.end() - 1);
		end_cycle = high_resolution_clock::now();
		time_taken_monotonus += duration_cast<duration<long long, ratio>>(end_cycle - start_cycle);
	}
	monotonus_end = high_resolution_clock::now();

	growing_begin = high_resolution_clock::now();
	for (int i = 0; i < amount; ++i)
	{
		container c(i / 2 + 10);
		for (auto& x : c) x = rand(engine);

		start_cycle = high_resolution_clock::now();
		sort(c.begin(), c.end() - 1);
		end_cycle = high_resolution_clock::now();
		time_taken_growing += duration_cast<duration<long long, ratio>>(end_cycle - start_cycle);
	}
	growing_end = high_resolution_clock::now();


	total_time_monotonus = duration_cast<duration<long long, ratio>>(monotonus_end - monotonus_begin);
	total_time_growing= duration_cast<duration<long long, ratio>>(growing_end - growing_begin);

	std::cout << "total time taken by testing function: "
		<< (total_time_monotonus + total_time_growing).count()
		<< "tp;\ntime taken by monotonus test: " << total_time_monotonus.count()
		<< "tp, time excluding generating: " << time_taken_monotonus.count() 
		<< "tp;\ntime taken by growing test: " << total_time_growing.count() 
		<< "tp, excluding generation: " << time_taken_growing.count()
		<< "\naverage monotonus time on " << amount << " tests: " 
		<< time_taken_monotonus.count() / amount << "tp" 
		<< std::endl;
}

int main()
{
	std::cout << __FUNCSIG__ << std::endl;
	time_test<std::vector<int>, std::micro>(std::sort<typename std::vector<int>::iterator>, 10000);
	time_test<std::vector<int>, std::milli>(slow_sort::slow_sort<typename std::vector<int>::iterator>, 10);


	return 0;
}