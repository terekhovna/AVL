#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>
#include<ctime>
#include "AVL.h"
#include "Random.h"

clock_t start, end;

BOOST_AUTO_TEST_CASE(myTestCase)
{
  BOOST_TEST(1 == 1);
  BOOST_TEST(true);
}