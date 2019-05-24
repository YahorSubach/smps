#ifndef SMPS_SERIALIZER_H_
#define SMPS_SERIALIZER_H_

#include "serializable.h"
#include "stl_collections_serialization.h"

namespace smps
{
	template<typename T>
	using ClearType = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

	template<class Collection>
	decltype(std::declval<Collection>().cbegin(), std::declval<serializable_object_type::CollectionType>()) CheckSMPSObjectGhostFunc(const Collection&);

	serializable_object_type::SerializableType CheckSMPSObjectGhostFunc(Serializable);

	serializable_object_type::GeneralType CheckSMPSObjectGhostFunc(...);
	serializable_object_type::GeneralType CheckSMPSObjectGhostFunc(const std::string& str);


	template<class ObjectType>
	class SerializationTypeSelector
	{
	public:
		typedef decltype(CheckSMPSObjectGhostFunc(std::declval<ObjectType>())) SerializationType;
	};

	template<class SerializationType, class GeneralSerializer, class CollectionSerializer, class SerializableSerializer, class SerializationDestination, class DeserializationSource>
	class DecisionSerializer;

	template<class GeneralSerializer, class CollectionSerializer, class SerializableSerializer, class SerializationDestination, class DeserializationSource>
	class DecisionSerializer<serializable_object_type::GeneralType, GeneralSerializer, CollectionSerializer, SerializableSerializer, SerializationDestination, DeserializationSource>
	{
	public:
		template<class SerializationDestination, class Type>
		static void Serialize(SerializationDestination& dest, const Type& obj)
		{
			GeneralSerializer::Serialize(dest, obj);
		}

		template<class DeserializationSource, class Type>
		static void Deserialize(const DeserializationSource& src, Type& obj)
		{
			GeneralSerializer::Deserialize(src, obj);
		}
	};


	template<class GeneralSerializer, class CollectionSerializer, class SerializableSerializer, class SerializationDestination, class DeserializationSource>
	class DecisionSerializer<serializable_object_type::CollectionType, GeneralSerializer, CollectionSerializer, SerializableSerializer, SerializationDestination, DeserializationSource>
	{
	public:
		template< class Type>
		static void Serialize(SerializationDestination& dest, const Type& obj)
		{
			CollectionSerializer::Serialize(dest, obj);
		}

		template< class Type>
		static void Deserialize(const DeserializationSource& src, Type& obj)
		{
			CollectionSerializer::Deserialize(src, obj);
		}
	};

	template<class GeneralSerializer, class CollectionSerializer, class SerializableSerializer, class SerializationDestination, class DeserializationSource>
	class DecisionSerializer<serializable_object_type::SerializableType, GeneralSerializer, CollectionSerializer, SerializableSerializer, SerializationDestination, DeserializationSource>
	{
	public:
		template< class Type>
		static void Serialize(SerializationDestination& dest, const Type& obj)
		{
			SerializableSerializer::Serialize(dest, obj);
		}

		template<class Type>
		static void Deserialize(const DeserializationSource& src, Type& obj)
		{
			SerializableSerializer::Deserialize(src, obj);
		}
	};


	template<class GeneralSerializer, class CollectionSerializer, class SerializableSerializer, class Mapper, class SerializationDestination, class DeserializationSource>
	class Serializer
	{
	public:
		template<class Type>
		static void Serialize(SerializationDestination& dest, const Type& obj)
		{
			DecisionSerializer<SerializationTypeSelector<ClearType<decltype(Mapper::Map(obj))>>::SerializationType, GeneralSerializer, CollectionSerializer, SerializableSerializer, SerializationDestination, DeserializationSource>::Serialize(dest, Mapper::Map(obj));
		}

		template<class Type>
		static void Deserialize(const DeserializationSource& src, Type& obj)
		{
			DecisionSerializer<SerializationTypeSelector<ClearType<Type>>::SerializationType, GeneralSerializer, CollectionSerializer, SerializableSerializer, SerializationDestination, DeserializationSource>::Deserialize(src, obj);
		}

	};

}
#endif  // SMPS_SERIALIZER_H_