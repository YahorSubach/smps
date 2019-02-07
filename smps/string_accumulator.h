#ifndef SMPS_STRING_ACCUMULATOR_H_
#define SMPS_STRING_ACCUMULATOR_H_

#include "Serializer.h"
#include "string_field_Serializer.h"

namespace smps
{

	template<class StringSerializerImplementation, class StringConstantProvider>
	class FieldCompositionStringAccumulator
	{
		bool is_empty;
	public:

		template<class FieldCompositionObjectType, int field_num>
		void Add(FieldCompositionObjectType& obj)
		{
			if (!is_empty)
				result += StringConstantProvider::separator;
			result += StringSerializerImplementation::Serialize(FieldCompositionObjectType::FieldAccessor<field_num>::GetName());
			result += StringConstantProvider::associator;
			result += StringSerializerImplementation::Serialize(FieldCompositionObjectType::FieldAccessor<field_num>::GetValue());
		}
		std::string Result()
		{
			return StringConstantProvider::prefix + result + StringConstantProvider::postfix;
		}
	};


	template<class StringSerializerImplementation, class StringConstantProvider, class FieldCompositionType>
	class FieldCompositionStringTypedSplitter
	{
		FieldCompositionType result;
		std::map<std::string, std::string> splitted_serialized_object;
	public:
		FieldCompositionStringTypedSplitter(std::string serialized_object)
		{
			std::string key;
			std::string value;

			int last_ind = 1;

			int sub_obj_counter = 0;
			bool is_str = false;

			for (int str_ind = last_ind; str_ind < serialized_object.length() - 1; str_ind++)
			{
				if (serialized_object[str_ind] == '\"')
					is_str = !is_str;

				if (serialized_object[str_ind] == '{' || serialized_object[str_ind] == '[')
					sub_obj_counter++;
				if (serialized_object[str_ind] == '}' || serialized_object[str_ind] == ']')
					sub_obj_counter--;


				if (serialized_object[str_ind] == ':' && sub_obj_counter == 0 && !is_str)
				{
					key = serialized_object.substr(last_ind, str_ind - last_ind);
					last_ind = str_ind + 1;
				}
				if (serialized_object[str_ind] == ',' && sub_obj_counter == 0 && !is_str)
				{
					value = serialized_object.substr(last_ind, str_ind - last_ind);
					last_ind = str_ind + 1;
					deser_map[key] = value;
				}
			}

			if (last_ind < serialized_object.length() - 2)
			{
				value = serialized_object.substr(last_ind, serialized_object.length() - 1 - last_ind);
				deser_map[key] = value;
			}
		}
		template<int field_num>
		void Restore()
		{
			FieldCompositionObjectType::FieldAccessor<field_num>::SetName(&result, splitted_serialized_object[FieldCompositionObjectType::FieldAccessor<field_num>::GetName()]);
		}
		FieldCompositionType Result() { return result; }
	};

	template<class StringSerializerImplementation, class StringConstantProvider>
	class FieldCompositionStringSplitter
	{
	private:
		FieldCompositionStringSplitter();
	public:
		template<class FieldCompositionType>
		static FieldCompositionStringTypedSplitter<StringSerializerImplementation, StringConstantProvider, FieldCompositionType> GetTypedSplitter(std::string& serialized_object)
		{
			return FieldCompositionStringTypedSplitter<StringSerializerImplementation, StringConstantProvider, FieldCompositionType>(serialized_object);
		}
	};

	template<class StringSerializerImplementation>
	class CollectionStringAccumulator
	{
		bool is_empty;
	public:

		template<class ValueType>
		void Add(ValueType& value)
		{
			if (!is_empty)
				result += StringConstantProvider::separator;
			result += StringSerializerImplementation::Serialize(value);
		}
		std::string Result()
		{
			return StringConstantProvider::prefix + result + StringConstantProvider::postfix;
		}
	};




	template<class SerializerImplementation>
	class StringAccumulator
	{
	protected:
		std::vector<std::pair<std::string, std::string>> ser_vec;
	public:
		template<class T>
		void add_field(std::string field_name, const T& field)
		{
			ser_vec.push_back(std::make_pair(field_name, StringFieldSerializer<SerializerImplementation>::Serialize(field)));
		}
		virtual std::string Result() = 0;
	};

	template<class SerializerImplementation>
	class StringParser
	{
	protected:
		std::string serialized_obj;
		std::map<std::string, std::string> deser_map;
	public:
		StringParser(std::string str) :serialized_obj(str) {}
		virtual void Split() = 0;

		template<class serializable_type, int field_ind>
		void get_field(serializable_type& obj)
		{
			std::map<std::string, std::string>::iterator it;
			it = deser_map.find(serializable_type::FieldAccessor<field_ind>::GetName());
			if (it != deser_map.end())
			{
				typedef decltype(serializable_type::FieldAccessor<field_ind>::GetField(&obj)) field_type;
				decltype(serializable_type::FieldAccessor<field_ind>::GetField(&obj)) field_value = StringFieldSerializer<SerializerImplementation>::template Deserialize<decltype(serializable_type::FieldAccessor<field_ind>::GetField(&obj))>(it->second);
				serializable_type::FieldAccessor<field_ind>::SetField(&obj, field_value);
			}
		}
	};
}
#endif  // SMPS_STRING_ACCUMULATOR_H_