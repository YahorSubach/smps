#ifndef SMPS_STRING_SERIALIZER_H_
#define SMPS_STRING_SERIALIZER_H_
#include "smps_common.h"

namespace smps
{
	namespace string_serializer
	{
		class StringUtil
		{
		public:
			static std::string_view Trim(std::string_view original)
			{
				int begin_cnt = 0;
				for (; begin_cnt < original.length() && (original[begin_cnt] == ' ' || original[begin_cnt] == '\t'); begin_cnt++);

				int end_cnt = 0;
				for (; end_cnt < original.length() && (original.length() - end_cnt > 0) && (original[original.length() - end_cnt - 1] == ' ' || original[original.length() - end_cnt - 1] == '\t'); end_cnt++);

				original.remove_prefix(begin_cnt);
				original.remove_suffix(end_cnt);

				return original;
			}
		};

		template<class SerializationDestination, class DeserializationSource>
		class GeneralSerializer
		{
		public:
			template< class Type>
			static void Serialize(SerializationDestination& dest, Type&& obj)
			{
				dest.append(std::to_string(obj));
			}

			template<>
			static void Serialize(SerializationDestination& dest, const std::string&& obj)
			{
				dest.append("\"");
				dest.append(obj);
				dest.append("\"");
			}


			template<class Type>
			static void Deserialize(DeserializationSource& src, Type&& obj)
			{
				std::stringstream ss(src);
				ss >> obj;
			}

			template<>
			static void Deserialize(DeserializationSource& src, const std::string&& obj)
			{
				std::string_view sview = StringUtil::Trim(src);

				assert(sview.front() == sview.back() == '"');
				sview.remove_prefix(1);
				sview.remove_suffix(1);

				obj = sview;
			}
		};


	}
}
#endif //SMPS_STRING_SERIALIZER_H_