// smps.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "serializer.h"
#include "serializable.h"
#include "json.h"
#include <iostream>

class test_a
{
private:
	int test_int;
	double test_double;
	std::string test_string;

public:
	test_a(): test_int(123), test_double(123.123),test_string("st ass")
	{}

	
	SMP_SERIALIZABLE(1)
	SER_MEMBER(1, test_int);
	//SER_MEMBER(2, test_double);
	//SER_MEMBER(3, test_string);


};


//class test_b/* : public smps::serializable<4>*/
//{
//private:
//	int test_int;
//	double test_double;
//	std::string test_string;
//	test_a test_test_a;
//public:
//	test_b() : test_int(456), test_double(456.456), test_string("i cock prosto")
//	{}
//
//	SER_MEMBER(1, test_int);
//	SER_MEMBER(2, test_double);
//	SER_MEMBER(3, test_string);
//	SER_MEMBER(4, test_test_a);
//};
//


//class t
//{
//private:
//	int a;
//public:
//
//	class tt {
//	public:
//		static decltype(a) f() { return 1; }
//	};
//
//
//	template<class T> T g()
//	{
//		return a;
//	}
//};


int main()
{
	test_a a;

	smps::json_accumulator ac;

	std::string res = smps::json_smps<test_a>::Serialize(a);

	

	system("pause");

	std::vector<int> ve;

	return 0;
}

