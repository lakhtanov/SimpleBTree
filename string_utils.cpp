#include "string_utils.h"

char * to_str(int x)
{
	char *buf;
	try
	{
		buf = new char[BUFFER_INT];
		sprintf(buf, "%d", x);
		return buf;
	}
	catch(...)
	{
		throw "Not enough memory";
	}
}

char * to_str(double x)
{
	char *buf;
	try
	{
		buf = new char[BUFFER_DOUBLE];
		sprintf(buf, "%.30lf", x);
		return buf;
	}
	catch(...)
	{
		throw "Not enough memory";
	}
}

char * to_str(const char *s)
{
	char *buf;
	int n = strlen(s);
	int i;
	try
	{
		buf = new char[n + 1];
		for (i = 0; i < n; i++)
			buf[i] = s[i];
		buf[n] = '\0';
		return buf;
	}					
	catch(...)
	{
		throw "Not enough memory";
	}
}