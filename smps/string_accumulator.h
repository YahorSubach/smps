#pragma once
#include "serializer.h"

namespace smps
{
	template<bool is_serializable_implementation, class serializer_implementation>
	class string_field_serializer;

	template<class serializer_implementation>
	class string_field_serializer<true, serializer_implementation>
	{
	public:
		template<class T>
		static std::string Serialize(const T& field)
		{
			return serializer_implementation::Serialize(field);
		}
	};

	template<class serializer_implementation>
	class string_field_serializer<false, serializer_implementation>
	{
	public:
		template<class T>
		static std::string Serialize(const T& field)
		{
			return std::to_string(field);
		}
		template<>
		static std::string Serialize(const std::string& field)
		{
			return "\"" + field + "\"";
		}
	};





	template<class serializer_implementation>
	class string_accumulator
	{
	protected:
		std::vector<std::pair<std::string, std::string>> ser_vec;
	public:
		template<class T>
		void add_field(std::string field_name, const T& field) 
		{ 
			ser_vec.push_back(std::make_pair(field_name, string_field_serializer<IS_SER_FIELD_IMPL(T), serializer_implementation>::Serialize(field)));
		}
		virtual std::string Result() = 0;
	};
}