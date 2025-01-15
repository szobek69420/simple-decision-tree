#define _CRT_SECURE_NO_WARNINGS

#include "value_type.h"
#include <string.h>
#include <stdio.h>


ValueTypeInt::ValueTypeInt(int value) :ValueTypeBase(), value(value) {}


void ValueTypeInt::operator=(const ValueTypeBase& other)
{
	this->value = ((const ValueTypeInt&)other).value;
}

bool ValueTypeInt::operator==(const ValueTypeBase& other) const
{
	return this->value == ((const ValueTypeInt&)other).value;
}

bool ValueTypeInt::operator<(const ValueTypeBase& other) const
{
	return this->value < ((const ValueTypeInt&)other).value;
}

bool ValueTypeInt::operator>(const ValueTypeBase& other) const
{
	return this->value > ((const ValueTypeInt&)other).value;
}

bool ValueTypeInt::operator<=(const ValueTypeBase& other) const
{
	return this->value <= ((const ValueTypeInt&)other).value;
}

bool ValueTypeInt::operator>=(const ValueTypeBase& other) const
{
	return this->value >= ((const ValueTypeInt&)other).value;
}

ValueTypeBase* ValueTypeInt::clone() const
{
	return (ValueTypeBase*)new ValueTypeInt(this->value);
}


std::string ValueTypeInt::toString() const
{
	char buffer[20];
	sprintf(buffer, "%d", this->value);
	return std::string(buffer);
}




ValueTypeFloat::ValueTypeFloat(float value) :ValueTypeBase(), value(value) {}


void ValueTypeFloat::operator=(const ValueTypeBase& other)
{
	this->value = ((const ValueTypeFloat&)other).value;
}

bool ValueTypeFloat::operator==(const ValueTypeBase& other) const
{
	return this->value == ((const ValueTypeFloat&)other).value;
}

bool ValueTypeFloat::operator<(const ValueTypeBase& other) const
{
	return this->value < ((const ValueTypeFloat&)other).value;
}

bool ValueTypeFloat::operator>(const ValueTypeBase& other) const
{
	return this->value > ((const ValueTypeFloat&)other).value;
}

bool ValueTypeFloat::operator<=(const ValueTypeBase& other) const
{
	return this->value <= ((const ValueTypeFloat&)other).value;
}

bool ValueTypeFloat::operator>=(const ValueTypeBase& other) const
{
	return this->value >= ((const ValueTypeFloat&)other).value;
}

ValueTypeBase* ValueTypeFloat::clone() const
{
	return (ValueTypeBase*)new ValueTypeFloat(this->value);
}

std::string ValueTypeFloat::toString() const
{
	char buffer[20];
	sprintf(buffer, "%.3f", this->value);
	return std::string(buffer);
}




ValueTypeString::ValueTypeString(const char* value) :ValueTypeBase()
{
	int valueLength = strlen(value);
	this->value = new char[valueLength + 1];
	strcpy(this->value, value);
}

ValueTypeString::~ValueTypeString()
{
	delete[] this->value;
}


void ValueTypeString::operator=(const ValueTypeBase& other)
{
	if (strcmp(this->value, ((const ValueTypeString&)other).value) == 0)
		return;

	delete[] this->value;
	int valueLength = strlen(((const ValueTypeString&)other).value);
	this->value = new char[valueLength + 1];
	strcpy(this->value, ((const ValueTypeString&)other).value);
}

bool ValueTypeString::operator==(const ValueTypeBase& other) const
{
	return strcmp(this->value, ((const ValueTypeString&)other).value) == 0;
}

bool ValueTypeString::operator<(const ValueTypeBase& other) const
{
	return strcmp(this->value, ((const ValueTypeString&)other).value) < 0;
}

bool ValueTypeString::operator>(const ValueTypeBase& other) const
{
	return strcmp(this->value, ((const ValueTypeString&)other).value) > 0;
}

bool ValueTypeString::operator<=(const ValueTypeBase& other) const
{
	return strcmp(this->value, ((const ValueTypeString&)other).value) <= 0;
}

bool ValueTypeString::operator>=(const ValueTypeBase& other) const
{
	return strcmp(this->value, ((const ValueTypeString&)other).value) >= 0;
}

ValueTypeBase* ValueTypeString::clone() const
{
	return (ValueTypeBase*)new ValueTypeString(this->value);
}

std::string ValueTypeString::toString() const
{
	return std::string(this->value);
}