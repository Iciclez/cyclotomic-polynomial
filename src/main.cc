#include "cyclotomic_polynomial.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>


std::ostream& operator<<(std::ostream& os, const std::vector<int32_t>& v)
{
	std::stringstream ss;
	if (v.size() > 0)
	{
		ss << '[';

		for (size_t n = 0; n < v.size(); ++n)
		{
			ss << v.at(n);
			if (v.size() - 1 == n)
			{
				ss << ']';
			}
			else
			{
				ss << ", ";
			}
		}

		return os << ss.str();
	}

	return os;
}

int main()
{
	auto start = std::chrono::high_resolution_clock::now();

	for (int32_t i = 1; i <= 500; ++i)
	{
		std::cout << i << ": " << cyclotomic_polynomial(i) << '\n';
	}

	auto end = std::chrono::high_resolution_clock::now();

	std::cout << " -> " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

	return 0;
}