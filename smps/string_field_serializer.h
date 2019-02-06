#ifndef SMPS_STRING_FIELD_SERIALIZER_H_
#define SMPS_STRING_FIELD_SERIALIZER_H_

#include "smps_class_wrappers.h"

namespace smps
{

	template<class SerializationType, class SerializerImplementation>
	class StringSerializableObjectSerializer;

	template<class SerializerImplementation>
	class StringFieldSerializer
	{
	public:
		template<class T>
		static std::string Serialize(const T& field)
		{
			return StringSerializableObjectSerializer<SerializationTypeSelector<T>::SerializationType, SerializerImplementation>::Serialize(field);
		}
		template<class T>
		static T Deserialize(const std::string& ser_field)
		{
			return StringSerializableObjectSerializer<SerializationTypeSelector<T>::SerializationType, SerializerImplementation>::template Deserialize<T>(ser_field);
		}
	};




	template<class SerializerImplementation>
	class StringSerializableObjectSerializer<serializable_object_type::SMPSObjectSerializationType, SerializerImplementation>
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
			return SerializerImplementation::template Deserialize<T>(ser_field);
		}
	};

	template<class SerializerImplementation>
	class StringSerializableObjectSerializer<serializable_object_type::BaseTypeSerializationType, SerializerImplementation>
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

	template<class SerializerImplementation>
	class StringSerializableObjectSerializer<serializable_object_type::CollectionSerializationType, SerializerImplementation>
	{
	public:

		template<class Iter>
		static std::string Serialize(Iter begin, Iter end)
		{
			std::stringstream ss;
			ss << "[";
			for (Iter it = begin; it != end; it++)
			{
				if (it != begin)
					ss << ",";
				ss << StringFieldSerializer<SerializerImplementation>::Serialize(*it);
			}
			ss << "]";
			return ss.str();
		}

		template<class T>
		static std::string Serialize(const T& field)
		{
			return Serialize(field.begin(), field.end());
		}

		template<class>
		class CollectionInserter;

		template<class ValueType>
		class CollectionInserter<std::vector<ValueType>>
		{
		public:
			static void Insert(std::vector<ValueType>& collection, ValueType value)
			{
				collection.push_back(value);
			}
			typedef ValueType ContentType;
		};

		template<class KeyType, class ValueType>
		class CollectionInserter<std::map<KeyType, ValueType>>
		{
		public:
			static void Insert(std::map<KeyType, ValueType>& collection, std::pair<KeyType, ValueType> value)
			{
				collection.insert(value);
			}

			typedef std::pair<KeyType, ValueType> ContentType;
		};


		template<class CollectionType>
		static CollectionType Deserialize(const std::string& ser_field)
		{
			int start_ind = 1;
			int end_ind = 1;
			CollectionType res;
			for (; end_ind < ser_field.length() - 1; end_ind++)
			{
				if (ser_field[end_ind] == ',')
				{
					CollectionInserter<CollectionType>::Insert(res, StringFieldSerializer<SerializerImplementation>::template Deserialize<CollectionInserter<CollectionType>::ContentType>(ser_field.substr(start_ind, end_ind - start_ind)));
					start_ind = end_ind + 1;
				}
			}
			return res;
		}
	};

	template<class SerializerImplementation>
	class StringSerializableObjectSerializer<serializable_object_type::PairSerializationType, SerializerImplementation>
	{
	public:

		template<class Pair>
		static std::string Serialize(const Pair& field)
		{
			return StringFieldSerializer<SerializerImplementation>::Serialize(smps::class_wrappers::make_pair(field));
		}

		template<class Pair>
		static Pair Deserialize(const std::string& ser_field)
		{
			typedef decltype(smps::class_wrappers::make_pair(std::declval<Pair>())) WrappedPair;
			return StringSerializableObjectSerializer<SerializationTypeSelector<WrappedPair>::SerializationType, SerializerImplementation>::template Deserialize<WrappedPair>(ser_field);
		}
	};
}
#endif  // SMPS_STRING_FIELD_SERIALIZER_H_