#ifndef SMPS_WRAP_SERIALIZER_H_
#define SMPS_WRAP_SERIALIZER_H_

#include "smps_common.h"

namespace smps
{
	namespace wrap_serializer
	{
		template<class WrapperSpec, class AddToCollectionsProvider, class Serializer>
		class CollectionSerializer
		{
			template<class SerializationDestination, class Type>
			static void Serialize(SerializationDestination&& dest, Type&& obj)
			{
				dest.append(WrapperSpec::collection_begin);
				auto it = obj.cbegin();
				if (it != obj.cend())
				{
					Serializer::Serialize(dest, *it);
					it++;
					for (; it != obj.cend(); it++)
					{
						dest.append(WrapperSpec::collection_delimiter);
						Serializer::Serialize(dest, *it);
					}

					dest.append(WrapperSpec::collection_end);
				}
				dest.append(WrapperSpec::collection_end);
			}


			template<class DeserializationSource, class Type>
			static void Deserialize(DeserializationSource&& src, Type&& obj)
			{
				assert(src.GetSpec() == WrapperSpec::collection_begin);
				auto item = src.GetValue();
				Type::value_type des_item;
				while (item != WrapperSpec::empty)
				{
					Serializer::Deserialize(item, des_item);
					AddToCollectionsProvider::Add(obj, des_item);
					auto spec = src.GetSpec();
					assert(spec == WrapperSpec::collection_delimiter || spec == WrapperSpec::collection_end);
				}
			}
		};
	}
}
#endif //SMPS_WRAP_SERIALIZER_H_