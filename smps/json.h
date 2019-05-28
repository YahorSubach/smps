#ifndef SMPS_JSON_H_
#define SMPS_JSON_H_

//#include "recursive_serializer.h"
#include "serializer.h"
#include "string_serializer.h"
#include "wrap_serializer.h"
namespace smps
{
	namespace json
	{
		class JSONBlockReader
		{
		private:
			const std::string_view str_view_;
			mutable size_t index_;

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

			bool Validate(const char& c) const
			{
				for (; index_ < str_view_.length(); index_++)
				{
					if (!(str_view_[index_] == ' ' || str_view_[index_] == '\t'))
						return c == str_view_[index_];
				}
				return false;
			}

			std::string_view Read() const
			{
				size_t begin_index = index_;
				bool string_reading = false;

				for (; index_ < str_view_.length(); index_++)
				{
					switch (str_view_[index_])
					{
					case ' ':
					case '\t':
					case '\r':
					case '\n':
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


			JSONBlockReader ReadBlock() const
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
					case '\r':
					case '\n':
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


		class JSONWrapper
		{
		public:
			template <class SerializationDestination>
			static void WrapCollectionBegin(SerializationDestination& dst)
			{
				dst.append("[");
			}
			template <class DeserializationSource>
			static bool UnwrapCollectionBegin(const DeserializationSource& src)
			{
				if (src.Validate('['))
				{
					src.Read();
					return true;
				}
				return false;
			}

			template <class SerializationDestination>
			static void WrapCollectionDelimiter(SerializationDestination& dst)
			{
				dst.append(",");
			}
			template < class DeserializationSource>
			static bool UnwrapCollectionDelimiter(const DeserializationSource& src)
			{
				if (src.Validate(','))
				{
					src.Read();
					return true;
				}
				return false;
			}

			template <class SerializationDestination>
			static void WrapCollectionEnd(SerializationDestination& dst)
			{
				dst.append("]");
			}
			template < class DeserializationSource>
			static bool UnwrapCollectionEnd(const DeserializationSource& src)
			{
				if (src.Validate(']'))
				{
					src.Read();
					return true;
				}
				return false;
			}

			template <class SerializationDestination>
			static void WrapSMPSSerializableBegin(SerializationDestination& dst)
			{
				dst.append("{");
			}
			template <class DeserializationSource>
			static bool UnwrapSMPSSerializableBegin(const DeserializationSource& src)
			{
				if (src.Validate('}'))
				{
					src.Read();
					return true;
				}
				return false;
			}

			template <class SerializationDestination>
			static void WrapSMPSSerializableDelimiter(SerializationDestination& dst)
			{
				dst.append(",");
			}
			template < class DeserializationSource>
			static bool UnwrapSMPSSerializableDelimiter(const DeserializationSource& src)
			{
				if (src.Validate(','))
				{
					src.Read();
					return true;
				}
				return false;
			}

			template <class SerializationDestination>
			static void WrapSMPSSerializableKeyValueDelimiter(SerializationDestination& dst)
			{
				dst.append(":");
			}
			template < class DeserializationSource>
			static bool UnwrapSMPSSerializableKeyValueDelimiter(const DeserializationSource& src)
			{
				if (src.Validate(':'))
				{
					src.Read();
					return true;
				}
				return false;
			}

			template <class SerializationDestination>
			static void WrapSMPSSerializableEnd(SerializationDestination& dst)
			{
				dst.append("}");
			}
			template < class DeserializationSource>
			static bool UnwrapSMPSSerializableEnd(const DeserializationSource& src)
			{
				if (src.Validate('}'))
				{
					src.Read();
					return true;
				}
				return false;
			}

		};



		using GenSer = string_serializer::GeneralSerializer<std::string, JSONBlockReader>;

		template<class ResultType = std::string>
		class JSONSerializerBase :public Serializer< string_serializer::GeneralSerializer<ResultType, JSONBlockReader>,
			smps::wrap_serializer::CollectionSerializer<JSONWrapper, stl::Appender, JSONSerializerBase<ResultType>, ResultType, JSONBlockReader>,
			smps::wrap_serializer::SMPSSerializableSerializer<JSONWrapper, JSONSerializerBase<ResultType>, ResultType, JSONBlockReader>,
			smps::stl::Mapper, ResultType, JSONBlockReader>
		{};

		class JSONSerializer
		{
		public:
			template<class ResultType, class Type>
			static void Serialize(ResultType& dest, const Type& obj)
			{
				JSONSerializerBase<ResultType>::Serialize(dest, obj);
			}

			template<class ResultType, class Type>
			static ResultType&& Serialize(const Type& obj)
			{
				ResultType res;
				JSONSerializerBase<ResultType>::Serialize(res, obj);
				return std::move(res);
			}

			template<class Type>
			static void Deserialize(const std::string& src, Type& obj)
			{
				JSONSerializerBase<>::Deserialize(JSONBlockReader(src), obj);
			}

			template< class Type>
			static Type Deserialize(const std::string& src)
			{
				Type res;
				JSONSerializerBase<>::Deserialize(JSONBlockReader(src), res);
				return res;
			}
		};

		class FormattedJSONString
		{
			std::string str_;
			int spacing_ = 0;
		public:
			std::string value;
			void append(const std::string& str)
			{

				if (str == "[" || str == "{")
				{
					value.append(str);
					spacing_++;
					value += '\n';
					for (int i = 0; i < spacing_; i++)
						value += "    ";
				}
				else
					if (str == "]" || str == "}")
					{
						spacing_--;
						value += '\n';
						for (int i = 0; i < spacing_; i++)
							value += "    ";
						value.append(str);
					}
					else
						if (str == ",")
						{
							value.append(str);
							value += '\n';
							for (int i = 0; i < spacing_; i++)
								value += "    ";
						}
						else
							value.append(str);
			}

		};

	}
}
#endif  // SMPS_JSON_H_