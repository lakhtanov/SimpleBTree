#include "my_string.h"

string::string()
{	
}

string::string(const string &s)
{
	str = s.str;
}

string::string(const char *s)
{
	int n = strlen(s);
	int i;
	for (i = 0; i < n; i++)
		str.push_back(s[i]);
}

string::~string()
{	
}

char & string::operator[](int num)
{
	return str[num];
}

const char & string::operator[](int num) const
{
	return str[num];
}

int string::size() const
{
	return str.size();
}

int string::length() const
{
	return str.size();
}

bool string::empty() const
{
	return str.empty();
}

string & string::operator=(const string & s)
{
	str = s.str;
	return *this;
}

string & string::operator+=(char c)
{
	str.push_back(c);
	return *this;
}

void string::clear()
{
	str.clear();
}

dynamic_array<string> string::split(const char *separator) const
{
	int separator_len = strlen(separator);
	dynamic_array<string> res;
	string cur;
	int i, j;
	bool flag;

	for (i = 0; i < str.size(); i++)
	{
		flag = false;
		for (j = 0; j < separator_len; j++)
			if (str[i] == separator[j])
			{
				flag = true;
				break;
			}
		
		if (flag && !cur.empty())
		{
			res.push_back(cur);
			cur.clear();
		}
		else
			cur += str[i];
	}

	if (!cur.empty())
	{
		res.push_back(cur);
		cur.clear();
	}
	return res;
}

string string::substr(int pos) const
{
	if (pos >= str.size())
		throw "Index out of range";
	return substr(pos, str.size() - pos);
}

string string::substr(int pos, int len) const
{
	if (pos + len > str.size())
		throw "Index out of range";

	int i;
	string res;

	for (i = pos; i < pos + len; i++)
		res += str[i];

	return res;
}

char * string::c_str() const
{
	char *res;
	int i;
	try
	{
		res = new char[str.size() + 1];
	}
	catch(...)
	{
		throw "Not enough memory";
	}
	for (i = 0; i < str.size(); i++)
		res[i] = str[i];
	res[str.size()] = '\0';
	return res;
}

bool string::operator<(const string &s) const
{
	int len = str.size() < s.size() ? str.size() : s.size();
	int i;
	for (i = 0; i < len; i++)
		if (str[i] != s.str[i])
			return str[i] < s.str[i];
	return !(len == s.size());
}

bool string::operator==(const string &s) const
{
	return str == s.str;
} 

bool string::operator<=(const string &s) const
{
	return (*this) < s || (*this) == s;
}

string string::operator+(const string &s) const
{
	int i;
	string ans(*this);
	for (i = 0; i < s.size(); i++)
		ans += s[i];
	return ans;
}

char * to_str(string s)
{
	return s.c_str();
}

string & string::operator+=(const string &s)
{
	int n = s.size();
	int i;
	for (i = 0; i < n; i++)
		str.push_back(s[i]);
	return *this;
}