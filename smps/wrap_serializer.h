#ifndef SMPS_WRAP_SERIALIZER_H_
#define SMPS_WRAP_SERIALIZER_H_

#include "smps_common.h"

namespace smps
{
	namespace wrap_serializer
	{
		template<class Wrapper, class Appender, class Serializer, class SerializationDestination, class DeserializationSource>
		class CollectionSerializer
		{
		public:
			template<class Type>
			static void Serialize(SerializationDestination& dest, const Type& obj)
			{
				Wrapper::WrapCollectionBegin(dest);
				auto it = obj.cbegin();
				if (it != obj.cend())
				{
					Serializer::Serialize(dest, *it);
					it++;
					for (; it != obj.cend(); it++)
					{
						Wrapper::WrapCollectionDelimiter(dest);
						Serializer::Serialize(dest, *it);
					}
				}
				Wrapper::WrapCollectionEnd(dest);
			}


			template< class Type>
			static void Deserialize(const DeserializationSource& src, Type& obj)
			{
				Wrapper::UnwrapCollectionBegin(src);
				typename Type::value_type item;
				while (true)
				{
					Serializer::Deserialize(src, item);
					Appender::Append(obj, item);
					if (!Wrapper::UnwrapCollectionDelimiter(src))
						break;
				}
				Wrapper::UnwrapCollectionEnd(src);
			}
		};

		template<class Wrapper, class Serializer, class SerializationDestination, class DeserializationSource>
		class SMPSSerializableSerializer
		{
			//std::map<Wrapper::FiledKeyType, SerializationDestination> deserialization_map_;
			template<class Type, int i>
			class RecursiveFieldsAccessor
			{
			public:
				static void Serialize(SerializationDestination& dest, const Type& obj)
				{
					if (i > 1) {
						RecursiveFieldsAccessor<Type, i - 1>::Serialize(dest, obj);
						Wrapper::WrapSMPSSerializableDelimiter(dest);
					}

					Serializer::Serialize(dest, Type::FieldAccessor<i>::GetName());
					Wrapper::WrapSMPSSerializableKeyValueDelimiter(dest);
					Serializer::Serialize(dest, Type::FieldAccessor<i>::GetField(obj));

				}

				static void Deserialize(const DeserializationSource& src, Type& obj/*, std::map<Wrapper::FieldHeader, DeserializationSource>& deserialization_map*/)
				{
					if (i > 1) {
						RecursiveFieldsAccessor<Type, i - 1>::Deserialize(src, obj/*, deserialization_map*/);
						Wrapper::UnwrapSMPSSerializableDelimiter(src);
					}

					std::string name;

					Serializer::Deserialize(src, name);
					Wrapper::UnwrapSMPSSerializableKeyValueDelimiter(src);
					if (name == Type::FieldAccessor<i>::GetName())
						Serializer::Deserialize(src, Type::FieldAccessor<i>::GetFieldAccessor(obj));

				}
			};

			template<class Type>
			class RecursiveFieldsAccessor<Type, 0>
			{
			public:

				static void Serialize(SerializationDestination& dest, const Type& obj)
				{}

				static void Deserialize(const DeserializationSource& src, Type& obj/*, DeserializationSource > & deserialization_map*/)
				{}
			};





		public:
			template<class Type>
			static void Serialize(SerializationDestination& dest, const Type& obj)
			{
				Wrapper::WrapSMPSSerializableBegin(dest);
				RecursiveFieldsAccessor<Type, Type::FieldCount::value>::Serialize(dest, obj);
				Wrapper::WrapSMPSSerializableEnd(dest);
			}


			template< class Type>
			static void Deserialize(const DeserializationSource& src, Type& obj)
			{
				//std::map<Wrapper::FieldHeader, DeserializationSource> deserialization_map;
				Wrapper::UnwrapSMPSSerializableBegin(src);
				RecursiveFieldsAccessor<Type, Type::FieldCount::value>::Deserialize(src, obj/*, deserialization_map*/);
				Wrapper::UnwrapSMPSSerializableEnd(src);
			}
		};

	}
}
#endif //SMPS_WRAP_SERIALIZER_H_