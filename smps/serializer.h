#ifndef SMPS_SERIALIZER_H_
#define SMPS_SERIALIZER_H_

#include "serializable.h"

namespace smps
{
	template<typename T>
	using ClearType = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

	namespace serializable_object_type
	{
		class GeneralType {};
		class SpecialType {};
		class CollectionType {};
		class SMPSerializableType {};
	}

	serializable_object_type::SMPSerializableType CheckSMPSObjectGhostFunc(const SMPSerializable&);
	serializable_object_type::GeneralType CheckSMPSObjectGhostFunc(...);

	template<class ObjectType>
	class SerializationTypeSelector
	{
	public:
		typedef decltype(CheckSMPSObjectGhostFunc(std::declval<ObjectType>())) SerializationType;
	};

	template<class SerializationType, class GeneralSerializer, class SpecialSerializer, class CollectionSerializer, class SMPSerializableSerializer>
	class DecisionSerializer;

	template<class GeneralSerializer, class SpecialSerializer, class CollectionSerializer, class SMPSerializableSerializer>
	class DecisionSerializer<serializable_object_type::GeneralType, GeneralSerializer, SpecialSerializer, CollectionSerializer, SMPSerializableSerializer>
	{
	public:
		template<class Destination, class Type>
		static void Serialize(Destination&& dest, Type&& obj)
		{
			GeneralSerializer::Serialize(dest, obj);
		}

		template<class Source, class Type>
		static void Deserialize(Source&& src, Type&& obj)
		{
			GeneralSerializer::Deserialize(src, obj);
		}
	};

	template<class GeneralSerializer, class SpecialSerializer, class CollectionSerializer, class SMPSerializableSerializer>
	class DecisionSerializer<serializable_object_type::SpecialType, GeneralSerializer, SpecialSerializer, CollectionSerializer, SMPSerializableSerializer>
	{
	public:
		template<class Destination, class Type>
		static void Serialize(Destination&& dest, Type&& obj)
		{
			SpecialSerializer::Serialize(dest, obj);
		}

		template<class Source, class Type>
		static void Deserialize(Source&& src, Type&& obj)
		{
			SpecialSerializer::Deserialize(src, obj);
		}
	};

	template<class GeneralSerializer, class SpecialSerializer, class CollectionSerializer, class SMPSerializableSerializer>
	class DecisionSerializer<serializable_object_type::CollectionType, GeneralSerializer, SpecialSerializer, CollectionSerializer, SMPSerializableSerializer>
	{
	public:
		template<class Destination, class Type>
		static void Serialize(Destination&& dest, Type&& obj)
		{
			CollectionSerializer::Serialize(dest, obj);
		}

		template<class Source, class Type>
		static void Deserialize(Source&& src, Type&& obj)
		{
			CollectionSerializer::Deserialize(src, obj);
		}
	};

	template<class GeneralSerializer, class SpecialSerializer, class CollectionSerializer, class SMPSerializableSerializer>
	class DecisionSerializer<serializable_object_type::SMPSerializableType, GeneralSerializer, SpecialSerializer, CollectionSerializer, SMPSerializableSerializer>
	{
	public:
		template<class Destination, class Type>
		static void Serialize(Destination&& dest, Type&& obj)
		{
			SMPSerializableSerializer::Serialize(dest, obj);
		}

		template<class Source, class Type>
		static void Deserialize(Source&& src, Type&& obj)
		{
			SMPSerializableSerializer::Deserialize(src, obj);
		}
	};


	template<class GeneralSerializer, class SpecialSerializer, class CollectionSerializer, class SMPSerializableSerializer>
	class Serializer
	{
	public:
		template<class Destination, class Type>
		static void Serialize(Destination&& dest, Type&& obj)
		{
			DecisionSerializer<SerializationTypeSelector<ClearType<Type>>::SerializationType, GeneralSerializer, SpecialSerializer, CollectionSerializer, SMPSerializableSerializer>::Serialize(dest, obj);
		}

		template<class Source, class Type>
		static void Deserialize(Source&& src, Type&& obj)
		{
			DecisionSerializer<SerializationTypeSelector<ClearType<Type>>::SerializationType, GeneralSerializer, SpecialSerializer, CollectionSerializer, SMPSerializableSerializer>::Deserialize(src, obj);
		}

	};

}
#endif  // SMPS_SERIALIZER_H_