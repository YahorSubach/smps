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
		template<class T>
		static T Deserialize(const std::string& ser_field)
		{
			return serializer_implementation::Deserialize<T>(ser_field);
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
		template<class T>
		static T Deserialize(const std::string& ser_field)
		{
			std::istringstream  ss(ser_field);
			T res;
			ss >> res;
			return res;
		}
		template<>
		static std::string Deserialize(const std::string& ser_field)
		{

			return ser_field.substr(1, ser_field.length() - 2);
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

	template<class serializer_implementation>
	class string_parser
	{
	protected:
		std::string serialized_obj;
		std::map<std::string, std::string> deser_map;
	public:
		string_parser(std::string str) :serialized_obj(str) {}
		virtual void Split() = 0;

		template<class serializable_type, int field_ind>
		void get_field(serializable_type& obj)
		{
			std::map<std::string, std::string>::iterator it;
			it = deser_map.find(serializable_type::field_accessor<field_ind>::GetName());
			if (it != deser_map.end())
			{
				typedef decltype(serializable_type::field_accessor<field_ind>::GetField(&obj)) field_type;
				field_type field_value = string_field_serializer<IS_SER_FIELD_IMPL(field_type), serializer_implementation>::Deserialize<field_type>(it->second);
				serializable_type::field_accessor<field_ind>::SetField(&obj, field_value);
			}
		}
	};


}