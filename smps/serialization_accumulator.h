#pragma once
#include "serializable.h"


namespace smps
{

	template<class acum_field_represent_type, class result_type>
	class serialization_accumulator
	{
	public:
		virtual void add_field(const acum_field_represent_type& wrapped_field) = 0;
		virtual result_type Result() = 0;
	};

	template<class result_type, class field_ser, class accumulator, class serialization_obj_type>
	class obj_serializer
	{
		template<int i>
		class fields_iterator : public fields_iterator<i - 1>
		{
		public:
			static void serialize_fields(accumulator& accum, const serialization_obj_type& obj)
			{
				fields_iterator<i - 1>::serialize_fields(accum, obj);
				auto field = field_ser::serialize(serialization_obj_type::field_accessor<i>::GetName(), serialization_obj_type::field_accessor<i>::GetField(&obj));
				accum.add_field(field);
			}
		};

		template<>
		class fields_iterator<0> {
		public:
			static void serialize_fields(accumulator& accum, const serialization_obj_type& obj)
			{}
		};

	public:
		static result_type Serialize(const serialization_obj_type& obj)
		{
			accumulator accum;
			fields_iterator<serialization_obj_type::field_count::value>::serialize_fields(accum, obj);
			return accum.Result();
		}
	};
}
