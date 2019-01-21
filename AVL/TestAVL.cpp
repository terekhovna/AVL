#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>
#include<ctime>
#include "AVL.h"
#include "Random.h"
#include <string>
#include <set>
using std::string;
using std::set;

clock_t start, end;

template<typename T>
using BST = AVL<T>;

template<typename T>
using Simple_BST = set<T>;


BOOST_AUTO_TEST_CASE(myTestCase)
{
	for(size_t it = 0; it < 10; it++)
	{
		BST<int> a;
		Simple_BST<int> b;
		size_t n = rnd<20>() + 10;
		for (size_t i = 0; i < n; ++i)
		{
			auto tmp = rnd();
			a.insert(tmp);
			b.insert(tmp);
		}
		a.erase(*b.begin());
		b.erase(b.begin());
		for (auto tmp : b)
		{
			BOOST_TEST(a.count(tmp));
		}
	}
}