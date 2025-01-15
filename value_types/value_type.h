#ifndef VALUE_TYPE_H
#define VALUE_TYPE_H

#include <string>

class ValueTypeBase {
public:
	virtual void operator=(const ValueTypeBase& other) = 0;
	virtual bool operator==(const ValueTypeBase& other) const = 0;
	virtual bool operator<(const ValueTypeBase& other) const = 0;
	virtual bool operator>(const ValueTypeBase& other) const = 0;
	virtual bool operator<=(const ValueTypeBase& other) const = 0;
	virtual bool operator>=(const ValueTypeBase& other) const = 0;
	
	virtual ValueTypeBase* clone() const = 0;
	virtual std::string toString() const = 0;
};

class ValueTypeInt : ValueTypeBase {
public:
	int value;

	ValueTypeInt(int value);
	void operator=(const ValueTypeBase& other);
	bool operator==(const ValueTypeBase& other) const;
	bool operator<(const ValueTypeBase& other) const;
	bool operator>(const ValueTypeBase& other) const;
	bool operator<=(const ValueTypeBase& other) const;
	bool operator>=(const ValueTypeBase& other) const;

	ValueTypeBase* clone() const;
	std::string toString() const;
};

class ValueTypeFloat : ValueTypeBase {
public:
	float value;

	ValueTypeFloat(float value);
	void operator=(const ValueTypeBase& other);
	bool operator==(const ValueTypeBase& other) const;
	bool operator<(const ValueTypeBase& other) const;
	bool operator>(const ValueTypeBase& other) const;
	bool operator<=(const ValueTypeBase& other) const;
	bool operator>=(const ValueTypeBase& other) const;

	ValueTypeBase* clone() const;
	std::string toString() const;
};

class ValueTypeString : ValueTypeBase {
public:
	char* value;

	ValueTypeString(const char* value);
	~ValueTypeString();
	void operator=(const ValueTypeBase& other);
	bool operator==(const ValueTypeBase& other) const;
	bool operator<(const ValueTypeBase& other) const;
	bool operator>(const ValueTypeBase& other) const;
	bool operator<=(const ValueTypeBase& other) const;
	bool operator>=(const ValueTypeBase& other) const;

	ValueTypeBase* clone() const;
	std::string toString() const;
};

#endif