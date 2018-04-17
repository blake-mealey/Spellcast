#include "Time.h"

Time::Time() : Time(0.0) {}

Time::Time(const double a_timeSeconds) : m_timeSeconds(a_timeSeconds) {}

Time Time::FromMilliseconds(const double a_timeMilliseconds) {
	return a_timeMilliseconds / 1000.0;
}

Time Time::FromSeconds(const double a_timeSeconds) {
	return a_timeSeconds;
}

Time Time::FromMinutes(const double a_timeMinutes) {
	return a_timeMinutes * 60.0;
}

Time Time::FromHours(const double a_timeHours) {
	return a_timeHours * 3600.0;
}

float Time::GetMinutes() const {
	return GetSeconds() / 60.0;
}

float Time::GetHours() const {
	return GetMinutes() / 60.0;
}

std::string Time::ToString() const {
	char buf[6];
	const int minutes = GetMinutes();
	const int seconds = GetSeconds();
	snprintf(buf, sizeof(buf), "%02d:%02d", minutes, seconds % 60);
	return std::string(buf);
}

float Time::GetSeconds() const {
	return m_timeSeconds;
}

float Time::GetMilliseconds() const {
	return GetSeconds() * 1000.0;
}

bool Time::operator<(const Time& a_other) const {
	return GetSeconds() < a_other.GetSeconds();
}

bool Time::operator<=(const Time& a_other) const {
	return GetSeconds() <= a_other.GetSeconds();
}

bool Time::operator>(const Time& a_other) const {
	return GetSeconds() > a_other.GetSeconds();
}

bool Time::operator>=(const Time& a_other) const {
	return GetSeconds() >= a_other.GetSeconds();
}

Time Time::operator-() const {
	return -GetSeconds();
}

Time Time::operator-(const Time& a_other) const {
	return GetSeconds() - a_other.GetSeconds();
}

Time Time::operator+(const Time& a_other) const {
	return GetSeconds() + a_other.GetSeconds();
}

void Time::operator+=(const Time& a_other) {
	m_timeSeconds += a_other.GetSeconds();
}

void Time::operator-=(const Time& a_other) {
	m_timeSeconds -= a_other.GetSeconds();
}

Time Time::operator*(const Time& a_other) const {
	return GetSeconds() * a_other.GetSeconds();
}

Time Time::operator/(const Time& a_other) const {
	return GetSeconds() / a_other.GetSeconds();
}

void Time::operator*=(const Time& a_other) {
	m_timeSeconds *= a_other.GetSeconds();
}

void Time::operator/=(const Time& a_other) {
	m_timeSeconds /= a_other.GetSeconds();
}
