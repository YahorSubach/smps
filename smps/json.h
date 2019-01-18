#pragma once
#include "string_accumulator.h"

namespace smps
{
	class json_accumulator :public string_accumulator<serializer<json_accumulator>>
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

	typedef serializer<json_accumulator> json_serializer;
}