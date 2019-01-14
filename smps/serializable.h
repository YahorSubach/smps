#pragma once
#include "smps_common.h"
#include "serializer.h"

#define VIRTUAL_SER_MEMBER(i) virtual bool ser_member(ser_field_ind_class<i>,std::vector<std::pair<std::string, std::string>>& serialization_vector){return false;} 
#define SER_MEMBER(i, name) void smps::serializable<i>::ser_mem_func(smps::ser_field_ind_class<i>, const smps::serializer ser) { smps::serializable<i-1>::ser_mem_func(smps::ser_field_ind_class<i-1>(), ser); serialization_vec.push_back(std::make_pair(std::string(#name), std::string(#name))); } 

namespace smps
{


	template <unsigned int n> class ser_field_ind_class {};

	template<unsigned int n> class serializable :public serializable<n - 1>
	{
	public:
		virtual void ser_mem_func(ser_field_ind_class<n> a, const serializer ser) = 0;
	};


	template<> class serializable<0> {
	protected:
		std::vector<std::pair<std::string, std::string>> serialization_vec;
		virtual void ser_mem_func(ser_field_ind_class<0>, const serializer ser) {};
	};


	/*class serializable
	{
	public:

		VIRTUAL_SER_MEMBER(0);
		VIRTUAL_SER_MEMBER(1);
		VIRTUAL_SER_MEMBER(2);
		VIRTUAL_SER_MEMBER(3);
		VIRTUAL_SER_MEMBER(4);
		VIRTUAL_SER_MEMBER(5);
		VIRTUAL_SER_MEMBER(6);
		VIRTUAL_SER_MEMBER(7);
		VIRTUAL_SER_MEMBER(8);
		VIRTUAL_SER_MEMBER(9);
		VIRTUAL_SER_MEMBER(10);
		VIRTUAL_SER_MEMBER(11);
		VIRTUAL_SER_MEMBER(12);
		VIRTUAL_SER_MEMBER(13);
		VIRTUAL_SER_MEMBER(14);
		VIRTUAL_SER_MEMBER(15);
		VIRTUAL_SER_MEMBER(16);
		VIRTUAL_SER_MEMBER(17);
		VIRTUAL_SER_MEMBER(18);
		VIRTUAL_SER_MEMBER(19);

		virtual ~serializable();
	};
	inline serializable::~serializable() {}*/
}
