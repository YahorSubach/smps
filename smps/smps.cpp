// smps.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "serializer.h"
#include "serializable.h"
#include <iostream>

class test_a : public smps::serializable<3>
{
private:
	int test_int;
	double test_double;
	std::string test_string;

public:
	test_a(): test_int(123), test_double(123.123),test_string("st ass")
	{}

	SER_MEMBER(1, test_int);
	SER_MEMBER(2, test_double);
	SER_MEMBER(3, test_string);
};


class test_b : public smps::serializable<4>
{
private:
	int test_int;
	double test_double;
	std::string test_string;
	test_a test_test_a;
public:
	test_b() : test_int(456), test_double(456.456), test_string("i cock prosto")
	{}

	SER_MEMBER(1, test_int);
	SER_MEMBER(2, test_double);
	SER_MEMBER(3, test_string);
	SER_MEMBER(4, test_test_a);
};



int main()
{
	test_b ob;
	std::cout << smps::str_maker<>::make_string(ob) << std::endl;

	system("pause");

	return 0;
}

