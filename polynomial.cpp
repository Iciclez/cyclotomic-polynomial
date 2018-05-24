#include "polynomial.hpp"
#include <algorithm>
#include <complex>
#include <iostream>

polynomial::polynomial()
{
}

polynomial::polynomial(const std::vector<term> &terms)
{
	this->terms = terms;
}

polynomial::polynomial(const std::vector<int32_t> &mask)
{
	this->insert_mask(mask);
}

polynomial::~polynomial()
{
}

const polynomial polynomial::operator+(const polynomial & p)
{
	return polynomial::add(*this, p);
}

const polynomial polynomial::operator-(const polynomial & p)
{
	return polynomial::subtract(*this, p);
}

const polynomial polynomial::operator*(const polynomial & p)
{
	return polynomial::multiply(*this, p);
}

const polynomial polynomial::operator/(const polynomial & p)
{
	return polynomial::divide(*this, p);
}

polynomial & polynomial::operator+=(const polynomial & p)
{
	*this = operator+(p);
	return *this;
}

polynomial & polynomial::operator-=(const polynomial & p)
{
	*this = operator-(p);
	return *this;
}

polynomial & polynomial::operator*=(const polynomial & p)
{
	*this = operator*(p);
	return *this;
}

polynomial & polynomial::operator/=(const polynomial & p)
{
	*this = operator/(p);
	return *this;
}

bool polynomial::operator==(const polynomial & p) const
{
	const term t1 = this->get_greatest_term();
	const term t2 = p.get_greatest_term();

	return t1.get_power() == t2.get_power() && t1.get_coefficient() == t2.get_coefficient();
}

bool polynomial::operator!=(const polynomial & p) const
{
	return !operator==(p);
}

bool polynomial::operator>(const polynomial & p) const
{
	const term t1 = this->get_greatest_term();
	const term t2 = p.get_greatest_term();

	return t1.get_power() > t2.get_power() || t1.get_coefficient() > t2.get_coefficient();
}

bool polynomial::operator<(const polynomial & p) const
{
	const term t1 = this->get_greatest_term();
	const term t2 = p.get_greatest_term();

	return t1.get_power() < t2.get_power() || t1.get_coefficient() < t2.get_coefficient();
}

bool polynomial::operator>=(const polynomial & p) const
{
	return operator>(p) || operator==(p);
}

bool polynomial::operator<=(const polynomial & p) const
{
	return operator<(p) || operator==(p);
}

void polynomial::insert_term(const term &t)
{
	bool b = false;
	for (std::vector<term>::iterator it = terms.begin(); it != terms.end(); ++it)
	{
		if ((*it).get_power() == t.get_power())
		{
			b = (*it).add(t);
			break;
		}
	}

	if (!b)
	{
		this->terms.push_back(t);
	}
}

void polynomial::insert_term(int32_t coefficient, int32_t power)
{
	this->insert_term(term(coefficient, power));
}

void polynomial::subtract_term(const term & t)
{
	bool b = false;
	for (std::vector<term>::iterator it = terms.begin(); it != terms.end(); ++it)
	{
		if ((*it).get_power() == t.get_power())
		{
			b = (*it).subtract(t);
			break;
		}
	}

	if (!b)
	{
		this->terms.push_back(term(-t.get_coefficient(), t.get_power()));
	}
}

void polynomial::subtract_term(int32_t coefficient, int32_t power)
{
	this->subtract_term(term(coefficient, power));
}

bool polynomial::insert_mask(const std::vector<int32_t>& mask)
{
	for (int32_t n = static_cast<int32_t>(mask.size() - 1); n >= 0; --n)
	{
		int32_t coefficient = mask.at(mask.size() - 1 - n);
		if (coefficient != 0)
		{
			this->insert_term(term(coefficient, n));
		}
	}

	return true;
}

const std::vector<term> polynomial::get_terms() const 
{
	return this->terms;
}

const term polynomial::at(size_t n) const
{
	for (const term t : this->get_terms())
	{
		if (t.get_power() == n)
		{
			return t;
		}
	}

	return term(0, static_cast<int32_t>(n));
}

const std::vector<int32_t> polynomial::get_mask()
{
	this->reorganize();

	std::vector<int32_t> v;
	if (terms.size() > 0)
	{
		v.resize(terms.at(0).get_power() + 1);

		for (const term & t : terms)
		{
			v.at(terms.at(0).get_power() - t.get_power()) = t.get_coefficient();
		}

	}
	

	return v;
}

