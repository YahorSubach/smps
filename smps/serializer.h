#ifndef SMPS_SERIALIZER_H_
#define SMPS_SERIALIZER_H_

#include "serializable.h"

namespace smps
{
	template<class T>
	class ConstAndRefRemover
	{
	public:
		typedef T ClearType;
	};

	template<class T>
	class ConstAndRefRemover<T&>
	{
	public:
		typedef T ClearType;
	};

	template<class T>
	class ConstAndRefRemover<const T&>
	{
	public:
		typedef T ClearType;
	};

	template<class T>
	class ConstAndRefRemover<const T>
	{
	public:
		typedef T ClearType;
	};


	namespace serializable_object_type
	{
		class BaseTypeSerializationType {};
		class CollectionSerializationType {};
		class PairSerializationType {};
		class FieldCompositionSerializationType {};
	}

	serializable_object_type::FieldCompositionSerializationType CheckSMPSObjectGhostFunc(const FieldComposition&);
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


	template<class SerializationType>
	class AnySerializationTypeSerializer;

	template<>
	class AnySerializationTypeSerializer<serializable_object_type::BaseTypeSerializationType>
	{
	public:
		template<class Applicator, class Type>
		static void Serialize(typename Applicator::AccumulatorType& accum, Type& obj)
		{
			Applicator::SerializeBaseType(accum, obj);
		}
	};

	template<>
	class AnySerializationTypeSerializer<serializable_object_type::CollectionSerializationType>
	{
	public:
		template<class Applicator, class Type>
		static void Serialize(typename Applicator::AccumulatorType& accum, Type& obj)
		{
			Applicator::SerializeCollection(accum, obj);
		}
	};

	template<>
	class AnySerializationTypeSerializer<serializable_object_type::FieldCompositionSerializationType>
	{
	public:
		template<class Applicator, class Type>
		static void Serialize(typename Applicator::AccumulatorType& accum, Type& obj)
		{
			Applicator::SerializeFieldsContainer(accum, obj);
		}
	};



	template<class Applicator>
	class Serializer
	{
	public:
		template<class Type>
		static void Serialize(typename Applicator::AccumulatorType& accum, Type& obj)
		{
			AnySerializationTypeSerializer<SerializationTypeSelector<ConstAndRefRemover<Type>::ClearType>::SerializationType>::template Serialize<Applicator, Type>(accum, obj);
		}
	};

}
#endif  // SMPS_SERIALIZER_H_