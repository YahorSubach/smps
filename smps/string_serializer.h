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

			static std::string Escape(const std::string& str)
			{
				std::string res;
				for (char c : str)
				{
					switch (c)
					{
					case '\\':
					case '"':
						res += '\\';
					default:
						res += c;
						break;
					}
				}
				return res;
			}

			static std::string Unescape(const std::string& str)
			{
				std::string res;
				char last_char = '\0';
				bool begin = true;
				for (char c : str)
				{
					switch (c)
					{
					case '\\':
					case '"':
						if (last_char == '\"')
							break;
					default:
						if (!begin)
							res += last_char;
						break;
					}
					last_char = c;
					begin = false;
				}
				if (!begin)
					res += last_char;
				return res;
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
			static void Serialize(SerializationDestination& dest, const std::string& obj)
			{
				dest.append("\"");
				dest.append(StringUtil::Escape(obj));
				dest.append("\"");
			}


			template<class Type>
			static void Deserialize(const DeserializationSource& src, Type& obj)
			{
				std::stringstream ss;
				ss << src.Read();
				ss >> obj;
			}

			template<>
			static void Deserialize(const DeserializationSource& src, std::string& obj)
			{

				assert(src.Read() == "\"");
				obj = StringUtil::Unescape(std::string(src.Read()));
				assert(src.Read() == "\"");
			}
		};

	}
}
#endif //SMPS_STRING_SERIALIZER_H_