const int32_t polynomial::get_degree() const
{
	int32_t degree = 0;
	
	for (const term &t : terms)
	{
		if (t.get_power() > degree)
		{
			degree = t.get_power();
		}
	}

	return degree;
}

const term polynomial::get_greatest_term() const
{
	int32_t degree = this->get_degree();
	for (size_t n = 0; n < terms.size(); ++n)
	{
		if (terms.at(n).get_power() == degree)
		{
			return terms.at(n);
		}
	}

	return term(0, 0);
}

void polynomial::reorganize()
{
	for (std::vector<term>::iterator it = terms.begin(); it != terms.end(); ++it)
	{
		if ((*it).get_coefficient() == 0)
		{
			terms.erase(it);
			--it;
		}
	}

	std::sort(terms.begin(), terms.end(), [](const term &t1, const term &t2) -> bool
	{
		return t1.get_power() > t2.get_power();
	});
	
}

const polynomial polynomial::add(const polynomial & p1, const polynomial & p2)
{
	polynomial p(p1);

	for (const term & t : p2.get_terms())
	{
		p.insert_term(t);
	}

	return p;
}

const polynomial polynomial::subtract(const polynomial & p1, const polynomial & p2)
{
	polynomial p(p1);

	for (size_t n = 0; n < p2.get_terms().size(); ++n)
	{
		p.subtract_term(p2.get_terms().at(n));
	}

	return p;
}

const polynomial polynomial::multiply(const polynomial & p1, const polynomial & p2)
{
	/*
	EXPIREMENTAL

	Let F_A = FFT(A, m, w)                        // time O(n log n)
    Let F_B = FFT(B, m, w)                        // time O(n log n)
    For i=1 to m, let F_C[i] = F_A[i]*F_B[i]      // time O(n)
    Output C = 1/m * FFT(F_C, m, w^{-1}).         // time O(n log n)
	*/

	polynomial p1_fft = fft(p1);
	return p1_fft;
	polynomial p2_fft = fft(p2);

	
	int32_t m = std::max(p1_fft.get_degree(), p2_fft.get_degree());
	for (int32_t i = 0; i < m; ++i)
	{

	}
	
	
	//return polynomial::foil(p1, p2);
}

//EXPIREMENTAL FUNCTION

std::ostream& operator<<(std::ostream &os, polynomial &p1)
{
	auto v = p1.get_mask();

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
const polynomial polynomial::fft(const polynomial & p1)
{
	int32_t size = p1.get_degree();
	if (size == 0)
	{
		return p1;
	}

	int32_t m = size / 2;

	polynomial p1_even;
	polynomial p1_odd;
	
	int32_t p1_even_power = 0;
	int32_t p1_odd_power = 0;

	for (const term &t : p1.get_terms())
	{
		//even degree
		if (t.get_power() % 2 == 0)
		{
			p1_even.insert_term(t.get_coefficient(), p1_even_power++);
		}
		else
		{
			p1_odd.insert_term(t.get_coefficient(), p1_odd_power++);
		}
	}

	polynomial fft_even = fft(p1_even);
	polynomial fft_odd = fft(p1_odd);

	polynomial p;
	for (int32_t n = 0; n < m; ++n)
	{
		term t = term(static_cast<int32_t>(std::polar(1.0, -2 * 3.141592653589793238460 * n / size).real()), 0) * fft_odd.at(n);
		p.insert_term(term::add(fft_even.at(n), t).second.get_coefficient(), n);
		p.insert_term(term::subtract(fft_even.at(n), t).second.get_coefficient(), n + m);
	}

	return p;
}


const polynomial polynomial::foil(const polynomial & p1, const polynomial & p2)
{
	polynomial p;
	std::vector<term> v1 = p1.get_terms();
	std::vector<term> v2 = p2.get_terms();

	for (size_t n = 0; n < v1.size(); ++n)
	{
		for (size_t m = 0; m < v2.size(); ++m)
		{
			p.insert_term(term::multiply(v1.at(n), v2.at(m)));
		}
	}

	return p;
}

const polynomial polynomial::divide(const polynomial &p1, const polynomial &p2, std::pair<polynomial, polynomial> *remainder)
{
	polynomial p;

	polynomial dividend(p1);
	polynomial divisor(p2);

	while(dividend >= divisor)
	{
		term t = term::divide(dividend.get_terms().at(0), divisor.get_terms().at(0));
		p.insert_term(t);
		dividend = polynomial::subtract(dividend, polynomial::multiply(divisor, polynomial(std::vector<term>({ t }))));
		dividend.reorganize();
	}

	if (remainder != nullptr)
	{
		*remainder = std::make_pair(dividend, divisor);
	}


	return p;
}
