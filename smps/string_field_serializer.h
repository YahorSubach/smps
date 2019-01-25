#ifndef SMPS_STRING_FIELD_SERIALIZER_H_
#define SMPS_STRING_FIELD_SERIALIZER_H_

namespace smps
{

	template<class SerializerImplementation, CollectionSerializer>
	class StringFieldSerializer
	{
	public:
		template<class T>
		static std::string Serialize(const T& field)
		{
			return StringFieldSerializerIsSerDepend<IS_SER_FIELD_IMPL(T), SerializerImplementation>::Serialize(field);
		}
		template<class T>
		static T Deserialize(const std::string& ser_field)
		{
			return StringFieldSerializerIsSerDepend<IS_SER_FIELD_IMPL(T), SerializerImplementation>::Deserialize<T>(ser_field);
		}
	};


	template<bool is_serializable_implementation, class SerializerImplementation>
	class StringFieldSerializerIsSerDepend;

	template<class SerializerImplementation>
	class StringFieldSerializerIsSerDepend<true, SerializerImplementation>
	{
	public:
		template<class T>
		static std::string Serialize(const T& field)
		{
			return SerializerImplementation::Serialize(field);
		}
		template<class T>
		static T Deserialize(const std::string& ser_field)
		{
			return SerializerImplementation::Deserialize<T>(ser_field);
		}
	};

	template<class SerializerImplementation>
	class StringFieldSerializerIsSerDepend<false, SerializerImplementation>
	{
	public:
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

		template<class Iter>
		static std::string Serialize(Iter begin, Iter end)
		{
			std::stringstream ss;
			ss << "[";
			for (Iter it = begin; it != end; it++)
				ss << StringFieldSerializer::Serialize(*it) << ",";
			ss<<"]"
			return ss.;
		}
		template<class T>
		static std::string Serialize(const std::vector<T>&)
		{
			
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
#endif  // SMPS_STRING_FIELD_SERIALIZER_H_