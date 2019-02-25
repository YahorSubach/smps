#ifndef SMPS_STRING_SERIALIZER_H_
#define SMPS_STRING_SERIALIZER_H_
#include "smps_common.h"

namespace smps
{
	namespace string_serializer
	{
		template<class StringAccumulator>
		class BaseClassSerializer
		{
		public:
			template<class Type>
			static void Serialize(StringAccumulator& accum, const Type& obj)
			{
				accum += std::to_string(obj);
			}

			template<>
			static void Serialize(StringAccumulator& accum, const std::string& obj)
			{
				accum += "\"";
				accum += obj;
				accum += "\"";
			}
		};

		class StringIterpreter
		{
		public:
			typedef std::string ResultType;
		};


		class IndentionStringAccumulator
		{
		private:
			std::string result_;
		public:
			typedef std::string ResultType;
			int indention_size = 0;
			void  operator+=(std::string& ser_obj)
			{
				result_ += ser_obj;
			}
			void  operator+=(const char* ser_obj)
			{
				result_ += ser_obj;
			}

			friend class BaseClassIndentionSerializer;
		};

		class BaseClassIndentionSerializer
		{
		public:
			template<class Type>
			static void Serialize(IndentionStringAccumulator& accum, const Type& obj)
			{
				accum.result_ += std::to_string(obj);
			}

			template<>
			static void Serialize(IndentionStringAccumulator& accum, const std::string& obj)
			{
				accum += "\"";
				accum.result_ += obj;
				accum += "\"";
			}
		};

	}
}
#endif //SMPS_STRING_SERIALIZER_H_