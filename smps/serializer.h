#ifndef SMPS_SERIALIZER_H_
#define SMPS_SERIALIZER_H_

#include "serializable.h"
#include "stl_collections_serialization.h"

namespace smps
{
	template<typename T>
	using ClearType = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

	serializable_object_type::SMPSerializableType CheckSMPSObjectGhostFunc(SMPSerializable);
	serializable_object_type::GeneralType CheckSMPSObjectGhostFunc(...);

	template<class ObjectType>
	class SerializationTypeSelector
	{
	public:
		typedef decltype(CheckSMPSObjectGhostFunc(std::declval<ObjectType>())) SerializationType;
	};

	template<class SerializationType, class GeneralSerializer, class SpecialSerializer, class CollectionSerializer, class SMPSerializableSerializer, class SerializationDestination, class DeserializationSource>
	class DecisionSerializer;

	template<class GeneralSerializer, class SpecialSerializer, class CollectionSerializer, class SMPSerializableSerializer, class SerializationDestination, class DeserializationSource>
	class DecisionSerializer<serializable_object_type::GeneralType, GeneralSerializer, SpecialSerializer, CollectionSerializer, SMPSerializableSerializer, SerializationDestination, DeserializationSource>
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

	template<class GeneralSerializer, class SpecialSerializer, class CollectionSerializer, class SMPSerializableSerializer, class SerializationDestination, class DeserializationSource>
	class DecisionSerializer<serializable_object_type::SpecialType, GeneralSerializer, SpecialSerializer, CollectionSerializer, SMPSerializableSerializer, SerializationDestination, DeserializationSource>
	{
	public:
		template<class Type>
		static void Serialize(SerializationDestination& dest, const Type& obj)
		{
			SpecialSerializer::Serialize(dest, obj);
		}

		template<class Type>
		static void Deserialize(const DeserializationSource& src, Type& obj)
		{
			SpecialSerializer::Deserialize(src, obj);
		}
	};

	template<class GeneralSerializer, class SpecialSerializer, class CollectionSerializer, class SMPSerializableSerializer, class SerializationDestination, class DeserializationSource>
	class DecisionSerializer<serializable_object_type::CollectionType, GeneralSerializer, SpecialSerializer, CollectionSerializer, SMPSerializableSerializer, SerializationDestination, DeserializationSource>
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

	template<class GeneralSerializer, class SpecialSerializer, class CollectionSerializer, class SMPSerializableSerializer, class SerializationDestination, class DeserializationSource>
	class DecisionSerializer<serializable_object_type::SMPSerializableType, GeneralSerializer, SpecialSerializer, CollectionSerializer, SMPSerializableSerializer, SerializationDestination, DeserializationSource>
	{
	public:
		template< class Type>
		static void Serialize(SerializationDestination& dest, const Type& obj)
		{
			SMPSerializableSerializer::Serialize(dest, obj);
		}

		template<class Type>
		static void Deserialize(const DeserializationSource& src, Type& obj)
		{
			SMPSerializableSerializer::Deserialize(src, obj);
		}
	};


	template<class GeneralSerializer, class SpecialSerializer, class CollectionSerializer, class SMPSerializableSerializer, class SerializationDestination, class DeserializationSource>
	class Serializer
	{
	public:
		template<class Type>
		static void Serialize(SerializationDestination& dest, const Type& obj)
		{
			DecisionSerializer<SerializationTypeSelector<ClearType<Type>>::SerializationType, GeneralSerializer, SpecialSerializer, CollectionSerializer, SMPSerializableSerializer, SerializationDestination, DeserializationSource>::Serialize(dest, obj);
		}

		template<class Type>
		static void Deserialize(const DeserializationSource& src, Type& obj)
		{
			DecisionSerializer<SerializationTypeSelector<ClearType<Type>>::SerializationType, GeneralSerializer, SpecialSerializer, CollectionSerializer, SMPSerializableSerializer, SerializationDestination, DeserializationSource>::Deserialize(src, obj);
		}

	};

}
#endif  // SMPS_SERIALIZER_H_