#ifndef _DYNAMIC_ARRAY_H_
#define _DYNAMIC_ARRAY_H_

#include <cstdlib>
#include <cstdio>
#include <memory.h>

template<class T>
class dynamic_array
{
	T *_arr;
	int _size, _len;

	bool memory_allocation_check(T *) const;
	bool index_check(int) const;
public:
	dynamic_array();
	dynamic_array(const dynamic_array<T> &);
	dynamic_array(int);			
	~dynamic_array();

	T & operator[](int);
	const T & operator[](int) const;
	T get(int) const;
	void push_back(T);
	void pop_back();
	bool empty() const;
	int size() const;
	T back() const;
	void clear();
	void resize(int);
	void resize(int, T);

	bool operator==(const dynamic_array &) const;
	bool operator!=(const dynamic_array &) const;

	dynamic_array<T> & operator=(const dynamic_array<T> &da);
	T * to_array() const;
};

//private
template<class T>
bool dynamic_array<T>::memory_allocation_check(T *pt) const
{
	return !pt;
}

template<class T>
bool dynamic_array<T>::index_check(int ind) const
{
	return ind < 0 || ind >= _size;
}

//public
template<class T>
dynamic_array<T>::dynamic_array()
{	
	_size = 0;
	//_len = 1;	
	_len = 0;
	_arr = NULL;
}

template<class T>
dynamic_array<T>::dynamic_array(const dynamic_array<T> &da)
{
	int i;
	T *tmp = new T[da._len];
	_size = da._size;
	_len = da._len;	
	_arr = tmp;
	for (i = 0; i < _size; i++)
		_arr[i] = da._arr[i];
}

template<class T>
dynamic_array<T>::dynamic_array(int sz)
{
	if (!sz)
	{
		_size = _len = 0;
		_arr = NULL;
	}
	else
	{
		try
		{
			_size = sz;
			_len = 1;
			while (_len < sz)
				_len <<= 1;
			_arr = new T[_len];		
		}
		catch(...)
		{
			throw "Not enough memory";
		}
	}
}

template<class T>
dynamic_array<T>::~dynamic_array()
{	
	delete[] _arr;
	_arr = NULL;
}

template<class T>
T & dynamic_array<T>::operator[](int ind)
{
	if (index_check(ind))
		throw "Index out of range";
	return _arr[ind];
}

template<class T>
const T & dynamic_array<T>::operator[](int ind) const
{
	if (index_check(ind))
		throw "Index out of range";
	return _arr[ind];
}


template<class T>
T dynamic_array<T>::get(int ind) const
{
	if (index_check(ind))
		throw "Index out of range";
	return _arr[ind];
}

template<class T>
void dynamic_array<T>::push_back(T elem)
{
	T *tmp = NULL;
	int i;	
	if (_size == _len)
	{	try
		{				
			tmp = new T[2 * _len > 1 ? 2 * _len : 1];
			for (i = 0; i < _len; i++)
				tmp[i] = _arr[i];

			delete[] _arr;
			_arr = tmp;		

			_len = 2 * _len > 1 ? 2 * _len : 1;
		}
		catch(...)
		{
			throw "Cannot push_back an element";
		}
	}
	_arr[_size++] = elem;
}

template<class T>
void dynamic_array<T>::pop_back()
{
	int i;
	T *tmp = NULL;
	if (!_size)
		throw "Cannot pop_back an element from the empty dynamic array";

	if (((_size - 1) << 2) < _len)
	{
		try
		{	
			tmp = new T[(_size - 1) << 1];
			_size--;
			for (i = 0; i < _size; i++)
				tmp[i] = _arr[i];
			delete[] _arr;
			_arr = tmp;		

			_len = _size << 1;
		}
		catch(...)
		{
			throw "Not enough memory";
		}
	}
	else
	{
		_size--;
	}
}

template<class T>
bool dynamic_array<T>::empty() const
{
	return _size == 0;
}

template<class T>
int dynamic_array<T>::size() const
{
	return _size;	
}

template<class T>
T dynamic_array<T>::back() const
{
	if (!_size)
		throw "Index out of range";
	return _arr[_size - 1];
}

template<class T>
void dynamic_array<T>::clear()
{	
	delete[] _arr;
	_size = 0, _len = 0;	
	_arr = NULL;
}

template<class T>
void dynamic_array<T>::resize(int sz)
{
	T *tmp = NULL;
	int new_len = sz < 1 ? sz : 1, i;
	while (new_len < sz)
		new_len <<= 1;
	try
	{
		if (new_len)
			tmp = new T[new_len];
	}
	catch(...)
	{
		throw "Not enough memory";
	}
	for (i = 0; i < (_size < sz ? _size : sz); i++)
		tmp[i] = _arr[i];
	delete[] _arr;			
	_arr = tmp;
	_size = sz;
	_len = new_len;
}

template<class T>
void dynamic_array<T>::resize(int sz, T elem)
{
	T *tmp = NULL;
	int new_len = sz < 1 ? sz : 1, i;
	while (new_len < sz)
		new_len <<= 1;
	try
	{
		if (new_len)
			tmp = new T[new_len];
	}
	catch(...)
	{
		throw "Not enough memory";
	}
	for (i = 0; i < sz; i++)
		tmp[i] = elem;
	delete[] _arr;			
	_arr = tmp;
	_size = sz;
	_len = new_len;
}

template<class T>
bool dynamic_array<T>::operator==(const dynamic_array &da) const
{
	int i;
	if (_size != da._size)
		return false;
	for (i = 0; i < _size; i++)
		if (_arr[i] != da._arr[i])
			return false;
	return true;	
}

template<class T>
bool dynamic_array<T>::operator!=(const dynamic_array &da) const
{
	int i;
	if (_size != da._size)
		return true;
	for (i = 0; i < _size; i++)
		if (_arr[i] != da._arr[i])
			return true;
	return false;	
}

template<class T>
dynamic_array<T> & dynamic_array<T>::operator=(const dynamic_array<T> &da)
{
	T *tmp_arr;	
	int i;
	try
	{
		tmp_arr = new T[da._len];
		delete[] _arr;
		_arr = tmp_arr;
		_size = da._size;
		_len = da._len;
		for (i = 0; i < _size; i++)
			_arr[i] = da._arr[i];
	}
	catch(...)
	{		
	}
	return *this;
}

template<class T>
T * dynamic_array<T>::to_array() const
{
	T *res;
	int i;
	try
	{
		res = new T[_size];
		for (i = 0; i < _size; i++)
			res[i] = _arr[i];
		return res;
	}
	catch(...)
	{
		throw "Not enough memory";
	}
}

#endif