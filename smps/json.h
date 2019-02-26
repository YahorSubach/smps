#ifndef SMPS_JSON_H_
#define SMPS_JSON_H_

#include "recursive_serializer.h"
#include "string_serializer.h"
namespace smps
{
	namespace json
	{
		typedef smps::string_serializer::IndentionStringAccumulator FormattedAccum;

		class JSONCollectionDecorator
		{
		public:
			static void DecorateCollectionBegin(std::string& accum)
			{
				accum += "[";
			}
			static void DecorateCollectionEnd(std::string& accum)
			{
				accum += "]";
			}
			static void DecorateCollectionElementSplit(std::string& accum)
			{
				accum += ",";
			}
		};

		class JSONFieldsContainerDecorator
		{
		public:
			static void DecorateContainerBegin(std::string& accum)
			{
				accum += "{";
			}
			static void DecorateContainerEnd(std::string& accum)
			{
				accum += "}";
			}
			static void DecorateContainerElementSplit(std::string& accum)
			{
				accum += ",";
			}
			static void DecorateFieldHeader(std::string& accum, int field_index, std::string& field_name)
			{
				accum += field_name;
				accum += ":";
			}
		};


		class JSONFormattedCollectionDecorator
		{
		public:
			static void DecorateCollectionBegin(FormattedAccum& accum)
			{
				accum += "\n";
				accum.indention_size++;
				for (size_t i = 0; i < accum.indention_size; i++)
					accum += "\t";
				accum += "[";
				accum.indention_size++;
				accum += "\n";
				for (size_t i = 0; i < accum.indention_size; i++)
					accum += "\t";
			}
			static void DecorateCollectionEnd(FormattedAccum& accum)
			{
				accum += "\n";
				accum.indention_size--;
				for (size_t i = 0; i < accum.indention_size; i++)
					accum += "\t";
				accum += "]";
				accum += "\n";
				accum.indention_size--;
				for (size_t i = 0; i < accum.indention_size; i++)
					accum += "\t";
			}
			static void DecorateCollectionElementSplit(FormattedAccum& accum)
			{
				accum += ",";
				accum += "\n";
				for (size_t i = 0; i < accum.indention_size; i++)
					accum += "\t";
			}
		};

		class JSONFormattedFieldsContainerDecorator
		{
		public:
			static void DecorateContainerBegin(FormattedAccum& accum)
			{
				accum += "{";
				accum.indention_size++;
				accum += "\n";
				for (size_t i = 0; i < accum.indention_size; i++)
					accum += "\t";
			}
			static void DecorateContainerEnd(FormattedAccum& accum)
			{
				accum += "\n";
				accum.indention_size--;
				for (size_t i = 0; i < accum.indention_size; i++)
					accum += "\t";
				accum += "}";
			}
			static void DecorateContainerElementSplit(FormattedAccum& accum)
			{
				accum += ",";
				accum += "\n";
				for (size_t i = 0; i < accum.indention_size; i++)
					accum += "\t";
			}
			static void DecorateFieldHeader(FormattedAccum& accum, int field_index, std::string& field_name)
			{
				accum += field_name;
				accum += ":";
			}
		};



		typedef ExplicitRecursiveSerializerApplicator<std::string, string_serializer::StringIterpreter, string_serializer::BaseClassSerializer<std::string>, JSONCollectionDecorator, JSONFieldsContainerDecorator> JSONApplicator;
		typedef ExplicitRecursiveSerializerApplicator<FormattedAccum, string_serializer::StringIterpreter, string_serializer::BaseClassIndentionSerializer, JSONFormattedCollectionDecorator, JSONFormattedFieldsContainerDecorator> JSONFormattedApplicator;
		typedef Serializer<JSONApplicator> JSONSerializer;
		typedef Serializer<JSONFormattedApplicator> JSONFormattedSerializer;
	}


	//class JsonParser;

	//class JsonAccumulator :public StringAccumulator<Serializer<JsonAccumulator, JsonParser>>
	//{
	//public:
	//	std::string Result()
	//	{
	//		std::stringstream ss;
	//		ss << "{";
	//		bool is_first_field = true;
	//		for (auto p : ser_vec)
	//		{
	//			if (!is_first_field)
	//				ss << ",";
	//			is_first_field = false;
	//			ss << p.first;
	//			ss << ":";
	//			ss << p.second;
	//		}
	//		ss << "}";
	//		return ss.str();
	//	}
	//};

	//class JsonParser :public StringParser<Serializer<JsonAccumulator, JsonParser>>
	//{
	//public:
	//	JsonParser(std::string ser_obj) :StringParser<Serializer<JsonAccumulator, JsonParser>>(ser_obj) {}
	//	void Split() {
	//		std::string key;
	//		std::string value;

	//		int last_ind = 1;

	//		int sub_obj_counter = 0;
	//		bool is_str = false;

	//		for (int str_ind = last_ind; str_ind < serialized_obj.length() - 1; str_ind++)
	//		{
	//			if (serialized_obj[str_ind] == '\"')
	//				is_str = !is_str;

	//			if (serialized_obj[str_ind] == '{' || serialized_obj[str_ind] == '[')
	//				sub_obj_counter++;
	//			if (serialized_obj[str_ind] == '}' || serialized_obj[str_ind] == ']')
	//				sub_obj_counter--;


	//			if (serialized_obj[str_ind] == ':' && sub_obj_counter == 0 && !is_str)
	//			{
	//				key = serialized_obj.substr(last_ind, str_ind - last_ind);
	//				last_ind = str_ind + 1;
	//			}
	//			if (serialized_obj[str_ind] == ',' && sub_obj_counter == 0 && !is_str)
	//			{
	//				value = serialized_obj.substr(last_ind, str_ind - last_ind);
	//				last_ind = str_ind + 1;
	//				deser_map[key] = value;
	//			}
	//		}

	//		if (last_ind < serialized_obj.length() - 2)
	//		{
	//			value = serialized_obj.substr(last_ind, serialized_obj.length() - 1 - last_ind);
	//			deser_map[key] = value;
	//		}
	//	}
	//};


	//typedef Serializer<JsonAccumulator, JsonParser> json_Serializer;
}

#endif  // SMPS_JSON_H_