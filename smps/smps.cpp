// smps.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "serializer.h"
#include "serializable.h"
#include "json.h"
#include <iostream>


class test_a :public smps::serializable_field
{
private:
	int test_int;
	double test_double;
	std::string test_string;

public:

	test_a() : test_int(123), test_double(123.123), test_string("st ass")
	{}

	SMPS_FIELDS(3)
	SMPS_FIELD(1, test_int);
	SMPS_FIELD(2, test_double);
	SMPS_FIELD(3, test_string);
};


class test_b :public smps::serializable_field
{
private:
	int test_int;
	double test_double;
	std::string test_string;
	test_a test_test_a;
public:
	test_b() : test_int(456), test_double(456.456), test_string("i cock prosto")
	{}

	SMPS_FIELDS(4)
	SMPS_FIELD(1, test_int);
	SMPS_FIELD(2, test_double);
	SMPS_FIELD(3, test_string);
	SMPS_FIELD(4, test_test_a);
};


int main()
{
	test_b a;


	std::string res = smps::json_serializer::Serialize(a);
	test_b aa = smps::json_serializer::Deserialize<test_b>(res);


	system("pause");

	std::vector<int> ve;

	return 0;
}

