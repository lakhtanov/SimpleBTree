#ifndef _MY_STRING_H_
#define _MY_STRING_H_

#include <cstring>

#include "dynamic_array.h"

class string
{
	dynamic_array<char> str;	
public:
	string();
	string(const string &);
	string(const char *);
	~string();

	char & operator[](int);
	const char & operator[](int) const;
	string & operator=(const string &);
	string & operator+=(char);
	string & operator+=(const string &);

	int size() const;
	int length() const;
	bool empty() const;
	void clear();

	char * c_str() const;
	dynamic_array<string> split(const char *separator = " \t\n") const;
	string substr(int) const;
	string substr(int, int) const;
	bool operator<(const string &) const;
	bool operator==(const string &) const;
	bool operator<=(const string &) const;
	string operator+(const string &) const;
};

char * to_str(string);

#endif