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

	template<class serializer_implementation >
	class string_parser
	{
	protected:
		std::string serialized_obj;
		std::map<std::string, std::string> deser_map;
	public:
		string_parser(std::string str) :serialized_obj(str){}
		virtual void Split() = 0;

		template<class serializable_type, int field_ind>
		void get_field(serializable_type& obj)
		{
			std::map<std::string, std::string>::iterator it;
			it = deser_map.find(serializable_type::field_accessor<field_ind>::GetNames());
			if (deser_map.find())
				serializable_type::field_accessor<field_ind>::SetField(obj, string_field_serializer<IS_SER_FIELD_IMPL(T), serializer_implementation>::Deserialize(it->second));
		}
	};


}