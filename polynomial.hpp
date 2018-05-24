#pragma once
#include <vector>
#include "term.hpp"

class polynomial
{
private:
	std::vector<term> terms;
public:
	polynomial();
	polynomial(const std::vector<term> &terms);
	polynomial(const std::vector<int32_t> &mask);
	~polynomial();

	const polynomial operator+(const polynomial &p);
	const polynomial operator-(const polynomial &p);
	const polynomial operator*(const polynomial &p);
	const polynomial operator/(const polynomial &p);
	polynomial &operator+=(const polynomial &p);
	polynomial &operator-=(const polynomial &p);
	polynomial &operator*=(const polynomial &p);
	polynomial &operator/=(const polynomial &p);
	bool operator==(const polynomial &p) const;
	bool operator!=(const polynomial &p) const;
	bool operator>(const polynomial &p) const;
	bool operator<(const polynomial &p) const;
	bool operator>=(const polynomial &p) const;
	bool operator<=(const polynomial &p) const;

	void insert_term(const term &t);
	void insert_term(int32_t coefficient, int32_t power);

	void subtract_term(const term &t);
	void subtract_term(int32_t coefficient, int32_t power);
	
	bool insert_mask(const std::vector<int32_t> &mask);

	const std::vector<term> get_terms() const;
	const term at(size_t n) const;

	const std::vector<int32_t> get_mask();
	const int32_t get_degree() const;
	const term get_greatest_term() const;

	void reorganize();

	static const polynomial add(const polynomial &p1, const polynomial &p2);
	static const polynomial subtract(const polynomial &p1, const polynomial &p2);
	static const polynomial multiply(const polynomial &p1, const polynomial &p2);
	//EXPIREMENTAL FUNCTION
	static const polynomial fft(const polynomial & p1);
	static const polynomial foil(const polynomial &p1, const polynomial &p2);
	static const polynomial divide(const polynomial &p1, const polynomial &p2, std::pair<polynomial, polynomial> *remainder = nullptr);
};

