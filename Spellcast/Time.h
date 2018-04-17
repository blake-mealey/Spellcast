#pragma once

#include <string>

class Time {
public:
	// Constructors
	Time();
	Time(double a_timeSeconds);

	// Builders
	static Time FromMilliseconds(double a_timeMilliseconds);
	static Time FromSeconds(double a_timeSeconds);
	static Time FromMinutes(double a_timeMinutes);
	static Time FromHours(double a_timeHours);

	// Unit getters
	float GetMilliseconds() const;
	float GetSeconds() const;
	float GetMinutes() const;
	float GetHours() const;

	std::string ToString() const;

	// Operator overloads
	bool operator<(const Time& a_other) const;
	bool operator<=(const Time& a_other) const;

	bool operator>(const Time& a_other) const;
	bool operator>=(const Time& a_other) const;

	Time operator-() const;
	Time operator-(const Time& a_other) const;
	Time operator+(const Time& a_other) const;

	void operator+=(const Time& a_other);
	void operator-=(const Time& a_other);

	Time operator*(const Time& a_other) const;
	Time operator/(const Time& a_other) const;

	void operator*=(const Time& a_other);
	void operator/=(const Time& a_other);

private:
	double m_timeSeconds;
};
