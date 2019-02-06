#ifndef SMPS_SERIALIZER_H_
#define SMPS_SERIALIZER_H_

#include "serializable.h"

namespace smps
{
	namespace serializable_object_type
	{
		class BaseTypeSerializationType {};
		class CollectionSerializationType {};
		class PairSerializationType {};
		class SMPSObjectSerializationType {};
	}

	serializable_object_type::SMPSObjectSerializationType CheckSMPSObjectGhostFunc(const SerializableField&);
	serializable_object_type::BaseTypeSerializationType CheckSMPSObjectGhostFunc(...);

	template<class ObjectType>
	class SerializationTypeSelector
	{
	public:
		typedef decltype(CheckSMPSObjectGhostFunc(std::declval<ObjectType>())) SerializationType;
	};

	template<class VectorContentType>
	class SerializationTypeSelector<std::vector<VectorContentType>>
	{
	public:
		typedef serializable_object_type::CollectionSerializationType SerializationType;
	};

	template<class MapKeyType, class MapValueType>
	class SerializationTypeSelector<std::map<MapKeyType, MapValueType>>
	{
	public:
		typedef serializable_object_type::CollectionSerializationType SerializationType;
	};

	template<class PairKeyType, class PairValueType>
	class SerializationTypeSelector<std::pair<PairKeyType, PairValueType>>
	{
	public:
		typedef serializable_object_type::PairSerializationType SerializationType;
	};



	template<class SerializrAccumulator, class SerializationParser>
	class Serializer
	{
		template<int i>
		class FieldsIterator : public FieldsIterator<i - 1>
		{
		public:
			template<class SerializableObjType>
			static void serialize_fields(SerializrAccumulator& accum, const SerializableObjType& obj)
			{
				FieldsIterator<i - 1>::serialize_fields(accum, obj);
				accum.add_field(SerializableObjType::FieldAccessor<i>::GetName(), SerializableObjType::FieldAccessor<i>::GetField(&obj));
			}
			template<class SerializableObjType>
			static void deserialize_fields(SerializationParser& parser, SerializableObjType& obj)
			{
				FieldsIterator<i - 1>::deserialize_fields(parser, obj);
				parser.get_field<SerializableObjType, i>(obj);
			}
		};

		template<>
		class FieldsIterator<0> {
		public:
			template<class SerializableObjType>
			static void serialize_fields(SerializrAccumulator& accum, const SerializableObjType& obj)
			{}
			template<class SerializableObjType>
			static void deserialize_fields(SerializationParser& accum, const SerializableObjType& obj)
			{}
		};

	public:
		template<class SerializableObjType>
		static decltype(std::declval<SerializrAccumulator>().Result()) Serialize(const SerializableObjType& obj)
		{
			SerializrAccumulator accum;
			FieldsIterator<SerializableObjType::FieldCount::value>::serialize_fields(accum, obj);
			return accum.Result();
		}

		template<class SerializableObjType>
		static SerializableObjType Deserialize(const decltype(std::declval<SerializrAccumulator>().Result())& serialized_obj)
		{
			SerializableObjType obj;
			SerializationParser parser(serialized_obj);
			parser.Split();
			FieldsIterator<SerializableObjType::FieldCount::value>::deserialize_fields(parser, obj);
			return obj;
		}
	};
}
#endif  // SMPS_SERIALIZER_H_