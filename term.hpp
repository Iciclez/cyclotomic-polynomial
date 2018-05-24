#pragma once
#include <cstdint>
#include <tuple>

class term
{
private:
	std::pair<int32_t, int32_t> term_pair;
public:
	term(const std::pair<int32_t, int32_t> &term_pair);
	term(int32_t coefficient, int32_t power);
	~term();

	const term operator+(const term &t);
	const term operator-(const term &t);
	const term operator*(const term &t);
	const term operator/(const term &t);
	term &operator+=(const term &t);
	term &operator-=(const term &t);
	term &operator*=(const term &t);
	term &operator/=(const term &t);

	bool add(const term &t);
	bool subtract(const term &t);
	bool multiply(const term &t);
	bool divide(const term &t);

	int32_t get_coefficient() const;
	int32_t get_power() const;

	static const std::pair<bool, term> add(const term &t1, const term &t2);
	static const std::pair<bool, term> subtract(const term &t1, const term &t2);
	static const term multiply(const term &t1, const term &t2);
	static const term divide(const term &t1, const term &t2);
};

