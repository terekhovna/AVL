#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>
#include<ctime>
#include "AVL.h"
#include "Random.h"

clock_t start, end;

BOOST_AUTO_TEST_CASE(myTestCase)
{
	AVL<int> a;
	a.insert(12);
	a.insert(15);
	a.insert(30);
	BOOST_CHECK_EQUAL(*a.lower_bound(12), 12);
	a.erase(12);
	BOOST_CHECK_EQUAL(*a.lower_bound(12), 15);
}