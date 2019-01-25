#ifndef SMPS_SERIALIZER_H_
#define SMPS_SERIALIZER_H_

#include "serializable.h"

namespace smps
{

	typedef char Small;
	class Big { char a[2]; };

	Small SmallIfSerializableFieldImpl(const SerializableField&);
	Big SmallIfSerializableFieldImpl(...);

#define IS_SER_FIELD_IMPL(type) sizeof(SmallIfSerializableFieldImpl(std::declval<type>()))==sizeof(Small)

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