#ifndef SMPS_WRAP_SERIALIZER_H_
#define SMPS_WRAP_SERIALIZER_H_

#include "smps_common.h"

namespace smps
{
	namespace wrap_serializer
	{
		template<class Wrapper, class AddToCollectionsProvider, class Serializer>
		class CollectionSerializer
		{
			template<class SerializationDestination, class Type>
			static void Serialize(SerializationDestination&& dest, const Type& obj)
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
				dest.append(WrapperSpec::collection_end);
				Wrapper::WrapCollectionEnd(dest);
			}


			template<class DeserializationSource, class Type>
			static void Deserialize(DeserializationSource& src, Type& obj)
			{
				Wrapper::UnwrapCollectionBegin(src);
				typename Type::value_type item;
				while (true)
				{
					Serializer::Deserialize(src, item);
					AddToCollectionsProvider::Add(obj, item);
					if (!Wrapper::UnwrapCollectionDelimiter(src))
						break;
				}
				Wrapper::UnwrapCollectionEnd(src);
			}
		};
	}
}
#endif //SMPS_WRAP_SERIALIZER_H_