#pragma once
#include "string_accumulator.h"

namespace smps
{
	class json_parser;

	class json_accumulator :public string_accumulator<serializer<json_accumulator, json_parser>>
	{
	public:
		std::string Result()
		{
			std::stringstream ss;
			ss << "{";
			bool is_first_field = true;
			for (auto p : ser_vec)
			{
				if (!is_first_field)
					ss << ",";
				is_first_field = false;
				ss << p.first;
				ss << ":";
				ss << p.second;
			}
			ss << "}";
			return ss.str();
		}
	};

	class json_parser :public string_parser<serializer<json_accumulator, json_parser>>
	{
	public:
		json_parser(std::string ser_obj) :string_parser<serializer<json_accumulator, json_parser>>(ser_obj) {}
		void Split() {
			std::string key;
			std::string value;

			int last_ind = 1;

			int sub_obj_counter = 0;
			bool is_str = false;

			for (int str_ind = last_ind; str_ind < serialized_obj.length() - 1; str_ind++)
			{
				if (serialized_obj[str_ind] == '\"')
					is_str = !is_str;

				if (serialized_obj[str_ind] == '{' || serialized_obj[str_ind] == '[')
					sub_obj_counter++;
				if (serialized_obj[str_ind] == '}' || serialized_obj[str_ind] == ']')
					sub_obj_counter--;


				if (serialized_obj[str_ind] == ':' && sub_obj_counter == 0 && !is_str)
				{
					key = serialized_obj.substr(last_ind, str_ind - last_ind);
					last_ind = str_ind + 1;
				}
				if (serialized_obj[str_ind] == ',' && sub_obj_counter == 0 && !is_str)
				{
					value = serialized_obj.substr(last_ind, str_ind - last_ind);
					last_ind = str_ind + 1;
					deser_map[key] = value;
				}
			}

			if (last_ind < serialized_obj.length() - 2)
			{
				value = serialized_obj.substr(last_ind, serialized_obj.length() - 1 - last_ind);
				deser_map[key] = value;
			}
		}
	};


	typedef serializer<json_accumulator, json_parser> json_serializer;
}