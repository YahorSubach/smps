#ifndef SMPS_JSON_H_
#define SMPS_JSON_H_

//#include "recursive_serializer.h"
#include "serializer.h"
#include "string_serializer.h"
namespace smps
{
	namespace json
	{
		class JSONBlockReader
		{
		private:
			const std::string_view str_view_;
			size_t index_;

		public:
			JSONBlockReader(const char* begin, size_t count) :str_view_(std::string_view(begin, count)), index_(0) {
				//size_t trim_count = 0;
				//for (; trim_count < str_view_.length() && (str_view_[trim_count] == ' ' || str_view_[trim_count] == '\t'); trim_count++);
				//str_view_.remove_prefix(trim_count);

				//trim_count = 0;
				//for (; trim_count < str_view_.length() && (str_view_[str_view_.length() - trim_count - 1] == ' ' || str_view_[str_view_.length() - trim_count - 1] == '\t'); trim_count++);
				//str_view_.remove_suffix(trim_count);
			}

			JSONBlockReader(const std::string& str) :str_view_(std::string_view(str.c_str(), str.length())), index_(0) {}

			const std::string_view& value() { return str_view_; }

			std::string_view Read()
			{
				size_t begin_index = index_;
				bool string_reading = false;

				for (; index_ < str_view_.length(); index_++)
				{
					switch (str_view_[index_])
					{
					case ' ':
					case '\t':
						if (begin_index == index_)
							begin_index++;
						break;
					case '\"':
						if (!string_reading)
						{
							string_reading = true;
						}
						else
						{
							if (str_view_[index_ - 1] != '\"')
							{
								index_++;
								return str_view_.substr(begin_index, index_ - begin_index);
							}
						}
						break;
					case '[':
					case ']':
					case '{':
					case '}':
					case ':':
					case ',':
						if (!string_reading)
						{
							if (begin_index == index_)
								index_++;

							return str_view_.substr(begin_index, index_ - begin_index);
						}
						break;
					default:
						break;
					}
				}
				return str_view_.substr(begin_index, index_ - begin_index);
			}


			JSONBlockReader ReadBlock()
			{
				size_t begin_index = index_;
				bool string_reading = false;

				size_t square_brackets_level = 0;
				size_t figure_brackets_level = 0;

				for (; index_ < str_view_.length(); index_++)
				{
					switch (str_view_[index_])
					{
					case ' ':
					case '\t':
						if (begin_index == index_)
							begin_index++;
						break;
					case '\"':
						if (!string_reading)
						{
							string_reading = true;
						}
						else
						{
							if (str_view_[index_ - 1] != '\"')
							{
								if (square_brackets_level == 0 && figure_brackets_level == 0)
								{
									index_++;
									return JSONBlockReader(str_view_.data() + begin_index, index_ - begin_index);
								}
							}
						}
						break;
					case '[':
						if (!string_reading)
							square_brackets_level++;
						break;
					case ']':
						if (!string_reading)
							square_brackets_level--;
						if (square_brackets_level == 0 && figure_brackets_level == 0)
						{
							index_++;
							return JSONBlockReader(str_view_.data() + begin_index, index_ - begin_index);
						}
						break;
					case '{':
						if (!string_reading)
							figure_brackets_level++;
						break;
					case '}':
						if (!string_reading)
							figure_brackets_level--;
						if (square_brackets_level == 0 && figure_brackets_level == 0)
						{
							index_++;
							return JSONBlockReader(str_view_.data() + begin_index, index_ - begin_index);
						}
						break;
					case ':':
					case ',':
						if (!string_reading)
						{
							if (square_brackets_level == 0 && figure_brackets_level == 0)
							{
								if (begin_index == index_)
									index_++;
								return JSONBlockReader(str_view_.data() + begin_index, index_ - begin_index);
							}
						}
						break;
					default:
						break;
					}
				}
				return JSONBlockReader(str_view_.data() + begin_index, index_ - begin_index);
			}
		};


		using GenSer = string_serializer::GeneralSerializer<std::string, JSONBlockReader>;
		using JSONSerializer = Serializer< GenSer, GenSer, GenSer, GenSer>;

		//	struct JSONStringPresentor
		//	{
		//	protected:
		//		const std::string& str_;
		//		int index_;
		//		enum ReadMode
		//		{
		//			UNKNOWN,
		//			VALUE,
		//			STRING,
		//			COLLECTION,
		//			SUB_OBJECT
		//		};
		//	public:
		//		JSONStringPresentor(const std::string& str) :str_(str), index_(0) {}

		//		bool IsEnd()
		//		{
		//			return index_ >= str_.length();
		//		}

		//		std::string_view Read()
		//		{
		//			int start_index = index_;
		//			int counter = 0;
		//			ReadMode read_mode = ReadMode::UNKNOWN;
		//			for (;; index_++)
		//			{
		//				assert(index_ < str_.length());

		//				if (str_[index_] == ' ' || str_[index_] == '\t')
		//					continue;

