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


		template<class StringAccumulator, class StringPresentor>
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

			template<class Type>
			static void Deserialize(StringPresentor& presentor, const Type& obj)
			{
				std::string_view sview = presentor.Read();
				std::stringstream ss(sview);
				ss >> obj;
			}

			template<>
			static void Deserialize(StringPresentor& accum, const std::string& obj)
			{
				std::string_view sview = StringUtil::Trim(presentor.Read());

				assert(sview.front() == sview.back() == '"');
				sview.remove_prefix(1);
				sview.remove_suffix(1);

				obj = sview;
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