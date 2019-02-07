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

	serializable_object_type::SMPSObjectSerializationType CheckSMPSObjectGhostFunc(const FieldComposition&);
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



	template<class SerializerAccumulator, class SerializationParser>
	class Serializer
	{
		template<int i>
		class FieldsIterator : public FieldsIterator<i - 1>
		{
		public:
			template<class SerializableObjType>
			static void serialize_fields(SerializerAccumulator& accum, const SerializableObjType& obj)
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
			static void serialize_fields(SerializerAccumulator& accum, const SerializableObjType& obj)
			{}
			template<class SerializableObjType>
			static void deserialize_fields(SerializationParser& accum, const SerializableObjType& obj)
			{}
		};

	public:
		template<class SerializableObjType>
		static decltype(std::declval<SerializerAccumulator>().Result()) Serialize(const SerializableObjType& obj)
		{
			SerializerAccumulator accum;
			FieldsIterator<SerializableObjType::FieldCount::value>::serialize_fields(accum, obj);
			return accum.Result();
		}

		template<class SerializableObjType>
		static SerializableObjType Deserialize(const decltype(std::declval<SerializerAccumulator>().Result())& serialized_obj)
		{
			SerializableObjType obj;
			SerializationParser parser(serialized_obj);
			parser.Split();
			FieldsIterator<SerializableObjType::FieldCount::value>::deserialize_fields(parser, obj);
			return obj;
		}
	};


	template<int i>
	class RecursiveFieldsAggregator : public RecursiveFieldsAggregator<i - 1>
	{
	public:
		template<class FieldCompositionSerializationAccumulator, class FieldCompositionObjectType>
		static void ProvideFieldsToAccumulator(FieldCompositionSerializationAccumulator& accum, const FieldCompositionObjectType& obj)
		{
			RecursiveFieldsAggregator<i - 1>::ProvideFieldsToAccumulator(accum, obj);
			accum.Add<FieldCompositionObjectType, i>(obj);
		}
		template<class FieldCompositionSerializationSplitter, class FieldCompositionObjectType>
		static void RestoreFieldsInSplitter(FieldCompositionSerializationSplitter& splitter)
		{
			RecursiveFieldsAggregator<i - 1>::RestoreFieldsInSplitter(splitter);
			splitter.Restore<FieldCompositionObjectType, i>();
		}
	};

	template<>
	class RecursiveFieldsAggregator<0> {
	public:
		template<class FieldCompositionSerializationAccumulator, class FieldCompositionObjectType>
		static void ProvideFieldsToAccumulator(FieldCompositionSerializationAccumulator& accum, const FieldCompositionObjectType& obj)
		{
		}
		template<class FieldCompositionSerializationSplitter, class FieldCompositionObjectType>
		static void RestoreFieldsInSplitter(FieldCompositionSerializationSplitter& splitter, const FieldCompositionObjectType& obj)
		{
		}
	};

	class CollectionAggregator
	{
	public:
		template<class CollectionSerializationAccumulator, class CollectionType>
		static void ProvideToAccumulator(CollectionSerializationAccumulator& accum, const CollectionType& col)
		{
			for (auto iter = col.begin(); iter != col.end(); iter++)
				accum.Add(*iter);
		}
		template<class CollectionSerializationSplitter>
		static void RestoreInSplitter(CollectionSerializationSplitter& splitter)
		{
			splitter.Restore();
		}
	};





	template<class NativeTypeSerializer, class FieldCompositionSerializer, class CollectionAccumulator, class CollectionParser>
	class CollectionSerializer
	{

	};

	template<class NativeTypeSerializer, class CollectionSerializer, class FieldCompositionAccumulator, class FieldCompositionSerializer>
	class FieldCompositionSerializer
	{
	public:
		template<class FieldCompositionObjectType>
		static decltype(std::declval<FieldCompositionAccumulator>.Result()) Serialize(FieldCompositionObjectType& object)
		{
			FieldCompositionAccumulator accum;
			RecursiveFieldsAggregator<FieldCompositionObjectType::FieldCount::value>::ProvideFieldsToAccumulator(accum, object);
			return accum.Result();
		}

	};

}
#endif  // SMPS_SERIALIZER_H_