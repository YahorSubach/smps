// smps.cpp : Defines the entry point for the console application.
//

#include "json.h"
#include <iostream>

//#include "smps_class_wrappers.h"

class test_a :public smps::Serializable
{
private:
	int test_int;
	double test_double;
	std::string test_string;
	std::vector<int> test_vec;

public:

	test_a() : test_int(123), test_double(123.123), test_string("st ass \"cock\"")
	{
		test_vec.push_back(1);
		test_vec.push_back(22);
		test_vec.push_back(333);
	}

	SMPS_FIELDS(4)
		SMPS_FIELD(1, test_int);
	SMPS_FIELD(2, test_double);
	SMPS_FIELD(3, test_string);
	SMPS_FIELD(4, test_vec);
};


class test_b :public smps::Serializable
{
public:
	int test_int;
	double test_double;
	std::string test_string;
	test_a test_test_a;
	std::vector<int> test_vec;
	std::map<int, std::vector<int>> test_map;
public:

	static test_a& F(test_b& obj)
	{
		return obj.test_test_a;
	}

	test_b() : test_int(456), test_double(456.456), test_string("i cock prosto")
	{
		test_vec.push_back(123);
		test_vec.push_back(456);
		test_map[100] = { 10,20,30 };
		test_map[200] = { 100,200,300 };
		test_map[300] = { 120,220,320 };
	}

	SMPS_FIELDS(6)
		SMPS_FIELD(1, test_int)
		SMPS_FIELD(2, test_double)
		SMPS_FIELD(3, test_string)
		SMPS_FIELD(4, test_test_a)
		SMPS_FIELD(5, test_vec)
		SMPS_FIELD(6, test_map)
};


std::pair<const int, int>&& F()
{
	return std::make_pair(1, 2);
}

int main()
{
	std::map<int, int> mm;
	auto p = std::make_pair(1, 2);
	mm.insert(p);

	smps::json::FormattedJSONString form_res;
	test_b obj;
	test_b des_obj;

	smps::json::JSONSerializer::Serialize(form_res, obj);
	smps::json::JSONSerializer::Deserialize(form_res.value, des_obj);

	std::cout << form_res.value;

	//smps::json::JSONSerializer::Serialize(form_res, aaa);
	//smps::json::JSONSerializer::Deserialize(res, tdes);

	using ttt = decltype(smps::CheckSMPSObjectGhostFunc(1));


	std::string test_str = "  [  field_a : \"1.23\" , field_b:  [1 , 23 , 5] ] ";
	//smps::json::JSONBlockReader reader(test_str.c_str(), test_str.length());

	std::string s = "abracadabra";
	const std::string& sr = s;
	int sss = 1;

	//smps::string_serializer::GeneralSerializer<std::string, smps::json::JSONBlockReader>::Serialize(res, s);
	//smps::string_serializer::GeneralSerializer<std::string, smps::json::JSONBlockReader>::Serialize(res, sr);
	//smps::string_serializer::GeneralSerializer<std::string, smps::json::JSONBlockReader>::Serialize(res, std::string("123"));



	auto& mr = smps::stl::Mapper::Map(std::make_pair(1, 2));


	/*
		smps::json::JSONSerializer::Serialize(res, s);
		smps::json::JSONSerializer::Serialize(res, std::string("aaaa"));*/

		//std::string des = 0;
		//smps::json::JSONSerializer::Deserialize(res, des);
	int a = 1;
	int& b = a;
	int c = 2;
	b = c;
	a = 3;
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

	return 0;
}

