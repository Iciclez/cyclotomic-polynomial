#include "term.hpp"



term::term(const std::pair<int32_t, int32_t> &term_pair)
{
	this->term_pair = term_pair;
}

term::term(int32_t coefficient, int32_t power)
{
	this->term_pair = std::make_pair(coefficient, power);
}

term::~term()
{
}

const term term::operator+(const term & t)
{
	return term::add(*this, t).second;
}

const term term::operator-(const term & t)
{
	return term::subtract(*this, t).second;
}

const term term::operator*(const term & t)
{
	return term::multiply(*this, t);
}

const term term::operator/(const term & t)
{
	return term::divide(*this, t);
}

term & term::operator+=(const term & t)
{
	*this = operator+(t);
	return *this;
}

term & term::operator-=(const term & t)
{
	*this = operator-(t);
	return *this;
}

term & term::operator*=(const term & t)
{
	*this = operator*(t);
	return *this;
}

term & term::operator/=(const term & t)
{
	*this = operator/(t);
	return *this;
}

bool term::add(const term & t)
{
	if (t.get_power() == this->get_power())
	{
		this->term_pair.first += t.get_coefficient();
		return true;
	}
	return false;
}

bool term::subtract(const term & t)
{
	if (t.get_power() == this->get_power())
	{
		this->term_pair.first -= t.get_coefficient();
		return true;
	}
	return false;
}

bool term::multiply(const term & t)
{
	this->term_pair.first *= t.get_coefficient();
	this->term_pair.second += t.get_power();
	return true;
}

bool term::divide(const term & t)
{
	this->term_pair.first /= t.get_coefficient();
	this->term_pair.second -= t.get_power();
	return true;
}

int32_t term::get_coefficient() const
{
	return this->term_pair.first;
}

int32_t term::get_power() const 
{
	return this->term_pair.second;
}

const std::pair<bool, term> term::add(const term & t1, const term & t2)
{
	if (t1.get_power() == t2.get_power())
	{
		return std::make_pair(true, term(t1.get_coefficient() + t2.get_coefficient(), t1.get_power()));
	}
	return std::make_pair(false, term(0, 0));
}

const std::pair<bool, term> term::subtract(const term & t1, const term & t2)
{
	if (t1.get_power() == t2.get_power())
	{
		return std::make_pair(true, term(t1.get_coefficient() - t2.get_coefficient(), t1.get_power()));
	}
	return std::make_pair(false, term(0, 0));
}

const term term::multiply(const term & t1, const term & t2)
{
	return term(t1.get_coefficient() * t2.get_coefficient(), t1.get_power() + t2.get_power());
}

const term term::divide(const term & t1, const term & t2)
{
	return term(t1.get_coefficient() / t2.get_coefficient(), t1.get_power() - t2.get_power());
}