		//				if (str_[index_] == '"')
		//				{
		//					if (read_mode == ReadMode::UNKNOWN)
		//					{
		//						start_index = index_;
		//						read_mode = ReadMode::STRING;
		//					}
		//					else if (read_mode == ReadMode::STRING)
		//					{
		//						index_++;
		//						return std::string_view(str_.c_str() + start_index, (size_t)(index_ - start_index));
		//					}
		//				}
		//				else if (str_[index_] == '[' && )
		//				{
		//					if (read_mode == ReadMode::UNKNOWN)
		//					{
		//						read_mode = ReadMode::COLLECTION;
		//					}
		//					else if(read_mode == ReadMode::COLLECTION)
		//					start_index = index_;
		//				}
		//				else if (str_[index_] == ']' && read_mode == ReadMode::COLLECTION)
		//				{
		//					index_++;
		//					return std::string_view(str_.c_str() + start_index, (size_t)(index_ - start_index));
		//				}
		//				else if (str_[index_] == '{' && read_mode == ReadMode::UNKNOWN)
		//				{
		//					read_mode = ReadMode::SUB_OBJECT;
		//					start_index = index_;
		//				}
		//				else if (str_[index_] == '}' && read_mode == ReadMode::SUB_OBJECT)
		//				{
		//					index_++;
		//					return std::string_view(str_.c_str() + start_index, (size_t)(index_ - start_index));
		//				}
		//				else if (str_[index_] == ',' && read_mode == ReadMode::VALUE)
		//				{
		//					index_++;
		//					return std::string_view(str_.c_str() + start_index, (size_t)(index_ - start_index - 1));
		//				}
		//				else if (read_mode == ReadMode::UNKNOWN)
		//					read_mode = ReadMode::VALUE;
		//			}
		//		}

		//	};

		//	typedef smps::string_serializer::IndentionStringAccumulator FormattedAccum;

		//	class JSONCollectionDecorator
		//	{
		//	public:
		//		static void DecorateCollectionBegin(std::string& accum)
		//		{
		//			accum += "[";
		//		}
		//		static void DecorateCollectionEnd(std::string& accum)
		//		{
		//			accum += "]";
		//		}
		//		static void DecorateCollectionElementSplit(std::string& accum)
		//		{
		//			accum += ",";
		//		}
		//		
		//		static void UndecorateCollectionBegin(JSONStringPresentor& presentor)
		//		{
		//			presentor.Read();
		//		}
		//		static void UndecorateCollectionEnd(JSONStringPresentor& accum)
		//		{
		//			accum += "]";
		//		}
		//		static void DecorateCollectionElementSplit(std::string& accum)
		//		{
		//			accum += ",";
		//		}
		//	};

		//	class JSONFieldsContainerDecorator
		//	{
		//	public:
		//		static void DecorateContainerBegin(std::string& accum)
		//		{
		//			accum += "{";
		//		}
		//		static void DecorateContainerEnd(std::string& accum)
		//		{
		//			accum += "}";
		//		}
		//		static void DecorateContainerElementSplit(std::string& accum)
		//		{
		//			accum += ",";
		//		}
		//		static void DecorateFieldHeader(std::string& accum, int field_index, std::string& field_name)
		//		{
		//			accum += field_name;
		//			accum += ":";
		//		}
		//	};


		//	class JSONFormattedCollectionDecorator
		//	{
		//	public:
		//		static void DecorateCollectionBegin(FormattedAccum& accum)
		//		{
		//			accum += "\n";
		//			accum.indention_size++;
		//			for (size_t i = 0; i < accum.indention_size; i++)
		//				accum += "\t";
		//			accum += "[";
		//			accum.indention_size++;
		//			accum += "\n";
		//			for (size_t i = 0; i < accum.indention_size; i++)
		//				accum += "\t";
		//		}
		//		static void DecorateCollectionEnd(FormattedAccum& accum)
		//		{
		//			accum += "\n";
		//			accum.indention_size--;
		//			for (size_t i = 0; i < accum.indention_size; i++)
		//				accum += "\t";
		//			accum += "]";
		//			accum += "\n";
		//			accum.indention_size--;
		//			for (size_t i = 0; i < accum.indention_size; i++)
		//				accum += "\t";
		//		}
		//		static void DecorateCollectionElementSplit(FormattedAccum& accum)
		//		{
		//			accum += ",";
		//			accum += "\n";
		//			for (size_t i = 0; i < accum.indention_size; i++)
		//				accum += "\t";
		//		}
		//	};

		//	class JSONFormattedFieldsContainerDecorator
		//	{
		//	public:
		//		static void DecorateContainerBegin(FormattedAccum& accum)
		//		{
		//			accum += "{";
		//			accum.indention_size++;
		//			accum += "\n";
		//			for (size_t i = 0; i < accum.indention_size; i++)
		//				accum += "\t";
		//		}
		//		static void DecorateContainerEnd(FormattedAccum& accum)
		//		{
		//			accum += "\n";
		//			accum.indention_size--;
		//			for (size_t i = 0; i < accum.indention_size; i++)
		//				accum += "\t";
		//			accum += "}";
		//		}
		//		static void DecorateContainerElementSplit(FormattedAccum& accum)
		//		{
		//			accum += ",";
		//			accum += "\n";
		//			for (size_t i = 0; i < accum.indention_size; i++)
		//				accum += "\t";
		//		}
		//		static void DecorateFieldHeader(FormattedAccum& accum, int field_index, std::string& field_name)
		//		{
		//			accum += field_name;
		//			accum += ":";
		//		}
		//	};


		//	


		//	typedef ExplicitRecursiveSerializerApplicator<std::string, string_serializer::StringIterpreter, string_serializer::BaseClassSerializer<std::string, std::string>, JSONCollectionDecorator, JSONFieldsContainerDecorator> JSONApplicator;
		//	//typedef ExplicitRecursiveSerializerApplicator<FormattedAccum, string_serializer::StringIterpreter, string_serializer::BaseClassIndentionSerializer, JSONFormattedCollectionDecorator, JSONFormattedFieldsContainerDecorator> JSONFormattedApplicator;
		//	typedef Serializer<JSONApplicator> JSONSerializer;
		//	typedef Serializer<JSONFormattedApplicator> JSONFormattedSerializer;
		//}


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
}
#endif  // SMPS_JSON_H_