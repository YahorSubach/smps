// smps.cpp : Defines the entry point for the console application.
//

#include "json.h"
#include <iostream>

//#include "smps_class_wrappers.h"

//class test_a :public smps::FieldComposition
//{
//private:
//	int test_int;
//	double test_double;
//	std::string test_string;
//	std::vector<int> test_vec;
//
//public:
//
//	test_a() : test_int(123), test_double(123.123), test_string("st ass")
//	{
//		test_vec.push_back(1);
//		test_vec.push_back(22);
//		test_vec.push_back(333);
//	}
//
//	SMPS_FIELDS(4)
//		SMPS_FIELD(1, test_int);
//	SMPS_FIELD(2, test_double);
//	SMPS_FIELD(3, test_string);
//	SMPS_FIELD(4, test_vec);
//};
//
//
//class test_b :public smps::FieldComposition
//{
//public:
//	int test_int;
//	double test_double;
//	std::string test_string;
//	test_a test_test_a;
//	std::vector<int> test_vec;
//	std::map<int, std::vector<int>> test_map;
//public:
//	test_b() : test_int(456), test_double(456.456), test_string("i cock prosto")
//	{
//	}
//
//	SMPS_FIELDS(6)
//		SMPS_FIELD(1, test_int)
//		SMPS_FIELD(2, test_double)
//		SMPS_FIELD(3, test_string)
//		SMPS_FIELD(4, test_test_a)
//		SMPS_FIELD(5, test_vec)
//		SMPS_FIELD(6, test_map)
//};
//
//
//class col_comp :public smps::FieldComposition
//{
//public:
//	std::map<int, std::vector<int>> test_map;
//	SMPS_FIELDS(1)
//		SMPS_FIELD(1, test_map)
//};
//
//
//template<class FirstType, class SecondType>
//class pair :public std::pair<FirstType, SecondType>
//{
//public:
//	pair(std::pair<FirstType, SecondType> stl_pair) :std::pair<FirstType, SecondType>(stl_pair) {}
//
//	FirstType GetFirst() { return std::pair<FirstType, SecondType>::first; }
//	void SetFirst(FirstType val) { std::pair<FirstType, SecondType>::second = val; }
//
//	SMPS_FIELDS(1)
//		SMPS_FIELD_EXT(1, first, GetFirst, SetFirst)
//
//};


int main()
{
	std::string res;
	smps::json::JSONSerializer::Serialize(res, "123123");
	std::string des = 0;
	smps::json::JSONSerializer::Deserialize(res, des);

	/*smps::SerializationTypeSelector<smps::ConstAndRefRemover<const std::vector<int>>::ClearType>::SerializationType tt;

	test_a a;


	std::string res;
	smps::string_serializer::IndentionStringAccumulator accum;

	smps::json::JSONFormattedSerializer::Serialize(accum, a);
	smps::json::JSONSerializer::Serialize(res, a);*/

	//test_b b;

	//std::vector<int> vec;
	//vec.push_back(1);
	//vec.push_back(2);
	//vec.push_back(3);
	//b.test_map[10] = vec;
	//vec.push_back(10);
	//b.test_map[20] = vec;
	//vec[2] = 999;
	//b.test_map[999] = vec;

	//std::string res2 = smps::json_Serializer::Serialize(b);

	//test_b desb = smps::json_Serializer::Deserialize<test_b>(res2);



	//col_comp b;

	//std::vector<int> vec;
	//vec.push_back(1);
	//vec.push_back(2);
	//vec.push_back(3);
	//b.test_map[10] = vec;
	//vec.push_back(10);
	//b.test_map[20] = vec;
	//vec[2] = 999;
	//b.test_map[999] = vec;

	//std::string res2 = smps::json_Serializer::Serialize(b);

	//col_comp desb = smps::json_Serializer::Deserialize<col_comp>(res2);




	system("pause");
	return 0;
}

