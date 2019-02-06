#ifndef SMPS_STRING_ACCUMULATOR_H_
#define SMPS_STRING_ACCUMULATOR_H_

#include "Serializer.h"
#include "string_field_Serializer.h"

namespace smps 
{

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