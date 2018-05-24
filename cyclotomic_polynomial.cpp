#include <cstdint>
#include <vector>
#include <sstream>
#include <iostream>
#include <functional>
#include <numeric>
#include <ctime>
#include <unordered_map>
#include "polynomial.hpp"


const std::vector<int32_t> cyclotomic_polynomial(uint32_t N)
{
	static std::unordered_map<uint32_t, std::vector<int32_t>> cache;

	std::function<bool(int32_t)> prime = [](int32_t p) -> bool
	{
		if (p <= 1)
		{
			return false;
		}

		if (p == 2)
		{
			return true;
		}

		int32_t m = static_cast<int32_t>(sqrt(p));
		for (int32_t i = 2; i <= m; i++)
		{
			if (p % i == 0)
			{
				return false;
			}
		}

		return true;
	};

	std::function<bool(int32_t, int32_t)> divisible = [](int32_t n, int32_t m) -> bool
	{
		return n % m == 0;
	};

	std::function<bool(int32_t)> odd = [](int32_t n) -> bool
	{
		return n % 2 != 0;
	};

	std::function<bool(uint32_t, int32_t)> power_of = [](uint32_t x, int32_t p) -> bool
	{
		while (((x % p) == 0) && x > 1)
		{
			x /= p;
		}
		return x == 1;
	};

	
	if (N == 1)
	{
		//v = polynomial(std::vector<term>({ term(1, 1), term(-1, 0) })).get_mask();
		return std::vector<int32_t>({ 1, -1 });
	}

	std::vector<int32_t> v;
	if (prime(N))
	{
		v.resize(N, 1);
	}
	else if (divisible(N, 2) && prime(N / 2) && odd(N / 2))
	{
		int32_t n = N / 2;
		v.reserve(n);
		for (int32_t i = 0; i < n; ++i)
		{
			v.push_back((!(i % 2)) ? 1 : -1);
		}
	}
	else if (N > 1 && power_of(N, 2))
	{
		v.resize(N / 2 + 1);

		v.at(0) = 1;
		v.at(v.size() - 1) = 1;
	}
	else if ((divisible(N, 12) && power_of(N / 12, 2))
		|| divisible(N, 18) && power_of(N / 18, 2))
	{
		v.resize(N / 3 + 1);

		v.at(0) = 1;
		v.at(v.size() / 2) = -1;
		v.at(v.size() - 1) = 1;
	}
	else if (std::gcd(N, 9) == 9 && power_of(N, 3))
	{
		v.resize(static_cast<int32_t>(N / 1.5) + 1);

		v.at(0) = 1;
		v.at(v.size() / 2) = 1;
		v.at(v.size() - 1) = 1;
	}
	else if (cache.count(N) > 0)
	{
		std::cout << "CACHE USED: " << N << std::endl;
		return cache.at(N);
	}
	else
	{

		polynomial p1(std::vector<term>({ term(1, N), term(-1, 0) }));
		polynomial p2(std::vector<term>({ term(1, 0) }));

		//factors of N
		std::vector<uint32_t> p2v = [&](uint32_t N) -> const std::vector<uint32_t>
		{
			std::vector<uint32_t> v;
			uint32_t n = N / 2;
			for (uint32_t i = 1; i <= n; ++i)
			{
				if (divisible(N, i))
				{
					v.push_back(i);
				}
			}
			return v;
		}(N);

		for (uint32_t n : p2v)
		{
			//TODO: can optimize by creating a std::map for our cyclotomic polynomials so there will be no reason to
			//recursively call the cyclotomic_polynomial function
			p2 *= polynomial(cyclotomic_polynomial(n));
		}

		v = polynomial(p1 / p2).get_mask();

		cache.emplace(N, v);
		std::cout << N << " CALCULATED USING RECURSIVE METHOD." << std::endl;
	}

	return v;
}

std::ostream& operator<<(std::ostream &os, const std::vector<int32_t> &v)
{
	std::stringstream ss;
	if (v.size() > 0)
	{
		ss << "[";

		for (size_t n = 0; n < v.size(); ++n)
		{
			ss << v.at(n);
			if (v.size() - 1 == n)
			{
				ss << "]";
			}
			else
			{
				ss << ", ";
			}
		}

		return os << ss.str() + "\r\n";
	}

	return os;
}

int main()
{
	/*clock_t begin = clock();

	for (int32_t i = 1; i <= 305; ++i)
	{
		std::cout << i << ": " << cyclotomic_polynomial(i);
	}

	clock_t end = clock();
	
	std::cout << "Elapsed: " <<  double(end - begin) / CLOCKS_PER_SEC << std::endl;
	*/

	polynomial p1(std::vector<int32_t>({ 0, 0, 1, 3 }));
	polynomial p2(std::vector<int32_t>({ 0, 2, 0, 2 }));

	auto result1 = polynomial::foil(p1, p2);
	auto result2 = polynomial::multiply(p1, p2);

	std::cout << result1.get_mask() << std::endl << result2.get_mask();

	getchar();
	return 0;
}