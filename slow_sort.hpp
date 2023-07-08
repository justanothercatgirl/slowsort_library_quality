#ifndef SLOW_SORT_JUSTANOTHERCATGIRL
#define SLOW_SORT_JUSTANOTHERCATGIRL

#include <concepts>
#include <iostream>
#include <iterator>
#include <vector>
#include <utility>

namespace slow_sort 
{

//for print function
template <typename container>
concept iterable = requires(container c)
{
	c.begin();
	c.end(); 
};

//checks whether comp function passed to sorting is valid
template <typename func, typename iter>
concept suiting_compare =
std::invocable<func,
	typename std::iterator_traits<iter>::value_type,
	typename std::iterator_traits<iter>::value_type>;

//print with range-based loop
template <iterable cont>
void print(const cont& c)
{
	for (const auto& x : c) std::cout << x << '\n';
	std::cout.put('\n');
}

//print with iterators
template <typename iter_type>
void print(iter_type b, iter_type e)
{
	for (; b <= e; ++b) std::cout << *b << '\n';
	std::cout.put('\n');
}

//the slow sort algorithm
template <std::random_access_iterator RAIter, typename lambda>
void slow_sort(RAIter begin, RAIter end, lambda comp)
	requires suiting_compare<lambda, RAIter>
{
	if (begin >= end) return;

	auto middle = begin + (std::distance(begin, end) / 2);
	slow_sort(begin, middle, comp);
	slow_sort(middle + 1, end, comp);

	if (comp(*end, *middle))
		std::iter_swap(middle, end);

	slow_sort(begin, end - 1, comp);

}

//sorting algorithm with default comparison
template <std::random_access_iterator RAIter>
void slow_sort(RAIter begin, RAIter end)
{
	using value_type = typename std::iterator_traits<RAIter>::value_type;
	slow_sort(begin, end, [](value_type a, value_type b) -> bool {return a < b; });
}

} //slow_sort

#endif //SLOW_SORT_JUSTANOTHERCATGIRL