#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>
#include <ctime>
#include "AVL.h"
#include "Random.h"
#include <algorithm>
#include <string>
#include <set>
using std::string;
using std::set;
using std::max;

clock_t start, end;

template<typename T>
using BST = AVL<T>;

template<typename T>
using Simple_BST = set<T>;

template<typename T>
Simple_BST<T> split(Simple_BST<T> &g, const T &key)
{
	Simple_BST<T> w;
	for (T tmp : g)
		if (tmp > key)
			w.insert(tmp);
	for (T tmp : w)
		g.erase(tmp);
	return std::move(w);
}

template<typename T>
void merge(Simple_BST<T> &g, Simple_BST<T> &&w)
{
	for (T tmp : w)
		g.insert(tmp);
	w.clear();
}

template<typename T>
inline void checkIterator(const BST<T> &A, const typename BST<T>::iterator &a, const Simple_BST<T> &B, const typename Simple_BST<T>::iterator &b)
{
	if (b == B.end())
		BOOST_TEST(a == A.end());
	else
		BOOST_CHECK_EQUAL(*a, *b);
}

template<typename T>
void checkwithSimple(const BST<T> &h, const Simple_BST<T> &g, T M = T{})
{
	if (M == T{})
	{
		for (T tmp : g)
		{
			BOOST_CHECK(h.count(tmp));
		}
		for (auto it = h.begin(); it != h.end(); ++it)
		{
			BOOST_CHECK(g.count(*it));
		}
	}
	else
	{
		for (T tmp = 0; tmp < M; ++tmp)
		{
			BOOST_CHECK_EQUAL(h.count(tmp), g.count(tmp));
			checkIterator(h, h.find(tmp), g, g.find(tmp));
			if (g.count(tmp))
			{
				checkIterator(h, ++h.find(tmp), g, ++g.find(tmp));
				if (g.find(tmp) != g.begin())
					checkIterator(h, --h.find(tmp), g, --g.find(tmp));
			}
			checkIterator(h, h.lower_bound(tmp), g, g.lower_bound(tmp));
			checkIterator(h, h.upper_bound(tmp), g, g.upper_bound(tmp));
		}
	}
}

/*find, count, insert, erase, lower, upper*/
template<typename T>
void check(size_t IT, T M, size_t chI, size_t chE, size_t chB = 5)
{
	BST<T> h;
	Simple_BST<T> g;
	T tmp;
	for (size_t it = 0; it < IT; ++it)
	{
		if (rnd<100>() < chI)
		{
			tmp = rnd() % M;
			h.insert(tmp);
			g.insert(tmp);
		}

		if (g.size() && rnd<100>() < chE)
		{
			do
			{
				tmp = rnd() % M;
			} while (!g.count(tmp));
			h.erase(tmp);
			g.erase(tmp);
		}
		if(rnd<100>() < chB)
			BOOST_TEST(h.checkBalance());
		checkwithSimple(h, g, M);
	}
}

template<typename T>
void polimorf(BST<T> &h, Simple_BST<T> &g, size_t IT, T M, size_t chI, size_t chE, T MD = T{})
{
	T tmp;
	for (size_t it = 0; it < IT; ++it)
	{
		if (rnd<100>() < chI)
		{
			tmp = rnd() % M + MD;
			h.insert(tmp);
			g.insert(tmp);
		}

		if (g.size() && rnd<100>() < chE)
		{
			do
			{
				tmp = rnd() % M + MD;
			} while (!g.count(tmp));
			h.erase(tmp);
			g.erase(tmp);
		}
	}
}

BOOST_AUTO_TEST_CASE(checkInFiCoEr)
{
	for(size_t chI = 20; chI <= 100; chI += 20)
		for (size_t chE = 20; chE <= 100; chE += 20)
		{
			check<int>(100, 30, chI, chE);
			check<int>(50, 50, chI, chE);
			check<int>(30, 100, chI, chE);
		}
}

template<typename T>
void checkmerge(size_t IT, T M, size_t chI, size_t chE, size_t IT2, T M2, size_t chI2, size_t chE2)
{
	BST<T> h1, h2;
	Simple_BST<T> g1, g2;
	polimorf(h1, g1, IT, M, chI, chE);
	polimorf(h2, g2, IT2, M2, chI2, chE2, M);
	h1.merge(std::move(h2));
	merge(g1, std::move(g2));
	BOOST_TEST(h1.checkBalance());
	checkwithSimple(h1, g1, M + M2);
}

template<typename T>
void checksplit(size_t IT, T M, size_t chI, size_t chE, T key)
{
	BST<T> h1;
	Simple_BST<T> g1;
	polimorf(h1, g1, IT, M, chI, chE);
	BST<T> h2{ h1.split(key) };
	Simple_BST<T> g2{ split(g1, key) };
	BOOST_TEST(h1.checkBalance());
	BOOST_TEST(h2.checkBalance());
	checkwithSimple(h1, g1, M);
	checkwithSimple(h2, g2, M);
}

BOOST_AUTO_TEST_CASE(checkMerge)
{
	checkmerge<int>(100, 30, 60, 40, 0, 0, 0, 0);
	checkmerge<int>(0, 0, 0, 0, 100, 30, 60, 40);
	checkmerge<int>(1000, 1000, 60, 40, 1000, 1000, 60, 40);
	checkmerge<int>(1000, 1000, 70, 30, 1000, 1000, 70, 30);
	for(size_t chI = 25; chI <= 100; chI += 25)
		for (size_t chE = 20; chE <= 80; chE += 30)
			for (size_t chI2 = 25; chI2 <= 100; chI2 += 25)
				for (size_t chE2 = 20; chE2 <= 80; chE2 += 30)
		{
			checkmerge<int>(100, 100, chI, chE, 100, 100, chI2, chE2);
		}
}

BOOST_AUTO_TEST_CASE(checkSplit)
{
	checksplit<int>(1000, 1000, 70, 30, -1);
	checksplit<int>(1000, 1000, 70, 30, 1001);
	for(size_t chI = 25; chI <= 100; chI += 25)
		for (size_t chE = 20; chE <= 80; chE += 30)
			for(size_t key = 100; key <= 300; key += 100)
		{
			checksplit<int>(500, 500, chI, chE, key);
		}
}