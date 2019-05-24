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

		template<class Wrapper, class AddToCollectionsProvider, class Serializer, class SerializationDestination, class DeserializationSource>
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
			};

			template<class Type>
			class RecursiveFieldsAccessor<Type, 0>
			{
			public:
				static void Serialize(SerializationDestination& dest, const Type& obj)
				{
				}
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
				Wrapper::UnwrapSMPSSerializableBegin(src);
				typename Type::value_type item;
				while (true)
				{
					Serializer::Deserialize(src, item);
					AddToSMPSSerializablesProvider::Add(obj, item);
					if (!Wrapper::UnwrapSMPSSerializableDelimiter(src))
						break;
				}
				Wrapper::UnwrapSMPSSerializableEnd(src);
			}
		};

	}
}
#endif //SMPS_WRAP_SERIALIZER_H_