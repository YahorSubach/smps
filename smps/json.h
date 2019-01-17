#pragma once
#include "serialization_accumulator.h"

namespace smps
{
	class json_field_serializer
	{
	public:
		template<class T>
		static std::pair<std::string, std::string> serialize(const std::string& name, const T& n) { return std::make_pair("\"" + name + "\"", std::to_string(n)); }
	};

	class json_accumulator :public serialization_accumulator<std::pair<std::string, std::string>, std::string>
	{
	protected:
		std::vector<std::pair<std::string, std::string>> ser_vec;
	public:
		void add_field(const std::pair<std::string, std::string>& field) { ser_vec.push_back(field); }
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

	template<class serialization_obj_type>
	class json_smps :public obj_serializer<std::string, json_field_serializer, json_accumulator, serialization_obj_type>
	{};
}