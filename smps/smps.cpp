// smps.cpp : Defines the entry point for the console application.
//

#include "json.h"
#include <iostream>
#include <algorithm>

class test_a :public smps::SerializableField
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


class test_b :public smps::SerializableField
{
private:
	int test_int;
	double test_double;
	std::string test_string;
	test_a test_test_a;
	std::vector<int> test_vec;
public:
	test_b() : test_int(456), test_double(456.456), test_string("i cock prosto")
	{
		test_vec.push_back(100);
		test_vec.push_back(200);
		test_vec.push_back(300);
	}

	SMPS_FIELDS(5)
	SMPS_FIELD(1, test_int);
	SMPS_FIELD(2, test_double);
	SMPS_FIELD(3, test_string);
	SMPS_FIELD(4, test_test_a);
	SMPS_FIELD(5, test_vec);
};


int main()
{
	test_b a;

	std::string res = smps::json_Serializer::Serialize(a);
	test_b aa = smps::json_Serializer::Deserialize<test_b>(res);

	std::sort()

	system("pause");

	std::vector<int> ve;

	return 0;
}

