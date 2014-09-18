#ifndef _PAIR_H_
#define _PAIR_H_

template<typename T1, typename T2>
class pair
{
	void init(T1, T2);
public:
	T1 first;
	T2 second;

	pair();
	pair(const pair &);
	pair(T1, T2);
	~pair();

	void operator=(const pair &);
};

template<typename T1, typename T2>
void pair<T1, T2>::init(T1 f, T2 s)
{
	first = f;
	second = s;
}

template<typename T1, typename T2>
pair<T1, T2>::pair()
{
}

template<typename T1, typename T2>
pair<T1, T2>::pair(const pair &p)
{
	init(p.first, p.second);
}

template<typename T1, typename T2>
pair<T1, T2>::pair(T1 f, T2 s)
{
	init(f, s);
}

template<typename T1, typename T2>
pair<T1, T2>::~pair()
{
}

template<typename T1, typename T2>
void pair<T1, T2>::operator=(const pair &pr)
{
	init(pr.first, pr.second);
}

#endif