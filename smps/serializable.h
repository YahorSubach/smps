#pragma once
#include "smps_common.h"
#include "serializer.h"



#define VIRTUAL_SER_MEMBER(i) virtual bool ser_member(ser_field_ind_class<i>,std::vector<std::pair<std::string, std::string>>& serialization_vector){return false;} 
#define SER_MEMBER(i, name) std::vector<std::pair<std::string, std::string>>* smps::serializable<i>::ser_mem_func(smps::ser_field_ind_class<i> t) \
	{ \
		((smps::serializable<i-1>*)(this))->ser_mem_func(smps::ser_field_ind_class<i-1>()); \
		serialization_vec.push_back(std::make_pair(std::string(#name), smps::str_maker<sizeof(smps::Test(name))==sizeof(smps::Small)>::make_string(name))); return &serialization_vec; \
	} 

namespace smps
{
	typedef char Small;
	class Big { char dummy[2]; };



	template <unsigned int n> class ser_field_ind_class {};

	template<unsigned int n> class serializable :public serializable<n - 1>
	{
	public:
		virtual std::vector<std::pair<std::string, std::string>>* ser_mem_func(ser_field_ind_class<n>) = 0;
	};


	template<> class serializable<0> {
	protected:
		mutable std::vector<std::pair<std::string, std::string>> serialization_vec;
	public:
		virtual std::vector<std::pair<std::string, std::string>>* ser_mem_func(ser_field_ind_class<0>) { return &serialization_vec; };
	};

	//template<> class serializable<1> {
	//protected:
	//	mutable std::vector<std::pair<std::string, std::string>> serialization_vec;
	//	virtual std::vector<std::pair<std::string, std::string>>* ser_mem_func(ser_field_ind_class<0>) { return &serialization_vec; };
	//};



	template<int n>
	Small Test(const serializable<n>&);

	Big Test(...);


	template<bool b = true>
	class str_maker;

	template<>
	class str_maker<true> {
	public:
		template<int n>
		static std::string make_string(serializable<n>& ob) {


			auto vec_ptr = ob.ser_mem_func(ser_field_ind_class<n>());
			return str_maker<false>::make_string(*vec_ptr);
		}
	};

	template<>
	class str_maker<false>
	{
	public:
		template<class T>
		static std::string make_string(const T& ob) { return std::to_string(ob); }

		template<>
		static std::string make_string(const std::string &ob) { return "\"" + ob + "\""; }


		static std::string make_string(const std::vector<std::pair<std::string, std::string>>& ser_vec)
		{
			std::stringstream ss;
			ss << "{";

			bool first = true;

			for (auto p : ser_vec)
			{
				if (!first)
					ss << ",";
				first = false;
				ss << "\"" << p.first << "\":" << p.second;
			}

			ss << "}";
			return ss.str();
		}

	};
}
