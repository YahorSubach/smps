// smps.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "serializer.h"
#include "serializable.h"


class test : public smps::serializable<1>
{
	int a;
public:
	//void smps::serializable<1>::ser_mem_func(smps::ser_field_ind_class<1> a, const smps::serializer ser) {smps::ser_field_ind_class<0> b;  smps::serializable<0>::ser_mem_func(b, ser); serialization_vec.push_back(std::make_pair<std::string, std::string>(std::string("123"), std::string("123")));}
	//void smps::serializable<1>::ser_mem_func(smps::ser_field_ind_class<1>, const serializer ser) { smps::serializable<0>::ser_mem_func(smps::ser_field_ind_class<0>(), ser); serialization_vec.push_back(std::make_pair(std::string(""), std::string(""))); }
	SER_MEMBER(1, a);
	//SER_MEMBER(2, a);
	//SER_MEMBER(3, a);
	//SER_MEMBER(4, a);
	//SER_MEMBER(5, a);
	//SER_MEMBER(6, a);
	//SER_MEMBER(7, a);
	//SER_MEMBER(8, a);
	//SER_MEMBER(9, a);
	//SER_MEMBER(10, a);
};



int main()
{
	test ob;
	smps::serializer a;
	ob.ser_mem_func(smps::ser_field_ind_class<1>(), a);


	return 0;
}

