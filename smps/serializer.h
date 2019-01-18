#pragma once
#include "serializable.h"


namespace smps
{

	typedef char Small;
	class Big { char a[2]; };

	Small SmallIfSerializableFieldImpl(const serializable_field&);
	Big SmallIfSerializableFieldImpl(...);

#define IS_SER_FIELD_IMPL(type) sizeof(SmallIfSerializableFieldImpl(std::declval<type>()))==sizeof(Small)


	template<class serialization_accumulator>
	class serializer
	{
		template<int i>
		class fields_iterator : public fields_iterator<i - 1>
		{
		public:
			template<class serializable_obj_type>
			static void serialize_fields(serialization_accumulator& accum, const serializable_obj_type& obj)
			{
				fields_iterator<i - 1>::serialize_fields(accum, obj);
				accum.add_field(serializable_obj_type::field_accessor<i>::GetName(), serializable_obj_type::field_accessor<i>::GetField(&obj));
			}
		};

		template<>
		class fields_iterator<0> {
		public:
			template<class serializable_obj_type>
			static void serialize_fields(serialization_accumulator& accum, const serializable_obj_type& obj)
			{}
		};

	public:
		template<class serializable_obj_type>
		static decltype(std::declval<serialization_accumulator>().Result()) Serialize(const serializable_obj_type& obj)
		{
			serialization_accumulator accum;
			fields_iterator<serializable_obj_type::field_count::value>::serialize_fields(accum, obj);
			return accum.Result();
		}

		template<class serializable_obj_type>
		static serializable_obj_type Deserialize(const decltype(std::declval<serialization_accumulator>().Result())& serialized_obj)
		{
			return serializable_obj_type();
		}

	};
}
