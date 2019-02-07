#ifndef SMPS_STRING_SERIALIZER_H_
#define SMPS_STRING_SERIALIZER_H_
#include "smps_common.h"

namespace smps
{
	class NativeTypeStringSerializer
	{
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
}




#endif //SMPS_STRING_SERIALIZER_H_